#include "stdafx.h"
#include "image.h"
#include "device.h"
#include "vector.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

using namespace Graphics;

Image::Image()
: LogContext("Graphics::Image")
, frame_width_(0.0f)
, frame_height_(0.0f)
, s0_(0.0f)
, s1_(1.0f)
, t0_(0.0f)
, t1_(1.0f)
{
}

Image::~Image()
{
}

void Image::Create(int frame_x, int frame_y, float frame_width, float frame_height, TexturePtr texture)
{
    LOG("Creating a [" << frame_width << "x" << frame_height << "] image");

    texture_        = texture;
    frame_width_    = frame_width;
    frame_height_   = frame_height;

    s0_ = float(frame_x) / float(texture->GetWidth());
    s1_ = (float(frame_x)+frame_width) / float(texture->GetWidth());
    t0_ = float(frame_y) / float(texture->GetHeight());
    t1_ = (float(frame_y)+frame_height) / float(texture->GetHeight());
}

void Image::Draw(float time_delta, DevicePtr device, const Vector& position)
{
    if(texture_ && texture_->GetId())
    {
        glBindTexture(GL_TEXTURE_2D, texture_->GetId());
        glBegin(GL_QUADS);
            glColor3f(1.0f, 1.0f, 1.0f);
            glTexCoord2f(s0_, t0_);
            glVertex2f(position.x_, position.y_);
            glTexCoord2f(s1_, t0_);
            glVertex2f(position.x_ + frame_width_, position.y_);
            glTexCoord2f(s1_, t1_);
            glVertex2f(position.x_ + frame_width_, position.y_ + frame_height_);
            glTexCoord2f(s0_, t1_);
            glVertex2f(position.x_, position.y_ + frame_height_);
        glEnd();
    }
}



ImagePtr Graphics::MakeImage(int frame_x, int frame_y, float frame_width, float frame_height, TexturePtr texture)
{
    ImagePtr image(new Image);
    image->Create(frame_x, frame_y, frame_width, frame_height, texture);
    return image;
}
