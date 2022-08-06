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

#include "linescanner.h"

#include "alpha4/common/linescanner.hpp"

#include <cstring>
#include <vector>

extern "C" {
typedef struct lscan_t {
	alp::LineScanner ln;
} lscan_t;

lscan_t *lscan_new(const char *ptr, const char *end) {
	lscan_t *lscan = (lscan_t *)malloc(sizeof(lscan_t));

	new (&lscan->ln)
		alp::LineScanner(ptr, (nullptr == end) ? (ptr + std::strlen(ptr)) : end);

	return lscan;
}

void lscan_free(lscan_t *lscan) {
	lscan->ln.~LineScanner();
	free((void *)lscan);
}

char *lscan_str(lscan_t *lscan, int mode, int flags) {
	std::string_view res;
	bool             success = false;

	switch (mode) {
		case LSCAN_MANY: success = lscan->ln.getString(res); break;
		case LSCAN_MINLINE: success = lscan->ln.getLnString(res); break;
		case LSCAN_MFIRST: success = lscan->ln.getLnFirstString(res); break;
		case LSCAN_MREMAINDER: success = lscan->ln.getLnRemainder(res); break;
		case LSCAN_MLINE:
			success = lscan->ln.getTrimmedLine(res, 0 != (flags & LSCAN_FREWIND));
			break;
		default: return nullptr;
	}

	if (!success) return nullptr;

	if (res.size() < 1) return nullptr;

	char *res_c = (char *)malloc(res.size() + 1);
	memcpy(res_c, res.data(), res.size());
	res_c[res.size()] = 0;

	return res_c;
}

int lscan_seekNewline(lscan_t *lscan, int alwaysAdvance) {
	return lscan->ln.seekNewLine(alwaysAdvance != 0) ? 1 : 0;
}
size_t lscan_tell(lscan_t *lscan) { return lscan->ln.tell(); }
size_t lscan_seek(lscan_t *lscan, int offset, int whence) {
	return lscan->ln.seek(offset, whence);
}
int lscan_eof(lscan_t *lscan) { return lscan->ln.eof(); }
int lscan_newline(lscan_t *lscan) { return lscan->ln.isNewLine(); }
}