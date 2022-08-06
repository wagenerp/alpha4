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

#ifndef ALPHAC_TYPES_VECTOR_H
#define ALPHAC_TYPES_VECTOR_H
#include "alpha4c/common/inline.h"
#include <math.h>

typedef struct vec3f_t {
	float x;
	float y;
	float z;
} vec3f_t;

ALPHA4C_INLINE(vec3f_t vec3f_set)(float x, float y, float z) {
	vec3f_t res = {x, y, z};
	return res;
}

ALPHA4C_INLINE(vec3f_t vec3f_add)(const vec3f_t *a, const vec3f_t *b) {
	vec3f_t res = {a->x + b->x, a->y + b->y, a->z + b->z};
	return res;
}

ALPHA4C_INLINE(vec3f_t vec3f_sub)(const vec3f_t *a, const vec3f_t *b) {
	vec3f_t res = {a->x - b->x, a->y - b->y, a->z - b->z};
	return res;
}

ALPHA4C_INLINE(float vec3f_dot)(const vec3f_t *a, const vec3f_t *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}
ALPHA4C_INLINE(vec3f_t vec3f_mul)(const vec3f_t *a, float f) {
	vec3f_t res = {a->x * f, a->y * f, a->z * f};
	return res;
}
ALPHA4C_INLINE(vec3f_t vec3f_div)(const vec3f_t *a, float f) {
	vec3f_t res = {a->x / f, a->y / f, a->z / f};
	return res;
}

ALPHA4C_INLINE(float vec3f_square)(const vec3f_t *a) {
	return a->x * a->x + a->y * a->y + a->z * a->z;
}
ALPHA4C_INLINE(float vec3f_norm)(const vec3f_t *a) {
	return sqrt(vec3f_square(a));
}

ALPHA4C_INLINE(vec3f_t vec3f_normal)(const vec3f_t *a) {
	return vec3f_div(a, sqrt(vec3f_square(a)));
}

#endif