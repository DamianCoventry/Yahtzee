#include "stdafx.h"
#include "viewport.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include <algorithm>

using namespace Graphics;

int ViewportManager::unique_id_ = 0;

void Viewport::Set()
{
    glViewport(x_, y_, w_, h_);
}

ViewportManager::ViewportManager()
{
}

int ViewportManager::Add(ViewportPtr viewport)
{
    int id = unique_id_++;
    viewports_.insert(std::make_pair(id, viewport));
    return id;
}

void ViewportManager::Remove(int id)
{
    Viewports::iterator itor = viewports_.find(id);
    if(itor != viewports_.end())
    {
        viewports_.erase(itor);
    }
}

void ViewportManager::Clear()
{
    viewports_.clear();
}

ViewportPtr ViewportManager::Get(int id) const
{
    Viewports::const_iterator itor = viewports_.find(id);
    return (itor == viewports_.end() ? ViewportPtr() : itor->second);
}
