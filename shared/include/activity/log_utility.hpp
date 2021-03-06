#ifndef LOG_BASE_HPP
#define LOG_BASE_HPP
#include <array>
#include <atomic>
#include <list>

#include "activity/activity.hpp"


struct log_header {
	
	log_header()
		: num_records(0)
		, hash{{0}}
	{ }
	
	int num_records;
	hash512 hash;
};

class log_utility {
friend class log_transaction;
public:
	typedef activity value_type;
	typedef std::list<value_type> container_type;
	typedef log_header header_type;
	typedef std::size_t size_type;
	typedef char serial_type;
	typedef serial_type* serial_ptr;
	

protected:
	virtual header_type& header() = 0;
	virtual container_type& log() = 0; 
	virtual void end_transaction() = 0;
};

#endif /* LOG_BASE_HPP */

