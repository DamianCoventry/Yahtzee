#include "stdafx.h"
#include "spriteslider.h"
#include "spritecontainer.h"
#include "image.h"
#include "device.h"
#include "color.h"
#include "../Input/device.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

using namespace Graphics;

SpriteSlider::SpriteSlider(SpriteEventHandler* event_handler)
: LogContext("Graphics::SpriteSlider")
, Sprite(event_handler)
, min_range_(0)
, max_range_(100)
, pos_(50)
, small_step_(1)
, large_step_(10)
{
    LOG("Creating...");
    UpdateGrabberXCoord();
}

SpriteSlider::~SpriteSlider()
{
    LOG("Deleting...");
}

void SpriteSlider::Think(float time_delta)
{
}

void SpriteSlider::Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus)
{
    if(!IsEnabled())
    {
        if(image_back_disabled_)
        {
            image_back_disabled_->Draw(time_delta, device, GetPosition());
        }
        if(image_grabber_disabled_)
        {
            image_grabber_disabled_->Draw(time_delta, device, GetPosition()+grabber_offset_);
        }
    }
    else
    {
        if(image_back_normal_)
        {
            image_back_normal_->Draw(time_delta, device, GetPosition());
        }
        if(image_grabber_normal_)
        {
            image_grabber_normal_->Draw(time_delta, device, GetPosition()+grabber_offset_);
        }

        if(sprite_focus == shared_from_this())
        {
            device->DrawFocusRect(GetPosition() + GetBoundingRectMin(),
                                  GetPosition() + GetBoundingRectMax(),
                                  Color(0.859375f, 0.85546875f, 0.6328125f));
        }
    }
}

void SpriteSlider::OnMouseMoved(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    if(sprites_clickable->GetCapture() == shared_from_this())
    {
        UpdateGrabberPos(sprite_cursor);
    }
}

void SpriteSlider::OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    sprites_clickable->SetCapture(shared_from_this());
    UpdateGrabberPos(sprite_cursor);
}

void SpriteSlider::OnMouseButtonReleased(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    if(sprites_clickable->GetCapture() == shared_from_this())
    {
        GetSpriteEventHandler()->OnSpriteSliderReleased(shared_from_this());
        sprites_clickable->ReleaseCapture();
    }
}

void SpriteSlider::OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    switch(key)
    {
    case DIK_DOWN:
    case DIK_LEFT:
        pos_ -= small_step_;
        if(pos_ < min_range_)
        {
            pos_ = min_range_;
        }
        UpdateGrabberXCoord();
        break;
    case DIK_UP:
    case DIK_RIGHT:
        pos_ += small_step_;
        if(pos_ > max_range_)
        {
            pos_ = max_range_;
        }
        UpdateGrabberXCoord();
        break;
    case DIK_PGDN:
        pos_ -= large_step_;
        if(pos_ < min_range_)
        {
            pos_ = min_range_;
        }
        UpdateGrabberXCoord();
        break;
    case DIK_PGUP:
        pos_ += large_step_;
        if(pos_ > max_range_)
        {
            pos_ = max_range_;
        }
        UpdateGrabberXCoord();
        break;
    case DIK_HOME:
        pos_ = max_range_;
        UpdateGrabberXCoord();
        break;
    case DIK_END:
        pos_ = min_range_;
        UpdateGrabberXCoord();
        break;
    }
}

void SpriteSlider::SetRange(int min_range, int max_range)
{
    min_range_ = min_range;
    max_range_ = max_range;
    if(pos_ < min_range_)
    {
        pos_ = min_range_;
    }
    else if(pos_ > max_range_)
    {
        pos_ = max_range_;
    }
    UpdateGrabberXCoord();
}

void SpriteSlider::SetPos(int pos)
{
    pos_ = pos;
    if(pos_ < min_range_)
    {
        pos_ = min_range_;
    }
    else if(pos_ > max_range_)
    {
        pos_ = max_range_;
    }
    UpdateGrabberXCoord();
}

void SpriteSlider::SetGrabberYCoord(float y_coord)
{
    if(image_grabber_normal_)
    {
        grabber_offset_.y_ = y_coord - float(image_grabber_normal_->GetHeight()/2);
        UpdateGrabberXCoord();
    }
}

void SpriteSlider::UpdateGrabberXCoord()
{
    if(image_grabber_normal_)
    {
        // Convert the logical position to a physical x coordinate.
        int pos_range = max_range_ - min_range_;

        float percent = float(pos_) / float(pos_range);
        float x_range = grabber_x_coords_.y_ - grabber_x_coords_.x_;

        grabber_offset_.x_ = (grabber_x_coords_.x_ + (x_range * percent)) - float(image_grabber_normal_->GetWidth()/2);
    }
}

void SpriteSlider::UpdateGrabberPos(SpritePtr sprite_cursor)
{
    float half_x = float(image_grabber_normal_->GetWidth() / 2);
    float min_x = GetPosition().x_ + grabber_x_coords_.x_;
    float max_x = GetPosition().x_ + grabber_x_coords_.y_;

    if(sprite_cursor->GetPosition().x_ >= min_x && sprite_cursor->GetPosition().x_ <= max_x)
    {
        grabber_offset_.x_ = (sprite_cursor->GetPosition().x_ - GetPosition().x_) - half_x;

        // Convert the physical x coordinate to a logical position.
        float x_range = grabber_x_coords_.y_ - grabber_x_coords_.x_;
        float percent = ((grabber_offset_.x_+half_x) - grabber_x_coords_.x_) / x_range;

        int pos_range = max_range_ - min_range_;

        pos_ = min_range_ + int(percent * float(pos_range));

        GetSpriteEventHandler()->OnSpriteSliderMoved(shared_from_this());
    }
}
