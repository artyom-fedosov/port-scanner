#include <iostream>
#include <string>
#include <vector>
#include <charconv>

#include <cstring>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

using ipaddr_t = std::string;
using ipaddrv4_t = std::string;
using port_t = uint16_t;
using ports_t = std::vector<port_t>;

constexpr port_t MIN_PORT = 0;
constexpr port_t MAX_PORT = 65535;
constexpr int TIMEOUT_MS = 200;

bool isIPv4(const ipaddr_t &ip) {
        in_addr address;
        return inet_pton(AF_INET, ip.c_str(), &address) == 1;
}

bool isPort(const char *port) {
        int prt;
        const char *portEnd = port + std::strlen(port);
        std::from_chars_result res = std::from_chars(port, portEnd, prt);
        return res.ec == std::errc {} and res.ptr == portEnd and prt >= MIN_PORT and
                prt <= MAX_PORT ? true : false;
}

bool isPortAccessible(const ipaddrv4_t &ip, const port_t port) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
                return false;

        sockaddr_in addr {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());

        fcntl(sockfd, F_SETFL, O_NONBLOCK);

        int res = connect(sockfd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
        if (res == -1) {
                fd_set fdset;
                FD_ZERO(&fdset);
                FD_SET(sockfd, &fdset);
                timeval tv = {TIMEOUT_MS / 1000, (TIMEOUT_MS % 1000) * 1000};

                if (select(sockfd + 1, NULL, &fdset, NULL, &tv) > 0) {
                        int err;
                        socklen_t len = sizeof(err);
                        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &err, &len);
                        close(sockfd);
                        return err == 0;
                } else {
                        close(sockfd);
                        return false;
                }
        }

        close(sockfd);
        return true;
}

int main(int argc, char **argv) {
        if (argc < 3) {
                std::cerr << "Arguments count must be >= 3." << std::endl;
                return 1;
        }

        if (!isIPv4(argv[1])) {
                std::cerr << "IPv4 address is not valid!" << std::endl;
                return 2;
        }
        ipaddrv4_t ipv4 {argv[1]};

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
                if (isPortAccessible(ipv4, port))
                        std::cout << port << "\tis accessible" << std::endl;
                else
                        std::cout << port << "\tis not accessible" << std::endl;
        }
}
