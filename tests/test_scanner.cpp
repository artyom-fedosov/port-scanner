#include <gtest/gtest.h>

#include "../include/scanner.hpp"

TEST(ScannerTest, ValidIPv4Address) {
    std::array<ipaddr_t, 5> arr{"127.0.0.1", "192.168.8.10", "132.89.0.12", "90.67.12.123",
                                "97.30.231.251"};

    for (auto const &el : arr) {
        EXPECT_NO_THROW(Scanner scanner(el, std::vector<char const *>{}));
    }
}

TEST(ScannerTest, ValidIPv6Address) {
    std::array<ipaddr_t, 5> arr{"::1", "2001:0db8:85a3:0000:0000:8a2e:0370:7334",
                                "fd12:3456:789a:0001::abcd", "2a02:6b8:0:1::1234",
                                "fe80::0202:b3ff:fe1e:8329"};

    for (auto const &el : arr) {
        EXPECT_NO_THROW(Scanner scanner(el, std::vector<char const *>{}));
    }
}

TEST(ScannerTest, InvalidIPAddress) {
    std::array<ipaddr_t, 5> arr{"invalid_ip", "", "112345.1235.12345.1235", "256.256.256.256",
                                "255.255.255.-1"};

    for (auto const &el : arr) {
        EXPECT_THROW(Scanner scanner(el, std::vector<char const *>{}), std::invalid_argument);
    }
}

TEST(ScannerTest, ValidPort) {
    std::vector<char const *> vec{"80", "53", "10", "0", "65535", "123"};

    EXPECT_NO_THROW(Scanner scanner("127.0.0.1", vec););
}

TEST(ScannerTest, InvalidPort) {
    EXPECT_THROW(Scanner scanner("127.0.0.1", std::vector{"70000"}), std::invalid_argument);

    EXPECT_THROW(Scanner scanner("127.0.0.1", std::vector{"-1"}), std::invalid_argument);

    EXPECT_THROW(Scanner scanner("127.0.0.1", std::vector{"65536"}), std::invalid_argument);

    EXPECT_THROW(Scanner scanner("127.0.0.1", std::vector{"-234"}), std::invalid_argument);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
