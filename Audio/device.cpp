#include "stdafx.h"
#include "device.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Utility/hwconfig.h"

using namespace Audio;

Device::Device()
: LogContext("Audio::Device")
, dsound_(NULL)
, primary_(NULL)
, enable_(false)
{
}

Device::~Device()
{
}

void Device::Initialise(HWND window, Utility::HwConfig* hw_config)
{
    LOG("Initialising audio sub system");

    HRESULT rv = CoCreateInstance(CLSID_DirectSound8, NULL, CLSCTX_INPROC, IID_IDirectSound8,
        reinterpret_cast<void**>(&dsound_));
    if(FAILED(rv))
    {
        THROW_COM("Could not create an instance of CLSID_DirectSound8");
    }

    rv = dsound_->Initialize(NULL);
    if(FAILED(rv))
    {
        THROW_COM("Could not initialise an instance of CLSID_DirectSound8");
    }

    rv = dsound_->SetCooperativeLevel(window, DSSCL_PRIORITY);
    if(FAILED(rv))
    {
        THROW_COM("Could not initialise an instance of CLSID_DirectSound8");
    }

    CreatePrimaryBuffer();
    SetPrimaryBufferFormat(hw_config->GetSps(), hw_config->GetBps(), hw_config->GetChannels());

    enable_ = hw_config->IsAudioEnabled();
}

void Device::Shutdown()
{
    LOG("Shutting down audio sub system");

    if(primary_)
    {
        primary_->Release();
        primary_ = NULL;
    }
    if(dsound_)
    {
        dsound_->Release();
        dsound_ = NULL;
    }
}

void Device::CreatePrimaryBuffer()
{
    LOG("Creating primary buffer");

    DSBUFFERDESC info;
    memset(&info, 0, sizeof(DSBUFFERDESC));

    info.dwSize         = sizeof(DSBUFFERDESC);
    info.dwFlags        = DSBCAPS_CTRL3D | DSBCAPS_LOCSOFTWARE | DSBCAPS_PRIMARYBUFFER;

    HRESULT rv = dsound_->CreateSoundBuffer(&info, &primary_, NULL);
    if(FAILED(rv))
    {
        THROW_COM("Could not create the primary sound buffer");
    }

    primary_->Play(0, 0, 0);
}

void Device::SetPrimaryBufferFormat(int sps, int bps, int chan)
{
    LOG("Setting primary buffer format to sps=[" << sps << "], bps=[" << bps << "], channels=[" << chan << "]");

    WAVEFORMATEX format;
    memset(&format, 0, sizeof(WAVEFORMATEX));

    format.wFormatTag       = WAVE_FORMAT_PCM;
    format.nChannels        = chan;
    format.nSamplesPerSec   = sps;
    format.wBitsPerSample   = bps;
    format.nBlockAlign      = (format.nChannels * format.wBitsPerSample) >> 3;
    format.nAvgBytesPerSec  = format.nSamplesPerSec * format.nBlockAlign;

    HRESULT rv = primary_->SetFormat(&format);
    if(FAILED(rv))
    {
        THROW_COM("Could not set the primary sound buffer's format");
    }
}
