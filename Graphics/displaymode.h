#ifndef INCLUDED_GRAPHICS_DISPLAYMODE
#define INCLUDED_GRAPHICS_DISPLAYMODE

#include <vector>
#include "../Utility/logcontext.h"

namespace Graphics
{

class DisplayMode :
    public Utility::LogContext
{
public:
    DisplayMode(int w, int h, int bpp, int hz);
    void Set();
    void operator=(const DisplayMode& rhs)
    { width_ = rhs.width_; height_ = rhs.height_; bpp_ = rhs.bpp_; hz_ = rhs.hz_; }
    bool Equals(int w, int h, int bpp, int hz) const
    { return (width_ == w && height_ == h && bpp == bpp_ && hz == hz_); }
private:
    int width_;
    int height_;
    int bpp_;
    int hz_;
};

void RestoreDisplayMode();

typedef std::vector<DisplayMode> DisplayModes;
DisplayModes GetDisplayModes();

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITE
