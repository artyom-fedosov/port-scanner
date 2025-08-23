#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <optional>

#include <unistd.h>

/**
 * @brief Alias for IP address represented as a string.
 */
using ipaddr_t = std::string;

/**
 * @brief Alias for a TCP port number type.
 */
using port_t = uint16_t;

/**
 * @brief Alias for a vector of TCP ports.
 */
using ports_t = std::vector<port_t>;


/**
 * @brief Minimum valid TCP port number.
 */
constexpr port_t MIN_PORT = 0;

/**
 * @brief Maximum valid TCP port number.
 */
constexpr port_t MAX_PORT = 65535;

/**
 * @brief Timeout in milliseconds for socket connection attempts.
 */
constexpr int TIMEOUT_MS = 200;


/**
 * @brief Checks if the IP address is a valid IPv4.
 *
 * @param ip IP address in string format.
 * @return true if the IP is valid IPv4, false otherwise.
 */
[[nodiscard]] bool isIPv4(const ipaddr_t &ip);

/**
 * @brief Checks if the IP address is a valid IPv6.
 *
 * @param ip IP address in string format.
 * @return true if the IP is valid IPv6, false otherwise.
 */
[[nodiscard]] bool isIPv6(const ipaddr_t &ip);

/**
 * @brief Parses a string into a port number.
 *
 * Parses the input C-string as an integer port number and checks
 * if it is within the valid range [MIN_PORT, MAX_PORT].
 *
 * @param str C-string representing the port.
 * @param[out] port Parsed port number if successful.
 * @return true if parsing succeeded and port is valid, false otherwise.
 */
[[nodiscard]] std::optional<port_t> parsePort(const char *str);

/**
 * @brief Checks if a TCP port is accessible on the given IP.
 *
 * Attempts to connect to the specified port on the given IP address
 * with a timeout. Works for both IPv4 and IPv6.
 *
 * @param ip IP address as string.
 * @param port TCP port number.
 * @return true if the port is accessible, false otherwise.
 */
[[nodiscard]] bool isPortAccessible(const ipaddr_t &ip, const port_t port);

/**
 * @brief Mutex used for synchronizing output.
 */
extern std::mutex mtx;

/**
 * @brief ANSI escape code for resetting terminal colors.
 */
constexpr char RESET[] = "\033[0m";

/**
 * @brief ANSI escape code for red text color.
 */
constexpr char RED[] = "\033[31m";

/**
 * @brief ANSI escape code for green text color.
 */
constexpr char GREEN[] = "\033[32m";

/**
 * @brief Checks if stdout is connected to a terminal.
 *
 * @return true if output is a terminal, false otherwise.
 */
[[nodiscard]] bool isTerminal();
