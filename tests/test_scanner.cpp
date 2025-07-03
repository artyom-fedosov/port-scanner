#include <cassert>
#include <iostream>

#include "../include/scanner.hpp"

void test_isIPv4() {
        assert(isIPv4("0.0.0.0"));
        assert(isIPv4("255.255.255.255"));
        assert(isIPv4("192.0.2.146"));
        assert(isIPv4("127.0.0.1"));
        assert(isIPv4("8.8.8.8"));
        assert(isIPv4("1.1.1.1"));

        assert(!isIPv4("256.256.256.256"));
        assert(!isIPv4("192.168.1"));
        assert(!isIPv4("192.168.1.1.1"));
        assert(!isIPv4("192.168.1.one"));
        assert(!isIPv4(""));
        assert(!isIPv4("::1"));
        assert(!isIPv4("1234"));

        std::cout << "test_isIPv4 passed\n";
}

void test_isIPv6() {
        assert(isIPv6("::1"));
        assert(isIPv6("2001:0db8:85a3:0000:0000:8a2e:0370:7334"));
        assert(isIPv6("fe80::1"));
        assert(isIPv6("::ffff:192.0.2.128"));
        assert(isIPv6("::"));

        assert(!isIPv6("127.0.0.1"));
        assert(!isIPv6("2001:::7334"));
        assert(!isIPv6("invalid"));
        assert(!isIPv6(""));
        assert(!isIPv6("1.1.1.1.1"));

        std::cout << "test_isIPv6 passed\n";
}

void test_parsePort() {
        port_t port;

        assert(parsePort("0", port) && port == 0);
        assert(parsePort("1", port) && port == 1);
        assert(parsePort("80", port) && port == 80);
        assert(parsePort("443", port) && port == 443);
        assert(parsePort("65535", port) && port == 65535);

        assert(!parsePort("-1", port));
        assert(!parsePort("65536", port));
        assert(!parsePort("70000", port));
        assert(!parsePort("abc", port));
        assert(!parsePort("22abc", port));
        assert(!parsePort("abc22", port));
        assert(!parsePort("", port));
        assert(!parsePort(" ", port));

        std::cout << "test_parsePort passed\n";
}

int main() {
        test_isIPv4();
        test_isIPv6();
        test_parsePort();

        std::cout << "All tests passed!" << std::endl;
}
