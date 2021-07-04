#include "stdafx.h"
#include "joystick.h"
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

Joystick::Joystick()
: Utility::LogContext("Input::Joystick")
, dinput_(NULL)
{
    device_ = NULL;
    memset(&guid_, 0, sizeof(GUID));
    memset(&state_, 0,sizeof(DIJOYSTATE));
    memset(&old_state_, 0,sizeof(DIJOYSTATE));
}

Joystick::~Joystick()
{
}

void Joystick::Initialise(IDirectInput8* dinput, HWND window, const GUID& guid)
{
    LOG("Initialising...");

    dinput_ = dinput;
    memcpy(&guid_, &(guid), sizeof(GUID));
    std::ostringstream oss;

    HRESULT rv = dinput_->CreateDevice(guid_, &device_, NULL);
    if(FAILED(rv))
    {
        THROW_COM("Could not create an instance of a joystick");
    }

    rv = device_->SetDataFormat(&c_dfDIJoystick);
    if(FAILED(rv))
    {
        THROW_COM("Could not set the data format for the Joystick device");
    }

    rv = device_->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if(FAILED(rv))
    {
        THROW_COM("Could not create an instance of GUID_SysJoystick");
    }

    rv = device_->EnumObjects(JoystickCallback, reinterpret_cast<void*>(this), DIDFT_AXIS);
    if(FAILED(rv))
    {
        THROW_COM("Could not enumerate the axies of the Joystick");
    }
}

void Joystick::Shutdown()
{
    LOG("Shutting down...");

    if(device_)
    {
        device_->Release();
        device_ = NULL;
    }
}

void Joystick::Activate()
{
    if(device_)
    {
        LOG("Activating...");

        HRESULT result = device_->Acquire();
        if(FAILED(result))
        {
            OutputDebugString("Failed to acquire Joystick\n");
        }
    }
}

void Joystick::Deactivate()
{
    if(device_)
    {
        LOG("Deactivating...");

        device_->Unacquire();
    }
}

void Joystick::OnBeginFrame()
{
    if(device_)
    {
        HRESULT rv = device_->Poll();
        if(FAILED(rv))
        {
            device_->Acquire();
            return;
        }

        rv = device_->GetDeviceState(sizeof(DIJOYSTATE), &state_);
        if(FAILED(rv))
        {
            device_->Acquire();
        }
    }
}

void Joystick::OnEndFrame()
{
    memcpy(&old_state_, &state_, sizeof(DIJOYSTATE));
}

bool Joystick::IsButtonPressed(int button)
{
    return (state_.rgbButtons[button] != 0) && (old_state_.rgbButtons[button] == 0);
}

bool Joystick::IsButtonReleased(int button)
{
    return (state_.rgbButtons[button] == 0) && (old_state_.rgbButtons[button] != 0);
}

bool Joystick::IsButtonHeld(int button)
{
    return (state_.rgbButtons[button] != 0);
}

bool Joystick::IsPovButtonPressed(Direction dir, int pov)
{
    int pos;
    switch(dir)
    {
    case DIR_UP:    pos =     0; break;
    case DIR_RIGHT: pos =  9000; break;
    case DIR_DOWN:  pos = 18000; break;
    case DIR_LEFT:  pos = 27000; break;
    }
    return (state_.rgdwPOV[pov] == pos) && (old_state_.rgdwPOV[pov] != pos);
}

bool Joystick::IsPovButtonReleased(Direction dir, int pov)
{
    int pos;
    switch(dir)
    {
    case DIR_UP:    pos =     0; break;
    case DIR_RIGHT: pos =  9000; break;
    case DIR_DOWN:  pos = 18000; break;
    case DIR_LEFT:  pos = 27000; break;
    }
    return (state_.rgdwPOV[pov] != pos) && (old_state_.rgdwPOV[pov] == pos);
}

bool Joystick::IsPovButtonHeld(Direction dir, int pov)
{
    int pos;
    switch(dir)
    {
    case DIR_UP:    pos =     0; break;
    case DIR_RIGHT: pos =  9000; break;
    case DIR_DOWN:  pos = 18000; break;
    case DIR_LEFT:  pos = 27000; break;
    }
    return (state_.rgdwPOV[pov] == pos);
}

void Joystick::GetMovement(int* x_delta, int* y_delta, int* z_delta)
{
    if(x_delta)
    {
        (*x_delta) = state_.lX;
    }
    if(y_delta)
    {
        (*y_delta) = state_.lY;
    }
    if(z_delta)
    {
        (*z_delta) = state_.lZ;
    }
}

void Joystick::GetRotation(int* x_delta, int* y_delta, int* z_delta)
{
    if(x_delta)
    {
        (*x_delta) = state_.lRx;
    }
    if(y_delta)
    {
        (*y_delta) = state_.lRy;
    }
    if(z_delta)
    {
        (*z_delta) = state_.lRz;
    }
}

int Joystick::GetSliderPos(int slider)
{
    if(slider == 0 || slider == 1)
    {
        return state_.rglSlider[slider];
    }
    return 0;
}


BOOL PASCAL Joystick::JoystickCallback(const DIDEVICEOBJECTINSTANCE* device, void* param)
{
    Joystick* this_ = reinterpret_cast<Joystick*>(param);

    DIPROPRANGE property;
    memset(&property, 0,sizeof(DIPROPRANGE));

    property.diph.dwSize        = sizeof(DIPROPRANGE); 
    property.diph.dwHeaderSize  = sizeof(DIPROPHEADER); 
    property.diph.dwHow         = DIPH_BYID; 
    property.diph.dwObj         = device->dwType; 
    property.lMin               = -Joystick::RANGE; 
    property.lMax               =  Joystick::RANGE; 

    HRESULT hr = this_->device_->SetProperty(DIPROP_RANGE, &property.diph);

    return FAILED(hr) ? DIENUM_STOP : DIENUM_CONTINUE;
}
