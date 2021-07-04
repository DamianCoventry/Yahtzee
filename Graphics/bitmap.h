#ifndef INCLUDED_GRAPHICS_BITMAP
#define INCLUDED_GRAPHICS_BITMAP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <boost/shared_array.hpp>
#include "../Utility/logcontext.h"

namespace Graphics
{

class Bitmap :
    public Utility::LogContext

{
public:
    Bitmap();
    ~Bitmap();

    void Load(const std::string& filename);
    void LoadGreyscale(const std::string& filename);
    void Save(const std::string& filename, int w, int h, int bpp, boost::shared_array<unsigned char> pixels);

    void Unload();

    void SetTransparentAlpha(bool flip_rgb = true);
    void SetTransparentAlpha(int trans_r, int trans_g, int trans_b, bool flip_rgb = true);
    void SetOpaqueAlpha(bool flip_rgb = true);

    int GetWidth() const    { return info_header_.biWidth; }
    int GetHeight() const   { return info_header_.biHeight; }
    int GetBpp() const      { return info_header_.biBitCount; }

    boost::shared_array<unsigned char> GetPixels() const { return pixels_; }

private:
    void Flip();

private:
    std::string filename_;
    BITMAPFILEHEADER file_header_;
    BITMAPINFOHEADER info_header_;
    boost::shared_array<unsigned char> pixels_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_BITMAP
