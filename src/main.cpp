#include <iostream>
#include <string>
#include <vector>
#include <charconv>

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

bool isIPv4(const ipaddr_t &ip) {
        in_addr address;
        return inet_pton(AF_INET, ip.c_str(), &address) == 1;
}

bool isIPv6(const ipaddr_t &ip) {
        in6_addr address6;
        return inet_pton(AF_INET6, ip.c_str(), &address6) == 1;
}

bool isPort(const char *port) {
        int prt;
        const char *portEnd = port + std::strlen(port);
        std::from_chars_result res = std::from_chars(port, portEnd, prt);
        return res.ec == std::errc {} and res.ptr == portEnd and prt >= MIN_PORT and
                prt <= MAX_PORT ? true : false;
}

bool isPortAccessible(const ipaddr_t &ip, const port_t port) {
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
        for (int i {2}; i < argc; ++i) {
                if (isPort(argv[i]))
                        ports.push_back(std::stoi(argv[i]));
                else {
                        std::cerr << "At least one of the inputted ports is not valid!" << std::endl;
                        return 3;
                }
        }

        for (auto port : ports) {
                if (isPortAccessible(ip, port))
                        std::cout << port << "\tis accessible" << std::endl;
                else
                        std::cout << port << "\tis not accessible" << std::endl;
        }
}
