#include "../include/scanner.hpp"

#include <iostream>
#include <thread>
#include <sstream>

int main(int argc, char **argv) {
        if (argc < 3) {
                std::cerr << "Arguments count must be >= 3.\n";
                return 1;
        }

        if (!isIPv4(argv[1]) and !isIPv6(argv[1])) {
                std::cerr << "IP address is not valid!\n";
                return 2;
        }
        const ipaddr_t ip {argv[1]};

        ports_t ports {};
        for (int i {2}; i < argc; ++i) {
                const std::optional<port_t> port {parsePort(argv[i])};
                if (port)
                        ports.push_back(port.value());
                else {
                        std::cerr << "At least one of the inputted ports is not valid!\n";
                        return 3;
                }
        }

        std::vector<std::thread> threads {};
        const bool isTerminalFlag {isTerminal()};
        for (const auto port : ports) {
                threads.push_back(std::thread {[&ip, port, isTerminalFlag]() {
                        const bool isAccessible {isPortAccessible(ip, port)};
                        std::stringstream ss {};

                        if (isTerminalFlag)
                                ss << (isAccessible ? GREEN : RED);
                        ss << port << "\tis" << (isAccessible ? " " : " not ") << "accessible";
                        if (isTerminalFlag)
                                ss << RESET;
                        ss << '\n';

                        std::lock_guard<std::mutex> lock {mtx};
                        std::cout << ss.rdbuf();
                }});
        }

        for (auto &thread : threads)
                thread.join();
}
