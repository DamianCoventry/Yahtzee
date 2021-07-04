#include "stdafx.h"
#include "bitmap.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

#define BITMAP_ID       0x4d42

using namespace Graphics;

Bitmap::Bitmap()
: Utility::LogContext("Graphics::Bitmap")
{
    memset(&file_header_, 0, sizeof(BITMAPFILEHEADER));
    memset(&info_header_, 0, sizeof(BITMAPINFOHEADER));
}

Bitmap::~Bitmap()
{
    Unload();
}

void Bitmap::Load(const std::string& filename)
{
    Unload();
    filename_ = filename;

    LOG("Loading [" << filename << "]");
    std::ifstream file(filename.c_str(), std::ios_base::binary);
    if(!file)
    {
        THROW_BASIC("Couldn't open the image file [" << filename << "]");
    }

    file.read(reinterpret_cast<char*>(&file_header_), sizeof(BITMAPFILEHEADER));
    file.read(reinterpret_cast<char*>(&info_header_), sizeof(BITMAPINFOHEADER));

    // check it's a bitmap File, is not compressed and is 24 bit color
    if(BITMAP_ID   != file_header_.bfType ||
       BI_RGB      != info_header_.biCompression ||
       24          != info_header_.biBitCount)
    {
        THROW_BASIC("The image file [" << filename << "] is either not a bitmap, or is compressed, or is not 24 BPP");
    }

    // read the rgb data - check if the bitmap's upside down first
    long size;
    if(info_header_.biHeight < 0)
    {
        size = info_header_.biWidth * -info_header_.biHeight * (info_header_.biBitCount >> 3);
    }
    else
    {
        size = info_header_.biWidth * info_header_.biHeight * (info_header_.biBitCount >> 3);
    }

    pixels_.reset(new unsigned char[size]);
    file.read(reinterpret_cast<char*>(pixels_.get()), size);

    // flip the bitmap's rgb values if necessary
    if(info_header_.biHeight >= 0)
    {
        Flip();
    }
    else
    {
        info_header_.biHeight = -info_header_.biHeight;
    }
}

void Bitmap::LoadGreyscale(const std::string& filename)
{
    Unload();
    filename_ = filename;

    LOG("Loading [" << filename << "]");
    std::ifstream file(filename.c_str(), std::ios_base::binary);
    if(!file)
    {
        THROW_BASIC("Couldn't open the image file [" << filename << "]");
    }

    unsigned char* temp = new unsigned char[info_header_.biWidth * info_header_.biHeight];

    int gray_index = 0;
    for(int bitmap_index = 0; bitmap_index < (int)info_header_.biSize; bitmap_index += 3)
    {
        temp[gray_index] = pixels_[bitmap_index];
        gray_index++;
    }

    pixels_.reset(temp);

    info_header_.biBitCount = 8;
    info_header_.biSize /= 3;
}

void Bitmap::Flip()
{
    unsigned char *temp;
    long bytes_per_scanline;
    long height;
    int row_index;

    // calc how many bytes per scanline
    bytes_per_scanline = info_header_.biWidth * (info_header_.biBitCount >> 3);

    // check if the bitmap's upside down
    if(info_header_.biHeight < 0)
    {
        temp = new unsigned char[-info_header_.biHeight * bytes_per_scanline];
        height = -info_header_.biHeight;
    }
    else
    {
        temp = new unsigned char[info_header_.biHeight * bytes_per_scanline];
        height = info_header_.biHeight;
    }

    // copy each line
    for(row_index = 0; row_index < height; row_index++)
    {
        memcpy(&temp[((height - 1) - row_index) * bytes_per_scanline]
              , &pixels_[row_index * bytes_per_scanline]
              , bytes_per_scanline);
    }

    // store the final pointer
    pixels_.reset(temp);
}

void Bitmap::SetTransparentAlpha(bool flip_rgb)
{
    unsigned char *temp = new unsigned char[info_header_.biWidth*info_header_.biHeight*4];
    SetTransparentAlpha(pixels_[2], pixels_[1], pixels_[0], flip_rgb);
}

void Bitmap::SetTransparentAlpha(int trans_r, int trans_g, int trans_b, bool flip_rgb)
{
    unsigned char *temp = new unsigned char[info_header_.biWidth*info_header_.biHeight*4];

    for(int y = 0; y < info_header_.biHeight; y++)
    {
        for(int x = 0; x < info_header_.biWidth; x++)
        {
            int offset = y * info_header_.biWidth + x;

            if(flip_rgb)
            {
                temp[(offset * 4) + 0] = pixels_[(offset * 3) + 2];
                temp[(offset * 4) + 1] = pixels_[(offset * 3) + 1];
                temp[(offset * 4) + 2] = pixels_[(offset * 3) + 0];
            }
            else
            {
                temp[(offset * 4) + 0] = pixels_[(offset * 3) + 0];
                temp[(offset * 4) + 1] = pixels_[(offset * 3) + 1];
                temp[(offset * 4) + 2] = pixels_[(offset * 3) + 2];
            }

            if(trans_b == pixels_[(offset * 3) + 0] && trans_g == pixels_[(offset * 3) + 1] && trans_r  == pixels_[(offset * 3) + 2])
            {
                temp[(offset * 4) + 3] = (unsigned char)0;
            }
            else
            {
                temp[(offset * 4) + 3] = (unsigned char)0xFF;
            }
        }
    }

    pixels_.reset(temp);
    info_header_.biBitCount = 32;
}

void Bitmap::SetOpaqueAlpha(bool flip_rgb)
{
    unsigned char *temp = new unsigned char[info_header_.biWidth*info_header_.biHeight*4];

    for(int y = 0; y < info_header_.biHeight; y++)
    {
        for(int x = 0; x < info_header_.biWidth; x++)
        {
            int offset = y * info_header_.biWidth + x;

            if(flip_rgb)
            {
                temp[(offset * 4) + 0] = pixels_[(offset * 3) + 2];
                temp[(offset * 4) + 1] = pixels_[(offset * 3) + 1];
                temp[(offset * 4) + 2] = pixels_[(offset * 3) + 0];
            }
            else
            {
                temp[(offset * 4) + 0] = pixels_[(offset * 3) + 0];
                temp[(offset * 4) + 1] = pixels_[(offset * 3) + 1];
                temp[(offset * 4) + 2] = pixels_[(offset * 3) + 2];
            }
            temp[(offset * 4) + 3] = (unsigned char)0xFF;
        }
    }

    pixels_.reset(temp);
    info_header_.biBitCount = 32;
}

void Bitmap::Save(const std::string& filename, int w, int h, int bpp, boost::shared_array<unsigned char> pixels)
{
    LOG("Saving [" << filename << "]");
    std::ofstream file(filename.c_str(), std::ios_base::binary);
    if(!file)
    {
        THROW_BASIC("Couldn't open the image file [" << filename << "]");
    }

    BITMAPFILEHEADER file_header;
    memset(&file_header, 0, sizeof(BITMAPFILEHEADER));
    file_header.bfType    = BITMAP_ID;
    file_header.bfSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO) + (w * h * (bpp >> 3));
    file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO);

    BITMAPINFO info;
    memset(&info, 0, sizeof(BITMAPINFO));
    info.bmiHeader.biSize           = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth          = w;
    info.bmiHeader.biHeight         = -h;
    info.bmiHeader.biPlanes         = 1;
    info.bmiHeader.biBitCount       = bpp;
    info.bmiHeader.biCompression    = BI_RGB;

    file.write(reinterpret_cast<char*>(&file_header), sizeof(BITMAPFILEHEADER));
    file.write(reinterpret_cast<char*>(&info), sizeof(BITMAPINFO));
    file.write(reinterpret_cast<char*>(pixels.get()), w * h * (bpp >> 3));
}

void Bitmap::Unload()
{
    if(!filename_.empty())
    {
        LOG("Unloading [" << filename_ << "]");
        memset(&file_header_, 0, sizeof(BITMAPFILEHEADER));
        memset(&info_header_, 0, sizeof(BITMAPINFOHEADER));
        pixels_.reset();
        filename_.clear();
    }
}
