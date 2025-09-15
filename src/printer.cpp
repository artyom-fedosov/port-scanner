#include "../include/printer.hpp"

#include <print>

#include <unistd.h>

[[nodiscard]] bool Printer::isTerminal() noexcept {
    return isatty(fileno(stdout));
}

void Printer::print(const std::vector<std::pair<port_t, bool>> &ports) {
    bool const isTerm{isTerminal()};

    for (auto const &port : ports) {
        if (isTerm) [[likely]] {
            std::println("{}{}\tis{}accessible{}", port.second ? GREEN : RED, port.first,
                         port.second ? " " : " not ", RESET);
        } else [[unlikely]] {
            std::println("{}\tis{}accessible", port.first, port.second ? " " : " not ");
        }
    }
}
