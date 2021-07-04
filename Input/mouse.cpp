#include "stdafx.h"
#include "mouse.h"
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

Mouse::Mouse()
: Utility::LogContext("Input::Mouse")
, dinput_(NULL)
{
    device_ = NULL;
    memset(&state_, 0,sizeof(DIMOUSESTATE));
    memset(&old_state_, 0,sizeof(DIMOUSESTATE));
}

Mouse::~Mouse()
{
}

void Mouse::Initialise(IDirectInput8* dinput, HWND window)
{
    LOG("Initialising...");

    dinput_ = dinput;
    std::ostringstream oss;

    HRESULT rv = dinput_->CreateDevice(GUID_SysMouse, &device_, NULL);
    if(FAILED(rv))
    {
        THROW_COM("Could not create an instance of GUID_SysMouse");
    }

    rv = device_->SetDataFormat(&c_dfDIMouse);
    if(FAILED(rv))
    {
        THROW_COM("Could not set the data format for the Mouse device");
    }

    rv = device_->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if(FAILED(rv))
    {
        THROW_COM("Could not create an instance of GUID_SysMouse");
    }
}

void Mouse::Shutdown()
{
    LOG("Shutting down...");

    if(device_)
    {
        device_->Release();
        device_ = NULL;
    }
}

void Mouse::Activate()
{
    if(device_)
    {
        LOG("Activating...");

        HRESULT result = device_->Acquire();
        if(FAILED(result))
        {
            OutputDebugString("Failed to acquire Mouse\n");
        }
    }
}

void Mouse::Deactivate()
{
    if(device_)
    {
        LOG("Deactivating...");

        device_->Unacquire();
    }
}

void Mouse::OnBeginFrame()
{
    if(device_)
    {
        HRESULT rv = device_->GetDeviceState(sizeof(DIMOUSESTATE), &state_);
        if(FAILED(rv))
        {
            device_->Acquire();
        }
    }
}

void Mouse::OnEndFrame()
{
    memcpy(&old_state_, &state_, sizeof(DIMOUSESTATE));
}

bool Mouse::IsButtonPressed(int button)
{
    return (state_.rgbButtons[button] != 0) && (old_state_.rgbButtons[button] == 0);
}

bool Mouse::IsButtonReleased(int button)
{
    return (state_.rgbButtons[button] == 0) && (old_state_.rgbButtons[button] != 0);
}

bool Mouse::IsButtonHeld(int button)
{
    return (state_.rgbButtons[button] != 0);
}

void Mouse::GetMovement(int* x_delta, int* y_delta)
{
    if(x_delta)
    {
        (*x_delta) = state_.lX;
    }
    if(y_delta)
    {
        (*y_delta) = state_.lY;
    }
}
