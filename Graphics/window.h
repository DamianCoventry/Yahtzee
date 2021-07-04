#ifndef INCLUDED_GRAPHICS_WINDOW
#define INCLUDED_GRAPHICS_WINDOW

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include "../Utility/logcontext.h"

namespace Utility
{
class HwConfig;
}

namespace Graphics
{

struct Vector;

struct WindowEventHandler
{
    virtual void OnWindowCreated() {}
    virtual void OnWindowDeleted() {}
    virtual bool OnWindowQueryClose() { return true; }
    virtual void OnWindowActivated() {}
    virtual void OnWindowDeactivated() {}
    virtual void OnWindowPaint(HDC dc) {}
    virtual void OnWindowEraseBackground() {}
    virtual void OnWindowMouseMove(const Vector& pos) {}
    virtual void OnWindowMouseLeave() {}
};

class Window :
    public Utility::LogContext

{
public:
    static void Register(HINSTANCE instance);
    static void Unregister(HINSTANCE instance);

public:
    Window(HINSTANCE instance, WindowEventHandler* event_handler, const std::string& window_title);
    ~Window();

    enum Style
    { STYLE_FULLSCREEN, STYLE_WINDOWED };

    void Create(int w, int h, Style style);
    void Create(Utility::HwConfig* hw_config);
    void Delete();

    void Show();
    void Hide();

    bool IsMinimised() const    { return IsIconic(handle_) == TRUE; }
    bool IsForeground() const   { return GetForegroundWindow() == handle_; }

    void SetStyle(Style style);
    Style GetStyle() const { return style_; }
    void ToggleStyle();

    void AllowCursor();
    void DisallowCursor();
    bool IsCursorAllowed() const { return cursor_allowed_; }

    HWND GetHandle() const { return handle_; }

private:
    static const char* window_class_;
    static unsigned long ex_style_fullscreen_;
    static unsigned long ex_style_windowed_;
    static unsigned long style_fullscreen_;
    static unsigned long style_windowed_;

    static LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    WindowEventHandler* event_handler_;
    HINSTANCE instance_;
    HWND handle_;
    Style style_;
    int width_, height_;
    bool cursor_allowed_;
    bool tracking_;
    std::string window_title_;
};

typedef boost::shared_ptr<Window> WindowPtr;

}       // namespace Grahics

#endif  // INCLUDED_GRAPHICS_WINDOW
