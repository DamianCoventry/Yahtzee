#ifndef INCLUDED_UTILITY_LOGMACROS
#define INCLUDED_UTILITY_LOGMACROS

#include <string>
#include <sstream>
#include "logcontext.h"

#define LOG(text__)                         \
{                                           \
    std::ostringstream oss__;               \
    oss__ << text__;                        \
    AddLogLine(oss__.str());                \
}

#define LOG_EXCEPTION(text__)                                           \
{                                                                       \
    std::ostringstream xlfhgvk__;                                       \
    xlfhgvk__ << "\n************** Exception Occurred **************\n";\
    xlfhgvk__ << "File:    " << __FILE__ << "\n";                       \
    xlfhgvk__ << "Line:    " << __LINE__ << "\n";                       \
    xlfhgvk__ << "Message: " << text__ << "\n";                         \
    xlfhgvk__ << "************************************************";    \
    AddLogLine(xlfhgvk__.str());                                        \
}

#endif  // INCLUDED_UTILITY_LOGMACROS
