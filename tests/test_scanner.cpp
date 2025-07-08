#include <cassert>
#include <iostream>

#include "../include/scanner.hpp"

void test_valid_ipv4_and_ipv6() {
        Scanner scannerIPv4("127.0.0.1", {"80", "443"});
        Scanner scannerIPv6("::1", {"80"});

        bool threw = false;
        try {
                Scanner invalidScanner("invalid_ip", {"80"});
        } catch (const std::invalid_argument &) {
                threw = true;
        }
        assert(threw);
}

void test_invalid_ports_in_constructor() {
        bool threw = false;
        try {
                Scanner scanner("127.0.0.1", {"80", "70000"});
        } catch (const std::invalid_argument &) {
                threw = true;
        }
        assert(threw);

        threw = false;
        try {
                Scanner scanner("127.0.0.1", {"abc"});
        } catch (const std::invalid_argument &) {
                threw = true;
        }
        assert(threw);
}

void test_scan_known_ports() {
        Scanner scanner("127.0.0.1", {"22", "80", "65535"});

        auto results = scanner.scan();
        assert(results.size() == 3);

        for (const auto &res : results) {
                assert(res.first == 22 || res.first == 80 ||
                       res.first == 65535);
                assert(res.second == true || res.second == false);
        }
}

int main() {
        test_valid_ipv4_and_ipv6();
        test_invalid_ports_in_constructor();
        test_scan_known_ports();

        std::cout << "All Scanner tests passed!\n";
}

