#include <gtest/gtest.h>

#include "../include/scanner.hpp"

TEST(ScannerTest, ValidIPv4Address) {
        EXPECT_NO_THROW(
                Scanner scanner ("127.0.0.1", std::vector<char const *> {});
        );
}

TEST(ScannerTest, ValidIPv6Address) {
        EXPECT_NO_THROW(
                Scanner scanner ("::1", std::vector<char const *> {});
        );
}

TEST(ScannerTest, InvalidIPAddress) {
        EXPECT_THROW(
                Scanner scanner("invalid_ip", std::vector<char const *> {}),
                     std::invalid_argument
        );
}

TEST(ScannerTest, ValidPort) {
        EXPECT_NO_THROW(
                Scanner scanner ("127.0.0.1",
                        std::vector<char const *> {"80", "53", "10"});
        );
}

TEST(ScannerTest, InvalidPort) {
        EXPECT_THROW(
                Scanner scanner("127.0.0.1",
                        std::vector<char const *> {"80", "70000"}),
                std::invalid_argument
        );

        EXPECT_THROW(
                Scanner scanner("abc",
                        std::vector<char const *> {"80", "70000"}),
                std::invalid_argument
        );
}

int main(int argc, char *argv[]) {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}
