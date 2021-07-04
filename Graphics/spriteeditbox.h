#ifndef INCLUDED_GRAPHICS_SPRITEEDITBOX
#define INCLUDED_GRAPHICS_SPRITEEDITBOX

#include "sprite.h"
#include <string>
#include "color.h"
#include "../Utility/logcontext.h"

namespace Audio
{
class Sound;
typedef boost::shared_ptr<Sound> SoundPtr;
}

namespace Graphics
{

class Image;
typedef boost::shared_ptr<Image> ImagePtr;

class Font;
typedef boost::shared_ptr<Font> FontPtr;

class SpriteEditBox :
    public Sprite,
    public Utility::LogContext

{
public:
    SpriteEditBox(SpriteEventHandler* event_handler);
    virtual ~SpriteEditBox();

    void SetBackImage(ImagePtr image)           { image_back_ = image; }
    void SetDisabledImage(ImagePtr image)       { image_disabled_ = image; }
    void SetFont(FontPtr font);
    void SetFontColor(const Color& color)       { color_font_ = color; }
    void SetRightMargin(float margin)           { right_margin_ = margin; }
    void SetLeftMargin(float margin)            { left_margin_ = margin; }
    void SetReadOnly(bool read_only = true)     { read_only_ = read_only; }
    void SetClickSound(Audio::SoundPtr sound)   { click_sound_ = sound; }

    void SetBoundingRect(const Vector& rect_min, const Vector& rect_max);

    void Think(float time_delta);
    void Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus);

    void OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held);

    void OnGetFocus(SpritePtr old_focus_sprite, SpriteContainerPtr sprites_clickable);
    void OnLoseFocus(SpritePtr new_focus_sprite, SpriteContainerPtr sprites_clickable);

    std::string GetText() const { return text_; }
    void SetText(const std::string& text);

private:
    void ScrollCursorIntoView();
    void CalcNumVisibleChars();
    void CalcCursorPixelOffset();
    void MoveCursorToPrevWord();
    void MoveCursorToNextWord();
    void CalcVertAlignment();

private:
    std::string text_;
    ImagePtr image_back_;
    ImagePtr image_disabled_;
    FontPtr font_;
    Color color_font_;
    Audio::SoundPtr click_sound_;

    bool draw_cursor_;
    bool read_only_;

    int first_char_;
    int num_chars_;
    int cursor_index_;

    float right_margin_;
    float left_margin_;
    float cursor_pixel_offset_;
    float cursor_timeout_;
    float vert_align_offset_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITEEDITBOX
