#ifndef INCLUDED_INPUT_MOUSE
#define INCLUDED_INPUT_MOUSE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <boost/shared_ptr.hpp>
#include "../Utility/logcontext.h"

namespace Input
{

class Mouse :
    public Utility::LogContext

{
public:
    Mouse();
    ~Mouse();

    void Initialise(IDirectInput8* dinput, HWND window);
    void Shutdown();

    void Activate();
    void Deactivate();

    void OnBeginFrame();
    void OnEndFrame();

    bool IsButtonPressed(int button);
    bool IsButtonReleased(int button);
    bool IsButtonHeld(int button);

    void GetMovement(int* x_delta, int* y_delta);

    enum Constants
    { NUM_BUTTONS = 3 };

private:
    IDirectInput8*          dinput_;
    IDirectInputDevice8*    device_;
    DIMOUSESTATE            state_;
    DIMOUSESTATE            old_state_;
};

typedef boost::shared_ptr<Mouse> MousePtr;

}       // namespace Input

#endif  // INCLUDED_INPUT_MOUSE
