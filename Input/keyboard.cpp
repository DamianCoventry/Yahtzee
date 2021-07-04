#include "stdafx.h"
#include "keyboard.h"
#include <sstream>
#include <iomanip>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

namespace
{
    std::string Hex(HRESULT rv)
    {
        std::ostringstream oss;
        oss << "0x" << std::setw(8) << std::setfill('0') << std::hex << rv;
        return oss.str();
    }
};

using namespace Input;

Keyboard::Keyboard()
: Utility::LogContext("Input::Keyboard")
, dinput_(NULL)
{
    device_ = NULL;
    memset(keys_, 0, sizeof(char)*NUM_KEYS);
    memset(old_keys_, 0, sizeof(char)*NUM_KEYS);
}

Keyboard::~Keyboard()
{
}

void Keyboard::Initialise(IDirectInput8* dinput, HWND window)
{
    LOG("Initialising...");

    dinput_ = dinput;
    std::ostringstream oss;

    HRESULT rv = dinput_->CreateDevice(GUID_SysKeyboard, &device_, NULL);
    if(FAILED(rv))
    {
        THROW_COM("Could not create an instance of GUID_SysKeyboard");
    }

    rv = device_->SetDataFormat(&c_dfDIKeyboard);
    if(FAILED(rv))
    {
        THROW_COM("Could not set the data format for the keyboard device");
    }

    rv = device_->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if(FAILED(rv))
    {
        THROW_COM("Could not create an instance of GUID_SysKeyboard");
    }
}

void Keyboard::Shutdown()
{
    LOG("Shutting down...");

    if(device_)
    {
        device_->Release();
        device_ = NULL;
    }
}

void Keyboard::Activate()
{
    if(device_)
    {
        LOG("Activating...");

        HRESULT result = device_->Acquire();
        if(FAILED(result))
        {
            OutputDebugString("Failed to acquire Keyboard\n");
        }
    }
}

void Keyboard::Deactivate()
{
    if(device_)
    {
        LOG("Deactivating...");

        device_->Unacquire();
    }
}

void Keyboard::OnBeginFrame()
{
    if(device_)
    {
        HRESULT rv = device_->GetDeviceState(sizeof(char)*NUM_KEYS, keys_);
        if(FAILED(rv))
        {
            device_->Acquire();
        }
    }
}

void Keyboard::OnEndFrame()
{
    memcpy(old_keys_, keys_, sizeof(char)*NUM_KEYS);
}

bool Keyboard::IsKeyPressed(int key)
{
    return ((keys_[key] & 0x80) != 0) && ((old_keys_[key] & 0x80) == 0);
}

bool Keyboard::IsKeyReleased(int key)
{
    return ((keys_[key] & 0x80) == 0) && ((old_keys_[key] & 0x80) != 0);
}

bool Keyboard::IsKeyHeld(int key)
{
    return ((keys_[key] & 0x80) != 0);
}
