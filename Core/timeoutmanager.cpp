#include "stdafx.h"
#include "timeoutmanager.h"
#include "timeouteventhandler.h"
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

using namespace Core;

int TimeoutManager::next_id_ = 0;

TimeoutManager::TimeoutManager()
{
}

int TimeoutManager::AddTimeout(TimeoutEventHandler* event_handler, unsigned long timeout_ms)
{
    TimeoutNode node;
    node.id_            = next_id_++;
    node.timeout_time_  = timeGetTime() + timeout_ms;
    node.event_handler_ = event_handler;
    timeout_nodes_.push_back(node);
    return node.id_;
}

void TimeoutManager::RemoveTimeout(int timeout_id)
{
    TimeoutNodes::iterator itor = std::find(timeout_nodes_.begin(), timeout_nodes_.end(), timeout_id);
    if(itor != timeout_nodes_.end())
    {
        itor->remove_ = true;
    }
}

void TimeoutManager::RemoveAllTimeouts(TimeoutEventHandler* event_handler)
{
    TimeoutNodes::iterator current = timeout_nodes_.begin();
    TimeoutNodes::iterator prev;
    while(current != timeout_nodes_.end())
    {
        prev = current;
        ++current;
        if(prev->event_handler_ == event_handler)
        {
            timeout_nodes_.erase(prev);
        }
    }
}

void TimeoutManager::CheckTimeouts()
{
    TimeoutNodes::iterator current = timeout_nodes_.begin();
    TimeoutNodes::iterator prev;
    while(current != timeout_nodes_.end())
    {
        prev = current;
        ++current;
        if(prev->remove_)
        {
            timeout_nodes_.erase(prev);
        }
    }

    unsigned long now = timeGetTime();
    for(current = timeout_nodes_.begin(); current != timeout_nodes_.end(); ++current)
    {
        if(now >= current->timeout_time_)
        {
            current->event_handler_->OnTimeout(current->id_);
            current->remove_ = true;
        }
    }
}
