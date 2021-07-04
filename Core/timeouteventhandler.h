#ifndef INCLUDED_CORE_TIMEOUTEVENTHANDLER
#define INCLUDED_CORE_TIMEOUTEVENTHANDLER

namespace Core
{

struct TimeoutEventHandler
{
    virtual void OnTimeout(int timeout_id) {}
};

}       // namespace Core

#endif  // INCLUDED_CORE_TIMEOUTEVENTHANDLER
