#ifndef INCLUDED_UTILITY_STRUTIL
#define INCLUDED_UTILITY_STRUTIL

#include <string>
#include <sstream>

namespace Utility
{

template<typename T>
std::string ToString(const T& items)
{
    if(items.empty())
    {
        return std::string();
    }
    std::ostringstream oss;
    typename T::const_iterator itor = items.begin();
    oss << *itor;
    ++itor;
    while(itor != items.end())
    {
        oss << ", " << *itor;
        ++itor;
    }
    return oss.str();
}

}       // namespace Utility

#endif  // INCLUDED_UTILITY_STRUTIL
