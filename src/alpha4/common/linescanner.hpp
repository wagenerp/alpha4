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

#ifndef ALPHA4_COMMON_LINESCANNER_HPP
#define ALPHA4_COMMON_LINESCANNER_HPP
#include "alpha4/common/error.hpp"
#include "alpha4/common/string.hpp"
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace alp {

class LineScanner {
public:
	enum ReadMode { AnyString, InLineString, FirstString, Remainder };
	enum : uint32_t {
		FUSE_ESCAPE = 0x01,
	};
	class ScanError : public Exception {
	public:
		using Exception::Exception;
	};

protected:
	const char *_data;
	const char *_p, *_end;
	int         _newLine;

public:
	LineScanner();
	LineScanner(const char *data);
	LineScanner(const char *data, size_t cb);
	LineScanner(const char *data, const char *e);
	~LineScanner();

	uint32_t flags = 0;

	void assign(const char *data);
	void assign(const char *data, size_t cb);
	void assign(const char *data, const char *e);

	void operator=(const char *data) { assign(data); }

	size_t tell() const;
	size_t seek(int offset, int origin);

	const char *data() const { return _data; }
	size_t      size() const { return (size_t)_end - (size_t)_data; }

	bool eof() const { return _p >= _end; }
	bool isNewLine() const { return _newLine != 0; }

	bool seekNewLine(bool alwaysAdvance);

	bool getAnyString(std::string_view &res);
	bool getLnString(std::string_view &res);
	bool getLnFirstString(std::string_view &res);
	template<ReadMode Mode = AnyString> bool getString(std::string_view &res) {
		switch (Mode) {
			case AnyString: return getAnyString(res);
			case InLineString: return getLnString(res);
			case FirstString: return getLnFirstString(res);
			case Remainder: return getLnRemainder(res);
		}
		return false;
	}

	bool getLnRemainder(std::string_view &res);
	bool getTrimmedLine(std::string_view &res, bool rewind);

	template<typename T, ReadMode Mode = AnyString> bool get(T &v) {
		std::string_view str;
		if (!getString<Mode>(str)) return false;
		return decodeString(str, v);
	}

	struct DecodeBuffer {
		std::vector<std::string> temporaries;
	};

	template<typename T, ReadMode Mode = AnyString> T decode(DecodeBuffer &buf) {
		if constexpr (std::is_same_v<T, const char *>) {
			buf.temporaries.emplace_back(decode<std::string>(buf));
			return buf.temporaries.back().c_str();

		} else {
			T rv;
			if (!get<T, Mode>(rv))
				thrower<ScanError>() << "failed to decode value" << over;
			return rv;
		}
	}

	template<typename... T> struct MyTuple {
		std::tuple<T...> v;
		template<typename... B>
		MyTuple<T..., B...> constexpr operator+(const MyTuple<B...> &b) const {
			return MyTuple<T..., B...>(std::tuple_cat(v, b.v));
		}
	};

	template<std::size_t Index, typename First, typename Next1, typename... NextN>
	constexpr auto decodeInOrder(const DecodeBuffer &buf) {
		return std::tuple_cat(
			std::tuple<First>(decodeString<First>(buf.temporaries[Index])),
			decodeInOrder<Index + 1, Next1, NextN...>(buf));
	}
	template<std::size_t Index, typename First>
	constexpr auto decodeInOrder(const DecodeBuffer &buf) {
		return std::tuple<First>(decodeString<First>(buf.temporaries[Index]));
	}

	template<typename... Args> auto decodeInOrder(DecodeBuffer &buf) {
		buf.temporaries.resize(sizeof...(Args));
		for (auto &tmp : buf.temporaries) {
			if (!get(tmp)) {
				thrower<ScanError>() << "failed to decode value" << over;
			}
		}

		return decodeInOrder<0, Args...>(buf);
	}

	template<typename... T> bool getAll(T &... v) {
		DecodeBuffer buf;
		try {
			auto args              = decodeInOrder<std::decay_t<T>...>(buf);
			std::tie<T &...>(v...) = args;
		} catch (const ScanError &) { return false; }
		return true;
	}

	template<typename RT, typename... Args>
	RT call(std::function<RT(Args...)> target) {
		DecodeBuffer buf;

		auto args = decodeInOrder<std::decay_t<Args>...>(buf);

		return std::apply(target, args);
	}
	template<typename RT, typename... Args> RT call(RT target(Args...)) {
		DecodeBuffer buf;
		auto         args = decodeInOrder<std::decay_t<Args>...>(buf);
		return std::apply(target, args);
	}
	template<typename F, typename... Args>
	auto call(F target, void (F::*)(Args...) const) {
		DecodeBuffer buf;
		auto         args = decodeInOrder<std::decay_t<Args>...>(buf);
		return std::apply(target, args);
	}

	template<typename F> auto call(F target) {
		return call(target, &F::operator());
	}

	template<typename F>
	static auto Call(F target, const char *str, const char *end = nullptr) {
		LineScanner ln;
		if (nullptr != end) {
			ln.assign(str, end);
		} else {
			ln.assign(str);
		}
		return ln.call(target);
	}
};

struct MultilineScanner {
protected:
	std::string _compound;

public:
	std::function<void(const std::string &)> callback;

	bool useContinuation = true;
	bool useComments     = true;
	char delimiter       = '\n';

	void processLine(const std::string &line);
	void flush();
};

} // namespace alp

#endif