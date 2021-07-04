#include "stdafx.h"
#include "window.h"
#include <windowsx.h>
#include <commctrl.h>
#include <stdexcept>
#include <sstream>
#include "vector.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Utility/hwconfig.h"
#include "../Yahtzee/resource.h"

#pragma warning(disable : 4311)         // pointer truncation from 'LPVOID' to 'LONG'
#pragma warning(disable : 4312)         // conversion from 'LONG' to 'Graphics::Window *' of greater size

using namespace Graphics;

const char* Window::window_class_           = "GameWindowClass";
unsigned long Window::ex_style_fullscreen_  = WS_EX_TOPMOST;
unsigned long Window::ex_style_windowed_    = WS_EX_WINDOWEDGE | WS_EX_APPWINDOW;
unsigned long Window::style_fullscreen_     = WS_POPUP;
unsigned long Window::style_windowed_       = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

void Window::Register(HINSTANCE instance)
{
    WNDCLASSEX wndclass;
    wndclass.cbSize         = sizeof(WNDCLASSEX);
    wndclass.style          = CS_OWNDC;
    wndclass.lpfnWndProc    = WindowProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = instance;
    wndclass.hIcon          = LoadIcon(instance, MAKEINTRESOURCE(IDI_MAIN));
    wndclass.hCursor        = NULL;//LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = NULL;
    wndclass.lpszMenuName   = NULL;
    wndclass.lpszClassName  = window_class_;
    wndclass.hIconSm        = NULL;

    if(!RegisterClassEx(&wndclass))
    {
        std::ostringstream oss;
        oss << "Couldn't register the main window's Window Class, GetLastError()=[" << GetLastError() << "]";
        throw std::runtime_error(oss.str());
    }
}

void Window::Unregister(HINSTANCE instance)
{
    UnregisterClass(window_class_, instance);
}



Window::Window(HINSTANCE instance, WindowEventHandler* event_handler, const std::string& window_title)
: Utility::LogContext("Graphics::Window")
, handle_(NULL)
, instance_(instance)
, style_(STYLE_WINDOWED)
, cursor_allowed_(false)
, window_title_(window_title)
, event_handler_(event_handler)
, tracking_(false)
{
}

Window::~Window()
{
}

void Window::Create(int w, int h, Style style)
{
    LOG("Creating a [" << (style == STYLE_FULLSCREEN ? "STYLE_FULLSCREEN" : "STYLE_WINDOWED") << "] window, size[" << w << "x" << h << "]");

    style_  = style;
    width_  = w;
    height_ = h;

    unsigned long ex_style, basic_style;
    int x_coord, y_coord, width, height;

    if(style == STYLE_FULLSCREEN)
    {
        ex_style    = ex_style_fullscreen_;
        basic_style = style_fullscreen_;
        x_coord     = 0;
        y_coord     = 0;
        width       = w;
        height      = h;
    }
    else // it must be STYLE_WINDOWED
    {
        ex_style    = ex_style_windowed_;
        basic_style = style_windowed_;

        RECT rect;
        rect.left   = 0;
        rect.top    = 0;
        rect.right  = w;
        rect.bottom = h;
        AdjustWindowRectEx(&rect, basic_style, FALSE, ex_style);

        width       = rect.right - rect.left;
        height      = rect.bottom - rect.top;
        x_coord     = (GetSystemMetrics(SM_CXSCREEN)/2) - (width/2);
        y_coord     = (GetSystemMetrics(SM_CYSCREEN)/2) - (height/2);
    }

    handle_ = CreateWindowEx(ex_style, window_class_, window_title_.c_str(), basic_style,
        x_coord, y_coord, width, height, NULL, NULL, instance_, reinterpret_cast<void*>(this));

    if(handle_ == NULL)
    {
        THROW_WIN32("Couldn't create the main window");
    }
}

void Window::Create(Utility::HwConfig* hw_config)
{
    Create(hw_config->GetWindowWidth(), hw_config->GetWindowHeight(),
        hw_config->IsFullscreen() ? STYLE_FULLSCREEN : STYLE_WINDOWED);
}

void Window::Delete()
{
    if(handle_)
    {
        LOG("Deleting window");
        DestroyWindow(handle_);
        handle_ = NULL;
    }
}

void Window::Show()
{
    LOG("Showing window");
    ShowWindow(handle_, SW_SHOW);
    UpdateWindow(handle_);
    SetForegroundWindow(handle_);
}

void Window::Hide()
{
    LOG("Hiding window");
    ShowWindow(handle_, SW_HIDE);
}

void Window::SetStyle(Style style)
{
    style_ = style;

    unsigned long ex_style, basic_style;
    int x_coord, y_coord, width, height;

    if(style == STYLE_FULLSCREEN)
    {
        LOG("Setting [STYLE_FULLSCREEN] style");
        ex_style    = ex_style_fullscreen_;
        basic_style = style_fullscreen_;
        x_coord     = 0;
        y_coord     = 0;
        width       = width_;
        height      = height_;
    }
    else // it must be STYLE_WINDOWED
    {
        LOG("Setting [STYLE_WINDOWED] style");
        ex_style    = ex_style_windowed_;
        basic_style = style_windowed_;

        RECT rect;
        rect.left   = 0;
        rect.top    = 0;
        rect.right  = width_;
        rect.bottom = height_;
        AdjustWindowRectEx(&rect, basic_style, FALSE, ex_style);

        width       = rect.right - rect.left;
        height      = rect.bottom - rect.top;
        x_coord     = (GetSystemMetrics(SM_CXSCREEN)/2) - (width/2);
        y_coord     = (GetSystemMetrics(SM_CYSCREEN)/2) - (height/2);
    }

    ShowWindow(handle_, SW_HIDE);
    SetWindowLongPtr(handle_, GWL_STYLE, basic_style);
    SetWindowLongPtr(handle_, GWL_EXSTYLE, ex_style);
    SetWindowPos(handle_, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    SetWindowPos(handle_, NULL, x_coord, y_coord, width, height, SWP_NOZORDER | SWP_NOREDRAW);
    Show();
}

void Window::ToggleStyle()
{
    SetStyle(style_ == STYLE_FULLSCREEN ? STYLE_WINDOWED : STYLE_FULLSCREEN);
}

void Window::AllowCursor()
{
    LOG("Allowing cursor to show");

    //LOG("GetLastError() before ClipCursor() is [" << GetLastError() << "]");
    //ClipCursor(NULL);
    //LOG("GetLastError() after ClipCursor() is [" << GetLastError() << "]");

    //while(ShowCursor(TRUE) < 0) {}

    cursor_allowed_ = true;
}

void Window::DisallowCursor()
{
    LOG("Disallowing cursor to show");

    //RECT rect;
    //GetClientRect(handle_, &rect);
    //ClientToScreen(handle_, (POINT*)&rect.left);
    //ClientToScreen(handle_, (POINT*)&rect.right);
    //LOG("GetLastError() before ClipCursor() is [" << GetLastError() << "]");
    //ClipCursor(&rect);
    //LOG("GetLastError() after ClipCursor() is [" << GetLastError() << "]");

    //while(ShowCursor(FALSE) >= 0) {}

    //GetWindowRect(handle_, &rect);
    //SetCursorPos(rect.left + ((rect.right  - rect.left)/2),
    //             rect.top  + ((rect.bottom - rect.top )/2));

    cursor_allowed_ = false;
}

LRESULT CALLBACK Window::WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LONG_PTR data = GetWindowLongPtr(window, GWLP_USERDATA);
    Window* this_ = reinterpret_cast<Window*>(data);

    switch(msg)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(window, &ps);
            this_->event_handler_->OnWindowPaint(dc);
            EndPaint(window, &ps);
            break;
        }
    case WM_ERASEBKGND:
        this_->event_handler_->OnWindowEraseBackground();
        return TRUE;
    case WM_MOUSEMOVE:
        {
            //if(GetForegroundWindow() == window)
            //{
            //    RECT rect;
            //    GetWindowRect(window, &rect);
            //    SetCursorPos(rect.left + ((rect.right  - rect.left)/2),
            //                 rect.top  + ((rect.bottom - rect.top )/2));
            //}
            if(!this_->tracking_)
            {
                this_->tracking_ = true;
                TRACKMOUSEEVENT tme;
                tme.cbSize      = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags     = TME_LEAVE;
                tme.hwndTrack   = window;
                _TrackMouseEvent(&tme);
            }
            int x = GET_X_LPARAM(lparam);
            int y = GET_Y_LPARAM(lparam);
            this_->event_handler_->OnWindowMouseMove(Vector(float(x), float(y)));
            break;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* create_info = reinterpret_cast<CREATESTRUCT*>(lparam);
            SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create_info->lpCreateParams));

            this_ = reinterpret_cast<Window*>(create_info->lpCreateParams);
            this_->event_handler_->OnWindowCreated();
            break;
        }
    case WM_DESTROY:
        this_->event_handler_->OnWindowDeleted();
        break;
    case WM_CLOSE:
        if(this_->event_handler_->OnWindowQueryClose())
        {
            break;      // Let DefWindowProc() call DestroyWindow()
        }
        return 0;
    case WM_ACTIVATE:
        if(LOWORD(wparam) == WA_INACTIVE)
        {
            this_->event_handler_->OnWindowDeactivated();
        }
        else
        {
            this_->event_handler_->OnWindowActivated();
        }
        break;
    case WM_SETCURSOR:
        SetCursor(NULL);
        break;
    case WM_NCACTIVATE:
        if(wparam == FALSE)
        {
            this_->event_handler_->OnWindowDeactivated();
        }
        else
        {
            this_->event_handler_->OnWindowActivated();
        }
        break;
    case WM_MOUSELEAVE:
        this_->event_handler_->OnWindowMouseLeave();
        this_->tracking_ = false;
        break;
    }

    return DefWindowProc(window, msg, wparam, lparam);
}
