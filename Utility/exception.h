#ifndef INCLUDED_UTILITY_EXCEPTION
#define INCLUDED_UTILITY_EXCEPTION

#include <sstream>
#include <stdexcept>
#include "logmacros.h"

#define THROW_BASIC(text__)                 \
{                                           \
    std::ostringstream oss__;               \
    oss__ << text__;                        \
    LOG_EXCEPTION(oss__.str())              \
    throw std::runtime_error(oss__.str());  \
}

#define THROW_WIN32(text__)                 \
{                                           \
    std::ostringstream oss__;               \
    oss__ << text__ << ", GetLastError()=[" << GetLastError() << "]";  \
    LOG_EXCEPTION(oss__.str())              \
    throw std::runtime_error(oss__.str());  \
}

#define THROW_COM(text__)                   \
{                                           \
    std::ostringstream oss__;               \
    oss__ << text__ << ", HRESULT=[" << Utility::ToHex(rv) << "]";  \
    LOG_EXCEPTION(oss__.str())              \
    throw std::runtime_error(oss__.str());  \
}

#define THROW_WS(text__)                    \
{                                           \
    std::ostringstream oss__;               \
    oss__ << text__ << ", WSAGetLastError=[" << WSAGetLastError() << "]";  \
    LOG_EXCEPTION(oss__.str())              \
    throw std::runtime_error(oss__.str());  \
}

#define THROW_WS2(the_exception, text__)     \
{                                           \
    std::ostringstream oss__;               \
    oss__ << text__ << ", WSAGetLastError=[" << WSAGetLastError() << "]";  \
    LOG_EXCEPTION(oss__.str())              \
    throw the_exception(oss__.str());       \
}

#endif  // INCLUDED_UTILITY_EXCEPTION
