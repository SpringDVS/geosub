#ifndef FORGE_HPP
#define FORGE_HPP
#include "protocol/dvsp_packet.hpp"

void run_forge(dvsp_msgtype type, netspace_addr target, std::string content, service_protocol proto);

#endif /* FORGE_HPP */

