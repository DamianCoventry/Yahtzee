#ifndef INCLUDED_NETWORK_DATABLOCK
#define INCLUDED_NETWORK_DATABLOCK

#include <boost/shared_array.hpp>

namespace Network
{

struct DataBlock
{
    DataBlock() : num_bytes_(0) {}
    DataBlock(const DataBlock& db) : num_bytes_(db.num_bytes_), bytes_(db.bytes_) {}
    int num_bytes_;
    boost::shared_array<char> bytes_;
};

}       // namespace Network

#endif  // INCLUDED_NETWORK_DATABLOCK
