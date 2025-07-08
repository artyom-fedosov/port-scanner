#include "../include/scanner.hpp"

#include <charconv>
#include <stdexcept>
#include <thread>

#include <cstring>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>

Scanner::Scanner(const ipaddr_t &ip, const std::vector<const char *> &ports)
                : ip_ {ip}, ports_ {}, mtx_ {} {
        if (!isIPv4(ip_) and !isIPv6(ip_))
                throw std::invalid_argument {"IP address is not valid!"};

        for (const auto prt : ports) {
                std::optional<port_t> port {parsePort(prt)};
                if (port)
                        ports_.push_back(port.value());
                else
                        throw std::invalid_argument {std::string {prt} +
                                " is not valid port!"};
        }
}

[[nodiscard]] std::vector<std::pair<port_t, bool>>
Scanner::scan() noexcept {
        std::vector<std::pair<port_t, bool>> result {};
        std::vector<std::thread> threads {};
        for (const auto port : ports_) {
                threads.push_back(std::thread {[this, port, &result]() {
                        const bool isAccessible {isPortAccessible(port)};
                        mtx_.lock();
                        result.push_back(std::pair {port, isAccessible});
                        mtx_.unlock();
                }});
        }

        for (auto &thread : threads)
                thread.join();

        return result;
}

[[nodiscard]] bool Scanner::isIPv4(const ipaddr_t &ip) const noexcept {
        in_addr address;
        return inet_pton(AF_INET, ip.c_str(), &address) == 1;
}

[[nodiscard]] bool Scanner::isIPv6(const ipaddr_t &ip) const noexcept {
        in6_addr address6;
        return inet_pton(AF_INET6, ip.c_str(), &address6) == 1;
}

[[nodiscard]] std::optional<port_t>
Scanner::parsePort(const char *str) const noexcept {
        int prt;
        const char *strEnd = str + std::strlen(str);
        const std::from_chars_result res = std::from_chars(str, strEnd, prt);

        if (res.ec != std::errc() or prt < MIN_PORT or prt > MAX_PORT or
            res.ptr != strEnd)
                return {};

        return {static_cast<port_t>(prt)};
}

[[nodiscard]] bool Scanner::isPortAccessible(const port_t port) const noexcept {
        addrinfo hints {}, *res = nullptr;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        std::string portStr = std::to_string(port);
        if (getaddrinfo(ip_.c_str(), portStr.c_str(), &hints, &res) != 0 ||
                        !res)
                return false;

        const int sockfd = socket(res->ai_family, SOCK_STREAM, 0);
        if (sockfd == -1) {
                freeaddrinfo(res);
                return false;
        }

        constexpr timeval timeout {TIMEOUT_MS / 1000,
                (TIMEOUT_MS % 1000) * 1000l};
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

        const int result = connect(sockfd, res->ai_addr, res->ai_addrlen);
        freeaddrinfo(res);
        close(sockfd);

        return result == 0;
}
