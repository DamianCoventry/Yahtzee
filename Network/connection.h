#ifndef INCLUDED_NETWORK_CONNECTION
#define INCLUDED_NETWORK_CONNECTION

#include <boost/shared_ptr.hpp>
#include "socket.h"
#include "ipaddress.h"
#include <list>

namespace Network
{

class Connection
{
public:
    Connection(const IpAddress& ip_address, SocketPtr socket);

    void Send(const DataBlock& db);

    const IpAddress& GetRemoteAddress() const   { return remote_address_; }
    SocketPtr GetSocket() const                 { return socket_; }

private:
    IpAddress remote_address_;
    SocketPtr socket_;
};

typedef boost::shared_ptr<Connection> ConnectionPtr;
typedef std::list<ConnectionPtr> ConnectionList;
typedef boost::shared_ptr<ConnectionList> ConnectionListPtr;

}       // namespace Network

#endif  // INCLUDED_NETWORK_CONNECTION
