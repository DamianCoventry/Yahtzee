#ifndef INCLUDED_AUDIO_DEVICE
#define INCLUDED_AUDIO_DEVICE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <boost/shared_ptr.hpp>
#include "../Utility/logcontext.h"

namespace Utility
{
class HwConfig;
}

namespace Audio
{

class Device :
    public Utility::LogContext
{
public:
    Device();
    ~Device();

    void Initialise(HWND window, Utility::HwConfig* hw_config);
    void Shutdown();

    void Enable(bool enable)    { enable_ = enable; }
    bool IsEnabled() const      { return enable_; }

    IDirectSound8* GetDSound() const { return dsound_; }

private:
    void CreatePrimaryBuffer();
    void SetPrimaryBufferFormat(int sps, int bps, int chan);

private:
    IDirectSound8* dsound_;
    IDirectSoundBuffer* primary_;
    bool enable_;
};

typedef boost::shared_ptr<Device> DevicePtr;

}       // namespace Audio

#endif  // INCLUDED_AUDIO_DEVICE
