#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sstream>
#include <iomanip>

typedef uint32_t ClientID_t;

inline static
ClientID_t cid_from_hex(std::string hex)
{
	return std::stoul(std::string("0x") + hex, nullptr, 16);
}

inline static
std::string cid_to_hex(ClientID_t cid)
{
	std::ostringstream os;
	os << std::setfill('0') << std::setw(sizeof(cid)*2) << std::hex << cid;
	return os.str();
}

class Client
{
public:
	virtual ~Client() {}

	enum class Type
	{
		INVALID = -1,
		DRONE,
		OPERATOR
	} type = Type::INVALID;
	ClientID_t id;
	std::string drone_name, serial;
};

#endif // !CLIENT_HPP