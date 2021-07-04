#include "stdafx.h"
#include "targa.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

#pragma warning(disable : 4311)     // warning C4311: 'reinterpret_cast' : pointer truncation from 'LPVOID' to 'LONG'
#pragma warning(disable : 4312)     // warning C4312: 'reinterpret_cast' : conversion from 'LONG' to 'Window *' of greater size

using namespace Graphics;

namespace
{
#pragma pack(push, 1)
    struct TgaHeader
    {
        unsigned char id_length_;
        unsigned char color_map_type_;
        unsigned char image_type_;
        unsigned short color_map_start_;
        unsigned short color_map_length_;
        unsigned char color_map_depth_;
        unsigned short x_offset_;
        unsigned short y_offset_;
        unsigned short width_;
        unsigned short height_;
        unsigned char pixel_depth_;
        unsigned char image_descriptor_;
    };
#pragma pack(pop)

    enum TgaType
    {
        TGA_NOIMAGEDATA         = 0,
        TGA_INDEXEDCOLOR        = 1,
        TGA_TRUECOLOR           = 2,
        TGA_MONOCHROME          = 3,
        TGA_INDEXEDCOLOR_RLE    = 9,
        TGA_TRUECOLOR_RLE       = 10,
        TGA_MONOCHROME_RLE      = 11,
    };
};

Targa::Targa()
: Utility::LogContext("Graphics::Targa")
, width_(0)
, height_(0)
, bpp_(0)
{
}

Targa::~Targa()
{
    Unload();
}

void Targa::Load(const std::string& filename)
{
    Unload();
    filename_ = filename;

    LOG("Loading [" << filename << "]");
    std::ifstream file(filename.c_str(), std::ios_base::binary);
    if(!file)
    {
        THROW_BASIC("Couldn't open the file [" << filename << "] for reading");
    }

    // get the image info
    TgaHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(TgaHeader));

    // currently only supporting true color non compressed images
    if(header.image_type_ != TGA_TRUECOLOR)
    {
        THROW_BASIC("Couldn't load the file [" << filename << "] is not a true color uncompressed image");
    }

    // check for the optional image id field
    if(header.id_length_ > 0)
    {
        char image_id[256];
        file.read(reinterpret_cast<char*>(image_id), header.id_length_);
        image_id[header.id_length_] = 0;
    }

    // check for the optional palette
    if(header.color_map_type_ > 0)
    {
        // currently we'll just seek past the palette
        file.seekg(header.color_map_length_ * (header.color_map_depth_ >> 3), std::ios_base::cur);
    }

    width_      = header.width_;
    height_     = header.height_;
    bpp_        = header.pixel_depth_;

    // read in the truecolor data
    pixels_.reset(new unsigned char[header.width_ * header.height_ * (header.pixel_depth_ >> 3)]);
    file.read(reinterpret_cast<char*>(pixels_.get()), header.width_ * header.height_ * (header.pixel_depth_ >> 3));

    ReorderRGB();
    Flip();
}

void Targa::Save(const std::string& filename, int w, int h, int bpp, boost::shared_array<unsigned char> pixels)
{
    LOG("Saving [" << filename << "]");
    std::ofstream file(filename.c_str(), std::ios_base::binary);
    if(!file)
    {
        THROW_BASIC("Couldn't open the file [" << filename << "] for writing");
    }

    TgaHeader header;
    memset(&header, 0, sizeof(TgaHeader));
    header.image_type_  = TGA_TRUECOLOR;
    header.width_       = width_;
    header.height_      = height_;
    header.pixel_depth_ = bpp_;

    file.write(reinterpret_cast<char*>(&header), sizeof(TgaHeader));
    file.write(reinterpret_cast<char*>(pixels.get()), w * h * (bpp >> 3));
}

void Targa::Unload()
{
    if(!filename_.empty())
    {
        LOG("Unloading [" << filename_ << "]");
        width_  = 0;
        height_ = 0;
        bpp_    = 0;
        pixels_.reset();
        filename_.clear();
    }
}

void Targa::Flip()
{
    // calc how many bytes per scanline
    long bytes_per_scanline = width_ * (bpp_ >> 3);

    // check if the bitmap's upside down
    unsigned char *temp = new unsigned char[height_ * bytes_per_scanline];

    // copy each line
    for(int row_index = 0; row_index < height_; row_index++)
    {
        memcpy(&temp[((height_ - 1) - row_index) * bytes_per_scanline]
              , &pixels_[row_index * bytes_per_scanline]
              , bytes_per_scanline);
    }

    // store the final pointer
    pixels_.reset(temp);
}

void Targa::ReorderRGB()
{
    unsigned char *temp = new unsigned char[width_*height_*4];

    for(int y = 0; y < height_; y++)
    {
        for(int x = 0; x < width_; x++)
        {
            int offset = y * width_ + x;

            temp[(offset * 4) + 0] = pixels_[(offset * 4) + 2];
            temp[(offset * 4) + 1] = pixels_[(offset * 4) + 1];
            temp[(offset * 4) + 2] = pixels_[(offset * 4) + 0];
            temp[(offset * 4) + 3] = pixels_[(offset * 4) + 3];
        }
    }

    pixels_.reset(temp);
}
