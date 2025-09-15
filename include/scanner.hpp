#pragma once

#include <mutex>
#include <optional>
#include <vector>

#include "types.hpp"

namespace {
int constexpr TIMEOUT_MS{200}; //!< \brief Timeout in milliseconds for socket connection attempts.
}

/*!
 * \class Scanner
 *
 * The Scanner class is designed to perform port scanning operations on a specified IP address for a
 * given vector of ports.
 */
class Scanner final {
  public:
    /*!
     * \brief Constructs a Scanner with a target IP address and a vector of ports to scan.
     *
     * Exception Safety: Strong guarantee. The object is constructed only if the IP address and all
     * port strings are valid; otherwise, no object is created.
     *
     * \param ip Target IP address to scan.
     * \param ports A vector of null-terminated C-string port representations (e.g., "80", "443") to
     * scan.
     *
     * \throws std::invalid_argument If the IP address or any port string is invalid.
     * \throws std::bad_alloc If memory allocation for internal storage fails.
     * \throws std::length_error If the requested storage size exceeds the maximum allowed size.
     */
    Scanner(ipaddr_t const &ip, std::vector<char const *> const &ports);

  public:
    /*!
     * \brief Scans all specified ports on the target IP address.
     *
     * Each port is scanned concurrently in a separate thread.
     *
     * \return A vector of pairs: <port number, is port accessible (true/false)>.
     */
    [[nodiscard]] std::vector<std::pair<port_t, bool>> scan() const;

  public:
    static port_t constexpr MIN_PORT{}; //!< \brief Minimum valid port number

    static port_t constexpr MAX_PORT{65535}; //!< \brief Maximum valid port number

  private:
    ipaddr_t m_ip; //!< \brief Target IP address.

    /*!
     * \brief Target ports.
     *
     * Exception safety: No-throw guarantee.
     */
    ports_t m_ports{};

    /*!
     * \brief Mutable mutex for thread safety.
     *
     * Exception safety: No-throw guarantee.
     */
    mutable std::mutex m_mtx{};

  private:
    /*!
     * \brief Checks if the given IP address string is a syntactically valid IPv4 address.
     *
     * Exception Safety: No-throw guarantee.
     *
     * \param ip The IP address string to validate.
     * \return true if the IP is а valid IPv4, false otherwise.
     */
    [[nodiscard]] bool isIPv4(ipaddr_t const &ip) const noexcept;

    /*!
     * \brief Checks if the given IP address string is a syntactically valid IPv6 address.
     *
     * Exception Safety: No-throw guarantee.
     *
     * \param ip The IP address string to validate.
     * \return true if the IP is а valid IPv6, false otherwise.
     */
    [[nodiscard]] bool isIPv6(ipaddr_t const &ip) const noexcept;

    /*!
     * \brief Attempts to parse a C-string into a port_t. It also validates if the parsed number
     * falls within the [MIN_PORT, MAX_PORT] range.
     *
     * Exception Safety: No-throw guarantee.
     *
     * \param str The C-string containing the potential port number.
     * \return An std::optional<port_t> containing the valid port number if parsing is successful
     * and the number is in range; otherwise, an empty std::optional.
     */
    [[nodiscard]] std::optional<port_t> parsePort(char const *str) const noexcept;

    /*!
     * \brief Attempts to establish a TCP connection to the specified port. This function
     * incorporates a timeout (TIMEOUT_MS).
     *
     * Exception Safety: Strong guarantee.
     *
     * \param port The TCP port number to check for accessibility.
     * \return true if a connection can be established (port is accessible), false otherwise.
     *
     * \throws std::bad_alloc If memory allocation for internal storage fails.
     */
    [[nodiscard]] bool isPortAccessible(port_t const port) const;
};
