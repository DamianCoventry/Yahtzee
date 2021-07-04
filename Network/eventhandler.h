#ifndef INCLUDED_NETWORK_EVENTHANDLER
#define INCLUDED_NETWORK_EVENTHANDLER

#include "socket.h"
#include "connection.h"
#include <list>

namespace Network
{

struct DataBlock;
class IpAddress;

class EventHandler
{
public:
    EventHandler(SocketListPtr socket_list, ConnectionListPtr connection_list);
    virtual ~EventHandler();

    virtual void OnDataBlockReceived(SocketPtr sock, const DataBlock& db, const IpAddress& ip_address) = 0;

protected:
    void SendToAllConnections(const DataBlock& db);
    void SendToAllConnectionsExcept(const DataBlock& db, const IpAddress& ip_address);
    void SendOobDataBlock(const IpAddress& ip_address, const DataBlock& db);
    void SendOobDataBlock(const IpAddress& ip_address, const DataBlock& db, Network::SocketPtr sock);
    SocketListPtr GetSocketList() const { return socket_list_; }
    ConnectionPtr GetConnection(const IpAddress& ip_address) const;

private:
    SocketListPtr socket_list_;
    ConnectionListPtr connection_list_;
};

typedef std::list<EventHandler*> EventHandlerList;

}       // namespace Network

#endif  // INCLUDED_NETWORK_EVENTHANDLER
