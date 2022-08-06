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

#ifndef ALPHA4_COMMON_ERROR_HPP
#define ALPHA4_COMMON_ERROR_HPP

#include "alpha4/types/token.hpp"
#include <exception>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>

namespace alp {

class Exception : public std::exception {
protected:
	const std::string _message;

public:
	Exception(std::string &&message) : _message(std::move(message)) {}
	Exception(const std::string &message) : _message(message) {}

	virtual const char *what() const noexcept override {
		return _message.c_str();
	}
};

template<typename Exception, typename... Args>
requires requires(const std::string s, Args... args) {
	Exception(s, args...);
}
&&std::is_base_of_v<std::exception, Exception> class thrower {
protected:
	std::stringstream   _ss;
	std::tuple<Args...> _args;

public:
	thrower(Args... args) : _args(args...) {}
	template<typename S> thrower &operator<<(const S &v) {
		_ss << v;
		return *this;
	}
	void operator<<(const over_t &) {
		throw std::apply(
			[this](Args... args) { return Exception(_ss.str(), args...); }, _args);
	}
};

} // namespace alp

#endif