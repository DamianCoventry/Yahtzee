#ifndef INCLUDED_GRAPHICS_VIEWPORT
#define INCLUDED_GRAPHICS_VIEWPORT

#include <boost/shared_ptr.hpp>
#include <map>

namespace Graphics
{

class Viewport
{
public:
    Viewport(int x, int y, int w, int h)
        : x_(x), y_(y), w_(w), h_(h) {}
    void Set();
private:
    int x_, y_;
    int w_, h_;
};

typedef boost::shared_ptr<Viewport> ViewportPtr;


class ViewportManager
{
public:
    ViewportManager();
    int Add(ViewportPtr viewport);
    void Remove(int id);
    void Clear();
    ViewportPtr Get(int id) const;
private:
    typedef std::map<int, ViewportPtr> Viewports;
    Viewports viewports_;
    static int unique_id_;
};

typedef boost::shared_ptr<ViewportManager> ViewportManagerPtr;


}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_VIEWPORT
