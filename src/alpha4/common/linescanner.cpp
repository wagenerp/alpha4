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

#include "linescanner.hpp"

#include <cstring>

namespace alp {

LineScanner::LineScanner() : _data(0), _p(0), _end(0), _newLine(1) {}

LineScanner::LineScanner(const char *data) : LineScanner() { assign(data); }
LineScanner::LineScanner(const char *data, size_t cb) : LineScanner() {
	assign(data, cb);
}
LineScanner::LineScanner(const char *data, const char *e) : LineScanner() {
	assign(data, e);
}
LineScanner::~LineScanner() {}

void LineScanner::assign(const char *data) {
	if (nullptr == data) {
		_p = _data = nullptr;
		_end       = nullptr;
		_newLine   = 1;
	} else {
		_data    = data;
		_p       = data;
		_end     = _data + strlen(data);
		_newLine = 1;
	}
}
void LineScanner::assign(const char *data, size_t cb) {
	_data    = data;
	_p       = data;
	_end     = _data + cb;
	_newLine = 1;
}
void LineScanner::assign(const char *data, const char *e) {
	_data    = data;
	_p       = data;
	_end     = e;
	_newLine = 1;
}

size_t LineScanner::tell() const { return (size_t)_p - (size_t)_data; }

size_t LineScanner::seek(int offset, int origin) {
	std::ptrdiff_t p;
	switch (origin) {
		default:
		case 0: p = offset; break;
		case 1: p = (std::ptrdiff_t)_p - (std::ptrdiff_t)_data + offset; break;
		case 2: p = (std::ptrdiff_t)_end - (std::ptrdiff_t)_data + offset; break;
	}
	if (p < 0)
		p = 0;
	else if (p > (std::ptrdiff_t)_end - (std::ptrdiff_t)_data)
		p = (std::ptrdiff_t)_end - (std::ptrdiff_t)_data;

	_p = _data + p;

	return (size_t)p;
}

bool LineScanner::seekNewLine(bool alwaysAdvance) {
	if (_newLine && (!alwaysAdvance)) return true;
	_newLine = 1;

	while ((_p < _end) && (*_p != '\n') && (*_p != '\r'))
		_p++;
	if (_p >= _end) return false;

	while ((_p < _end) && ((*_p == '\n') || (*_p == '\r')))
		_p++;
	if (_p >= _end) return false;

	return true;
}

bool LineScanner::getAnyString(std::string_view &res) {
	int  longstring = 0, esc = 0, doesc = flags & FUSE_ESCAPE;
	char quote;
	_newLine = 0;

	size_t      res_len = 0;
	const char *res_ptr = nullptr;

	while (_p < _end) {
		if (longstring) {
			if (esc)
				esc = 0;
			else if (doesc && (*_p == '\\')) {
				esc = true;
			} else if (*_p == quote) {
				res_len = (size_t)_p - (size_t)res_ptr;
				_p++;
				res = {res_ptr, res_len};
				return true;
			}
			_p++;
		} else if (*_p > ' ') {
			if (!res_ptr) {
				res_ptr = _p;
				if ((*_p == '"') || (*_p == '\'')) {
					longstring = 1;
					quote      = *_p;
					res_ptr++;
				}
			}
			_p++;
		} else {
			if (res_ptr) res_len = (size_t)_p - (size_t)res_ptr;

			if ((_p < _end) && ((*_p == '\n') || (*_p == '\r'))) {
				if (res_ptr) _newLine = 1;
				while ((_p < _end) && ((*_p == '\n') || (*_p == '\r')))
					_p++;
			} else {
				_p++;
			}

			res = {res_ptr, res_len};
			if (res_ptr) return true;
		}
	}
	if (res_ptr) {
		res_len = (size_t)_end - (size_t)res_ptr;
		res     = {res_ptr, res_len};
		return true;
	}
	return false;
}

bool LineScanner::getLnString(std::string_view &res) {
	if (_newLine) return false;
	while ((_p < _end) && (*_p <= ' ')) {
		if ((*_p == '\n') || (*_p == '\r')) {
			while ((_p < _end) && ((*_p == '\n') || (*_p == '\r')))
				_p++;
			_newLine = 1;
			return false;
		}
		_p++;
	}

	return getString(res);
}

bool LineScanner::getLnFirstString(std::string_view &res) {
	if (!_newLine && !seekNewLine(0)) { return false; }

	return getString(res);
}

bool LineScanner::getLnRemainder(std::string_view &res) {
	if (_p >= _end) { return false; }
	size_t      res_len = 0;
	const char *res_ptr = _p;

	while ((_p < _end) && (*_p != '\r') && (*_p != '\n')) {
		_p++;
	}
	res_len = (size_t)_p - (size_t)res_ptr;
	res     = {res_ptr, res_len};
	return true;
}

bool LineScanner::getTrimmedLine(std::string_view &res, bool rewind) {
	const char *p0;
	const char *p, *f, *l;
	if (rewind) {
		for (p0 = _p - 1; (p0 >= _data) && (*p0 <= ' '); p0--)
			;
		if (p0 < _data) return false;
	} else {
		for (p0 = _p; (p0 < _end) && (*p0 <= ' '); p0++)
			;
		if (p0 >= _end) return false;
	}

	f = l = p0;
	for (p = p0; (p >= _data) && (*p != '\n') && (*p != '\r'); p--)
		if (*p > ' ') f = p;
	for (p = p0; (p < _end) && (*p != '\n') && (*p != '\r'); p++)
		if (*p > ' ') l = p;

	res = {f, (size_t)(l - f + 1)};

	return true;
}

void MultilineScanner::processLine(const std::string &line) {
	char quote  = 0;
	bool escape = false;
	bool empty  = true;
	if (line.empty()) {
		flush();
		return;
	}
	bool isContinuation = useContinuation & (line.back() == '\\');

	size_t end = isContinuation ? line.size() - 1 : line.size();
	for (size_t i = 0; i < end; i++) {
		const char c = line[i];

		if (quote != 0) {
			if (escape) {
				escape = false;
			} else if (c == '\\') {
				escape = true;
			} else if (c == quote) {
				quote = 0;
			}
		} else if (c == '#') {
			end = i;
			break;
		} else {
			if ((c != ' ') & (c != '\t')) empty = false;
			if ((c == '\'') | (c == '"')) quote = c;
		}
	}

	if (empty) {
		if (!isContinuation) flush();
		return;
	}

	if (_compound.empty() && !isContinuation)
		callback(line.substr(0, end));
	else {
		_compound += delimiter + line.substr(0, end);
		if (!isContinuation) flush();
	}
}

void MultilineScanner::flush() {
	if (_compound.empty()) return;
	callback(_compound);
	_compound.clear();
}

} // namespace alp