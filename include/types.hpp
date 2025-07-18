#pragma once

#include <vector>
#include <string>

#include <cstdint>

using ipaddr_t = std::string; //!< \brief IP address type
using port_t = uint16_t; //!< \brief Port number type
using ports_t = std::vector<port_t>; //!< \brief Vector of ports type
