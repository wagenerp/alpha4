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

#ifndef ALPHA4_COMMON_CLI_HPP
#define ALPHA4_COMMON_CLI_HPP
#include "alpha4/common/error.hpp"
#include "alpha4/common/string.hpp"

#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>
namespace alp {
class CLEX : public Exception {
public:
	using Exception::Exception;
};
struct CLI {
	struct CommandLine {
		char **argv;
		int    argn;
		int    argi;

		std::stringstream context = {};

		std::stringstream &newContext() {
			context = std::stringstream();
			return context;
		}

		template<typename T> std::remove_reference_t<T> decode() {
			if (argi >= argn) {
				thrower<CLEX>() << "missing argument(s) for " << context.str() << over;
			}
			std::decay_t<T> res;
			if (!decodeString(argv[argi], res)) {
				thrower<CLEX>() << "invalid argument " << argv[argi] << " for "
												<< context.str() << over;
			}
			return res;
		}
		template<int Index, typename First> constexpr auto decodeIndexed() {
			if (argi + Index >= argn) {
				thrower<CLEX>() << "missing argument(s) for " << context.str() << over;
			}
			return std::tuple<First>(decodeString<First>(argv[argi + Index]));
		}

		template<int Index, typename First, typename Next1, typename... NextN>
		constexpr auto decodeSequence() {
			return std::tuple_cat(
				decodeIndexed<Index, First>(),
				decodeSequence<Index + 1, Next1, NextN...>());
		}
		template<int Index, typename First> constexpr auto decodeSequence() {
			return decodeIndexed<Index, First>();
		}

		template<typename... Args> auto decodeSequence() {
			return decodeSequence<0, Args...>();
		}
	};
	struct Command {
		std::function<void(CommandLine &)> parse;

		template<typename F, typename... Args>
		Command(F target, void (F::*)(Args...) const) {
			parse = [target](CommandLine &cmdline) {
				if constexpr (sizeof...(Args) < 1) {
					target();
				} else {
          auto args = cmdline.decodeSequence<std::decay_t<Args>...>();
					std::apply(target, args);
				}
			};
		}

		template<typename F> Command(F target) : Command(target, &F::operator()) {}
	};
	struct Switch {
		char        shortName;
		std::string longName;
		std::string description;
		Command     cmd;
	};
	std::string          synopsis    = "";
	std::string          description = "";
	std::vector<Switch>  switches    = {};
	std::vector<Command> arguments   = {};

	void printHelp(std::ostream &os);
	bool process(int argn, char **argv);
};
} // namespace alp

#endif