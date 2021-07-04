#ifndef INCLUDED_GRAPHICS_SPRITECURSOR
#define INCLUDED_GRAPHICS_SPRITECURSOR

#include "sprite.h"
#include "../Utility/logcontext.h"

namespace Graphics
{

class ImageList;
typedef boost::shared_ptr<ImageList> ImageListPtr;

class SpriteCursor :
    public Sprite,
    public Utility::LogContext
{
public:
    SpriteCursor(SpriteEventHandler* event_handler);
    ~SpriteCursor();

    void SetImageList(ImageListPtr image_list)  { image_list_ = image_list; }
    void Think(float time_delta);
    void Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus);

private:
    float animate_counter_;
    float animate_threshold_;
    ImageListPtr image_list_;
    float direction_counter_;
    float direction_change_;
    bool direction_;
};

typedef boost::shared_ptr<SpriteCursor> SpriteCursorPtr;

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITECURSOR
