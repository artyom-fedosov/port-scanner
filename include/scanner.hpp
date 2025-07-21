#pragma once

#include <vector>
#include <mutex>
#include <optional>

#include "types.hpp"

namespace {
        port_t constexpr MIN_PORT = 0; //!< \brief Minimum valid port number
        port_t constexpr MAX_PORT = 65535; //!< \brief Maximum valid port number
        int constexpr TIMEOUT_MS = 200; //!< \brief Timeout in milliseconds for
                                        //!< socket connection attempts
}

/*!
 * \class Scanner
 *
 * The Scanner class is designed to perform port scanning operations on a
 * specified IP address for a given set of ports.
 */
class Scanner {
public:
        /*!
        * \brief Constructs a Scanner with a target IP address and a list of
        * ports to scan.
        *
        * Validates the IP address and parses the port strings into numeric port
        * values. Throws std::invalid_argument if the IP or any port is invalid.
        *
        * \param ip Target IP address to scan.
        * \param ports Vector of C-string port representations to be scanned.
        *
        * \throws std::invalid_argument If the IP address or any port string is
        * invalid.
        */
        Scanner(ipaddr_t const &ip, std::vector<char const *> const &ports);

public:
        /*!
        * @brief Scans all specified ports on the target IP address.
        *
        * Each port is scanned concurrently in a separate thread.
        *
        * @return A vector of pairs: <port number, is port accessible
        * (true/false)>.
        */
        [[nodiscard]] std::vector<std::pair<port_t, bool>> scan() noexcept;

private:
        ipaddr_t m_ip;
        ports_t m_ports;
        std::mutex m_mtx;

private:
        /*!
        * \brief Checks if the given IP address string is a syntactically valid
        * IPv4 address.
        *
        * \param ip The IP address string to validate.
        * \return true if the IP is а valid IPv4, false otherwise.
        */
        [[nodiscard]] bool isIPv4(ipaddr_t const &ip) const noexcept;

        /*!
        * \brief Checks if the given IP address string is a syntactically valid
        * IPv6 address.
        *
        * \param ip The IP address string to validate.
        * \return true if the IP is а valid IPv6, false otherwise.
        */
        [[nodiscard]] bool isIPv6(ipaddr_t const &ip) const noexcept;

        /*!
        * \brief Attempts to parse a C-string into a port_t. It also validates
        * if the parsed number falls within the [MIN_PORT, MAX_PORT] range.
        *
        * \param str The C-string containing the potential port number.
        * \return An std::optional<port_t> containing the valid port number if
        * parsing is successful and the number is in range; otherwise, an empty
        * std::optional.
        */
        [[nodiscard]] std::optional<port_t>
        parsePort(char const *str) const noexcept;

        /*!
        * \brief Attempts to establish a TCP connection to the specified port.
        * This function incorporates a timeout (TIMEOUT_MS).
        *
        * \param port The TCP port number to check for accessibility.
        * \return true if a connection can be established (port is accessible),
        * false otherwise.
        */
        [[nodiscard]] bool isPortAccessible(port_t const port) const noexcept;
};
