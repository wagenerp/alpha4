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

#include <cstdarg>
#include <cstdio>
extern "C" {
#include "stringbuilder.h"
}
#include <cstring>
#include <string>

extern "C" {
typedef struct sbuilder_t {
  std::string str;
  size_t cbUsed = 0;

  void reserve(size_t cb) { str.resize(std::max(cbUsed + cb, str.size()), 0); }
} sbuilder_t;

sbuilder_t *sbuilder_new() { return new sbuilder_t; }

void sbuilder_sprintf(sbuilder_t *sbuilder, const char *fmt, ...) {
  va_list args, args2;
  va_start(args, fmt);

  va_copy(args2, args);

  int ccRequired = vsnprintf(nullptr, 0, fmt, args2);
  va_end(args2);
  if (ccRequired > 0) {
    sbuilder->reserve(ccRequired + 1);

    int cc = vsnprintf(sbuilder->str.data() + sbuilder->cbUsed, ccRequired + 1,
                       fmt, args);

    if (cc > 0) {
      sbuilder->cbUsed += cc;
      sbuilder->str[sbuilder->cbUsed] = 0;
    }
  }
  va_end(args);
}
void sbuilder_append(sbuilder_t *sbuilder, const char *str) {
  size_t cc = std::strlen(str);
  sbuilder->reserve(cc + 1);
  strcat(sbuilder->str.data() + sbuilder->cbUsed, str);
  sbuilder->cbUsed += cc;
}

size_t sbuilder_size(sbuilder_t *sbuilder) { return sbuilder->cbUsed; }
char *sbuilder_str(sbuilder_t *sbuilder) { return sbuilder->str.data(); }

void sbuilder_free(sbuilder_t *sbuilder) { delete sbuilder; }
}