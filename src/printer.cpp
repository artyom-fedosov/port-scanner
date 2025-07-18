#include "../include/printer.hpp"

#include <sstream>
#include <iostream>

#include <unistd.h>

[[nodiscard]] bool Printer::isTerminal() noexcept {
        return isatty(fileno(stdout));
}

void Printer::print(const std::vector<std::pair<port_t, bool>> &ports)
                noexcept {
        for (auto const &port : ports) {
                std::stringstream ss {};

                if (isTerminal()) {
                        ss << (port.second ? GREEN : RED) <<
                                port.first << "\tis" <<
                                (port.second ? " " : " not ") <<
                                "accessible" << RESET << '\n';
                }
                else {
                        ss << port.first << "\tis" <<
                                (port.second ? " " : " not ") <<
                                "accessible\n";
                }

                std::cout << ss.rdbuf();
        }
}
