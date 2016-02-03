#ifndef MOCK_ACTIVITY_HEADER_HPP
#define MOCK_ACTIVITY_HEADER_HPP
#include "activity.hpp"

struct activity_header_m : public activity_header
{
	activity_header_m() {
		char data[11] = "HelloWorld";
		hash = std::move(hash_sha512(data, 10));
		timestamp = get_timestamp();
		signature[4] = 21;
		msg_size = 0;
		type = activity_type::root_verify;	
	}
	
};

#endif /* MOCK_ACTIVITY_HEADER_HPP */
