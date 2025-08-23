#include "../include/scanner.hpp"

#include <charconv>

#include <cstring>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>

std::mutex mtx;

[[nodiscard]] bool isIPv4(const ipaddr_t &ip) {
        in_addr address;
        return inet_pton(AF_INET, ip.c_str(), &address) == 1;
}

[[nodiscard]] bool isIPv6(const ipaddr_t &ip) {
        in6_addr address6;
        return inet_pton(AF_INET6, ip.c_str(), &address6) == 1;
}

[[nodiscard]] bool parsePort(const char *str, port_t &port) {
        int prt;
        const char *strEnd = str + std::strlen(str);
        std::from_chars_result res = std::from_chars(str, strEnd, prt);

        if (res.ec != std::errc() or prt < MIN_PORT or prt > MAX_PORT or res.ptr != strEnd)
                return false;

        port = static_cast<port_t>(prt);
        return true;
}

[[nodiscard]] bool isPortAccessible(const ipaddr_t &ip, const port_t port) {
        addrinfo hints {}, *res = nullptr;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        std::string portStr = std::to_string(port);
        if (getaddrinfo(ip.c_str(), portStr.c_str(), &hints, &res) != 0 || !res)
                return false;

        int sockfd = socket(res->ai_family, SOCK_STREAM, 0);
        if (sockfd == -1) {
                freeaddrinfo(res);
                return false;
        }

        constexpr timeval timeout = {TIMEOUT_MS / 1000, (TIMEOUT_MS % 1000) * 1000l};
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

        int result = connect(sockfd, res->ai_addr, res->ai_addrlen);
        freeaddrinfo(res);
        close(sockfd);

        return result == 0;
}

[[nodiscard]] bool isTerminal() {
        return isatty(fileno(stdout));
}
