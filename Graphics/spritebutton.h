#ifndef INCLUDED_GRAPHICS_SPRITEBUTTON
#define INCLUDED_GRAPHICS_SPRITEBUTTON

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

class SpriteButton :
    public Sprite,
    public Utility::LogContext
{
public:
    SpriteButton(SpriteEventHandler* event_handler);
    ~SpriteButton();

    void SetDownImage(ImagePtr image)           { image_down_ = image; }
    void SetUpImage(ImagePtr image)             { image_up_ = image; }
    void SetHoverImage(ImagePtr image)          { image_hover_ = image; }
    void SetDisabledImage(ImagePtr image)       { image_disabled_ = image; }
    void SetClickSound(Audio::SoundPtr sound)   { click_sound_ = sound; }

    void Think(float time_delta);
    void Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus);

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
    ImagePtr image_down_;
    ImagePtr image_up_;
    ImagePtr image_hover_;
    ImagePtr image_disabled_;
    Audio::SoundPtr click_sound_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITEBUTTON
