#ifndef DVSP_PACKET_HPP
#define DVSP_PACKET_HPP
#include "protocol_common.hpp"

struct dvsp_header {
	dvsp_msgtype type;
	std::uint32_t size;
	netspace_ipv4 addr_orig;
	netspace_ipv4 addr_dest;
	
	dvsp_header()
		: type(dvsp_msgtype::undefined)
		, size(0)
		, addr_orig{0}
		, addr_dest{0}
		{ }
};

class dvsp_packet
{
public:
	typedef char serial_type;
	typedef serial_type* serial_ptr;
	typedef char generic_type;
	typedef generic_type* generic_ptr;
	typedef std::size_t size_type;
	
public:
	dvsp_packet();
	dvsp_packet(const serial_ptr serial);
	dvsp_packet(const dvsp_packet& orig);
	virtual ~dvsp_packet();
	
	dvsp_header& header() noexcept;
	const dvsp_header& header() const noexcept;
	
	void clear();
	
	template<typename T>
	void copy_content(const T* data, size_type size) noexcept {
		reset(size);

		auto s = reinterpret_cast<const generic_type*>(data);
		std::copy(s, s+size, m_content);
	};
	
	void str_content(std::string data) noexcept;
	
	const generic_type& content() const;
	std::string to_string() const;
	
	serial_ptr serialise() const;

private:
	dvsp_header m_header;
	generic_ptr m_content;
	
	void deserialise(const serial_ptr serial) noexcept;
	inline void reset(size_type size) {
		if(m_content) delete[] m_content;
		m_header.size = size;
		
		if(!size) return;
		
		m_content = new generic_type[size];
		
			
	};

};

#endif /* DVSP_PACKET_HPP */

