#include "stdafx.h"
#include "device.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "vector.h"
#include "color.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Utility/hwconfig.h"

using namespace Graphics;

Device::Device()
: LogContext("Graphics::Device")
, dc_(NULL)
, rc_(NULL)
{
}

Device::~Device()
{
}

void Device::Initialise(HWND window, Utility::HwConfig* hw_config)
{
    LOG("Initialising graphics sub system");

    PIXELFORMATDESCRIPTOR format;
    int index;

    dc_ = GetDC(window);
    if(NULL == dc_)
    {
        THROW_WIN32("Couldn't get a DC for the window");
    }

    memset(&format, 0, sizeof(PIXELFORMATDESCRIPTOR));

    format.nSize         = sizeof(PIXELFORMATDESCRIPTOR);
    format.nVersion      = 1;
    format.dwFlags       = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    format.iPixelType    = PFD_TYPE_RGBA;
    format.cColorBits    = hw_config->GetBpp();
    format.cDepthBits    = hw_config->GetDepthBits();
    format.iLayerType    = PFD_MAIN_PLANE;

    index = ChoosePixelFormat(dc_, &format);
    if(!index)
    {
        THROW_WIN32("This graphics adapter doesn't support the required pixel format");
    }

    LOG("Setting pixel format to 32 BPP, 24bit depth buffer");
    if(!SetPixelFormat(dc_, index, &format))
    {
        THROW_WIN32("This graphics adapter couldn't set the required pixel format");
    }

    rc_ = wglCreateContext(dc_);
    if(NULL == rc_)
    {
        THROW_WIN32("Couldn't create an RC for the window");
    }

    if(!wglMakeCurrent(dc_, rc_))
    {
        THROW_WIN32("Couldn't make our RC current for the window");
    }

    hw_config_ = hw_config;

    int max_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
    LOG("GL_MAX_TEXTURE_SIZE = " << max_size);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Device::Shutdown(HWND window)
{
    LOG("Shutting down graphics sub system");

    if(rc_ && dc_)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(rc_);

        ReleaseDC(window, dc_);
    }
}

void Device::Set3D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60, float(hw_config_->GetWindowWidth()) / float(hw_config_->GetWindowHeight()), 1.0f, 10000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Device::Set2D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0f, float(hw_config_->GetWindowWidth()), float(hw_config_->GetWindowHeight()), 0.0f, -100.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Device::OnBeginFrame()
{
    //glClear(GL_COLOR_BUFFER_BIT);
}

void Device::OnEndFrame()
{
    SwapBuffers(dc_);
}

void Device::DrawEmptyRect(const Vector& tl, const Vector& br, const Color& color)
{
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINE_LOOP);
        glColor3f(color.GetRed(), color.GetGreen(), color.GetBlue());
        glVertex2f(tl.x_, tl.y_);
        glVertex2f(br.x_, tl.y_);
        glVertex2f(br.x_, br.y_);
        glVertex2f(tl.x_, br.y_);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void Device::DrawFilledRect(const Vector& tl, const Vector& br, const Color& color)
{
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glColor3f(color.GetRed(), color.GetGreen(), color.GetBlue());
        glVertex2f(tl.x_, tl.y_);
        glVertex2f(br.x_, tl.y_);
        glVertex2f(br.x_, br.y_);
        glVertex2f(tl.x_, br.y_);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void Device::DrawFocusRect(const Vector& tl, const Vector& br, const Color& color)
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0f0f);
    glBegin(GL_LINE_LOOP);
        glColor3f(color.GetRed(), color.GetGreen(), color.GetBlue());
        glVertex2f(tl.x_ + 1.0f, tl.y_);
        glVertex2f(br.x_, tl.y_);
        glVertex2f(br.x_, br.y_-1.0f);
        glVertex2f(tl.x_ + 1.0f, br.y_-1.0f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glEnable(GL_TEXTURE_2D);
}
