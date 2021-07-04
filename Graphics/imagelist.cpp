#include "stdafx.h"
#include "imagelist.h"
#include "device.h"
#include "vector.h"
#include "image.h"
#include "bitmap.h"
#include "targa.h"
#include <boost/algorithm/string.hpp>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

using namespace Graphics;

ImageList::ImageList()
: LogContext("Graphics::ImageList")
, current_frame_(0)
, num_frames_(0)
, num_frames_in_row_(0)
, frame_width_(0.0f)
, frame_height_(0.0f)
, frame_s_(1.0f)
, frame_t_(1.0f)
{
}

ImageList::~ImageList()
{
    Delete();
}

void ImageList::Create(const std::string& filename, int num_frames, float frame_width, float frame_height, bool transparent, bool flip_rgb)
{
    LOG("Creating a [" << frame_width << "x" << frame_height << "] imagelist from [" << filename << "]");

    int image_width, image_height;
    if(!boost::algorithm::ifind_first(filename, ".bmp").empty())
    {
        Bitmap image_file;
        image_file.Load(filename);
        transparent ? image_file.SetTransparentAlpha(flip_rgb) : image_file.SetOpaqueAlpha(flip_rgb);
        image_width     = image_file.GetWidth();
        image_height    = image_file.GetHeight();

        texture_.reset(new Texture);
        texture_->Create(image_width, image_height, image_file.GetPixels());
    }
    else if(!boost::algorithm::ifind_first(filename, ".tga").empty())
    {
        Targa image_file;
        image_file.Load(filename);
        image_width     = image_file.GetWidth();
        image_height    = image_file.GetHeight();

        texture_.reset(new Texture);
        texture_->Create(image_width, image_height, image_file.GetPixels());
    }
    else
    {
        throw std::runtime_error("Unsupported image image_file type");
    }

    num_frames_         = num_frames;
    frame_width_        = frame_width;
    frame_height_       = frame_height;
    num_frames_in_row_  = image_width / int(frame_width_);
    frame_s_            = frame_width_ / float(image_width);
    frame_t_            = frame_height_ / float(image_height);
}

void ImageList::Delete()
{
    LOG("Deleting a [" << frame_width_ << "x" << frame_height_ << "] imagelist");
    current_frame_  = 0;
    num_frames_     = 0;
    num_frames_in_row_ = 0;
    frame_width_    = 0.0f;
    frame_height_   = 0.0f;
    frame_s_        = 1.0f;
    frame_t_        = 1.0f;
    texture_.reset();
}

void ImageList::Draw(float time_delta, DevicePtr device, const Vector& position)
{
    Draw(time_delta, device, position, current_frame_);
}

void ImageList::Draw(float time_delta, DevicePtr device, const Vector& position, int image)
{
    if(image >= 0 && image < num_frames_ && texture_ && texture_->GetId())
    {
        int col = image % num_frames_in_row_;
        int row = image / num_frames_in_row_;

        float s0 = float(col) * frame_s_;
        float s1 = s0 + frame_s_;
        float t0 = float(row) * frame_t_;
        float t1 = t0 + frame_t_;

        glBindTexture(GL_TEXTURE_2D, texture_->GetId());
        glBegin(GL_QUADS);
            glColor3f(1.0f, 1.0f, 1.0f);
            glTexCoord2f(s0, t0); glVertex2f(position.x_, position.y_);
            glTexCoord2f(s1, t0); glVertex2f(position.x_ + frame_width_, position.y_);
            glTexCoord2f(s1, t1); glVertex2f(position.x_ + frame_width_, position.y_ + frame_height_);
            glTexCoord2f(s0, t1); glVertex2f(position.x_, position.y_ + frame_height_);
        glEnd();
    }
}

void ImageList::IncCurrentFrame()
{
    current_frame_++;
    if(current_frame_ >= num_frames_)
    {
        current_frame_ = 0;
    }
}

void ImageList::DecCurrentFrame()
{
    current_frame_--;
    if(current_frame_ < 0)
    {
        current_frame_ = num_frames_ - 1;
    }
}

void ImageList::SetCurrentFrame(int image)
{
    if(image >= 0 && image < num_frames_)
    {
        current_frame_ = image;
    }
}
