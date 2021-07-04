#include "stdafx.h"
#include "socket.h"
#include "../Utility/exception.h"
#include <sstream>

using namespace Network;

void Socket::InitialiseLibrary(unsigned short version)
{
    WSADATA wsa_data;
    int err_code = WSAStartup(version, &wsa_data);
    if(err_code)
    {
        std::ostringstream oss;
        oss << "Couldn't start Winsock, err_code=[" << err_code << "]";
        throw std::runtime_error(oss.str());
    }
}

void Socket::ShutdownLibrary()
{
    WSACleanup();
}




Socket::Socket(const IpAddress& ip_address)
: LogContext("Socket")
, handle_(INVALID_SOCKET)
{
    Create(ip_address);
}

Socket::~Socket()
{
    Delete();
}

void Socket::Create(const IpAddress& ip_address)
{
    if(handle_ != INVALID_SOCKET)
    {
        Delete();
    }

    handle_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if(handle_ == INVALID_SOCKET)
    {
        THROW_WS("Couldn't create a socket")
    }

    // Set to non blocking
    unsigned long arg = 1;
    if(ioctlsocket(handle_, FIONBIO, &arg) == SOCKET_ERROR)
    {
        THROW_WS("Couldn't set socket [" << handle_ << "] to non-blocking")
    }

    // Set to broadcastable
    int value = 1;
    if(setsockopt(handle_, SOL_SOCKET, SO_BROADCAST, (const char*)&value, sizeof(int)) == SOCKET_ERROR)
    {
        THROW_WS("Couldn't set socket [" << handle_ << "] to broadcastable")
    }

    if(bind(handle_, (const sockaddr*)&ip_address, sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        THROW_WS("Couldn't bind the socket [" << handle_ << "] to the address [" << ip_address.ToLongString() << "]")
    }

    ip_address_ = ip_address;
}

void Socket::Delete()
{
    if(handle_ != INVALID_SOCKET)
    {
        LOG("Deleting socket [" << handle_ << "]");
        closesocket(handle_);
        handle_ = INVALID_SOCKET;
    }
}

DataBlock Socket::Read(IpAddress& ip_address) const
{
    if(handle_ == INVALID_SOCKET)
    {
        return DataBlock();
    }

    int from_len = sizeof(sockaddr_in);
    char buffer[1024];
    DataBlock db;

    int num_bytes = recvfrom(handle_, buffer, 1024, 0, (sockaddr*)&ip_address, &from_len);
    switch(num_bytes)
    {
    case 0:
        THROW_WS2(SocketException, "The socket [" << handle_ << "] was closed by the remote party");
    case -1:
        if(WSAGetLastError() != WSAEWOULDBLOCK)
        {
            THROW_WS2(SocketException, "An error was detected on socket [" << handle_ << "]");
        }
        break;
    default:
        db.num_bytes_ = num_bytes;
        db.bytes_.reset(new char[num_bytes]);
        memcpy(db.bytes_.get(), buffer, sizeof(char)*num_bytes);
        break;
    }

    return db;
}

int Socket::Write(const IpAddress& ip_address, const DataBlock& db) const
{
    if(handle_ == INVALID_SOCKET)
    {
        return 0;
    }

    int num_bytes = sendto(handle_, db.bytes_.get(), db.num_bytes_, 0, (const sockaddr*)&ip_address, sizeof(sockaddr_in));
    if(num_bytes == SOCKET_ERROR)
    {
        if(WSAGetLastError() != WSAEWOULDBLOCK)
        {
            THROW_WS2(SocketException, "An error was detected on socket [" << handle_ << "]");
        }
    }

    return num_bytes;
}
