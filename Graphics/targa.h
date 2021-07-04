#ifndef INCLUDED_GRAPHICS_TARGA
#define INCLUDED_GRAPHICS_TARGA

#include <string>
#include <boost/shared_array.hpp>
#include "../Utility/logcontext.h"

namespace Graphics
{

class Targa :
    public Utility::LogContext
{
public:
    Targa();
    ~Targa();

    void Load(const std::string& filename);
    void Save(const std::string& filename, int w, int h, int bpp, boost::shared_array<unsigned char> pixels);
    void Unload();

    int GetWidth() const    { return width_; }
    int GetHeight() const   { return height_; }
    int GetBpp() const      { return bpp_; }

    boost::shared_array<unsigned char> GetPixels() const { return pixels_; }

private:
    void ReorderRGB();
    void Flip();

private:
    std::string filename_;
    int width_;
    int height_;
    int bpp_;
    boost::shared_array<unsigned char> pixels_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_TARGA
