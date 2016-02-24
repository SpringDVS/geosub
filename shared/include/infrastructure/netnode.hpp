/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   netnode.hpp
 * Author: cfg
 *
 * Created on 19 February 2016, 16:09
 */

#ifndef NETNODE_HPP
#define NETNODE_HPP
#include "infrastructure/net_common.hpp"


class netnode {
	
public:
	netnode();
	netnode(netnode_type type);
	netnode(netnode_type type, std::string host, std::string address);
	netnode(netnode_type type, std::string host, netspace_addr address);
	netnode(const netnode& orig);
	virtual ~netnode();
	
	netnode_type type();
	void set_type(netnode_type t);
	
	const netspace_addr& address() const;
	void set_address(netspace_addr addr);
	
	std::string hostname();
	void set_hostname(std::string host);
private:

	netnode_type m_type;
	netspace_addr m_addr;
	std::string m_hostname;
};

#endif /* NETNODE_HPP */

