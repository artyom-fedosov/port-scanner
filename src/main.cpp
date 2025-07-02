#include <iostream>
#include <string>
#include <vector>
#include <charconv>
#include <thread>
#include <mutex>

#include <cstring>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>

using ipaddr_t = std::string;
using port_t = uint16_t;
using ports_t = std::vector<port_t>;

constexpr port_t MIN_PORT = 0;
constexpr port_t MAX_PORT = 65535;
constexpr int TIMEOUT_MS = 200;

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

        constexpr timeval timeout = {TIMEOUT_MS / 1000, (TIMEOUT_MS % 1000) * 1000};
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

        int result = connect(sockfd, res->ai_addr, res->ai_addrlen);
        freeaddrinfo(res);
        close(sockfd);

        return result == 0;
}

std::mutex mtx;

int main(int argc, char **argv) {
        if (argc < 3) {
                std::cerr << "Arguments count must be >= 3." << std::endl;
                return 1;
        }

        if (!isIPv4(argv[1]) and !isIPv6(argv[1])) {
                std::cerr << "IP address is not valid!" << std::endl;
                return 2;
        }
        ipaddr_t ip {argv[1]};

        ports_t ports {};
        port_t port {};
        for (int i {2}; i < argc; ++i) {
                if (parsePort(argv[i], port))
                        ports.push_back(port);
                else {
                        std::cerr << "At least one of the inputted ports is not valid!" << std::endl;
                        return 3;
                }
        }

        std::vector<std::thread> threads {};
        for (const auto port : ports) {
                threads.push_back(std::thread {[&ip, port]() {
                        bool isAccessible {isPortAccessible(ip, port)};
                        std::lock_guard<std::mutex> lock {mtx};
                        std::cout << port << "\tis" << (isAccessible ? " " : " not ") << "accessible"
                                  << std::endl;
                }});
        }

        for (auto &thread : threads)
                thread.join();
}
