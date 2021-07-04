#ifndef INCLUDED_NETWORK_IPADDRESS
#define INCLUDED_NETWORK_IPADDRESS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <string>
#include <vector>
#include "../Utility/logcontext.h"

namespace Network
{

class IpAddress
    : public sockaddr_in
    , public Utility::LogContext
{
public:
    IpAddress();
    IpAddress(const std::string& ip_address, unsigned short port);
    IpAddress(const sockaddr& address);
    IpAddress(const sockaddr_in& address);

    void SetAddress(const std::string& ip_address);
    void SetPort(unsigned short port);

    std::string ToString() const;
    std::string ToLongString() const;
    unsigned short GetPort() const;

	const IpAddress& operator =(const IpAddress& sa);
	const IpAddress& operator =(const sockaddr& sa);
	const IpAddress& operator =(const sockaddr_in& sin);
	bool operator <(const IpAddress& s) const;	
	bool operator ==(const IpAddress& s) const;	
	bool operator !=(const IpAddress& s) const;	
	operator sockaddr();
	operator sockaddr_in();

private:
    void SetAddress_i(const std::string& ip_address);
};

typedef std::vector<IpAddress> IpAddresses;
std::string ToString(const IpAddresses& ip_addresses);

}       // namespace Network

#endif  // INCLUDED_NETWORK_IPADDRESS
