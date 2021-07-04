#include "stdafx.h"
#include "spritelistctrl.h"
#include <boost/algorithm/string.hpp>
#include "../Input/device.h"
#include "image.h"
#include "font.h"
#include "device.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Audio/sound.h"

using namespace Graphics;

SpriteListCtrl::SpriteListCtrl(SpriteEventHandler* event_handler)
: LogContext("Graphics::SpriteListCtrl")
, Sprite(event_handler)
, cur_sel_(-1)
, top_index_(0)
, item_height_(15)
, page_size_(10)
, right_margin_(10.0f)
, left_margin_(10.0f)
, vert_align_offset_(0.0f)
, string_hovering_index_(-1)
, color_string_default_(0.0f, 0.0f, 0.0f)
, color_string_selected_(0.8f, 0.8f, 0.8f)
, color_string_highlighted_(0.8f, 0.8f, 0.0f)
, color_selection_rect_(0.8f, 0.8f, 0.2f)
, color_header_(1.0f, 1.0f, 1.0f)
, last_click_time_(0)
{
    LOG("Creating...");
}

SpriteListCtrl::~SpriteListCtrl()
{
    LOG("Deleting...");
}

void SpriteListCtrl::Think(float time_delta)
{
}

void SpriteListCtrl::Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus)
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
        // Draw the header first.
        Columns::iterator col;
        Vector x_coord(GetPosition());
        font_string_->SetColor(IsEnabled() ? color_header_ : Graphics::Color(0.0f));
        for(col = columns_.begin(); col != columns_.end(); ++col)
        {
            std::string text(col->name_.substr(0, CalcNumVisibleChars(col->name_, float(col->width_))));
            font_string_->Write(text, device, x_coord + Graphics::Vector(left_margin_, vert_align_offset_));
            x_coord.x_ += col->width_;
        }

        // Draw all the data.
        Vector position(GetPosition());
        position.y_ += item_height_;
        if(!IsEnabled())
        {
            font_string_->SetColor(Graphics::Color(0.0f));
        }
        for(int i = top_index_, j = 0; i < int(rows_.size()) && j < total_num_strings_visible_; i++, j++)
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
                    font_string_->SetColor(rows_[i].color_);
                }
            }

            std::vector<std::string>::iterator col;
            Vector x_coord(position);
            int index;
            for(col = rows_[i].strings_.begin(), index = 0; col != rows_[i].strings_.end(); ++col, ++index)
            {
                std::string text(col->substr(0, CalcNumVisibleChars(*col, float(columns_[index].width_))));
                font_string_->Write(text, device, x_coord + Graphics::Vector(left_margin_, vert_align_offset_));
                x_coord.x_ += columns_[index].width_;
            }

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

void SpriteListCtrl::SetBoundingRect(const Vector& rect_min, const Vector& rect_max)
{
    Sprite::SetBoundingRect(rect_min, rect_max);
    total_num_strings_visible_ = int(rect_max.y_) / item_height_;
    // Take one off for the header.
    total_num_strings_visible_--;
}

void SpriteListCtrl::Sort(int column, bool ascending)
{
    if(rows_.empty())
    {
        return;
    }
    if(column < 0 || column >= int(rows_[0].strings_.size()))
    {
        return;
    }
    Rows sorted_rows_;
    Rows::iterator i, j;
    try
    {
        for(i = rows_.begin(); i != rows_.end(); ++i)
        {
            int inserted = false;
            for(j = sorted_rows_.begin(); j != sorted_rows_.end() && !inserted; ++j)
            {
                int a = boost::lexical_cast<int>(i->strings_[column]);
                int b = boost::lexical_cast<int>(j->strings_[column]);
                if((ascending && a < b) || (!ascending && a > b))
                {
                    // Insert the row at this iterator
                    sorted_rows_.insert(j, *i);
                    inserted = true;
                    break;
                }
            }
            if(!inserted)
            {
                // Make 'i' the last item.
                sorted_rows_.push_back(*i);
            }
        }

        rows_ = sorted_rows_;
    }
    catch(boost::bad_lexical_cast& e)
    {
        LOG_EXCEPTION("Tried to sort a SpriteListCtrl on column " << column << ". " << e.what());
    }
}

void SpriteListCtrl::OnMouseLeave(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    string_hovering_index_ = -1;
}

void SpriteListCtrl::OnMouseMoved(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    if(IsPointColliding(sprite_cursor->GetPosition()))
    {
        string_hovering_index_ = int(sprite_cursor->GetPosition().y_ - GetPosition().y_) / item_height_;
        // Take one off for the header.
        string_hovering_index_--;
    }
    else
    {
        string_hovering_index_ = -1;
    }
}

void SpriteListCtrl::OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable)
{
    int string_index = int(sprite_cursor->GetPosition().y_ - GetPosition().y_) / item_height_;
    int clicked_on_index = top_index_ + string_index;
    // Take one off for the header.
    clicked_on_index--;
    if(clicked_on_index >= 0 && clicked_on_index < int(rows_.size()))
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

void SpriteListCtrl::OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    if(rows_.empty())
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
        if(cur_sel_ >= int(rows_.size()))
        {
            cur_sel_ = int(rows_.size())-1;
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
        if(cur_sel_ >= int(rows_.size()))
        {
            cur_sel_ = int(rows_.size())-1;
        }
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
        break;
    case DIK_HOME:
        cur_sel_ = 0;
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
        break;
    case DIK_END:
        cur_sel_ = int(rows_.size())-1;
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
        break;
    }
    ScrollCurSelStringIntoView();
}

void SpriteListCtrl::OnGetFocus(SpritePtr old_focus_sprite, SpriteContainerPtr sprites_clickable)
{
}

void SpriteListCtrl::OnLoseFocus(SpritePtr new_focus_sprite, SpriteContainerPtr sprites_clickable)
{
}

int SpriteListCtrl::AddColumn(const std::string& text, int width)
{
    Column column;
    column.name_    = text;
    column.width_   = width;
    int new_index = int(columns_.size());
    columns_.push_back(column);

    Rows::iterator itor_row;
    for(itor_row = rows_.begin(); itor_row != rows_.end(); ++itor_row)
    {
        itor_row->strings_.push_back(std::string());
    }

    return new_index;
}

void SpriteListCtrl::RemoveColumn(int column)
{
    if(column >= 0 && column < int(columns_.size()))
    {
        Columns::iterator itor_column = columns_.begin();
        std::advance(itor_column, column);
        columns_.erase(itor_column);

        Rows::iterator itor_row;
        for(itor_row = rows_.begin(); itor_row != rows_.end(); ++itor_row)
        {
            std::vector<std::string>::iterator itor_column = itor_row->strings_.begin();
            std::advance(itor_column, column);
            itor_row->strings_.erase(itor_column);
        }
    }
}

int SpriteListCtrl::AddRow(const std::string& column_0_text)
{
    if(columns_.empty())
    {
        return -1;
    }
    Row row(color_string_default_);
    Columns::iterator itor_column;
    for(itor_column = columns_.begin(); itor_column != columns_.end(); ++itor_column)
    {
        row.strings_.push_back(std::string());
    }
    row.strings_[0] = column_0_text;

    int new_index = int(rows_.size());
    rows_.push_back(row);

    GetSpriteEventHandler()->OnSpriteStringAdded(shared_from_this());
    return new_index;
}

void SpriteListCtrl::RemoveRow(int row)
{
    if(row >= 0 && row < int(rows_.size()))
    {
        GetSpriteEventHandler()->OnSpriteStringRemoved(shared_from_this());
        Rows::iterator itor = rows_.begin();
        std::advance(itor, row);
        rows_.erase(itor);
    }
}

int SpriteListCtrl::FindRow(const std::string& column_0_text)
{
    int i = 0;
    Rows::iterator itor;
    for(itor = rows_.begin(); itor != rows_.end(); ++itor, ++i)
    {
        if(boost::algorithm::iequals(column_0_text, itor->strings_.front()))
        {
            return i;
        }
    }
    return -1;
}

void SpriteListCtrl::SetString(int row, int column, const std::string& text)
{
    if(row >= 0 && row < int(rows_.size()))
    {
        if(column >= 0 && column < int(rows_[row].strings_.size()))
        {
            rows_[row].strings_[column] = text;
        }
    }
}

std::string SpriteListCtrl::GetString(int row, int column) const
{
    if(row >= 0 && row < int(rows_.size()))
    {
        if(column >= 0 && column < int(rows_[row].strings_.size()))
        {
            return rows_[row].strings_[column];
        }
    }
    return std::string();
}

void SpriteListCtrl::SetRowColor(int row, const Color& color)
{
    if(row >= 0 && row < int(rows_.size()))
    {
        rows_[row].color_ = color;
    }
}

void SpriteListCtrl::DeleteAllColumns()
{
    columns_.clear();
    rows_.clear();
}

void SpriteListCtrl::DeleteAllRows()
{
    rows_.clear();
    cur_sel_ = -1;
}

void SpriteListCtrl::SetCurSel(int index)
{
    if(index >= 0 && index < int(rows_.size()))
    {
        cur_sel_ = index;
        ScrollCurSelStringIntoView();
        GetSpriteEventHandler()->OnSpriteSelectionChanged(shared_from_this());
    }
}

void SpriteListCtrl::SelectStringPartial(const std::string& text)
{
    Rows::iterator row;
    int i = 0;
    for(row = rows_.begin(); row != rows_.end(); ++row, ++i)
    {
        if(!row->strings_.empty())
        {
            if(boost::algorithm::ifind_first(text, row->strings_[0]))
            {
                SetCurSel(i);
                return;
            }
        }
    }
}

void SpriteListCtrl::SelectStringExact(const std::string& text)
{
    Rows::iterator row;
    int i = 0;
    for(row = rows_.begin(); row != rows_.end(); ++row, ++i)
    {
        if(!row->strings_.empty())
        {
            if(boost::algorithm::iequals(text, row->strings_[0]))
            {
                SetCurSel(i);
                return;
            }
        }
    }
}

void SpriteListCtrl::ScrollCurSelStringIntoView()
{
    if(cur_sel_ == -1)
    {
        return;
    }

    if(cur_sel_ < top_index_)
    {
        top_index_ = cur_sel_;
    }
    else if((cur_sel_ - top_index_) >= total_num_strings_visible_)
    {
        top_index_ = cur_sel_ - (total_num_strings_visible_-1);
    }
}

void SpriteListCtrl::CalcVertAlignment()
{
    if(font_string_)
    {
        float text_height = font_string_->GetPixelHeight("BIMygjqp");
        //float bbox_height = GetBoundingRectMax().y_ - GetBoundingRectMin().y_;
        //vert_align_offset_ = (bbox_height/2.0f) + (text_height/2.0f);
        vert_align_offset_ = text_height;
    }
}

int SpriteListCtrl::CalcNumVisibleChars(const std::string& text, float column_width)
{
    if(font_string_ && !text.empty())
    {
        return font_string_->GetNumChars(text, column_width-(left_margin_+right_margin_));
    }
    return 0;
}
