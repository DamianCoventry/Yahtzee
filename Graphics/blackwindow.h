#ifndef INCLUDED_GRAPHICS_BLACKWINDOW
#define INCLUDED_GRAPHICS_BLACKWINDOW

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <boost/shared_ptr.hpp>

namespace Graphics
{

class BlackWindow
{
public:
    BlackWindow(HINSTANCE instance);
    ~BlackWindow();
    HWND GetHandle() const { return handle_; }
private:
    HINSTANCE instance_;
    HWND handle_;
    static const char* class_name_;
};

typedef boost::shared_ptr<BlackWindow> BlackWindowPtr;

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_BLACKWINDOW
