#include "stdafx.h"
#include "spritestatic.h"
#include "spritecontainer.h"
#include "image.h"
#include "device.h"
#include "color.h"
#include "font.h"
#include "imagelist.h"
#include "../Input/device.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

using namespace Graphics;

SpriteStatic::SpriteStatic(SpriteEventHandler* event_handler)
: LogContext("Graphics::SpriteStatic")
, Sprite(event_handler)
, current_frame_(0)
, left_margin_(10.0f)
, right_margin_(10.0f)
, top_margin_(20.0f)
, color_font_(1.0f, 1.0f, 1.0f)
{
    LOG("Creating...");
}

SpriteStatic::~SpriteStatic()
{
    LOG("Deleting...");
}

void SpriteStatic::SetImageList(ImageListPtr imagelist)
{
    imagelist_ = imagelist;
}

int SpriteStatic::GetImageCount() const
{
    if(imagelist_)
    {
        return imagelist_->GetImageCount();
    }
    return 0;
}

void SpriteStatic::Think(float time_delta)
{
}

void SpriteStatic::Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus)
{
    if(image_)
    {
        image_->Draw(time_delta, device, GetPosition());
    }
    else if(imagelist_ && current_frame_ >= 0 && current_frame_ < GetImageCount())
    {
        imagelist_->Draw(time_delta, device, GetPosition(), current_frame_);
    }

    if(font_)
    {
        Vector position(GetPosition() + Graphics::Vector(left_margin_, top_margin_));
        std::string text(text_.substr(0, num_chars_));

        font_->SetColor(IsEnabled() ? color_font_ : Graphics::Color(0.0f));
        font_->Write(text, device, position);
    }
}

void SpriteStatic::OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    GetSpriteEventHandler()->OnSpriteClicked(shared_from_this());
}

void SpriteStatic::IncrementCurrentFrame_Wrap()
{
    if(imagelist_)
    {
        ++current_frame_;
        if(current_frame_ >= imagelist_->GetImageCount())
        {
            current_frame_ = 0;
        }
    }
}

bool SpriteStatic::IncrementCurrentFrame_Clamp()
{
    if(imagelist_)
    {
        ++current_frame_;
        if(current_frame_ >= imagelist_->GetImageCount())
        {
            current_frame_ = imagelist_->GetImageCount()-1;
            return false;
        }
    }
    return true;
}

void SpriteStatic::SetText(const std::string& text)
{
    text_ = text;
    if(font_ && !text_.empty())
    {
        num_chars_ = font_->GetNumChars(text_, GetBoundingRectMax().x_-(left_margin_+right_margin_));
    }
}
