#include <iostream>

#include "../include/scanner.hpp"
#include "../include/printer.hpp"

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
                Printer::print(scanner.scan());
        } catch (std::exception &e) {
                std::cerr << e.what() << '\n';
                return 2;
        }
}
