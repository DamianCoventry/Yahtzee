#ifndef INCLUDED_INPUT_JOYSTICK
#define INCLUDED_INPUT_JOYSTICK

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <boost/shared_ptr.hpp>
#include "../Utility/logcontext.h"

namespace Input
{

class Joystick :
    public Utility::LogContext

{
public:
    Joystick();
    ~Joystick();

    void Initialise(IDirectInput8* dinput, HWND window, const GUID& guid);
    void Shutdown();

    void Activate();
    void Deactivate();

    void OnBeginFrame();
    void OnEndFrame();

    bool IsButtonPressed(int button);
    bool IsButtonReleased(int button);
    bool IsButtonHeld(int button);

    enum Direction { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };
    bool IsPovButtonPressed(Direction dir, int pov = 0);
    bool IsPovButtonReleased(Direction dir, int pov = 0);
    bool IsPovButtonHeld(Direction dir, int pov = 0);

    void GetMovement(int* x_delta, int* y_delta, int* z_delta);
    void GetRotation(int* x_delta, int* y_delta, int* z_delta);

    int GetSliderPos(int slider);

    enum CONSTANTS
    { RANGE = 1000, NUM_BUTTONS = 32, NUM_SLIDERS = 2, NUM_POVS = 4 };

private:
    static BOOL PASCAL JoystickCallback(const DIDEVICEOBJECTINSTANCE* device, void* param);

private:
    IDirectInput8* dinput_;
    IDirectInputDevice8* device_;
    DIJOYSTATE state_;
    DIJOYSTATE old_state_;
    GUID guid_;
};

typedef boost::shared_ptr<Joystick> JoystickPtr;

}       // namespace Input

#endif  // INCLUDED_INPUT_JOYSTICK
