#ifndef INCLUDED_GRAPHICS_DEVICE
#define INCLUDED_GRAPHICS_DEVICE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <boost/shared_ptr.hpp>
#include "../Utility/logcontext.h"

namespace Utility
{
class HwConfig;
}

namespace Graphics
{

struct Vector;
class Color;

class Device :
    public Utility::LogContext
{
public:
    Device();
    ~Device();

    void Initialise(HWND window, Utility::HwConfig* hw_config);
    void Shutdown(HWND window);

    void Set3D();
    void Set2D();

    void OnBeginFrame();
    void OnEndFrame();

    void DrawEmptyRect(const Vector& tl, const Vector& br, const Color& color);
    void DrawFilledRect(const Vector& tl, const Vector& br, const Color& color);
    void DrawFocusRect(const Vector& tl, const Vector& br, const Color& color);

private:
    HDC dc_;
    HGLRC rc_;
    Utility::HwConfig* hw_config_;
};

typedef boost::shared_ptr<Device> DevicePtr;

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_DEVICE
