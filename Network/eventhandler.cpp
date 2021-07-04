#include "stdafx.h"
#include "eventhandler.h"
#include "datablock.h"
#include "ipaddress.h"

using namespace Network;

EventHandler::EventHandler(SocketListPtr socket_list, ConnectionListPtr connection_list)
: socket_list_(socket_list)
, connection_list_(connection_list)
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::SendToAllConnections(const DataBlock& db)
{
    ConnectionList::const_iterator conn;
    for(conn = connection_list_->begin(); conn != connection_list_->end(); ++conn)
    {
        (*conn)->GetSocket()->Write((*conn)->GetRemoteAddress(), db);
    }
}

void EventHandler::SendToAllConnectionsExcept(const DataBlock& db, const IpAddress& ip_address)
{
    ConnectionList::const_iterator conn;
    for(conn = connection_list_->begin(); conn != connection_list_->end(); ++conn)
    {
        if((*conn)->GetRemoteAddress() != ip_address)
        {
            (*conn)->GetSocket()->Write((*conn)->GetRemoteAddress(), db);
        }
    }
}

void EventHandler::SendOobDataBlock(const IpAddress& ip_address, const DataBlock& db)
{
    SocketList::const_iterator sock;
    for(sock = socket_list_->begin(); sock != socket_list_->end(); ++sock)
    {
        (*sock)->Write(ip_address, db);
    }
}

void EventHandler::SendOobDataBlock(const IpAddress& ip_address, const DataBlock& db, Network::SocketPtr sock)
{
    sock->Write(ip_address, db);
}

ConnectionPtr EventHandler::GetConnection(const IpAddress& ip_address) const
{
    Network::ConnectionList::iterator itor;
    for(itor = connection_list_->begin(); itor != connection_list_->end(); ++itor)
    {
        if((*itor)->GetRemoteAddress() == ip_address)
        {
            return *itor;
        }
    }
    return ConnectionPtr();
}
