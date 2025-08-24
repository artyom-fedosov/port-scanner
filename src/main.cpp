#include <iostream>

#include "../include/scanner.hpp"
#include "../include/printer.hpp"

int main(int argc, char *argv[]) {
        if (argc < 3) [[unlikely]] {
                std::cerr << "Usage: " << argv[0] << " <ip> <port1> [port2]...\n";
                return 1;
        }

        try {
                ipaddr_t const ip {argv[1]};
                std::vector<char const *> const ports (&argv[2], &argv[argc]);
                Scanner const scanner {ip, ports};
                Printer::print(scanner.scan());
        } catch (std::length_error &e) {
                std::cerr << "Length error: " << e.what() << '\n';
                return 2;
        } catch (std::bad_alloc &e) {
                std::cerr << "Memory allocation failed: " << e.what() << '\n';
                return 3;
        } catch (std::invalid_argument &e) {
                std::cerr << "Invalid argument: " << e.what() << '\n';
                return 4;
        } catch (std::system_error &e) {
                std::cerr << "System error: " << e.what() << '\n';
                return 5;
        } catch (...) {
                std::cerr << "Unexpected error occurred.\n";
                return -1;
        }
}
