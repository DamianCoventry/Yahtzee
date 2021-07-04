#include "stdafx.h"
#include "displaymode.h"
#include "../Utility/exception.h"
#include "../Utility/logmacros.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace Graphics;

DisplayMode::DisplayMode(int w, int h, int bpp, int hz)
: Utility::LogContext("Graphics::DisplayMode")
, width_(w)
, height_(h)
, bpp_(bpp)
, hz_(hz)
{
}

void DisplayMode::Set()
{
    DEVMODE dev_mode;
    memset(&dev_mode, 0, sizeof(DEVMODE));
    dev_mode.dmSize             = sizeof(DEVMODE);
    dev_mode.dmPelsWidth        = width_;
    dev_mode.dmPelsHeight       = height_;
    dev_mode.dmBitsPerPel       = bpp_;
    dev_mode.dmDisplayFrequency = hz_;
    dev_mode.dmFields           = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY | DM_BITSPERPEL;

    LOG("Setting mode [" << width_ << "x" << height_ << ", " << bpp_ << " BPP, " << hz_ << " Hz]");

    if(::ChangeDisplaySettings(&dev_mode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
        THROW_WIN32("Couldn't set the display mode to [" << width_ << "x" << height_ << ", " << bpp_ << " BPP, " << hz_ << " Hz]");
    }
}

void Graphics::RestoreDisplayMode()
{
    // Passing these parameters pulls previously set values out of the registry and uses
    // them to set the display mode.
    ::ChangeDisplaySettings(NULL, 0);
}


Graphics::DisplayModes Graphics::GetDisplayModes()
{
    Graphics::DisplayModes modes;
    DEVMODE dev_mode;

    int i = 0;
    while(EnumDisplaySettings(NULL, i, &dev_mode))
    {
        // I don't care about anything smaller than 640x480
        if(dev_mode.dmPelsWidth >= 640 && dev_mode.dmPelsHeight >= 480)
        {
            modes.push_back(DisplayMode(dev_mode.dmPelsWidth, dev_mode.dmPelsHeight,
                                        dev_mode.dmBitsPerPel, dev_mode.dmDisplayFrequency));
        }
        i++;
    }

    return modes;
}
