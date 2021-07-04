#include "stdafx.h"
#include "wave.h"
#include <fstream>
#include <sstream>
#include <boost/scoped_array.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

using namespace Audio;

Wave::Wave()
: LogContext("Audio::Wave")
, bps_(0)
, sps_(0)
, channels_(0)
, length_(0)
{
}

Wave::~Wave()
{
    Unload();
}

void Wave::Load(const std::string& filename)
{
    Unload();

    LOG("Loading [" << filename << "]");
    filename_ = filename;

    std::ifstream file(filename.c_str(), std::ios_base::binary);
    if(!file)
    {
        THROW_BASIC("Couldn't open the file [" << filename << "] for reading");
    }

    unsigned long bytes_read = 0;
    unsigned long chunk_id = 0;
    file.read(reinterpret_cast<char*>(&chunk_id), sizeof(unsigned long));
    if(chunk_id != MAKEFOURCC('R','I','F','F'))
    {
        THROW_BASIC("The file [" << filename << "] is not a valid wave file (no RIFF chunk)");
    }
    bytes_read += sizeof(unsigned long);

    std::streamoff file_length = 0;
    file.read(reinterpret_cast<char*>(&file_length), sizeof(unsigned long));
    bytes_read += sizeof(unsigned long);

    file.read(reinterpret_cast<char*>(&chunk_id), sizeof(unsigned long));
    if(chunk_id != MAKEFOURCC('W','A','V','E'))
    {
        THROW_BASIC("The file [" << filename << "] is not a valid wave file (no WAVE chunk)");
    }
    bytes_read += sizeof(unsigned long);

    unsigned long chunk_size;
    while(bytes_read < static_cast<unsigned long>(file_length+8))
    {
        chunk_id = 0;
        file.read(reinterpret_cast<char*>(&chunk_id), sizeof(unsigned long));
        bytes_read += sizeof(unsigned long);
        char* name = reinterpret_cast<char*>(&chunk_id);

        chunk_size = 0;
        file.read(reinterpret_cast<char*>(&chunk_size), sizeof(unsigned long));
        bytes_read += sizeof(unsigned long);

        switch(chunk_id)
        {
        case MAKEFOURCC('f','m','t',' '):
            ProcessFormatChunk(file, chunk_size);
            break;
        case MAKEFOURCC('d','a','t','a'):
            ProcessDataChunk(file, chunk_size);
            break;
        default:
            file.seekg(chunk_size, std::ios_base::cur);
            break;
        }
        bytes_read += chunk_size;
    }

    LOG("Read [" << length_ << "] bytes of audio data.  Audio format is [" << sps_ << "][" << bps_ << "][" << channels_ << "]");
}

void Wave::Unload()
{
    if(bytes_)
    {
        LOG("Unoading [" << filename_ << "]");
        bps_ = sps_ = channels_ = length_ = 0;
        bytes_.reset();
    }
}

void Wave::ProcessFormatChunk(std::ifstream& file, unsigned long chunk_size)
{
    boost::scoped_array<char> temp(new char[chunk_size]);
    file.read(temp.get(), chunk_size);

    WAVEFORMATEX* format = reinterpret_cast<WAVEFORMATEX*>(temp.get());

    sps_        = format->nSamplesPerSec;
    bps_        = format->wBitsPerSample;
    channels_   = format->nChannels;
}

void Wave::ProcessDataChunk(std::ifstream& file, unsigned long chunk_size)
{
    bytes_.reset(new unsigned char[chunk_size]);
    file.read(reinterpret_cast<char*>(bytes_.get()), chunk_size);
    length_ = chunk_size;
}
