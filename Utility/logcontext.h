#ifndef INCLUDED_UTILITY_LOGCONTEXT
#define INCLUDED_UTILITY_LOGCONTEXT

#include <boost/shared_ptr.hpp>
#include <fstream>
#include <string>

namespace Utility
{

void OpenLogFile(const std::string& filename);
std::ofstream& GetLogFile();

std::string ToHex(long rv);

class LogContext
{
public:
    LogContext(const std::string& context)
        : log_file_(GetLogFile()), context_(context) {}

    void AddLogLine(const std::string& text) const;

private:
    std::ofstream& log_file_;
    std::string context_;
};

typedef boost::shared_ptr<LogContext> LogContextPtr;

}       // namespace Utility

#endif  // INCLUDED_UTILITY_LOGCONTEXT
