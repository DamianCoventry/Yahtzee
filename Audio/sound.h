#ifndef INCLUDED_SOUND
#define INCLUDED_SOUND

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <string>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include "../Utility/logcontext.h"
#include "wave.h"

namespace Audio
{

class Device;
typedef boost::shared_ptr<Device> DevicePtr;

class Sound :
    public Utility::LogContext
{
public:
    Sound(DevicePtr audio);
    ~Sound();
    void Create(const Wave& wave);
    void Delete();
    void Play(bool looped = false) const;
    void SetVolume(long volume);

private:
    void CopyAudioData(IDirectSoundBuffer* buffer, boost::shared_array<unsigned char> bytes, int num_bytes);

private:
    enum CONSTANTS
    { NUM_BUFFERS = 8 };

    DevicePtr audio_;
    IDirectSound8* dsound_;
    IDirectSoundBuffer* buffers_[NUM_BUFFERS];
    mutable int current_buffer_;
};

typedef boost::shared_ptr<Sound> SoundPtr;

}       // namespace Audio

#endif  // INCLUDED_SOUND
