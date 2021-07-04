#include "stdafx.h"
#include "spriteeditbox.h"
#include "../Graphics/device.h"
#include "../Input/device.h"
#include "image.h"
#include "font.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Audio/sound.h"

using namespace Graphics;

SpriteEditBox::SpriteEditBox(SpriteEventHandler* event_handler)
: LogContext("Graphics::SpriteEditBox")
, Sprite(event_handler)
, color_font_(1.0f, 1.0f, 1.0f)
, first_char_(0)
, num_chars_(0)
, cursor_index_(0)
, cursor_pixel_offset_(0.0f)
, cursor_timeout_(0.0f)
, draw_cursor_(true)
, read_only_(false)
, right_margin_(10.0f)
, left_margin_(10.0f)
, vert_align_offset_(0.0f)
{
    LOG("Creating...");
}

SpriteEditBox::~SpriteEditBox()
{
    LOG("Deleting...");
}

void SpriteEditBox::Think(float time_delta)
{
}

void SpriteEditBox::Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus)
{
    if(IsEnabled())
    {
        if(image_back_)
        {
            image_back_->Draw(time_delta, device, GetPosition());
        }
    }
    else
    {
        if(image_disabled_)
        {
            image_disabled_->Draw(time_delta, device, GetPosition());
        }
    }

    if(font_)
    {
        Vector position(GetPosition() + Graphics::Vector(left_margin_, vert_align_offset_));
        std::string text(text_.substr(first_char_, num_chars_));

        font_->SetColor(IsEnabled() ? color_font_ : Graphics::Color(0.0f));
        font_->Write(text, device, position);

        if(sprite_focus == shared_from_this() && !read_only_ && IsEnabled())
        {
            cursor_timeout_ += time_delta;
            if(cursor_timeout_ >= 0.25f)
            {
                cursor_timeout_ = 0.0f;
                draw_cursor_ = !draw_cursor_;
            }

            if(draw_cursor_)
            {
                position += Vector(cursor_pixel_offset_, 0.0f);
                font_->Write(std::string("_"), device, position);
            }
        }
    }

    if(sprite_focus == shared_from_this()/* && !read_only_ && IsEnabled()*/)
    {
        device->DrawFocusRect(GetPosition() + GetBoundingRectMin(),
                              GetPosition() + GetBoundingRectMax(),
                              Color(0.859375f, 0.85546875f, 0.6328125f));
    }
}

void SpriteEditBox::OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    float cursor_x_offset = (sprite_cursor->GetPosition().x_ - GetPosition().x_) - left_margin_;
    cursor_index_ = first_char_ + font_->GetNumChars(text_.substr(first_char_), cursor_x_offset);
    CalcCursorPixelOffset();
    if(click_sound_) click_sound_->Play();
    GetSpriteEventHandler()->OnSpriteClicked(shared_from_this());
}

void SpriteEditBox::OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    if(read_only_ || !IsEnabled())
    {
        return;
    }
    switch(key)
    {
    case DIK_LEFT:
        if(ctrl_held)
        {
            MoveCursorToPrevWord();
        }
        else
        {
            --cursor_index_;
            if(cursor_index_ < 0)
            {
                cursor_index_ = 0;
            }
        }
        ScrollCursorIntoView();
        break;
    case DIK_RIGHT:
        if(ctrl_held)
        {
            MoveCursorToNextWord();
        }
        else
        {
            ++cursor_index_;
            if(cursor_index_ > int(text_.length()))
            {
                cursor_index_ = int(text_.length());
            }
        }
        ScrollCursorIntoView();
        break;
    case DIK_HOME:
        cursor_index_ = 0;
        ScrollCursorIntoView();
        break;
    case DIK_END:
        cursor_index_ = int(text_.length());
        ScrollCursorIntoView();
        break;
    case DIK_DELETE:
        if(text_.length())
        {
            text_.erase(cursor_index_, 1);
            CalcNumVisibleChars();
        }
        break;
    case DIK_BACK:
        if(text_.length())
        {
            --cursor_index_;
            if(cursor_index_ < 0)
            {
                cursor_index_ = 0;
            }
            text_.erase(cursor_index_, 1);
        }
        ScrollCursorIntoView();
        break;
    case DIK_RETURN:
    case DIK_NUMPADENTER:
        GetSpriteEventHandler()->OnSpriteEnterPressed(shared_from_this());
        break;
    default:
        {
            char c = (shift_held ? Input::ToUpperCaseCharacter(key) : Input::ToLowerCaseCharacter(key));
            if(c)
            {
                text_.insert(cursor_index_, 1, c);
                cursor_index_++;
                ScrollCursorIntoView();
                GetSpriteEventHandler()->OnSpriteTextChanged(shared_from_this());
            }
            break;
        }
    }
}

void SpriteEditBox::OnGetFocus(SpritePtr old_focus_sprite, SpriteContainerPtr sprites_clickable)
{
}

void SpriteEditBox::OnLoseFocus(SpritePtr new_focus_sprite, SpriteContainerPtr sprites_clickable)
{
}

void SpriteEditBox::SetText(const std::string& text)
{
    text_ = text;
    first_char_ = 0;
    cursor_index_ = int(text.length());
    ScrollCursorIntoView();
}

void SpriteEditBox::ScrollCursorIntoView()
{
    if(cursor_index_ < first_char_)
    {
        first_char_ = cursor_index_;
    }
    else if(font_)
    {
        std::string visible_text = text_.substr(first_char_, cursor_index_-first_char_);
        float pixel_width = font_->GetPixelWidth(visible_text);
        while(pixel_width >= (GetBoundingRectMax().x_-(left_margin_+right_margin_)))
        {
            ++first_char_;
            if(first_char_ >= int(text_.size()))
            {
                first_char_ = int(text_.size()) - 1;
                break;
            }

            visible_text = text_.substr(first_char_, cursor_index_-first_char_);
            pixel_width = font_->GetPixelWidth(visible_text);
        }
    }

    CalcNumVisibleChars();
    CalcCursorPixelOffset();
}

void SpriteEditBox::CalcNumVisibleChars()
{
    if(font_ && !text_.empty())
    {
        num_chars_ = font_->GetNumChars(text_.substr(first_char_), GetBoundingRectMax().x_-(left_margin_+right_margin_));
    }
}

void SpriteEditBox::CalcCursorPixelOffset()
{
    if(font_)
    {
        std::string visible_text = text_.substr(first_char_, cursor_index_-first_char_);
        cursor_pixel_offset_ = font_->GetPixelWidth(visible_text);
    }
}

void SpriteEditBox::MoveCursorToPrevWord()
{
    if(!cursor_index_)
    {
        return;
    }
    std::string::size_type pos = text_.find_last_not_of(" \t", cursor_index_-1);
    if(pos == std::string::npos)
    {
        cursor_index_ = 0;
    }
    else
    {
        pos = text_.find_last_of(" \t", pos);
        cursor_index_ = (pos == std::string::npos ? 0 : int(pos+1));
    }
}

void SpriteEditBox::MoveCursorToNextWord()
{
    if(cursor_index_ >= int(text_.length()))
    {
        return;
    }
    std::string::size_type pos = text_.find_first_of(" \t", cursor_index_+1);
    if(pos == std::string::npos)
    {
        cursor_index_ = int(text_.length());
    }
    else
    {
        pos = text_.find_first_not_of(" \t", pos);
        cursor_index_ = (pos == std::string::npos ? int(text_.length()) : int(pos));
    }
}

void SpriteEditBox::SetBoundingRect(const Vector& rect_min, const Vector& rect_max)
{
    Sprite::SetBoundingRect(rect_min, rect_max);
    CalcVertAlignment();
}

void SpriteEditBox::SetFont(FontPtr font)
{
    font_ = font;
    CalcVertAlignment();
}

void SpriteEditBox::CalcVertAlignment()
{
    if(font_)
    {
        float text_height = font_->GetPixelHeight("BIMygjqp");
        //float bbox_height = GetBoundingRectMax().y_ - GetBoundingRectMin().y_;
        //vert_align_offset_ = (bbox_height/2.0f) + (text_height/2.0f);
        vert_align_offset_ = text_height;
    }
}
