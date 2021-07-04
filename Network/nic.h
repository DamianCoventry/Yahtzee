#ifndef INCLUDED_NETWORK_NIC
#define INCLUDED_NETWORK_NIC

#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>


namespace Network
{

class Nic;
typedef boost::shared_ptr<Nic> NicPtr;

class Nics
{
public:
    // Creators
    Nics() { Refresh(); }

    // Manipulators
    void Refresh();

    // Accessors
    NicPtr Get(const std::string& mac_address) const;
    NicPtr MoveFirst() const;
    NicPtr MoveNext() const;
    int GetCount() const { return int(nic_map_.size()); }

private:
    // Data
    typedef std::map<std::string, NicPtr> NicMap;
    mutable NicMap::const_iterator current_nic_;
    NicMap nic_map_;
};



class Nic
{
public:
    // Creators
    Nic(const std::string& description, const std::string& mac_address,
        const std::vector<std::string>& ip_addresses, const std::vector<std::string>& ip_masks)
        : description_(description), mac_address_(mac_address)
        , ip_addresses_(ip_addresses), ip_masks_(ip_masks) {}

    // Accessors
    const std::string& GetDescription() const               { return description_; }
    const std::string& GetMacAddress() const                { return mac_address_; }
    const std::vector<std::string>& GetIpAddresses() const  { return ip_addresses_; }
    const std::vector<std::string>& GetIpMasks() const      { return ip_masks_; }

    // Mutators
    void AddIpAddresses(const std::vector<std::string>& ip_addresses)
    {
        std::copy(ip_addresses.begin(), ip_addresses.end(), std::back_inserter(ip_addresses_));
    }

    void AddIpMasks(const std::vector<std::string>& ip_masks)
    {
        std::copy(ip_masks.begin(), ip_masks.end(), std::back_inserter(ip_masks_));
    }

private:
    // Data
    std::string description_;
    std::string mac_address_;
    std::vector<std::string> ip_addresses_;
    std::vector<std::string> ip_masks_;
};

}       // namespace Network

#endif  // INCLUDED_NETWORK_NIC
