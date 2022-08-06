/* Copyright 2022 Peter Wagener <mail@peterwagener.net>

This file is part of the Alpha4 library.

Alpha4 is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

Alpha4 is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Alpha4. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ALPHA4_COMMON_LOGGER_HPP
#define ALPHA4_COMMON_LOGGER_HPP
#include "alpha4/types/token.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
namespace alp {
class Logger : public std::enable_shared_from_this<Logger> {
public:
	enum struct Type { E, W, I, D, T };

	struct SourcePointer {
		std::string fn;
		int         line = -1;
	};

	struct Message {
	private:
		std::weak_ptr<Logger> _logger;
		bool                  _done = false;
		const size_t          _messageNumber;

	public:
		const std::chrono::system_clock::time_point tBirth;
		std::chrono::system_clock::time_point       tEmit;
		Type                                        type;
		SourcePointer                               src;
		std::stringstream                           msg;

		constexpr bool done() const { return _done; }

		Message(std::shared_ptr<Logger> logger, Type type, size_t number) :
			_logger(logger),
			_messageNumber(number),
			tBirth(std::chrono::system_clock::now()),
			type(type) {}

		Message(Message &&m) :
			_logger(m._logger),
			_done(m._done),
			_messageNumber(m._messageNumber),
			tBirth(m.tBirth),
			tEmit(m.tEmit),
			type(m.type),
			src(m.src),
			msg(std::move(m.msg)) {
			m._logger.reset();
		}
		~Message() { *this << over; }

		template<typename T> constexpr Message &operator<<(const T &v) {
			msg << v;
			return *this;
		}

		Message &operator<<(const SourcePointer &v) {
			src = v;
			return *this;
		}

		void operator<<(const over_t &) {
			if (!_done) {
				tEmit = std::chrono::system_clock::now();
				_done = true;
				if (auto logger = _logger.lock(); logger) logger->emit(*this);
			}
		}

		Message &operator<<(const flush_t &) {
			if (!_done) {
				tEmit = std::chrono::system_clock::now();
				if (auto logger = _logger.lock(); logger) logger->emit(*this);
			}

			return *this;
		}
	};

	struct MutedMessage {
		template<typename T> constexpr MutedMessage &operator<<(const T &) {
			return *this;
		}
	};

	template<typename T> struct Colored {
		const T &v;
		constexpr Colored(const T &v) : v(v) {}

		constexpr friend std::ostream &
		operator<<(std::ostream &os, const Colored &wrapper) {
			if constexpr (std::is_same_v<T, Type>) {
				switch (wrapper.v) {
					case Type::E: os << "\x1b[31;1mERR\x1b[30;0m"; break;
					case Type::W: os << "\x1b[33;1mWRN\x1b[30;0m"; break;
					case Type::I: os << "INF"; break;
					case Type::D: os << "\x1b[34;1mDEB\x1b[30;0m"; break;
					case Type::T: os << "\x1b[35;1mTRC\x1b[30;0m"; break;
				}
			} else {
				os << wrapper.v;
			}

			return os;
		}
	};

protected:
	size_t _messageCounter = 0;

public:
	const std::chrono::system_clock::time_point tBirth;

	static inline std::string
	Format(const std::chrono::system_clock::time_point &tp) {
		const std::time_t t = std::chrono::system_clock::to_time_t(tp);

		char buf[64];
		std::strftime(buf, sizeof(buf), "%Y-%m-%d %H%M", std::localtime(&t));

		return buf;
	}

	Logger() : tBirth(std::chrono::system_clock::now()) {}
	virtual ~Logger() {}
	static std::shared_ptr<Logger> GetPointer();
	static Logger &                Get() { return *GetPointer(); }

	template<Type type> Message startMessage() {
		return Message(shared_from_this(), type, _messageCounter++);
	}

	virtual void emit(const Message &msg);
};

} // namespace alp
#define LOG(type)                                        \
	if (auto logger = ::alp::Logger::GetPointer(); logger) \
		(*logger).startMessage<::alp::Logger::Type::type>()  \
			<< ::alp::Logger::SourcePointer {                  \
			__FILE__, __LINE__                                 \
		}

#endif