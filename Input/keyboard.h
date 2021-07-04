#ifndef INCLUDED_INPUT_KEYBOARD
#define INCLUDED_INPUT_KEYBOARD

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <boost/shared_ptr.hpp>
#include "../Utility/logcontext.h"

namespace Input
{

class Keyboard :
    public Utility::LogContext

{
public:
    Keyboard();
    ~Keyboard();

    void Initialise(IDirectInput8* dinput, HWND window);
    void Shutdown();

    void Activate();
    void Deactivate();

    void OnBeginFrame();
    void OnEndFrame();

    bool IsKeyPressed(int key);
    bool IsKeyReleased(int key);
    bool IsKeyHeld(int key);

    enum Constants
    { NUM_KEYS = 256 };

private:
    IDirectInput8*          dinput_;
    IDirectInputDevice8*    device_;
    char                    keys_[NUM_KEYS];
    char                    old_keys_[NUM_KEYS];
};

typedef boost::shared_ptr<Keyboard> KeyboardPtr;

}       // namespace Input

#endif  // INCLUDED_INPUT_KEYBOARD
