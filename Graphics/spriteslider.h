#ifndef INCLUDED_GRAPHICS_SPRITESLIDER
#define INCLUDED_GRAPHICS_SPRITESLIDER

#include "sprite.h"
#include "../Utility/logcontext.h"

namespace Graphics
{

class Image;
typedef boost::shared_ptr<Image> ImagePtr;

class SpriteSlider :
    public Sprite,
    public Utility::LogContext
{
public:
    SpriteSlider(SpriteEventHandler* event_handler);
    ~SpriteSlider();

    void SetBackNormalImage(ImagePtr image)         { image_back_normal_ = image; }
    void SetBackDisabledImage(ImagePtr image)       { image_back_disabled_ = image; }
    void SetGrabberNormalImage(ImagePtr image)      { image_grabber_normal_ = image; }
    void SetGrabberDisabledImage(ImagePtr image)    { image_grabber_disabled_ = image; }

    void SetRange(int min_range, int max_range);
    void SetPos(int pos);
    void SetGrabberXCoords(const Vector& line)      { grabber_x_coords_ = line; UpdateGrabberXCoord(); }
    void SetGrabberYCoord(float y_coord);
    void SetSmallStep(int small_step)               { small_step_ = small_step; }
    void SetLargeStep(int large_step)               { large_step_ = large_step; }

    int GetPos() const { return pos_; }

    void Think(float time_delta);
    void Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus);

    void OnMouseMoved(SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);
    void OnMouseButtonReleased(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);

    void OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held);

private:
    void UpdateGrabberXCoord();
    void UpdateGrabberPos(SpritePtr sprite_cursor);

private:
    ImagePtr image_back_normal_;
    ImagePtr image_back_disabled_;
    ImagePtr image_grabber_normal_;
    ImagePtr image_grabber_disabled_;
    int min_range_;
    int max_range_;
    int pos_;
    int small_step_;
    int large_step_;
    Vector grabber_x_coords_;
    Vector grabber_offset_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITEBUTTON
