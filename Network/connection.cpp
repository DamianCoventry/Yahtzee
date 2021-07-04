#include "stdafx.h"
#include "connection.h"

using namespace Network;

Connection::Connection(const IpAddress& ip_address, SocketPtr socket)
: remote_address_(ip_address), socket_(socket)
{
}

void Connection::Send(const DataBlock& db)
{
    socket_->Write(remote_address_, db);
}
