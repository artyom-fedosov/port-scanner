#include "../include/scanner.hpp"

#include <iostream>
#include <sstream>

#include <unistd.h>

/**
 * \brief ANSI escape code for resetting terminal colors.
 */
char constexpr RESET[] = "\033[0m";

/**
 * \brief ANSI escape code for red text color.
 */
char constexpr RED[] = "\033[31m";

/**
 * \brief ANSI escape code for green text color.
 */
char constexpr GREEN[] = "\033[32m";

/**
 * \brief Checks if stdout is connected to a terminal.
 *
 * \return true if output is a terminal, false otherwise.
 */
[[nodiscard]] bool isTerminal() {
        return isatty(fileno(stdout));
}

int main(int argc, char *argv[]) {
        if (argc < 3) {
                std::cerr << "Arguments count must be >= 3.\n";
                return 1;
        }

        std::vector<char const *> ports {};
        ports.reserve(static_cast<size_t>(argc) - 2);
        for (int i {2}; i < argc; ++i)
                ports.push_back(argv[i]);

        try {
                Scanner scanner {argv[1], ports};
                std::vector<std::pair<port_t, bool>> results {scanner.scan()};
                bool const isTerminalFlag {isTerminal()};

                for (auto const &result : results) {
                        std::stringstream ss {};

                        if (isTerminalFlag) {
                                ss << (result.second ? GREEN : RED) <<
                                        result.first << "\tis" <<
                                        (result.second ? " " : " not ") <<
                                        "accessible" << RESET << '\n';
                        }
                        else {
                                ss << result.first << "\tis" <<
                                        (result.second ? " " : " not ") <<
                                        "accessible\n";
                        }

                        std::cout << ss.rdbuf();
                }
        } catch (std::exception &e) {
                std::cerr << e.what() << '\n';
                return 2;
        }
}
