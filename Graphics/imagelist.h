#ifndef INCLUDED_GRAPHICS_IMAGELIST
#define INCLUDED_GRAPHICS_IMAGELIST

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <vector>
#include "../Utility/logcontext.h"

namespace Graphics
{

struct Vector;

class Texture;
typedef boost::shared_ptr<Texture> TexturePtr;

class Device;
typedef boost::shared_ptr<Device> DevicePtr;

class Image;
typedef boost::shared_ptr<Image> ImagePtr;

class ImageList :
    public Utility::LogContext

{
public:
    ImageList();
    ~ImageList();

    void Create(const std::string& filename, int num_frames, float frame_width, float frame_height, bool transparent = true, bool flip_rgb = true);
    void Delete();

    void Draw(float time_delta, DevicePtr device, const Vector& position);
    void Draw(float time_delta, DevicePtr device, const Vector& position, int frame);

    void SetCurrentFrame(int image);
    void IncCurrentFrame();
    void DecCurrentFrame();

    int GetCurrentFrame() const         { return current_frame_; }
    int GetImageCount() const           { return num_frames_; }

private:
    int current_frame_;
    int num_frames_;
    int num_frames_in_row_;
    float frame_width_;
    float frame_height_;
    float frame_s_;
    float frame_t_;
    TexturePtr texture_;
};

typedef boost::shared_ptr<ImageList> ImageListPtr;

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_IMAGELIST
