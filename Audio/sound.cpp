#include "stdafx.h"
#include "sound.h"
#include <sstream>
#include <iomanip>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "device.h"

using namespace Audio;

namespace
{
    std::string Hex(HRESULT rv)
    {
        std::ostringstream oss;
        oss << "0x" << std::setw(8) << std::setfill('0') << std::hex << rv;
        return oss.str();
    }
};

Sound::Sound(DevicePtr audio)
: LogContext("Audio::Sound")
, audio_(audio)
, dsound_(audio->GetDSound())
{
    for(int i = 0; i < NUM_BUFFERS; i++)
    {
        buffers_[i] = NULL;
    }
}

Sound::~Sound()
{
    Delete();
}

void Sound::Create(const Wave& wave)
{
    LOG("Creating a sound object from [" << wave.GetLength() << "] bytes of audio data");

    WAVEFORMATEX format;
    memset(&format, 0, sizeof(WAVEFORMATEX));
    format.cbSize           = sizeof(WAVEFORMATEX);
    format.wFormatTag       = WAVE_FORMAT_PCM;
    format.nSamplesPerSec   = wave.GetSps();
    format.wBitsPerSample   = wave.GetBps();
    format.nChannels        = wave.GetChannels();
    format.nBlockAlign      = (format.nChannels * format.wBitsPerSample) / 8;
    format.nAvgBytesPerSec  = format.nSamplesPerSec * format.nBlockAlign;

    DSBUFFERDESC info;
    memset(&info, 0, sizeof(DSBUFFERDESC));
    info.dwSize         = sizeof(DSBUFFERDESC);
    info.dwFlags        = DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE | DSBCAPS_STATIC;
    info.dwBufferBytes  = wave.GetLength();
    info.lpwfxFormat    = &format;

    IDirectSoundBuffer* buffer = NULL;
    HRESULT rv = dsound_->CreateSoundBuffer(&info, &buffer, NULL);
    if(FAILED(rv))
    {
        THROW_BASIC("Could not create an instance of a DirectSoundBuffer object [" << Hex(rv) << "]");
    }

    current_buffer_ = 0;

    CopyAudioData(buffer, wave.GetBytes(), wave.GetLength());

    buffers_[0] = buffer;
    for(int i = 1; i < NUM_BUFFERS; i++)
    {
        dsound_->DuplicateSoundBuffer(buffer, &buffers_[i]);
    }
}

void Sound::Delete()
{
    LOG("Deleting a sound object");
    for(int i = 0; i < NUM_BUFFERS; i++)
    {
        if(buffers_[i])
        {
            buffers_[i]->Release();
            buffers_[i] = NULL;
        }
    }
}

void Sound::Play(bool looped) const
{
    if(!audio_->IsEnabled())
    {
        return;
    }

    buffers_[current_buffer_]->Play(0, 0, looped ? DSBPLAY_LOOPING : 0);

    current_buffer_++;
    if(current_buffer_ >= NUM_BUFFERS)
    {
        current_buffer_ = 0;
    }
}

void Sound::CopyAudioData(IDirectSoundBuffer* buffer, boost::shared_array<unsigned char> bytes, int num_bytes)
{
    unsigned char* ptr1, *ptr2;
    DWORD num1, num2;

    HRESULT rv = buffer->Lock(
        0, num_bytes,
        reinterpret_cast<void**>(&ptr1), &num1,
        reinterpret_cast<void**>(&ptr2), &num2,
        DSBLOCK_FROMWRITECURSOR);
    if(FAILED(rv))
    {
        THROW_BASIC("Could not lock an instance of a DirectSoundBuffer object [" << Hex(rv) << "]");
    }

    memcpy(ptr1, bytes.get(), num1);
    if(ptr2 && num2)
    {
        memcpy(ptr2, bytes.get() + num1, num2);
    }

    buffer->Unlock(ptr1, num1, ptr2, num2);
}

void Sound::SetVolume(long volume)
{
    // Clamp the value
    if(volume < 0) volume = 0;
    else if(volume > 100) volume = 100;

    // Convert it to a value DirectSound expects.
    long range = DSBVOLUME_MAX - DSBVOLUME_MIN;
    float percent = float(volume) / 100.0f;
    long converted_volume = DSBVOLUME_MIN + long(float(range) * percent);

    // Apply it.
    for(int i = 0; i < NUM_BUFFERS; i++)
    {
        if(buffers_[i])
        {
            buffers_[i]->SetVolume(converted_volume);
        }
    }
}
