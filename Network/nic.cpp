#include "stdafx.h"
#include "nic.h"

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>

#include <sstream>
#include <iomanip>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iphlpapi.h>
#include <iptypes.h>

using namespace Network;

namespace
{
    std::string ByteArrayToString(unsigned char* bytes, int num_bytes)
    {
        std::ostringstream oss;
        for(int i = 0; i < num_bytes; i++)
        {
            oss << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned int>(bytes[i]);
        }
        return oss.str();
    }
}

void Nics::Refresh()
{
    nic_map_.clear();

    // First call the function to find out how many bytes we need
    // to allocate.
    boost::scoped_ptr<IP_ADAPTER_INFO> temp_ptr(new IP_ADAPTER_INFO);
    ULONG size = sizeof(IP_ADAPTER_INFO);
    GetAdaptersInfo(temp_ptr.get(), &size);

    // Now allocate this many bytes, it might've returned
    // the same number but that's ok.
    boost::scoped_array<char> bytes(new char[size]);
    IP_ADAPTER_INFO* adapter_head = reinterpret_cast<IP_ADAPTER_INFO*>(bytes.get());

    int rv = GetAdaptersInfo(adapter_head, &size);
    if(rv == NO_ERROR)
    {
        IP_ADAPTER_INFO* adapter_current = adapter_head;
        while(adapter_current)
        {
            // Save the info for this adapter.
            std::string description(adapter_current->Description);
            std::string mac_address(ByteArrayToString(adapter_current->Address, 6));
            std::vector<std::string> ip_addresses, ip_masks;

            IP_ADDR_STRING* current_ip = &adapter_current->IpAddressList;
            while(current_ip)
            {
                ip_addresses.push_back(current_ip->IpAddress.String);
                ip_masks.push_back(current_ip->IpMask.String);
                current_ip = current_ip->Next;
            }

            // Ok, this is really fucked up.  MAC addresses are supposed to be unique,
            // but windows, in its infinite wisdom, can actually have two
            // adapters with the same MAC address!?!?!?  We've seen this on a 
            // customer site, see job #1226.
            NicMap::iterator itor = nic_map_.find(mac_address);
            if(itor == nic_map_.end())
            {
                nic_map_.insert(std::make_pair(mac_address, NicPtr(new Nic(description, mac_address, ip_addresses, ip_masks))));
            }
            else
            {
                itor->second->AddIpAddresses(ip_addresses);
                itor->second->AddIpMasks(ip_masks);
            }

            // Move to the next adaptor, if there is one.
            adapter_current = adapter_current->Next;
        }
    }
}

NicPtr Nics::Get(const std::string& mac_address) const
{
    NicMap::const_iterator itor = nic_map_.find(mac_address);
    return (itor == nic_map_.end() ? NicPtr() : itor->second);
}

NicPtr Nics::MoveFirst() const
{
    if(nic_map_.empty())
    {
        return NicPtr();
    }
    current_nic_ = nic_map_.begin();
    return current_nic_->second;
}

NicPtr Nics::MoveNext() const
{
    if(nic_map_.empty() || current_nic_ == nic_map_.end())
    {
        return NicPtr();
    }
    ++current_nic_;
    if(current_nic_ == nic_map_.end())
    {
        return NicPtr();
    }
    return current_nic_->second;
}
