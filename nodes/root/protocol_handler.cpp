#include "protocol_handler.hpp"
#include "http_service.hpp"

using rcode = dvsp_rcode;

protocol_handler::protocol_handler(netspace_table& table, metaspace_gsn& msgsn)
	: m_nstable(table)
	, m_msgsn(msgsn)
{ }

protocol_handler::protocol_handler(const protocol_handler& orig)
	: m_nstable(orig.m_nstable)
	, m_msgsn(orig.m_msgsn)
{ }

protocol_handler::~protocol_handler() {
}

packet_uptr protocol_handler::process_packet(const dvsp_packet& packet, const netspace_addr& addr) {
	
	switch(packet.header().type) {
		case dvsp_msgtype::gsn_register_host:
			return register_host(packet, addr);
		case dvsp_msgtype::gsn_unregister_host:
			return unregister_host(packet, addr);
		case dvsp_msgtype::gsn_resolution:
			return resolve_gsn(packet);
		case dvsp_msgtype::gsn_local_area:
			return local_gsn(packet);
		case dvsp_msgtype::gtn_root_nodes:
			return root_nodes(packet);
		case dvsp_msgtype::gsn_hostname:
			return hostname(packet);
		case dvsp_msgtype::gsn_request:
			return query(packet);
		default:
			return response(dvsp_rcode::malformed_content);
	}
}

packet_uptr protocol_handler::register_host(const dvsp_packet& packet, const netspace_addr& addr) {

	// GSN management should not hop
	// ToDo: End point problems -- Fix this behaviour
	//if(addr.to_v4().to_bytes() != packet.header().addr_orig)
	//	return response(dvsp_rcode::network_error); // Network error
	
	if(m_nstable.find_addr(addr.to_string()) != m_nstable.end())
		return response(dvsp_rcode::netspace_error); // Table error

	//auto st = reinterpret_cast<const frame_register&>(packet.content());
	auto st = packet.content_as<frame_register>();
	
	if(st.type >= static_cast<char>(netnode_type::_final))
		return response(dvsp_rcode::malformed_content); // Malformed content
	
	std::string hostname;
	hostname.assign(st.hostname.data(), st.len);
	
	netspace_node n(static_cast<netnode_type>(st.type), hostname, addr.to_string(), static_cast<service_protocol>(st.protcol));
	
	m_nstable.add_node(n);
	return response(dvsp_rcode::ok);
}

packet_uptr protocol_handler::unregister_host(const dvsp_packet& packet, const netspace_addr& addr) {
	// GSN management should not hop
	// ToDo: End point problems -- Fix this behaviour
	//if(addr.to_v4().to_bytes() != packet.header().addr_orig)
	//	return response(dvsp_rcode::network_error); // Network error
	packet.size();
	auto it = m_nstable.find_addr(addr.to_string());
	if(it == m_nstable.end())
		return response(dvsp_rcode::netspace_error); // Table error
	
	m_nstable.erase_node(it);
	return response(dvsp_rcode::ok);
}

packet_uptr protocol_handler::resolve_gsn(const dvsp_packet& packet) {
	/* Note: This really should be UDP instead of building and tearing
	 * connections to perform gsn resolution through TCP
	 */
	netspace_url url(packet.to_string());
	auto& route = url.static_route();
	route.pop_back();

	if(url.static_route().size() == 0 && url.geosub_query() != "") {
		// Here: Run a search on the metaspace -- needs impl
		auto it =  m_nstable.find_host(m_msgsn.resolve_gsn(url.geosub_query()));

		if(it == m_nstable.end()) return response(rcode::netspace_error);
		packet_uptr p(new dvsp_packet);
		p->header() = packet.header();
		p->header().type = dvsp_msgtype::gsn_response;
		auto frame = construct_frame_address((*it).address(), (*it).protocol());
		
		p->copy_content(&frame, sizeof(frame));
		return p;
	}
	
	
	/* TODO: This should be GSN identifier -- not
	 * a particular host acting as a root node on
	 * the GSN
	 */
	auto it = m_nstable.find_host(route.back());

	if(it == m_nstable.end()) {
		return response(rcode::netspace_error);
	}
	
	
	auto nodeip = (*it).address();
	if(route.size() == 1) { 
		packet_uptr p(new dvsp_packet);
		p->header() = packet.header();
		p->header().type = dvsp_msgtype::gsn_response;
		auto frame = construct_frame_address((*it).address(), (*it).protocol());
		p->copy_content(&frame, sizeof(frame));
		return p;
	}

	/*
	 * If there is more of a static route, then we
	 * need to pass the request onto the next node.
	 * We change the inbound address address reference
	 * to say we need to pass it on.
	 */	
	packet_uptr p(new dvsp_packet);
	p->header() = packet.header();
	p->str_content(url.to_string());
	p->header().addr_dest = nodeip.to_v4().to_bytes();
	
	return p;
	
}

packet_uptr protocol_handler::query_gsn(const dvsp_packet& packet) {
	packet.content();
	return response(rcode::ok);
}

packet_uptr protocol_handler::local_gsn(const dvsp_packet& packet) {
	auto nodes = nodes_of(netnode_type::org);
	auto ptr = construct_frame_network(nodes);
	packet_uptr p(new dvsp_packet);
	p->header() = packet.header();
	p->header().type = dvsp_msgtype::gsn_response;
	p->copy_content(ptr, ptr->size);
	delete[] ptr;
	return p;
}

packet_uptr protocol_handler::root_nodes(const dvsp_packet& packet) {
	auto nodes = nodes_of(netnode_type::root);
	auto ptr = construct_frame_network(nodes);
	packet_uptr p(new dvsp_packet);
	p->header() = packet.header();
	p->header().type = dvsp_msgtype::gsn_response;
	p->copy_content(ptr, ptr->size);
	delete[] ptr;
	return p;
}

std::vector<netspace_ipv4> protocol_handler::nodes_of(netnode_type type) {
	std::vector<netspace_ipv4> nodes;
	for(auto& n : m_nstable) {
		if(n.type() == type)
			nodes.push_back(n.address().to_v4().to_bytes());
	}
	return nodes;
}

packet_uptr protocol_handler::hostname(const dvsp_packet& packet) {
	auto addr = packet.content_as<netspace_ipv4>();
	
	auto it = m_nstable.find_addr(ipv4_to_netspace_addr(addr));
	if(it == std::end(m_nstable)) return response(rcode::netspace_error);
	auto node = *it;
	packet_uptr p(new dvsp_packet);
	p->header().type = dvsp_msgtype::gsn_response;
	auto fh = construct_frame_hostname(node.hostname(), node.protocol());
	p->copy_content(&fh, sizeof(fh));
	return p;;
}



packet_uptr protocol_handler::response(dvsp_rcode code) {
	packet_uptr p(new dvsp_packet);

	p->header().type = dvsp_msgtype::gsn_response;
	auto frc = construct_frame_response_code(code);
	p->copy_content(&frc, sizeof(frame_response_code));
	return p;
}

packet_uptr protocol_handler::query(const dvsp_packet& packet) {
	auto in = packet;
	for(auto& node : m_nstable) {
		in.header().addr_dest = node.address().to_v4().to_bytes();
		
		if(node.protocol() == service_protocol::http) {
			auto frame = http_frame(in, node.hostname());
			http_service::send_frame(frame, in.header().addr_dest);
		}
	}
	
	return response(rcode::ok);
}

