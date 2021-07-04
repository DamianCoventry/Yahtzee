#ifndef INCLUDED_GRAPHICS_SPLASHSCREEN
#define INCLUDED_GRAPHICS_SPLASHSCREEN

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Graphics
{

class SplashScreen
{
public:
    SplashScreen(HINSTANCE instance, int dialog_id);
    ~SplashScreen();

    void Show();
    void Hide();
    void Center();

    void SetPos(int pos);
    void SetTotalItems(int total);

private:
    static INT_PTR CALLBACK SplashDialogProc(HWND dialog, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    HWND handle_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPLASHSCREEN
