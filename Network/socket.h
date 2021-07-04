#ifndef INCLUDED_NETWORK_SOCKET
#define INCLUDED_NETWORK_SOCKET

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <stdexcept>

#include "../Utility/logcontext.h"
#include "datablock.h"
#include "ipaddress.h"

namespace Network
{

class SocketException
    : public std::runtime_error
{
public:
    SocketException(const std::string& text)
        : runtime_error(text) {}
};

class Socket
    : public Utility::LogContext
{
public:
    static void InitialiseLibrary(unsigned short version = MAKEWORD(2,2));
    static void ShutdownLibrary();

public:
    Socket(const IpAddress& ip_address);
    ~Socket();

    DataBlock Read(IpAddress& ip_address) const;
    int Write(const IpAddress& ip_address, const DataBlock& db) const;

    const IpAddress& GetBoundAddress() const { return ip_address_; }

private:
    void Create(const IpAddress& ip_address);
    void Delete();

private:
    SOCKET handle_;
    IpAddress ip_address_;
};

typedef boost::shared_ptr<Socket> SocketPtr;
typedef std::vector<SocketPtr> SocketList;
typedef boost::shared_ptr<SocketList> SocketListPtr;

}       // namespace Network

#endif  // INCLUDED_NETWORK_SOCKET
