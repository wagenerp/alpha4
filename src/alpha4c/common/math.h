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

#ifndef ALPHAC_COMMON_MATH_H
#define ALPHAC_COMMON_MATH_H
#include "alpha4c/common/inline.h"
#include <math.h>
#include <stdlib.h>

ALPHA4C_INLINE(float fclamp)(float v) {
	if (!(v > 0)) return 0;
	if (!(v < 1)) return 1;
	return v;
}

ALPHA4C_INLINE(float randf)() { return (float)rand() / (float)RAND_MAX; }

#endif