#include "stdafx.h"
#include "logcontext.h"
#include <sstream>
#include <iomanip>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace Utility;

namespace
{
    std::ofstream log_file_;

    std::string Timestamp()
    {
        SYSTEMTIME sys_time;
        GetSystemTime(&sys_time);

        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << sys_time.wHour << ":"
            << std::setfill('0') << std::setw(2) << sys_time.wMinute << ":"
            << std::setfill('0') << std::setw(2) << sys_time.wSecond << "."
            << std::setfill('0') << std::setw(3) << sys_time.wMilliseconds;

        return oss.str();
    }
}

void Utility::OpenLogFile(const std::string& filename)
{
    log_file_.open(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
}

std::string Utility::ToHex(long rv)
{
    std::ostringstream oss;
    oss << "0x" << std::setw(8) << std::setfill('0') << std::hex << rv;
    return oss.str();
}

std::ofstream& Utility::GetLogFile()
{
    return log_file_;
}

void LogContext::AddLogLine(const std::string& text) const
{
    log_file_ << Timestamp() << " [" << context_ << "] " << text << "\n";
    log_file_.flush();
}
