#include "stdafx.h"
#include "spritelistbox.h"
#include "../Input/device.h"
#include "image.h"
#include "font.h"
#include "device.h"
#include <algorithm>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Audio/sound.h"

using namespace Graphics;

SpriteListBox::SpriteListBox(SpriteEventHandler* event_handler)
: LogContext("Graphics::SpriteListBox")
, Sprite(event_handler)
, cur_sel_(-1)
, top_index_(0)
, item_height_(20)
, page_size_(10)
, right_margin_(10.0f)
, left_margin_(10.0f)
, vert_align_offset_(0.0f)
, string_hovering_index_(-1)
, color_string_normal_(0.0f, 0.0f, 0.0f)
, color_string_selected_(0.8f, 0.8f, 0.8f)
, color_string_highlighted_(0.8f, 0.8f, 0.0f)
, color_selection_rect_(0.0f, 0.5f, 1.0f)
, max_count_(100)
, last_click_time_(0)
{
    LOG("Creating...");
}

SpriteListBox::~SpriteListBox()
{
    LOG("Deleting...");
}

void SpriteListBox::Think(float time_delta)
{
}

void SpriteListBox::Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus)
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

    if(font_string_)
    {
        Vector position(GetPosition());
        if(!IsEnabled())
        {
            font_string_->SetColor(Graphics::Color(0.0f));
        }
        for(int i = top_index_, j = 0; i < int(strings_.size()) && j < total_num_strings_visible_; i++, j++)
        {
            if(IsEnabled())
            {
                if(i == cur_sel_)
                {
                    device->DrawFilledRect(position,
                        position + Vector(GetBoundingRectMax().x_, float(item_height_)),
                        color_selection_rect_);
                    font_string_->SetColor(i == string_hovering_index_ ? color_string_highlighted_ : color_string_selected_);
                }
                else if(i == string_hovering_index_)
                {
                    font_string_->SetColor(color_string_highlighted_);
                }
                else
                {
                    if (strings_[i].bg_colour_)
                    {
                        device->DrawFilledRect(position,
                            position + Vector(GetBoundingRectMax().x_, float(item_height_)),
                            *strings_[i].bg_colour_);
                    }
                    if (strings_[i].fg_colour_)
                        font_string_->SetColor(*strings_[i].fg_colour_);
                    else
                        font_string_->SetColor(color_string_normal_);
                }
            }

            font_string_->Write(strings_[i].string_.substr(0, CalcNumVisibleChars(strings_[i].string_)),
                                device,
                                position + Graphics::Vector(left_margin_, vert_align_offset_));

            position.y_ += item_height_;
        }
    }

    if(IsEnabled() && sprite_focus == shared_from_this())
    {
        device->DrawFocusRect(GetPosition() + GetBoundingRectMin(),
                              GetPosition() + GetBoundingRectMax(),
                              Color(0.859375f, 0.85546875f, 0.6328125f));
    }
}

void SpriteListBox::ScrollToBottom()
{
    ScrollStringIntoView(int(strings_.size())-1);
}

void SpriteListBox::ScrollStringIntoView(int index)
{
    if(index < 0)
    {
        return;
    }

    if(index < top_index_)
    {
        top_index_ = index;
    }
    else if((index - top_index_) >= total_num_strings_visible_)
    {
        top_index_ = index - (total_num_strings_visible_-1);
    }
}

void SpriteListBox::SetBoundingRect(const Vector& rect_min, const Vector& rect_max)
{
    Sprite::SetBoundingRect(rect_min, rect_max);
    total_num_strings_visible_ = int(rect_max.y_) / item_height_;
}

void SpriteListBox::OnMouseLeave(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    string_hovering_index_ = -1;
}

void SpriteListBox::OnMouseMoved(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    if(IsPointColliding(sprite_cursor->GetPosition()))
    {
        string_hovering_index_ = int(sprite_cursor->GetPosition().y_ - GetPosition().y_) / item_height_;
    }
    else
    {
        string_hovering_index_ = -1;
    }
}

void SpriteListBox::OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    int string_index = int(sprite_cursor->GetPosition().y_ - GetPosition().y_) / item_height_;
    int clicked_on_index = top_index_ + string_index;
    if(clicked_on_index >= 0 && clicked_on_index < int(strings_.size()))
    {
        if(cur_sel_ != clicked_on_index)
        {
            GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
        }
        cur_sel_ = clicked_on_index;
        ScrollCurSelStringIntoView();

        unsigned long now = timeGetTime();
        if(now - last_click_time_ <= 500)
        {
            GetSpriteEventHandler()->OnSpriteDoubleClicked(shared_from_this());
        }
        last_click_time_ = now;
    }
}

void SpriteListBox::OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    if(strings_.empty())
    {
        return;
    }
    switch(key)
    {
    case DIK_UP:
        --cur_sel_;
        if(cur_sel_ < 0)
        {
            cur_sel_ = 0;
        }
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
        break;
    case DIK_DOWN:
        ++cur_sel_;
        if(cur_sel_ >= int(strings_.size()))
        {
            cur_sel_ = int(strings_.size())-1;
        }
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
        break;
    case DIK_PGUP:
        cur_sel_ -= page_size_;
        if(cur_sel_ < 0)
        {
            cur_sel_ = 0;
        }
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
        break;
    case DIK_PGDN:
        cur_sel_ += page_size_;
        if(cur_sel_ >= int(strings_.size()))
        {
            cur_sel_ = int(strings_.size())-1;
        }
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
        break;
    case DIK_HOME:
        cur_sel_ = 0;
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
        break;
    case DIK_END:
        cur_sel_ = int(strings_.size())-1;
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
        break;
    }
    ScrollCurSelStringIntoView();
}

void SpriteListBox::OnGetFocus(SpritePtr old_focus_sprite, SpriteContainerPtr sprites_clickable)
{
}

void SpriteListBox::OnLoseFocus(SpritePtr new_focus_sprite, SpriteContainerPtr sprites_clickable)
{
}

void SpriteListBox::AddString(const std::string& text, const std::optional<Color>& specific_fg_colour, const std::optional<Color>& specific_bg_colour)
{
    LOG("Adding the string [" << text << "]");
    strings_.push_back({ text, specific_fg_colour, specific_bg_colour });
    while(strings_.size() > max_count_)
    {
        strings_.pop_front();
    }
    LOG("There are [" << int(strings_.size()) << "] items in the container");
    GetSpriteEventHandler()->OnSpriteStringAdded(shared_from_this());
}

void SpriteListBox::AddWrappedString(const std::string& text, const std::optional<Color>& specific_fg_colour, const std::optional<Color>& specific_bg_colour)
{
    std::string remaining_text(text);
    while(!remaining_text.empty())
    {
        int num_visible_chars = CalcNumVisibleChars(remaining_text);
        std::string text_to_add(remaining_text.substr(0, num_visible_chars));
        remaining_text = remaining_text.substr(num_visible_chars);

        if(!text_to_add.empty())
        {
            strings_.push_back({ text_to_add, specific_fg_colour, specific_bg_colour });
            while(strings_.size() > max_count_)
            {
                strings_.pop_front();
            }
        }
    }

    GetSpriteEventHandler()->OnSpriteStringAdded(shared_from_this());
}

void SpriteListBox::RemoveString(const std::string& text)
{
    auto itor = std::find_if(strings_.begin(), strings_.end(), [&text](const StringInfo& si) { return text == si.string_; });
    if(itor != strings_.end())
    {
        // Are we removing the currently selected string?
        if(cur_sel_ >= 0 && cur_sel_ < int(strings_.size()))
        {
            if(text == strings_[cur_sel_].string_)
            {
                cur_sel_ = -1;
            }
        }

        GetSpriteEventHandler()->OnSpriteStringRemoved(shared_from_this());
        strings_.erase(itor);
    }
}

void SpriteListBox::RemoveString(int index)
{
    if(index >= 0 && index < int(strings_.size()))
    {
        // Are we removing the currently selected string?
        if(index == cur_sel_)
        {
            cur_sel_ = -1;
        }

        auto itor = strings_.begin();
        std::advance(itor, index);
        strings_.erase(itor);
    }
}

void SpriteListBox::SetString(int index, const std::string& text, const std::optional<Color>& specific_fg_colour, const std::optional<Color>& specific_bg_colour)
{
    if(index >= 0 && index < int(strings_.size()))
    {
        strings_[index] = { text, specific_fg_colour, specific_bg_colour };
    }
}

int SpriteListBox::FindString(const std::string& text)
{
    auto itor = std::find_if(strings_.begin(), strings_.end(), [&text](const StringInfo& si) { return text == si.string_; });
    if(itor == strings_.end())
    {
        return -1;
    }
    return int(std::distance(strings_.begin(), itor));
}

void SpriteListBox::ResetContent()
{
    strings_.clear();
    cur_sel_ = -1;
    top_index_ = 0;
}

void SpriteListBox::SetCurSel(int index)
{
    if(index >= 0 && index < int(strings_.size()))
    {
        cur_sel_ = index;
        ScrollCurSelStringIntoView();
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
    }
}

std::string SpriteListBox::GetString(int index) const
{
    if(index >= 0 && index < int(strings_.size()))
    {
        return strings_[index].string_;
    }
    return std::string();
}

void SpriteListBox::ScrollCurSelStringIntoView()
{
    ScrollStringIntoView(cur_sel_);
}

int SpriteListBox::CalcNumVisibleChars(const std::string& text)
{
    if(font_string_ && !text.empty())
    {
        return font_string_->GetNumChars(text, GetBoundingRectMax().x_-(left_margin_+right_margin_));
    }
    return 0;
}

void SpriteListBox::CalcVertAlignment()
{
    if(font_string_)
    {
        float text_height = font_string_->GetPixelHeight("BIMygjqp");
        //float bbox_height = GetBoundingRectMax().y_ - GetBoundingRectMin().y_;
        //vert_align_offset_ = (bbox_height/2.0f) + (text_height/2.0f);
        vert_align_offset_ = text_height;
    }
}

void SpriteListBox::SetMaxStringCount(std::size_t max_count)
{
    max_count_ = max_count;
    LOG("max_count_ = [" << max_count_ << "]");
    while(strings_.size() > max_count_)
    {
        strings_.pop_front();
    }
    LOG("There are [" << int(strings_.size()) << "] items in the container");
}
