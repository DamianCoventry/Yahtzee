#ifndef INCLUDED_GRAPHICS_IMAGE
#define INCLUDED_GRAPHICS_IMAGE

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include "../Utility/logcontext.h"
#include "texture.h"

namespace Graphics
{

struct Vector;

class Device;
typedef boost::shared_ptr<Device> DevicePtr;

class Image :
    public Utility::LogContext

{
public:
    Image();
    ~Image();

    void Create(int frame_x, int frame_y, float frame_width, float frame_height, TexturePtr texture);
    void Draw(float time_delta, DevicePtr device, const Vector& position);

    int GetWidth() const    { return int(frame_width_); }
    int GetHeight() const   { return int(frame_height_); }

private:
    float frame_width_;
    float frame_height_;
    float s0_, s1_;
    float t0_, t1_;
    TexturePtr texture_;
};

typedef boost::shared_ptr<Image> ImagePtr;

ImagePtr MakeImage(int frame_x, int frame_y, float frame_width, float frame_height, TexturePtr texture);

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_IMAGE
