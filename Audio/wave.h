#ifndef INCLUDED_AUDIO_WAVE
#define INCLUDED_AUDIO_WAVE

#include <string>
#include <boost/shared_array.hpp>
#include "../Utility/logcontext.h"

namespace Audio
{

class Wave :
    public Utility::LogContext
{
public:
    Wave();
    ~Wave();

    void Load(const std::string& filename);
    void Unload();

    int GetBps() const      { return bps_; }
    int GetSps() const      { return sps_; }
    int GetChannels() const { return channels_; }
    int GetLength() const   { return length_; }

    boost::shared_array<unsigned char> GetBytes() const { return bytes_; }

private:
    void ProcessFormatChunk(std::ifstream& file, unsigned long chunk_size);
    void ProcessDataChunk(std::ifstream& file, unsigned long chunk_size);

private:
    std::string filename_;
    int bps_;
    int sps_;
    int channels_;
    int length_;
    boost::shared_array<unsigned char> bytes_;
};

}       // namespace Audio

#endif  // INCLUDED_AUDIO_WAVE
