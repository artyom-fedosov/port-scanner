#include <iostream>

#include "../include/scanner.hpp"
#include "../include/printer.hpp"

int main(int argc, char *argv[]) {
	 if (argc < 3) [[unlikely]] {
                std::cerr << "Arguments count must be >= 3.\n";
                return 1;
        }

        ipaddr_t ip {argv[1]};
        std::vector<char const *> ports (&argv[2], &argv[argc]);
        try {
                Scanner scanner {ip, ports};
                Printer::print(scanner.scan());
        } catch (std::exception &e) {
                std::cerr << e.what() << '\n';
                return 2;
        }
}
