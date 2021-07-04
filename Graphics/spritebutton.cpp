#include "stdafx.h"
#include "spritebutton.h"
#include "spritecontainer.h"
#include "image.h"
#include "device.h"
#include "color.h"
#include "../Input/device.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Audio/sound.h"

using namespace Graphics;

SpriteButton::SpriteButton(SpriteEventHandler* event_handler)
: LogContext("Graphics::SpriteButton")
, Sprite(event_handler)
, pressed_(false)
, is_hovering_(false)
, key_held_(false)
{
    LOG("Creating...");
}

SpriteButton::~SpriteButton()
{
    LOG("Deleting...");
}

void SpriteButton::Think(float time_delta)
{
}

void SpriteButton::Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus)
{
    if(!IsEnabled())
    {
        if(image_disabled_)
        {
            image_disabled_->Draw(time_delta, device, GetPosition());
        }
    }
    else
    {
        if(pressed_)
        {
            if(is_hovering_ || key_held_)
            {
                // Draw a pressed button.
                if(image_down_)
                {
                    image_down_->Draw(time_delta, device, GetPosition());
                }
            }
            else
            {
                // Draw an unpressed button.
                if(image_up_)
                {
                    image_up_->Draw(time_delta, device, GetPosition());
                }
            }
        }
        else if(is_hovering_)
        {
            // Draw an unpressed button with a hover highlight.
            if(image_hover_)
            {
                image_hover_->Draw(time_delta, device, GetPosition());
            }
        }
        else
        {
            // Draw an unpressed button.
            if(image_up_)
            {
                image_up_->Draw(time_delta, device, GetPosition());
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

void SpriteButton::OnMouseEnter(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    is_hovering_ = true;
}

void SpriteButton::OnMouseLeave(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    is_hovering_ = false;
}

void SpriteButton::OnMouseMoved(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    is_hovering_ = IsPointColliding(sprite_cursor->GetPosition());
}

void SpriteButton::OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    sprites_clickable->SetCapture(shared_from_this());
    pressed_ = true;
}

void SpriteButton::OnMouseButtonReleased(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    if(sprites_clickable->GetCapture() == shared_from_this())
    {
        sprites_clickable->ReleaseCapture();
        pressed_ = false;

        if(is_hovering_ && IsEnabled())
        {
            if(click_sound_) click_sound_->Play();
            GetSpriteEventHandler()->OnSpriteClicked(shared_from_this());
        }
    }
}

void SpriteButton::OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    if(key == DIK_SPACE)
    {
        key_held_ = pressed_ = true;
    }
}

void SpriteButton::OnKeyReleased(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    if(key == DIK_SPACE && key_held_ && pressed_ && IsEnabled())
    {
        key_held_ = pressed_ = false;
        if(click_sound_) click_sound_->Play();
        GetSpriteEventHandler()->OnSpriteClicked(shared_from_this());
    }
}

void SpriteButton::OnGetFocus(SpritePtr old_focus_sprite, SpriteContainerPtr sprites_clickable)
{
}

void SpriteButton::OnLoseFocus(SpritePtr new_focus_sprite, SpriteContainerPtr sprites_clickable)
{
    key_held_ = pressed_ = false;
}

void SpriteButton::OnCaptureStolen(SpritePtr new_capture_sprite, SpriteContainerPtr sprites_clickable)
{
    key_held_ = pressed_ = false;
}
