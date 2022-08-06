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

#include "alpha4/common/cli.hpp"

#include "alpha4/common/logger.hpp"
#include "alpha4/common/string.hpp"
#include <iostream>
namespace alp {
void CLI::printHelp(std::ostream &os) {
	if (!synopsis.empty()) { os << synopsis << "\n"; }

	if (!description.empty()) {
		if (!synopsis.empty()) os << "  ";
		os << description << "\n";
	}

	os << "Options:\n";

	for (const auto &sw : switches) {
		if ((sw.shortName != 0) & !sw.longName.empty()) {
			os << "  -" << sw.shortName << "|--" << sw.longName << "\n";
		} else if ((sw.shortName != 0)) {
			os << "  -" << sw.shortName << "\n";

		} else if (!sw.longName.empty()) {
			os << "  --" << sw.longName << "\n";
		} else {
			continue;
		}
		if (!sw.description.empty()) { os << "    " << sw.description << "\n"; }
	}
}

bool CLI::process(int argn, char **argv) {
	CommandLine cl{argv, argn, 1};
	bool        verbatim = false;
	try {
		for (; cl.argi < cl.argn;) {
			auto arg = cl.argv[cl.argi++];
			if (!verbatim & (arg[0] == '-')) {
				if (arg[1] == '-') {
					arg += 2;
					if (0 == *arg) {
						verbatim = true;
						continue;
					}
					for (const auto &sw : switches) {
						if (sw.longName == arg) {
							cl.newContext() << "switch --" << sw.longName;
							sw.cmd.parse(cl);
						}
					}
				} else {
					for (auto pshortName = arg + 1; *pshortName; pshortName++) {
						for (const auto &sw : switches) {
							if (sw.shortName == *pshortName) {
								cl.newContext() << "switch -" << sw.shortName;
								sw.cmd.parse(cl);
							}
						}
					}
				}
				continue;
			}
		}
	} catch (const CLEX &e) {
		printHelp(std::cerr);
		std::cerr.flush();
		LOG(E) << e.what() << alp::over;

		return false;

	} catch (const std::exception &e) {
		LOG(E) << "fatal: " << e.what() << alp::over;
		return false;
	} catch (...) {
		LOG(E) << "fatal error (unknown)" << alp::over;
		return false;
	}
	return true;
}
} // namespace alp