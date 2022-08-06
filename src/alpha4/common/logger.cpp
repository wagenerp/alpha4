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

#include "logger.hpp"

#include <memory>
namespace alp {

std::shared_ptr<Logger> Logger::GetPointer() {
	static std::shared_ptr<Logger> inst = nullptr;
	if (!inst) inst = std::make_shared<Logger>();
	return inst;
}
void Logger::emit(const Message &msg) {
	// if (!msg.done()) return;
	const auto dtCompose =
		msg.done() ? std::chrono::duration_cast<std::chrono::duration<double>>(
									 msg.tEmit - msg.tBirth)
									 .count()
							 : 0.0;

	std::cerr << Format(msg.tEmit) << " [" << std::setprecision(2) << std::fixed
						<< std::chrono::duration_cast<std::chrono::duration<double>>(
								 msg.tEmit - tBirth)
								 .count()
						<< "s] " << Colored(msg.type) << " ";

	if (dtCompose > -0.05) std::cerr << "(took " << dtCompose << "s) ";
	std::cerr << msg.msg.str() << "\n";
}
} // namespace alp
