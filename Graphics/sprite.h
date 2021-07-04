#ifndef INCLUDED_GRAPHICS_SPRITE
#define INCLUDED_GRAPHICS_SPRITE

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "vector.h"

namespace Graphics
{

class Device;
typedef boost::shared_ptr<Device> DevicePtr;

class Sprite;
typedef boost::shared_ptr<Sprite> SpritePtr;

class SpriteContainer;
typedef boost::shared_ptr<SpriteContainer> SpriteContainerPtr;

struct SpriteEventHandler
{
    virtual void OnSpriteClicked(SpritePtr sprite) {}
    virtual void OnSpriteGetFocus(SpritePtr sprite) {}
    virtual void OnSpriteLoseFocus(SpritePtr sprite) {}
    virtual void OnSpriteTextChanged(SpritePtr sprite) {}
    virtual void OnSpriteSelectionChanged(SpritePtr sprite) {}
    virtual void OnSpriteStringAdded(SpritePtr sprite) {}
    virtual void OnSpriteStringRemoved(SpritePtr sprite) {}
    virtual void OnSpriteSliderMoved(SpritePtr sprite) {}
    virtual void OnSpriteSliderReleased(SpritePtr sprite) {}
    virtual void OnSpriteEnterPressed(SpritePtr sprite) {}
    virtual void OnSpriteDoubleClicked(SpritePtr sprite) {}
};

class Sprite :
    public boost::enable_shared_from_this<Sprite>
{
public:
    Sprite(SpriteEventHandler* event_handler);
    virtual ~Sprite();

    virtual void Think(float time_delta) {}
    virtual void Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus);

    virtual void SetPosition(const Vector& position) { position_ = position; }
    Vector GetPosition() const { return position_; }

    void Enable(bool enable = true)     { enabled_ = enable; }
    bool IsEnabled() const              { return enabled_; }

    virtual void SetBoundingRect(const Vector& rect_min, const Vector& rect_max) { rect_min_ = rect_min; rect_max_ = rect_max; }
    bool IsPointColliding(const Vector& point) const;
    Vector GetBoundingRectMin() const    { return rect_min_; }
    Vector GetBoundingRectMax() const    { return rect_max_; }

    virtual bool IsFocusable() const { return true; }

    virtual void OnGetFocus(SpritePtr old_focus_sprite, SpriteContainerPtr sprites_clickable) {}
    virtual void OnLoseFocus(SpritePtr new_focus_sprite, SpriteContainerPtr sprites_clickable) {}

    virtual void OnCaptureStolen(SpritePtr new_capture_sprite, SpriteContainerPtr sprites_clickable) {}

    virtual void OnMouseEnter(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable) {}
    virtual void OnMouseLeave(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable) {}
    virtual void OnMouseMoved(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable) {}
    virtual void OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable) {}
    virtual void OnMouseButtonHeld(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable) {}
    virtual void OnMouseButtonReleased(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable) {}

    virtual void OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held) {}
    virtual void OnKeyHeld(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held) {}
    virtual void OnKeyReleased(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held) {}

    SpriteEventHandler* GetSpriteEventHandler() const { return event_handler_; }

private:
    SpriteEventHandler* event_handler_;
    Vector position_;
    Vector rect_min_;
    Vector rect_max_;
    bool enabled_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITE
