#include "stdafx.h"
#include "blackwindow.h"

using namespace Graphics;

const char* BlackWindow::class_name_ = "BlackWindowClass";

BlackWindow::BlackWindow(HINSTANCE instance)
{
    WNDCLASS wndclass;
    wndclass.style          = CS_OWNDC;
    wndclass.lpfnWndProc    = DefWindowProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = instance;
    wndclass.hIcon          = NULL;
    wndclass.hCursor        = NULL;
    wndclass.hbrBackground  = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wndclass.lpszMenuName   = NULL;
    wndclass.lpszClassName  = class_name_;

    RegisterClass(&wndclass);

    handle_ = CreateWindowEx(WS_EX_TOPMOST, class_name_, class_name_, WS_POPUP, 0, 0,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, instance, NULL);

    ShowWindow(handle_, SW_SHOW);
    UpdateWindow(handle_);
    SetForegroundWindow(handle_);
}

BlackWindow::~BlackWindow()
{
    DestroyWindow(handle_);
}
