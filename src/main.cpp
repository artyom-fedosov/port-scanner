#include "../include/scanner.hpp"

#include <iostream>
#include <thread>

int main(int argc, char **argv) {
        if (argc < 3) {
                std::cerr << "Arguments count must be >= 3.\n";
                return 1;
        }

        if (!isIPv4(argv[1]) and !isIPv6(argv[1])) {
                std::cerr << "IP address is not valid!\n";
                return 2;
        }
        ipaddr_t ip {argv[1]};

        ports_t ports {};
        port_t port {};
        for (int i {2}; i < argc; ++i) {
                if (parsePort(argv[i], port))
                        ports.push_back(port);
                else {
                        std::cerr << "At least one of the inputted ports is not valid!\n";
                        return 3;
                }
        }

        std::vector<std::thread> threads {};
        const bool isTerminalFlag {isTerminal()};
        for (const auto port : ports) {
                threads.push_back(std::thread {[&ip, port, isTerminalFlag]() {
                        bool isAccessible {isPortAccessible(ip, port)};
                        std::lock_guard<std::mutex> lock {mtx};

                        if (isTerminalFlag)
                                std::cout << (isAccessible ? GREEN : RED);
                        std::cout << port << "\tis" << (isAccessible ? " " : " not ") << "accessible";
                        if (isTerminalFlag)
                                std::cout << RESET;
                        std::cout << '\n';
                }});
        }

        for (auto &thread : threads)
                thread.join();
}
