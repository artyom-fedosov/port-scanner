#include "../include/printer.hpp"

#include <iostream>

#include <unistd.h>

[[nodiscard]] bool Printer::isTerminal() noexcept {
        return isatty(fileno(stdout));
}

void Printer::print(const std::vector<std::pair<port_t, bool>> &ports)
                noexcept {
        bool const isTerm {isTerminal()};
        for (auto const &port : ports) {
                if (isTerm)
                        std::cout << (port.second ? GREEN : RED);
                std::cout << port.first << "\tis" <<
                        (port.second ? " " : " not ") << "accessible";
                if (isTerm)
                        std::cout << RESET << '\n';
        }
}
