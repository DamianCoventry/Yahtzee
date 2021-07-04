#include "stdafx.h"
#include "splashscreen.h"
#include <commctrl.h>
#include "../Yahtzee/resource.h"

using namespace Graphics;

SplashScreen::SplashScreen(HINSTANCE instance, int dialog_id)
{
    handle_ = CreateDialog(instance, MAKEINTRESOURCE(dialog_id), NULL, SplashDialogProc);
}

SplashScreen::~SplashScreen()
{
    DestroyWindow(handle_);
}

void SplashScreen::Show()
{
    ShowWindow(handle_, SW_SHOW);
    UpdateWindow(handle_);
}

void SplashScreen::Hide()
{
    ShowWindow(handle_, SW_HIDE);
}

void SplashScreen::Center()
{
    RECT rect;
    GetWindowRect(handle_, &rect);

    int x = (GetSystemMetrics(SM_CXSCREEN)/2) - ((rect.right - rect.left)/2);
    int y = (GetSystemMetrics(SM_CYSCREEN)/2) - ((rect.bottom - rect.top)/2);

    SetWindowPos(handle_, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE);

    InvalidateRect(handle_, NULL, FALSE);
    UpdateWindow(handle_);
}

void SplashScreen::SetPos(int pos)
{
    SendDlgItemMessage(handle_, IDC_PROGRESS, PBM_SETPOS, pos, 0);
}

void SplashScreen::SetTotalItems(int total)
{
    SendDlgItemMessage(handle_, IDC_PROGRESS, PBM_SETRANGE, 0, MAKELPARAM(0, total));
}

INT_PTR CALLBACK SplashScreen::SplashDialogProc(HWND dialog, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        return TRUE;
    }
    return FALSE;
}

