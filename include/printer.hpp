#pragma once

#include "types.hpp"

namespace {
        /**
        * \brief ANSI escape code for resetting terminal colors.
        */
        static char constexpr RESET[] = "\033[0m";

        /**
        * \brief ANSI escape code for red text color.
        */
        static char constexpr RED[] = "\033[31m";

        /**
        * \brief ANSI escape code for green text color.
        */
        static char constexpr GREEN[] = "\033[32m";
}

/*!
 * \class Printer
 *
 * \brief A utility class for printing formatted output to the terminal.
 *
 * The Printer class provides static methods to check if the output is a terminal
 * and to print port status information with ANSI color formatting.
 */
class Printer final {
public:
        /**
        * \brief Checks if stdout is connected to a terminal.
        *
        * \return true if output is a terminal, false otherwise.
        */
        [[nodiscard]] static bool isTerminal() noexcept;

        static void print(std::vector<std::pair<port_t, bool>> const &ports)
                noexcept;
};
