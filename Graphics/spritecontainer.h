#ifndef INCLUDED_GRAPHICS_SPRITECONTAINER
#define INCLUDED_GRAPHICS_SPRITECONTAINER

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <list>
#include "../Utility/logcontext.h"

namespace Graphics
{

class Sprite;
typedef boost::shared_ptr<Sprite> SpritePtr;

class Device;
typedef boost::shared_ptr<Device> DevicePtr;

class SpriteContainer :
    public boost::enable_shared_from_this<SpriteContainer>,
    public Utility::LogContext

{
public:
    typedef std::list<SpritePtr> Sprites;

public:
    SpriteContainer();
    ~SpriteContainer();

    Sprites::iterator begin()           { return sprites_.begin(); }
    Sprites::iterator end()             { return sprites_.end(); }
    Sprites::reverse_iterator rbegin()  { return sprites_.rbegin(); }
    Sprites::reverse_iterator rend()    { return sprites_.rend(); }

    void Think(float time_delta);
    void Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus = SpritePtr());

    void Prepend(SpritePtr sprite, bool allow_duplicates = true);
    void Append(SpritePtr sprite, bool allow_duplicates = true);
    void Remove(SpritePtr sprite);
    void Replace(SpritePtr sprite_old, SpritePtr sprite_new);
    void Clear();
    bool IsEmpty() const;
    bool IsPresent(SpritePtr sprite) const;
    bool IsAbsent(SpritePtr sprite) const { return !IsPresent(sprite); }

    void SetFocus(SpritePtr sprite);
    SpritePtr GetFocus() const      { return sprite_focus_; }

    void MoveFocusFirst();
    void MoveFocusLast();
    void MoveFocusNext();
    void MoveFocusPrev();

    void SetCapture(SpritePtr sprite);
    SpritePtr GetCapture() const    { return sprite_capture_; }
    void ReleaseCapture();

private:
    SpritePtr GetFirstFocusableSprite() const;
    SpritePtr GetLastFocusableSprite() const;

private:
    Sprites sprites_;
    SpritePtr sprite_focus_;
    SpritePtr sprite_capture_;
};

typedef boost::shared_ptr<SpriteContainer> SpriteContainerPtr;

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITECONTAINER
