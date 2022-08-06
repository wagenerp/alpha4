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

#ifndef ALPHA4C_COMMON_STRINGBUILDER_H
#define ALPHA4C_COMMON_STRINGBUILDER_H
#include <stdlib.h>

typedef struct sbuilder_t sbuilder_t;

sbuilder_t *sbuilder_new();
void sbuilder_sprintf(sbuilder_t *sbuilder, const char *fmt, ...);
void sbuilder_append(sbuilder_t *sbuilder, const char *str);
size_t sbuilder_size(sbuilder_t *sbuilder);
char *sbuilder_str(sbuilder_t *sbuilder);
void sbuilder_free(sbuilder_t *sbuilder);

#endif