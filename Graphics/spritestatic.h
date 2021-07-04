#ifndef INCLUDED_GRAPHICS_SPRITESTATIC
#define INCLUDED_GRAPHICS_SPRITESTATIC

#include "sprite.h"
#include "color.h"
#include "../Utility/logcontext.h"

namespace Graphics
{

class ImageList;
typedef boost::shared_ptr<ImageList> ImageListPtr;

class Image;
typedef boost::shared_ptr<Image> ImagePtr;

class Font;
typedef boost::shared_ptr<Font> FontPtr;

class SpriteStatic :
    public Sprite,
    public Utility::LogContext
{
public:
    SpriteStatic(SpriteEventHandler* event_handler);
    ~SpriteStatic();

    void SetImage(ImagePtr image)           { image_ = image; }
    void SetText(const std::string& text);
    void SetLeftMargin(float margin)        { left_margin_ = margin; }
    void SetRightMargin(float margin)       { right_margin_ = margin; }
    void SetTopMargin(float margin)         { top_margin_ = margin; }

    void SetFont(FontPtr font)              { font_ = font; }
    void SetFontColor(const Color& color)   { color_font_ = color; }

    void SetImageList(ImageListPtr imagelist);
    ImageListPtr GetImageList() const   { return imagelist_; }

    void SetCurrentFrame(int image)     { current_frame_ = image; }
    int GetCurrentFrame() const         { return current_frame_; }
    int GetImageCount() const;
    void IncrementCurrentFrame_Wrap();
    bool IncrementCurrentFrame_Clamp();

    void Think(float time_delta);
    void Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus);

    void OnMouseButtonPressed(int button, SpritePtr sprite_cursor, SpriteContainerPtr sprites_clickable);

    bool IsFocusable() const { return false; }

private:
    int current_frame_;
    ImageListPtr imagelist_;
    ImagePtr image_;
    std::string text_;
    float left_margin_;
    float right_margin_;
    float top_margin_;
    int num_chars_;
    Color color_font_;
    FontPtr font_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITESTATIC
