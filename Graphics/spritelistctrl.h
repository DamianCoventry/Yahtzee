#ifndef INCLUDED_GRAPHICS_SPRITELISTCTRL
#define INCLUDED_GRAPHICS_SPRITELISTCTRL

#include "sprite.h"
#include <string>
#include <vector>
#include "color.h"
#include "../Utility/logcontext.h"

namespace Graphics
{

class Font;
typedef boost::shared_ptr<Font> FontPtr;

class Image;
typedef boost::shared_ptr<Image> ImagePtr;

class SpriteListCtrl :
    public Sprite,
    public Utility::LogContext
{
public:
    SpriteListCtrl(SpriteEventHandler* event_handler);
    ~SpriteListCtrl();

    void Think(float time_delta);
    void Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus);

    void SetBackImage(ImagePtr image)                   { image_back_ = image; }
    void SetDisabledImage(ImagePtr image)               { image_disabled_ = image; }
    void SetStringFont(FontPtr font)                    { font_string_ = font; CalcVertAlignment(); }
    void SetStringDefaultColor(const Color& color)      { color_string_default_ = color; }
    void SetStringSelectedColor(const Color& color)     { color_string_selected_ = color; }
    void SetStringHighlightedColor(const Color& color)  { color_string_highlighted_ = color; }
    void SetSelectionRectColor(const Color& color)      { color_selection_rect_ = color; }
    void SetHeaderColor(const Color& color)             { color_header_ = color; }
    void SetItemHeight(int height)                      { item_height_ = height; CalcVertAlignment(); }
    void SetPageSize(int page_size)                     { page_size_ = page_size; }
    void SetRightMargin(float margin)                   { right_margin_ = margin; }
    void SetLeftMargin(float margin)                    { left_margin_ = margin; }

    void SetBoundingRect(const Vector& rect_min, const Vector& rect_max);

    void Sort(int column, bool ascending = true);

    void OnMouseLeave(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnMouseMoved(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);

    void OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held);

    void OnGetFocus(SpritePtr old_focus_sprite, SpriteContainerPtr sprites_clickable);
    void OnLoseFocus(SpritePtr new_focus_sprite, SpriteContainerPtr sprites_clickable);

    int AddColumn(const std::string& text, int width);
    void RemoveColumn(int column);

    int AddRow(const std::string& column_0_text);
    void RemoveRow(int row);

    int FindRow(const std::string& column_0_text);

    int GetRowCount() const { return int(rows_.size()); }

    void SetString(int row, int column, const std::string& text);
    std::string GetString(int row, int column) const;

    void SetRowColor(int row, const Color& color);

    void DeleteAllColumns();
    void DeleteAllRows();

    void SetCurSel(int row);
    int GetCurSel() const { return cur_sel_; }
    void SelectStringPartial(const std::string& text);
    void SelectStringExact(const std::string& text);

private:
    void ScrollCurSelStringIntoView();
    void CalcVertAlignment();
    int CalcNumVisibleChars(const std::string& text, float column_width);

private:
    struct Column
    {
        std::string name_;
        int width_;
    };
    typedef std::vector<Column> Columns;
    Columns columns_;

    struct Row
    {
        Row(const Color& color) : color_(color) {}
        Color color_;
        std::vector<std::string> strings_;
    };
    typedef std::vector<Row> Rows;
    Rows rows_;

    int top_index_;
    int cur_sel_;
    int item_height_;
    int page_size_;
    int string_hovering_index_;
    int total_num_strings_visible_;
    float right_margin_;
    float left_margin_;
    float vert_align_offset_;
    unsigned long last_click_time_;
    ImagePtr image_back_;
    ImagePtr image_disabled_;
    FontPtr font_string_;
    Color color_string_default_;
    Color color_string_selected_;
    Color color_string_highlighted_;
    Color color_selection_rect_;
    Color color_header_;
};

}       // namespace Graphics

#endif  // included_graphics_spritelistctrl
