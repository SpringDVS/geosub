/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   netnode.cpp
 * Author: cfg
 * 
 * Created on 19 February 2016, 16:09
 */

#include "infrastructure/netnode.hpp"

netspace_node::netspace_node()
	: m_type(netnode_type::undefined), m_addr(),  m_hostname(), m_proto(service_protocol::dvsp)
{ }
netspace_node::netspace_node(netnode_type type, service_protocol proto)
	: m_type(type), m_addr(),  m_hostname(), m_proto(proto)
{ }

netspace_node::netspace_node(netnode_type type, std::string host, std::string address, service_protocol proto) 
	: m_type(type), m_addr(netspace_addr::from_string(address)), m_hostname(host), m_proto(proto)
{ }

netspace_node::netspace_node(netnode_type type, std::string host, netspace_addr address, service_protocol proto) 
	: m_type(type), m_addr(address), m_hostname(host), m_proto(proto)
{ }

netspace_node::netspace_node(const netspace_node& orig)
	: m_type(orig.m_type), m_addr(orig.m_addr), m_hostname(orig.m_hostname), m_proto(orig.m_proto)
	, m_suid(orig.m_suid)
{ }

netspace_node::netspace_node(std::string entry)
: m_type(netnode_type::undefined), m_addr(), m_hostname(), m_proto() 
{

	auto offset = 0;
	char state = 0;
	for(auto i = 0u; i < entry.length(); i++) {
		if(entry[i] == '|') {		
			if(state == 0) {
				m_suid = entry.substr(offset, i);
				offset = ++i;
			} else if(state == 1) {
				m_hostname = entry.substr(offset, i-offset);
				offset = ++i;
				m_addr = netspace_addr::from_string(entry.substr(i));
				break;
			}
			state++;
		}
	}
	
}

netspace_node::~netspace_node() 
{ }


netnode_type netspace_node::type() {
	return m_type;
}

void netspace_node::set_type(netnode_type t) {
	m_type = t;
}

const netspace_addr& netspace_node::address() const {
	return m_addr;
}

void netspace_node::set_address(netspace_addr addr) {
	m_addr = addr;
}

std::string netspace_node::hostname() {
	return m_hostname;
}

void netspace_node::set_hostname(std::string host) {
	m_hostname = host;
}

service_protocol netspace_node::protocol() {
	return m_proto;
}

void netspace_node::set_protocol(service_protocol proto) {
	m_proto = proto;
}
void netspace_node::set_suid(std::string suid) {
	m_suid = suid;
}

std::string netspace_node::suid() {
	return m_suid;
}










