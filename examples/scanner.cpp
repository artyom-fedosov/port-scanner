#include <algorithm>
#include <iostream>

#include "scanner.hpp"

int main() {
    try {
        ipaddr_t const ip{"192.168.8.1"};
        std::vector<char const *> const ports{"80", "443", "40"};
        Scanner const scanner{ip, ports};

        std::vector<std::pair<port_t, bool>> result{scanner.scan()};
        std::for_each(result.cbegin(), result.cend(), [](std::pair<port_t, bool> const &pair) {
            std::cout << pair.first << '\t' << pair.second << '\n';
        });
    } catch (std::length_error &e) {
        std::cerr << "Length error: " << e.what() << '\n';
        return 2;
    } catch (std::bad_alloc &e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';
        return 3;
    } catch (std::invalid_argument &e) {
        std::cerr << "Invalid argument: " << e.what() << '\n';
        return 4;
    } catch (...) {
        std::cerr << "Unexpected error occurred.\n";
        return -1;
    }
}
