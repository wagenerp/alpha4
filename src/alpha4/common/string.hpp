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

#ifndef ALPHA_COMMON_STRING_HPP
#define ALPHA_COMMON_STRING_HPP
#include "alpha4/common/error.hpp"
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
namespace alp {

template<typename T> struct StringTranscoder;

// decode numbers
template<typename T>
requires std::is_integral_v<T>
	&&std::is_unsigned_v<T> struct StringTranscoder<T> {
	bool operator()(const std::string_view &s, T &v) {
		char *             endptr = nullptr;
		unsigned long long vt     = strtoull(s.data(), &endptr, 0);

		if (endptr <= s.data()) return false;
		v = (T)(vt);
		if ((unsigned long long)(v) != vt) return false;
		return true;
	}
};

template<typename T>
requires std::is_integral_v<T>
	&&std::is_signed_v<T> struct StringTranscoder<T> {
	bool operator()(const std::string_view &s, T &v) {
		char *    endptr = nullptr;
		long long vt     = strtoll(s.data(), &endptr, 0);

		if (endptr <= s.data()) return false;
		v = (T)(vt);
		if ((long long)(v) != vt) return false;
		return true;
	}
};

template<typename T>
requires std::is_floating_point_v<T> struct StringTranscoder<T> {
	bool operator()(const std::string_view &s, T &v) {
		char *      endptr = nullptr;
		long double vt     = strtold(s.data(), &endptr);

		if (endptr <= s.data()) return false;
		v = (T)(vt);
		// if ((long double)(v) != vt) return false;
		return true;
	}
};

// decode generics

template<typename T>
requires std::is_convertible_v<
	std::string_view,
	std::decay_t<T>> struct StringTranscoder<T> {
	bool operator()(const std::string_view &s, T &v) {
		try {
			v = s;
		} catch (...) { return false; }
		return true;
	}
};

template<typename T> struct StringTranscoder<std::optional<T>> {
	bool operator()(const std::string_view &s, std::optional<T> &v) {
		std::decay_t<T> res;
		v.reset();
		if (!StringTranscoder<T>()(s, res)) return false;
		v = res;
		return true;
	}
};

template<> struct StringTranscoder<std::string> {
	bool operator()(const std::string_view &s, std::string &v) {
		try {
			v = s;
		} catch (...) { return false; }
		return true;
	}
};

template<typename T> bool decodeString(const char *str, T &v) {
	return StringTranscoder<T>()(str, v);
}
template<typename T> bool decodeString(const std::string_view &str, T &v) {
	return StringTranscoder<T>()(str, v);
}
template<typename T> bool decodeString(const std::string &str, T &v) {
	return StringTranscoder<T>()(str, v);
}

inline bool decodeString(const char *str, const char *&v) {
	v = str;
	return true;
}

inline bool decodeString(const std::string &str, std::string &v) {
	v = str;
	return true;
}
inline bool decodeString(const std::string &str, std::string_view &v) {
	v = str;
	return true;
}

inline bool decodeString(const std::string &str, const char *&v) {
	v = str.c_str();
	return true;
}

// throwing decode variant
template<typename T> std::decay_t<T> decodeString(const char *str) {
	std::decay_t<T> res;
	if (!decodeString(str, res)) {
		throw std::range_error("string decoding failed");
	}
	return res;
}
template<typename T> std::decay_t<T> decodeString(const std::string &str) {
	std::decay_t<T> res;
	if (!decodeString(str, res)) {
		throw std::range_error("string decoding failed");
	}
	return res;
}
template<typename T> std::decay_t<T> decodeString(const std::string_view &str) {
	std::decay_t<T> res;
	if (!decodeString(str, res)) {
		throw std::range_error("string decoding failed");
	}
	return res;
}

} // namespace alp
#endif