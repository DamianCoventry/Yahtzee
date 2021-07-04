#ifndef INCLUDED_GRAPHICS_SPRITELISTBOX
#define INCLUDED_GRAPHICS_SPRITELISTBOX

#include "sprite.h"
#include <string>
#include <deque>
#include <optional>
#include "color.h"
#include "../Utility/logcontext.h"

namespace Graphics
{

class Font;
typedef boost::shared_ptr<Font> FontPtr;

class Image;
typedef boost::shared_ptr<Image> ImagePtr;

class SpriteListBox :
    public Sprite,
    public Utility::LogContext

{
public:
    SpriteListBox(SpriteEventHandler* event_handler);
    ~SpriteListBox();

    void Think(float time_delta);
    void Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus);

    void SetBackImage(ImagePtr image)                   { image_back_ = image; }
    void SetDisabledImage(ImagePtr image)               { image_disabled_ = image; }
    void SetStringFont(FontPtr font)                    { font_string_ = font; CalcVertAlignment(); }
    void SetStringNormalColor(const Color& color)       { color_string_normal_ = color; }
    void SetStringSelectedColor(const Color& color)     { color_string_selected_ = color; }
    void SetStringHighlightedColor(const Color& color)  { color_string_highlighted_ = color; }
    void SetSelectionRectColor(const Color& color)      { color_selection_rect_ = color; }
    void SetItemHeight(int height)                      { item_height_ = height; CalcVertAlignment(); }
    void SetPageSize(int page_size)                     { page_size_ = page_size; }
    void SetRightMargin(float margin)                   { right_margin_ = margin; }
    void SetLeftMargin(float margin)                    { left_margin_ = margin; }

    void ScrollToBottom();
    void ScrollStringIntoView(int index);

    void SetBoundingRect(const Vector& rect_min, const Vector& rect_max);

    void OnMouseLeave(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnMouseMoved(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);

    void OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held);

    void OnGetFocus(SpritePtr old_focus_sprite, SpriteContainerPtr sprites_clickable);
    void OnLoseFocus(SpritePtr new_focus_sprite, SpriteContainerPtr sprites_clickable);

    void AddString(const std::string& text, const std::optional<Color>& specific_fg_colour = std::nullopt, const std::optional<Color>& specific_bg_colour = std::nullopt);
    void RemoveString(const std::string& text);
    void RemoveString(int index);
    void AddWrappedString(const std::string& text, const std::optional<Color>& specific_fg_colour = std::nullopt, const std::optional<Color>& specific_bg_colour = std::nullopt);
    void SetString(int index, const std::string& text, const std::optional<Color>& specific_fg_colour = std::nullopt, const std::optional<Color>& specific_bg_colour = std::nullopt);
    void SetStringColours(const std::optional<Color>& specific_fg_colour, const std::optional<Color>& specific_bg_colour);
    void ClearStringColours(const std::optional<Color>& specific_fg_colour, const std::optional<Color>& specific_bg_colour);
    int FindString(const std::string& text);
    int GetStringCount() const { return int(strings_.size()); }
    std::string GetString(int index) const;
    void SetMaxStringCount(std::size_t max_count);

    void ResetContent();

    void SetCurSel(int index);
    int GetCurSel() const { return cur_sel_; }

private:
    void ScrollCurSelStringIntoView();
    int CalcNumVisibleChars(const std::string& text);
    void CalcVertAlignment();

private:
    int top_index_;
    int cur_sel_;
    int item_height_;
    int page_size_;
    int string_hovering_index_;
    int total_num_strings_visible_;
    std::size_t max_count_;
    unsigned long last_click_time_;

    float right_margin_;
    float left_margin_;
    float vert_align_offset_;

    struct StringInfo
    {
        std::string string_;
        std::optional<Color> fg_colour_;
        std::optional<Color> bg_colour_;
    };
    std::deque<StringInfo> strings_;
    ImagePtr image_back_;
    ImagePtr image_disabled_;
    FontPtr font_string_;
    Color color_string_normal_;
    Color color_string_selected_;
    Color color_string_highlighted_;
    Color color_selection_rect_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITELISTBOX
