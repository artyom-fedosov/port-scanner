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

Scanner::Scanner(ipaddr_t const &ip, std::vector<char const *> const &ports)
                : ip_ {ip}, ports_ {}, mtx_ {} {
        if (not isIPv4(ip_) and not isIPv6(ip_))
                throw std::invalid_argument {"IP address is not valid!"};

        ports_.reserve(ports.size());
        for (auto const prt : ports) {
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
        for (auto const port : ports_) {
                threads.push_back(std::thread {[this, port, &result]() {
                        bool const isAccessible {isPortAccessible(port)};
                        mtx_.lock();
                        result.push_back(std::pair {port, isAccessible});
                        mtx_.unlock();
                }});
        }

        for (auto &thread : threads)
                thread.join();

        return result;
}

[[nodiscard]] bool Scanner::isIPv4(ipaddr_t const &ip) const noexcept {
        in_addr address;
        return inet_pton(AF_INET, ip.c_str(), &address) == 1;
}

[[nodiscard]] bool Scanner::isIPv6(ipaddr_t const &ip) const noexcept {
        in6_addr address6;
        return inet_pton(AF_INET6, ip.c_str(), &address6) == 1;
}

[[nodiscard]] std::optional<port_t>
Scanner::parsePort(char const *str) const noexcept {
        int prt;
        char const *strEnd = str + std::strlen(str);
        std::from_chars_result const res = std::from_chars(str, strEnd, prt);

        if (res.ec not_eq std::errc() or prt < MIN_PORT or prt > MAX_PORT or
            res.ptr not_eq strEnd)
                return {};

        return {static_cast<port_t>(prt)};
}

[[nodiscard]] bool Scanner::isPortAccessible(port_t const port) const noexcept {
        addrinfo hints {}, *res = nullptr;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        std::string portStr = std::to_string(port);
        if (getaddrinfo(ip_.c_str(), portStr.c_str(), &hints, &res) not_eq 0 or
                        not res)
                return false;

        int const sockfd = socket(res->ai_family, SOCK_STREAM, 0);
        if (sockfd == -1) {
                freeaddrinfo(res);
                return false;
        }

        timeval constexpr timeout {TIMEOUT_MS / 1000,
                (TIMEOUT_MS % 1000) * 1000l};
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

        int const result = connect(sockfd, res->ai_addr, res->ai_addrlen);
        freeaddrinfo(res);
        close(sockfd);

        return result == 0;
}
