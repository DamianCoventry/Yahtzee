#ifndef INCLUDED_CORE_TIMEOUTMANAGER
#define INCLUDED_CORE_TIMEOUTMANAGER

#include <boost/shared_ptr.hpp>
#include <list>

namespace Core
{

struct TimeoutEventHandler;

class TimeoutManager
{
public:
    TimeoutManager();
    int AddTimeout(TimeoutEventHandler* event_handler, unsigned long timeout_ms);
    void RemoveTimeout(int timeout_id);
    void RemoveAllTimeouts(TimeoutEventHandler* event_handler);
    void CheckTimeouts();
private:
    static int next_id_;
    struct TimeoutNode
    {
        TimeoutNode() : remove_(false) {}
        bool operator==(int rhs) const
        { return id_ == rhs; }
        int id_;
        unsigned long timeout_time_;
        TimeoutEventHandler* event_handler_;
        bool remove_;
    };
    typedef std::list<TimeoutNode> TimeoutNodes;
    TimeoutNodes timeout_nodes_;
};

typedef boost::shared_ptr<TimeoutManager> TimeoutManagerPtr;

}       // namespace Core

#endif  // INCLUDED_CORE_TIMEOUTMANAGER
