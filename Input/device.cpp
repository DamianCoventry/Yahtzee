#include "stdafx.h"
#include "device.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "keyboard.h"
#include "mouse.h"
#include "joystick.h"
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

Device::Device(InputEventHandler* event_handler)
: LogContext("Input::Device")
, keyboard_(new Keyboard)
, mouse_(new Mouse)
, joystick_(new Joystick)
, event_handler_(event_handler)
{
    for(int i = 0; i < Joystick::NUM_SLIDERS; i++)
    {
        slider_pos[i] = 0;
    }
}

Device::~Device()
{
}

void Device::Initialise(HINSTANCE instance, HWND window)
{
    LOG("Initialising input sub system");

    std::ostringstream oss;

    HRESULT rv = CoCreateInstance(CLSID_DirectInput8, NULL, CLSCTX_INPROC, IID_IDirectInput8,
        reinterpret_cast<void**>(&dinput_));
    if(FAILED(rv))
    {
        THROW_COM("Could not create an instance of CLSID_IDirectInput8");
    }

    rv = dinput_->Initialize(instance, DIRECTINPUT_VERSION);
    if(FAILED(rv))
    {
        THROW_COM("Could not Initialize an instance of CLSID_IDirectInput8");
    }

    keyboard_->Initialise(dinput_, window);
    mouse_->Initialise(dinput_, window);
}

void Device::Shutdown()
{
    LOG("Shutting down input sub system");

    if(dinput_)
    {
        keyboard_->Shutdown();
        mouse_->Shutdown();
        joystick_->Shutdown();

        dinput_->Release();
        dinput_ = NULL;
    }
}

std::vector<Device::JoystickInfo> Device::EnumerateJoysticks()
{
    LOG("Enumerating joysticks");

    std::vector<JoystickInfo> info;
    dinput_->EnumDevices(DI8DEVCLASS_GAMECTRL, InputCallback, reinterpret_cast<void*>(&info), DIEDFL_ATTACHEDONLY);

    return info;
}

void Device::ChooseJoystick(HWND window, const GUID& guid)
{
    joystick_->Initialise(dinput_, window, guid);
}

void Device::OnBeginFrame()
{
    keyboard_->OnBeginFrame();
    mouse_->OnBeginFrame();
    joystick_->OnBeginFrame();
}

void Device::OnEndFrame()
{
    keyboard_->OnEndFrame();
    mouse_->OnEndFrame();
    joystick_->OnEndFrame();
}

void Device::Activate()
{
    LOG("Activating input devices");

    keyboard_->Activate();
    mouse_->Activate();
    joystick_->Activate();
}

void Device::Deactivate()
{
    LOG("Deactivating input devices");

    keyboard_->Deactivate();
    mouse_->Deactivate();
    joystick_->Deactivate();
}

void Device::Poll()
{
    int i, x, y, z;

    // Keyboard.
    bool shift_held = (keyboard_->IsKeyHeld(DIK_LSHIFT)     || keyboard_->IsKeyHeld(DIK_RSHIFT));
    bool ctrl_held  = (keyboard_->IsKeyHeld(DIK_LCONTROL)   || keyboard_->IsKeyHeld(DIK_RCONTROL));
    bool alt_held   = (keyboard_->IsKeyHeld(DIK_LMENU)      || keyboard_->IsKeyHeld(DIK_RMENU));
    bool win_held   = (keyboard_->IsKeyHeld(DIK_LWIN)       || keyboard_->IsKeyHeld(DIK_RWIN));
    for(i = 0; i < Keyboard::NUM_KEYS; i++)
    {
        if(keyboard_->IsKeyPressed(i))
        {
            event_handler_->OnKeyPressed(i, shift_held, ctrl_held, alt_held, win_held);
        }
        else if(keyboard_->IsKeyHeld(i))
        {
            event_handler_->OnKeyHeld(i, shift_held, ctrl_held, alt_held, win_held);
        }
        else if(keyboard_->IsKeyReleased(i))
        {
            event_handler_->OnKeyReleased(i, shift_held, ctrl_held, alt_held, win_held);
        }
    }

    // Mouse.
    for(i = 0; i < Mouse::NUM_BUTTONS; i++)
    {
        if(mouse_->IsButtonPressed(i))
        {
            event_handler_->OnMouseButtonPressed(i);
        }
        else if(mouse_->IsButtonHeld(i))
        {
            event_handler_->OnMouseButtonHeld(i);
        }
        else if(mouse_->IsButtonReleased(i))
        {
            event_handler_->OnMouseButtonReleased(i);
        }
    }

    x = y = 0;
    mouse_->GetMovement(&x, &y);
    if(x || y)
    {
        event_handler_->OnMouseMoved(x, y);
    }

    // Joystick.
    for(i = 0; i < Joystick::NUM_BUTTONS; i++)
    {
        if(joystick_->IsButtonPressed(i))
        {
            event_handler_->OnJoyButtonPressed(i);
        }
        else if(joystick_->IsButtonHeld(i))
        {
            event_handler_->OnJoyButtonHeld(i);
        }
        else if(joystick_->IsButtonReleased(i))
        {
            event_handler_->OnJoyButtonReleased(i);
        }
    }

    for(i = 0; i < Joystick::NUM_POVS; i++)
    {
        for(x = (int)Joystick::DIR_LEFT; x <= (int)Joystick::DIR_DOWN; x++)
        {
            if(joystick_->IsPovButtonPressed((Joystick::Direction)x, i))
            {
                event_handler_->OnJoyPovButtonPressed((InputEventHandler::JoyPovDirection)x, i);
            }
            else if(joystick_->IsPovButtonHeld((Joystick::Direction)x, i))
            {
                event_handler_->OnJoyPovButtonHeld((InputEventHandler::JoyPovDirection)x, i);
            }
            else if(joystick_->IsPovButtonReleased((Joystick::Direction)x, i))
            {
                event_handler_->OnJoyPovButtonReleased((InputEventHandler::JoyPovDirection)x, i);
            }
        }
    }

    x = y = z = 0;
    joystick_->GetMovement(&x, &y, &z);
    if(x || y || z)
    {
        event_handler_->OnJoyMoved(x, y, z);
    }

    x = y = z = 0;
    joystick_->GetRotation(&x, &y, &z);
    if(x || y || z)
    {
        event_handler_->OnJoyRotated(x, y, z);
    }

    for(i = 0; i < Joystick::NUM_SLIDERS; i++)
    {
        x = joystick_->GetSliderPos(i);
        if(x != slider_pos[i])
        {
            event_handler_->OnJoySliderMoved(i, x);
            slider_pos[i] = x;
        }
    }
}


BOOL PASCAL Device::InputCallback(const DIDEVICEINSTANCE* device, void* param)
{
    if(device && param)
    {
        std::vector<Device::JoystickInfo>* joy_info = reinterpret_cast<std::vector<Device::JoystickInfo>*>(param);

        Device::JoystickInfo info;
        memcpy(&info.guid_, &device->guidInstance, sizeof(GUID));
        info.name_ = device->tszProductName;

        joy_info->push_back(info);
    }

    return DIENUM_CONTINUE;
}

char Input::ToUpperCaseCharacter(int dik)
{
    switch(dik)
    {
    case DIK_1: return '!';
    case DIK_2: return '@';
    case DIK_3: return '#';
    case DIK_4: return '$';
    case DIK_5: return '%';
    case DIK_6: return '^';
    case DIK_7: return '&';
    case DIK_8: return '*';
    case DIK_9: return '(';
    case DIK_0: return ')';
    case DIK_MINUS: return '_';
    case DIK_EQUALS: return '+';
    case DIK_Q: return 'Q';
    case DIK_W: return 'W';
    case DIK_E: return 'E';
    case DIK_R: return 'R';
    case DIK_T: return 'T';
    case DIK_Y: return 'Y';
    case DIK_U: return 'U';
    case DIK_I: return 'I';
    case DIK_O: return 'O';
    case DIK_P: return 'P';
    case DIK_LBRACKET: return '{';
    case DIK_RBRACKET: return '}';
    case DIK_A: return 'A';
    case DIK_S: return 'S';
    case DIK_D: return 'D';
    case DIK_F: return 'F';
    case DIK_G: return 'G';
    case DIK_H: return 'H';
    case DIK_J: return 'J';
    case DIK_K: return 'K';
    case DIK_L: return 'L';
    case DIK_SEMICOLON: return ':';
    case DIK_APOSTROPHE: return '"';
    case DIK_BACKSLASH: return '|';
    case DIK_Z: return 'Z';
    case DIK_X: return 'X';
    case DIK_C: return 'C';
    case DIK_V: return 'V';
    case DIK_B: return 'B';
    case DIK_N: return 'N';
    case DIK_M: return 'M';
    case DIK_COMMA: return '<';
    case DIK_PERIOD: return '>';
    case DIK_MULTIPLY: return '*';
    case DIK_SPACE: return ' ';
    case DIK_NUMPAD7: return '7';
    case DIK_NUMPAD8: return '8';
    case DIK_NUMPAD9: return '9';
    case DIK_SUBTRACT: return '-';
    case DIK_NUMPAD4: return '4';
    case DIK_NUMPAD5: return '5';
    case DIK_NUMPAD6: return '6';
    case DIK_ADD: return '+';
    case DIK_NUMPAD1: return '1';
    case DIK_NUMPAD2: return '2';
    case DIK_NUMPAD3: return '3';
    case DIK_NUMPAD0: return '0';
    case DIK_DECIMAL: return '.';
    case DIK_COLON: return ':';
    case DIK_UNDERLINE: return '_';
    }
    return 0;
}

char Input::ToLowerCaseCharacter(int dik)
{
    switch(dik)
    {
    case DIK_1: return '1';
    case DIK_2: return '2';
    case DIK_3: return '3';
    case DIK_4: return '4';
    case DIK_5: return '5';
    case DIK_6: return '6';
    case DIK_7: return '7';
    case DIK_8: return '8';
    case DIK_9: return '9';
    case DIK_0: return '0';
    case DIK_MINUS: return '-';
    case DIK_EQUALS: return '=';
    case DIK_Q: return 'q';
    case DIK_W: return 'w';
    case DIK_E: return 'e';
    case DIK_R: return 'r';
    case DIK_T: return 't';
    case DIK_Y: return 'y';
    case DIK_U: return 'u';
    case DIK_I: return 'i';
    case DIK_O: return 'o';
    case DIK_P: return 'p';
    case DIK_LBRACKET: return '[';
    case DIK_RBRACKET: return ']';
    case DIK_A: return 'a';
    case DIK_S: return 's';
    case DIK_D: return 'd';
    case DIK_F: return 'f';
    case DIK_G: return 'g';
    case DIK_H: return 'h';
    case DIK_J: return 'j';
    case DIK_K: return 'k';
    case DIK_L: return 'l';
    case DIK_SEMICOLON: return ';';
    case DIK_APOSTROPHE: return '\'';
    case DIK_BACKSLASH: return '\\';
    case DIK_Z: return 'z';
    case DIK_X: return 'x';
    case DIK_C: return 'c';
    case DIK_V: return 'v';
    case DIK_B: return 'b';
    case DIK_N: return 'n';
    case DIK_M: return 'm';
    case DIK_COMMA: return ',';
    case DIK_PERIOD: return '.';
    case DIK_MULTIPLY: return '*';
    case DIK_SPACE: return ' ';
    case DIK_NUMPAD7: return '7';
    case DIK_NUMPAD8: return '8';
    case DIK_NUMPAD9: return '9';
    case DIK_SUBTRACT: return '-';
    case DIK_NUMPAD4: return '4';
    case DIK_NUMPAD5: return '5';
    case DIK_NUMPAD6: return '6';
    case DIK_ADD: return '+';
    case DIK_NUMPAD1: return '1';
    case DIK_NUMPAD2: return '2';
    case DIK_NUMPAD3: return '3';
    case DIK_NUMPAD0: return '0';
    case DIK_DECIMAL: return '.';
    case DIK_COLON: return ':';
    case DIK_UNDERLINE: return '_';
    }
    return 0;
}

int Input::ToDInputKey(char c)
{
    switch(c)
    {
    case '!': return DIK_1;
    case '@': return DIK_2;
    case '#': return DIK_3;
    case '$': return DIK_4;
    case '%': return DIK_5;
    case '^': return DIK_6;
    case '&': return DIK_7;
    case '*': return DIK_8;
    case '(': return DIK_9;
    case ')': return DIK_0;
    case '_': return DIK_MINUS;
    case '+': return DIK_EQUALS;
    case 'Q': return DIK_Q;
    case 'W': return DIK_W;
    case 'E': return DIK_E;
    case 'R': return DIK_R;
    case 'T': return DIK_T;
    case 'Y': return DIK_Y;
    case 'U': return DIK_U;
    case 'I': return DIK_I;
    case 'O': return DIK_O;
    case 'P': return DIK_P;
    case '{': return DIK_LBRACKET;
    case '}': return DIK_RBRACKET;
    case 'A': return DIK_A;
    case 'S': return DIK_S;
    case 'D': return DIK_D;
    case 'F': return DIK_F;
    case 'G': return DIK_G;
    case 'H': return DIK_H;
    case 'J': return DIK_J;
    case 'K': return DIK_K;
    case 'L': return DIK_L;
    case ':': return DIK_SEMICOLON;
    case '"': return DIK_APOSTROPHE;
    case '|': return DIK_BACKSLASH;
    case 'Z': return DIK_Z;
    case 'X': return DIK_X;
    case 'C': return DIK_C;
    case 'V': return DIK_V;
    case 'B': return DIK_B;
    case 'N': return DIK_N;
    case 'M': return DIK_M;
    case '<': return DIK_COMMA;
    case '>': return DIK_PERIOD;
    case ' ': return DIK_SPACE;
    case '7': return DIK_7;
    case '8': return DIK_8;
    case '9': return DIK_9;
    case '-': return DIK_MINUS;
    case '4': return DIK_4;
    case '5': return DIK_5;
    case '6': return DIK_6;
    case '1': return DIK_1;
    case '2': return DIK_2;
    case '3': return DIK_3;
    case '0': return DIK_0;
    case '.': return DIK_PERIOD;
    case '=': return DIK_EQUALS;
    case 'q': return DIK_Q;
    case 'w': return DIK_W;
    case 'e': return DIK_E;
    case 'r': return DIK_R;
    case 't': return DIK_T;
    case 'y': return DIK_Y;
    case 'u': return DIK_U;
    case 'i': return DIK_I;
    case 'o': return DIK_O;
    case 'p': return DIK_P;
    case '[': return DIK_LBRACKET;
    case ']': return DIK_RBRACKET;
    case 'a': return DIK_A;
    case 's': return DIK_S;
    case 'd': return DIK_D;
    case 'f': return DIK_F;
    case 'g': return DIK_G;
    case 'h': return DIK_H;
    case 'j': return DIK_J;
    case 'k': return DIK_K;
    case 'l': return DIK_L;
    case ';': return DIK_SEMICOLON;
    case '\'': return DIK_APOSTROPHE;
    case '\\': return DIK_BACKSLASH;
    case 'z': return DIK_Z;
    case 'x': return DIK_X;
    case 'c': return DIK_C;
    case 'v': return DIK_V;
    case 'b': return DIK_B;
    case 'n': return DIK_N;
    case 'm': return DIK_M;
    case ',': return DIK_COMMA;
    }
    return 0;
}
