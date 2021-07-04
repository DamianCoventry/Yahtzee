#ifndef INCLUDED_GRAPHICS_FONT
#define INCLUDED_GRAPHICS_FONT

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include "color.h"
#include "../Utility/logcontext.h"

namespace Graphics
{

struct Vector;

class Device;
typedef boost::shared_ptr<Device> DevicePtr;

class Font :
    public Utility::LogContext

{
public:
    Font();
    ~Font();

    void Create(HWND window, const std::string& face_name, int point_size, bool bold = false);
    void Delete();

    void Write(const std::string& text, DevicePtr device, const Vector& position);

    void SetColor(const Color& color) { color_ = color; }

    float GetPixelWidth(const std::string& text) const;
    float GetPixelHeight(const std::string& text) const;
    int GetNumChars(const std::string& text, float pixel_width) const;

private:
    std::string face_name_;
    int point_size_;
    Color color_;
    HWND window_;
    HDC dc_;
    HFONT font_;
    unsigned int gl_list_;
};

typedef boost::shared_ptr<Font> FontPtr;

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_FONT
