#pragma once

#include <string_view>

#include "types.hpp"

namespace {
        /*!
        * \brief ANSI escape code for resetting terminal colors.
        */
        std::string_view constexpr RESET {"\033[0m"};

        /*!
        * \brief ANSI escape code for red text color.
        */
        std::string_view constexpr RED {"\033[31m"};

        /*!
        * \brief ANSI escape code for green text color.
        */
        std::string_view constexpr GREEN {"\033[32m"};
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
        /*!
        * \brief Checks if stdout is connected to a terminal.
        *
        * Exception safety: No-throw guarantee.
        *
        * \return true if output is a terminal, false otherwise.
        */
        [[nodiscard]] static bool isTerminal() noexcept;

        /*!
        * \brief Prints to stdout the formated information about the ports
        * accessibility.
        *
        * Exception safety: Basic guarantee.
        *
        * \throws std::bad_alloc If memory allocation for internal storage
        * fails.
        * \throws std::system_error If writing to stdout fails.
        */
        static void print(std::vector<std::pair<port_t, bool>> const &ports);
};
