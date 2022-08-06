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

#ifndef ALPHA4_COMMON_GUARD_HPP
#define ALPHA4_COMMON_GUARD_HPP

#include "alpha4/common/guard.hpp"
#include <functional>
namespace alp {

struct Guard {
	std::function<void()> func;

	template<typename F> requires requires(F f) { noexcept(f()); }
	Guard(F func) : func(func) {}
	Guard(const Guard &) = delete;
	Guard(Guard &&)      = delete;

	~Guard() {
		try {
			if (func) func();
		} catch (...) {}
	}

	void cancel() { func = nullptr; }
};

} // namespace alp
#endif