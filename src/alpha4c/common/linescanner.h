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

#ifndef ALPHA4C_COMMON_LINESCANNER_H
#define ALPHA4C_COMMON_LINESCANNER_H
#ifdef __cplusplus
extern "C" {
#endif
#include "inline.h"
#include <stdlib.h>

typedef struct lscan_t lscan_t;

#define LSCAN_MANY 0
#define LSCAN_MINLINE 1
#define LSCAN_MFIRST 2
#define LSCAN_MREMAINDER 3
#define LSCAN_MLINE 4
#define LSCAN_FESCAPE 0x01
#define LSCAN_FREWIND 0x02

lscan_t *lscan_new(const char *ptr, const char *end);
void     lscan_free(lscan_t *lscan);
char *   lscan_str(lscan_t *lscan, int mode, int flags);
int      lscan_seekNewline(lscan_t *lscan, int alwaysAdvance);
size_t   lscan_tell(lscan_t *lscan);
size_t   lscan_seek(lscan_t *lscan, int offset, int whence);
int      lscan_eof(lscan_t *lscan);
int      lscan_newline(lscan_t *lscan);

#define LSCAN_SHORTHAND(ident, type, decode)                               \
	ALPHA4C_INLINE(int lscan_##ident)(lscan_t * lscan, type * v, int mode) { \
		char *raw = lscan_str(lscan, mode, 0);                                 \
		if (!raw) return 0;                                                    \
		int success = 1;                                                       \
		{decode} free(raw);                                                    \
		return success;                                                        \
	}

#define LSCAN_SHORTHAND_CONV(ident, type, intermediate, decode) \
	LSCAN_SHORTHAND(ident, type, {                                \
		intermediate w;                                             \
		{decode};                                                   \
		*v = (type)w;                                               \
		if ((intermediate)(*v) != w) success = 0;                   \
	})

LSCAN_SHORTHAND(double, double, *v = atof(raw);)
LSCAN_SHORTHAND_CONV(float, float, double, w = atof(raw);)
LSCAN_SHORTHAND(int, int, *v = atoi(raw);)
LSCAN_SHORTHAND(long, long, *v = atol(raw);)
LSCAN_SHORTHAND(longlong, long long, *v = atoll(raw);)
LSCAN_SHORTHAND_CONV(uint, unsigned int, int, w = atoi(raw);)
LSCAN_SHORTHAND_CONV(ulong, unsigned long, long, w = atol(raw);)
LSCAN_SHORTHAND_CONV(ulonglong, unsigned long long, long long, w = atoll(raw);)

#undef LSCAN_SHORTHAND_CONV
#undef LSCAN_SHORTHAND
#ifdef __cplusplus
}
#endif
#endif