#include "stdafx.h"
#include "font.h"
#include "device.h"
#include <windowsx.h>
#include <gl/gl.h>
#include <sstream>
#include "vector.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

using namespace Graphics;

namespace
{
    const int num_characters_     = 96;
    const int first_character_    = 32;
}

Font::Font()
: Utility::LogContext("Graphics::Font")
, color_(1.0f, 1.0f, 1.0f)
, gl_list_(0)
, point_size_(0)
, window_(NULL)
, dc_(NULL)
, font_(NULL)
{
}

Font::~Font()
{
    Delete();
}

void Font::Create(HWND window, const std::string& face_name, int point_size, bool bold)
{
    LOG("Creating [" << face_name << "], point_size=[" << point_size << "]");

    window_ = window;
    dc_ = GetDC(window);

    font_ = CreateFont(
        -MulDiv(point_size, GetDeviceCaps(dc_, LOGPIXELSY), 72),
        0, 0, 0,
        bold ? FW_BOLD : FW_NORMAL,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        face_name.c_str());

    if(font_ == NULL)
    {
        ReleaseDC(window, dc_);

        THROW_BASIC("Couldn't create a font with the face name [" << face_name
            << "] and point size [" << point_size
            << "], GetLastError()=[" << GetLastError() << "]");
    }

    gl_list_ = glGenLists(num_characters_);

    HFONT old_font = SelectFont(dc_, font_);
    wglUseFontBitmaps(dc_, first_character_, num_characters_, gl_list_);
    SelectFont(dc_, old_font);

    face_name_ = face_name;
    point_size_ = point_size;
}

void Font::Delete()
{
    if(font_)
    {
        LOG("Deleting [" << face_name_ << "], point_size=[" << point_size_ << "]");

        if(window_ && dc_)
        {
            ReleaseDC(window_, dc_);
            window_ = NULL;
            dc_ = NULL;
        }
        if(gl_list_)
        {
            glDeleteLists(gl_list_, num_characters_);
            gl_list_ = 0;
        }

        DeleteFont(font_);
        font_ = NULL;
    }
}

void Font::Write(const std::string& text, DevicePtr device, const Vector& position)
{
    glDisable(GL_TEXTURE_2D);

    glColor3f(color_.GetRed(), color_.GetGreen(), color_.GetBlue());
    glRasterPos2f(position.x_, position.y_);

    glPushAttrib(GL_LIST_BIT);
        glListBase(gl_list_ - first_character_);
        glCallLists(int(text.size()), GL_UNSIGNED_BYTE, text.c_str());
    glPopAttrib();

    glEnable(GL_TEXTURE_2D);
}

float Font::GetPixelWidth(const std::string& text) const
{
    SIZE size;
    HFONT old_font = SelectFont(dc_, font_);
    GetTextExtentPoint32(dc_, text.c_str(), int(text.length()), &size);
    SelectFont(dc_, old_font);
    return float(size.cx);
}

float Font::GetPixelHeight(const std::string& text) const
{
    SIZE size;
    HFONT old_font = SelectFont(dc_, font_);
    GetTextExtentPoint32(dc_, text.c_str(), int(text.length()), &size);
    SelectFont(dc_, old_font);
    return float(size.cy);
}

int Font::GetNumChars(const std::string& text, float pixel_width) const
{
    if(text.empty())
    {
        return 0;
    }
    std::string::size_type num_chars = text.length();
    while(GetPixelWidth(text.substr(0, num_chars)) >= pixel_width)
    {
        num_chars--;
        if(num_chars == 0)
        {
            break;
        }
    }
    return int(num_chars);
}
