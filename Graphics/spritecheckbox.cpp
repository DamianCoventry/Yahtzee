#include "stdafx.h"
#include "spritecheckbox.h"
#include "spritecontainer.h"
#include "image.h"
#include "device.h"
#include "color.h"
#include "../Input/device.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Audio/sound.h"

using namespace Graphics;

SpriteCheckBox::SpriteCheckBox(SpriteEventHandler* event_handler)
: LogContext("Graphics::SpriteCheckBox")
, Sprite(event_handler)
, pressed_(false)
, is_hovering_(false)
, key_held_(false)
, checked_(UNCHECKED)
{
    LOG("Creating...");
}

SpriteCheckBox::~SpriteCheckBox()
{
    LOG("Deleting...");
}

void SpriteCheckBox::Think(float time_delta)
{
}

void SpriteCheckBox::Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus)
{
    if(!IsEnabled())
    {
        if(image_disabled_[checked_])
        {
            image_disabled_[checked_]->Draw(time_delta, device, GetPosition());
        }
    }
    else
    {
        if(pressed_)
        {
            if(is_hovering_ || key_held_)
            {
                // Draw a pressed button.
                if(image_down_[checked_])
                {
                    image_down_[checked_]->Draw(time_delta, device, GetPosition());
                }
            }
            else
            {
                // Draw an unpressed button.
                if(image_up_[checked_])
                {
                    image_up_[checked_]->Draw(time_delta, device, GetPosition());
                }
            }
        }
        else if(is_hovering_)
        {
            // Draw an unpressed button with a hover highlight.
            if(image_hover_[checked_])
            {
                image_hover_[checked_]->Draw(time_delta, device, GetPosition());
            }
        }
        else
        {
            // Draw an unpressed button.
            if(image_up_[checked_])
            {
                image_up_[checked_]->Draw(time_delta, device, GetPosition());
            }
        }

        if(sprite_focus == shared_from_this())
        {
            device->DrawFocusRect(GetPosition() + GetBoundingRectMin(),
                                  GetPosition() + GetBoundingRectMax(),
                                  Color(0.859375f, 0.85546875f, 0.6328125f));
        }
    }
}

void SpriteCheckBox::OnMouseEnter(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    is_hovering_ = true;
}

void SpriteCheckBox::OnMouseLeave(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    is_hovering_ = false;
}

void SpriteCheckBox::OnMouseMoved(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    is_hovering_ = IsPointColliding(sprite_cursor->GetPosition());
}

void SpriteCheckBox::OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    sprites_clickable->SetCapture(shared_from_this());
    pressed_ = true;
}

void SpriteCheckBox::OnMouseButtonReleased(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    if(sprites_clickable->GetCapture() == shared_from_this())
    {
        sprites_clickable->ReleaseCapture();
        pressed_ = false;

        if(is_hovering_ && IsEnabled())
        {
            checked_ = (checked_ == CHECKED ? UNCHECKED : CHECKED);
            if(click_sound_) click_sound_->Play();
            GetSpriteEventHandler()->OnSpriteClicked(shared_from_this());
        }
    }
}

void SpriteCheckBox::OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    if(key == DIK_SPACE)
    {
        key_held_ = pressed_ = true;
    }
}

void SpriteCheckBox::OnKeyReleased(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    if(key == DIK_SPACE && key_held_ && pressed_ && IsEnabled())
    {
        key_held_ = pressed_ = false;
        checked_ = (checked_ == CHECKED ? UNCHECKED : CHECKED);
        if(click_sound_) click_sound_->Play();
        GetSpriteEventHandler()->OnSpriteClicked(shared_from_this());
    }
}

void SpriteCheckBox::OnGetFocus(SpritePtr old_focus_sprite, SpriteContainerPtr sprites_clickable)
{
}

void SpriteCheckBox::OnLoseFocus(SpritePtr new_focus_sprite, SpriteContainerPtr sprites_clickable)
{
    key_held_ = pressed_ = false;
}

void SpriteCheckBox::OnCaptureStolen(SpritePtr new_capture_sprite, SpriteContainerPtr sprites_clickable)
{
    key_held_ = pressed_ = false;
}
