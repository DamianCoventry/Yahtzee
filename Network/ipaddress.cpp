#include "stdafx.h"
#include "ipaddress.h"
#include "../Utility/exception.h"
#include <sstream>

using namespace Network;

IpAddress::IpAddress()
: LogContext("IpAddress")
{
	memset(this, 0, sizeof(sockaddr));
    sin_family  = AF_INET;
}

IpAddress::IpAddress(const std::string& ip_address, unsigned short port)
: LogContext("IpAddress")
{
	memset(this, 0, sizeof(sockaddr));
    sin_family  = AF_INET;
    SetAddress_i(ip_address);
    SetPort(port);
}

IpAddress::IpAddress(const sockaddr& address)
: LogContext("IpAddress")
{
    memcpy(this, &address, sizeof(sockaddr));
}

IpAddress::IpAddress(const sockaddr_in& address)
: LogContext("IpAddress")
{
    memcpy(this, &address, sizeof(sockaddr_in));
}

void IpAddress::SetAddress(const std::string& ip_address)
{
    SetAddress_i(ip_address);
}

void IpAddress::SetPort(unsigned short port)
{
    sin_port = htons(port);
}

std::string IpAddress::ToString() const
{
    return inet_ntoa(sin_addr);
}

std::string IpAddress::ToLongString() const
{
    std::ostringstream oss;
    oss << inet_ntoa(sin_addr) << ":" << ntohs(sin_port);
    return oss.str();
}

unsigned short IpAddress::GetPort() const
{
    return ntohs(sin_port);
}

void IpAddress::SetAddress_i(const std::string& ip_address)
{
    if(isalpha(ip_address[0]))
    {
		// Winsock cleans up the lpHostEnt pointer
		hostent* host_entry = gethostbyname(ip_address.c_str());
		if(host_entry == NULL)
		{
			THROW_WS("Bad address [" << ip_address << "]")
		}
		sin_addr.s_addr =((in_addr*)host_entry->h_addr)->s_addr;
    }
    else
    {
        sin_addr.s_addr = inet_addr(ip_address.c_str());
        if(sin_addr.s_addr == INADDR_NONE)
        {
            THROW_WS("Bad address [" << ip_address << "]")
        }
    }
}

const IpAddress& IpAddress::operator =(const IpAddress& sa)
{
    if((void const*) &sa !=(void const*) this)
    {
	    memcpy(this, &sa, sizeof(IpAddress));
    }
    return *this;
}

const IpAddress& IpAddress::operator =(const sockaddr& sa)
{
    if((void const*) &sa !=(void const*) this)
    {
	    memcpy(this, &sa, sizeof(sockaddr));
    }
	return *this;
}

const IpAddress& IpAddress::operator =(const sockaddr_in& sin)
{
    if((void const*) &sin !=(void const*) this)
    {
	    memcpy(this, &sin, sizeof(sockaddr_in));
    }
    return *this;
}

bool IpAddress::operator <(const IpAddress& s) const
{
    if(s.sin_addr.s_addr == sin_addr.s_addr)
    {
        return s.sin_port < sin_port;
    }
    return s.sin_addr.s_addr < sin_addr.s_addr;
}

bool IpAddress::operator ==(const IpAddress& s) const
{
	return s.sin_addr.s_addr == sin_addr.s_addr &&
           s.sin_port == sin_port;
}

bool IpAddress::operator !=(const IpAddress& s) const
{
	return s.sin_addr.s_addr != sin_addr.s_addr ||
           s.sin_port != sin_port;
}

IpAddress::operator sockaddr()
{
	return *(reinterpret_cast<sockaddr*>(this));
}

IpAddress::operator sockaddr_in()
{
	return *(reinterpret_cast<sockaddr_in*>(this));
}




std::string Network::ToString(const IpAddresses& ip_addresses)
{
    if(ip_addresses.empty())
    {
        return std::string();
    }
    std::ostringstream oss;
    IpAddresses::const_iterator itor = ip_addresses.begin();
    oss << itor->ToString();
    ++itor;
    while(itor != ip_addresses.end())
    {
        oss << ", " << itor->ToString();
        ++itor;
    }
    return oss.str();
}
