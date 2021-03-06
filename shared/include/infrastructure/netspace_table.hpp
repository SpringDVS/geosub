/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   netspace.hpp
 * Author: cfg
 *
 * Created on 19 February 2016, 16:07
 */

#ifndef NETSPACE_HPP
#define NETSPACE_HPP

#include <vector>
#include "infrastructure/netnode.hpp"
class netspace_table {
public:
	typedef std::vector<netspace_node> container_type;
	typedef container_type::size_type size_type;
	typedef container_type::iterator iterator;
	typedef container_type::const_iterator const_iterator;
	
public:
	netspace_table();
	netspace_table(const netspace_table& orig);
	virtual ~netspace_table();
	
	void add_node(const netspace_node& node) noexcept;
	void erase_node(const_iterator it) noexcept;
	
	// Algos
	iterator find_addr(const netspace_addr& addr) noexcept;
	iterator find_addr(const std::string& addr) noexcept;
	
	iterator find_host(const std::string& host) noexcept;
	
	iterator find_type(netnode_type nt);
	iterator find_type(netnode_type nt, iterator start);
	
	iterator find_suid(const std::string& suid) noexcept;
	
	// container interface
	netspace_node& at(size_type index);
	size_type size() const;
	bool empty() const;
	void clear() noexcept;
	
	
	iterator begin();
	iterator end();
private:
	
	std::vector<netspace_node> m_table;
};

#endif /* NETSPACE_HPP */

