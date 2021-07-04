#ifndef INCLUDED_GRAPHICS_SPRITECHECKBOX
#define INCLUDED_GRAPHICS_SPRITECHECKBOX

#include "sprite.h"
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

class SpriteCheckBox :
    public Sprite,
    public Utility::LogContext
{
public:
    SpriteCheckBox(SpriteEventHandler* event_handler);
    ~SpriteCheckBox();

    enum Checked { CHECKED = 0, UNCHECKED = 1 };
    void SetDownImage(ImagePtr image, Checked checked)      { image_down_[checked] = image; }
    void SetUpImage(ImagePtr image, Checked checked)        { image_up_[checked] = image; }
    void SetHoverImage(ImagePtr image, Checked checked)     { image_hover_[checked] = image; }
    void SetDisabledImage(ImagePtr image, Checked checked)  { image_disabled_[checked] = image; }
    void SetClickSound(Audio::SoundPtr sound)               { click_sound_ = sound; }

    void Think(float time_delta);
    void Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus);

    void SetChecked(Checked checked)    { checked_ = checked; }
    bool IsChecked() const              { return (checked_ == CHECKED); }

    void OnMouseEnter(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnMouseLeave(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnMouseMoved(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnMouseButtonReleased(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);

    void OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held);
    void OnKeyReleased(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held);

    void OnGetFocus(SpritePtr old_focus_sprite, SpriteContainerPtr sprites_clickable);
    void OnLoseFocus(SpritePtr new_focus_sprite, SpriteContainerPtr sprites_clickable);

    void OnCaptureStolen(SpritePtr new_capture_sprite, SpriteContainerPtr sprites_clickable);

private:
    bool pressed_;
    bool is_hovering_;
    bool key_held_;
    Checked checked_;
    ImagePtr image_down_[2];
    ImagePtr image_up_[2];
    ImagePtr image_hover_[2];
    ImagePtr image_disabled_[2];
    Audio::SoundPtr click_sound_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITEBUTTON
