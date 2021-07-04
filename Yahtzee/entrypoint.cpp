#include "stdafx.h"
#include "yahtzee.h"
#include "../Graphics/splashscreen.h"
#include "resource.h"
#include <exception>
#include <commctrl.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, INT cmd_show)
{
    try
    {
        INITCOMMONCONTROLSEX icc;
        icc.dwSize  = sizeof(INITCOMMONCONTROLSEX);
        icc.dwICC   = ICC_PROGRESS_CLASS;
        InitCommonControlsEx(&icc);

        Graphics::SplashScreen splash(instance, IDD_SPLASH);
        splash.Show();

        Yahtzee yahtzee(instance, splash);
        yahtzee.Initialise();

        splash.Hide();

        yahtzee.Run();
        yahtzee.Shutdown();
    }
    catch(std::exception& e)
    {
        MessageBox(GetDesktopWindow(), e.what(), "std::exception", MB_OK | MB_ICONINFORMATION);
    }
    return 0;
}
