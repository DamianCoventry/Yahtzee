#ifndef INCLUDED_INPUT_DEVICE
#define INCLUDED_INPUT_DEVICE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include "joystick.h"
#include "../Utility/logcontext.h"

namespace Input
{

char ToUpperCaseCharacter(int dik);
char ToLowerCaseCharacter(int dik);
int ToDInputKey(char c);

class Keyboard;
typedef boost::shared_ptr<Keyboard> KeyboardPtr;

class Mouse;
typedef boost::shared_ptr<Mouse> MousePtr;

struct InputEventHandler
{
    virtual void OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held) {}
    virtual void OnKeyHeld(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held) {}
    virtual void OnKeyReleased(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held) {}

    virtual void OnMouseButtonPressed(int button) {}
    virtual void OnMouseButtonHeld(int button) {}
    virtual void OnMouseButtonReleased(int button) {}
    virtual void OnMouseMoved(int x_delta, int y_delta) {}

    virtual void OnJoyButtonPressed(int button) {}
    virtual void OnJoyButtonHeld(int button) {}
    virtual void OnJoyButtonReleased(int button) {}
    enum JoyPovDirection { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };
    virtual void OnJoyPovButtonPressed(JoyPovDirection dir, int pov) {}
    virtual void OnJoyPovButtonHeld(JoyPovDirection dir, int pov) {}
    virtual void OnJoyPovButtonReleased(JoyPovDirection dir, int pov) {}
    virtual void OnJoyMoved(int x_delta, int y_delta, int z_delta) {}
    virtual void OnJoyRotated(int x_delta, int y_delta, int z_delta) {}
    virtual void OnJoySliderMoved(int slider, int pos) {}
};

class Device :
    public Utility::LogContext
{
public:
    Device(InputEventHandler* event_handler);
    ~Device();

    void Initialise(HINSTANCE instance, HWND window);
    void Shutdown();

    struct JoystickInfo
    {
        std::string name_;
        GUID guid_;
    };
    std::vector<JoystickInfo> EnumerateJoysticks();
    void ChooseJoystick(HWND window, const GUID& guid);

    void OnBeginFrame();
    void OnEndFrame();
    void Activate();
    void Deactivate();
    void Poll();

    KeyboardPtr GetKeyboard() const     { return keyboard_; }
    MousePtr GetMouse() const           { return mouse_; }
    JoystickPtr GetJoystick() const     { return joystick_; }

private:
    static BOOL PASCAL InputCallback(const DIDEVICEINSTANCE* device, void* param);

private:
    InputEventHandler* event_handler_;
    IDirectInput8* dinput_;
    KeyboardPtr keyboard_;
    MousePtr mouse_;
    JoystickPtr joystick_;
    int slider_pos[Joystick::NUM_SLIDERS];
};

typedef boost::shared_ptr<Device> DevicePtr;

}       // namespace Input

#endif  // INCLUDED_INPUT_DEVICE
