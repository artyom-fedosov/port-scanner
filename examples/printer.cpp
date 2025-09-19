#include <iostream>

#include "printer.hpp"

int main() {
    std::cout << Printer::isTerminal() << '\n';

    try {
        std::vector<std::pair<port_t, bool>> ports{{80, true}, {443, false}};
        Printer::print(ports);
    } catch (std::bad_alloc &e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';
        return 1;
    } catch (std::system_error &e) {
        std::cerr << "System error: " << e.what() << '\n';
        return 2;
    } catch (...) {
        std::cerr << "Unexpected error occurred.\n";
        return -1;
    }
}
