#ifndef PROTOCOL_HANDLER_HPP
#define PROTOCOL_HANDLER_HPP
#include "infrastructure/netspace_table.hpp"
#include "infrastructure/netspace_url.hpp"
#include "protocol/dvsp_packet.hpp"
#include "infrastructure/metaspace_gsn.hpp"

class protocol_handler {
public:
	protocol_handler(netspace_table& table, metaspace_gsn& msgsn);
	protocol_handler(const protocol_handler& orig);
	virtual ~protocol_handler();
	
	packet_uptr process_packet(const dvsp_packet& packet, const netspace_addr& addr);

private:

	netspace_table& m_nstable;
	metaspace_gsn& m_msgsn;
	
	packet_uptr register_host(const dvsp_packet& packet, const netspace_addr& addr);
	packet_uptr unregister_host(const dvsp_packet& packet, const netspace_addr& addr);
	packet_uptr resolve_gsn(const dvsp_packet& packet);
	packet_uptr query_gsn(const dvsp_packet& packet);
	
	packet_uptr local_gsn(const dvsp_packet& packet);
	packet_uptr root_nodes(const dvsp_packet& packet);
	
	packet_uptr hostname(const dvsp_packet& packet);
	
	packet_uptr query(const dvsp_packet& packet);
	std::string query_all(dvsp_packet packet, std::string query);
	std::string query_one(dvsp_packet packet, std::string suid, std::string query);
	
	std::vector<netspace_node> nodes_of(netnode_type type);
	
	packet_uptr response(dvsp_rcode code);
};

#endif /* PROTOCOL_HANDLER_HPP */

