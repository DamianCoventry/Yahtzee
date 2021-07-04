#ifndef INCLUDED_STATEMENUABOUT
#define INCLUDED_STATEMENUABOUT

#include "../Core/state.h"
#include "../Utility/logcontext.h"

class StateMenuAbout :
    public Core::State,
    public Utility::LogContext
{
public:
    StateMenuAbout(Core::Engine* engine);
    ~StateMenuAbout();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);
    void OnSpriteClicked(Graphics::SpritePtr sprite);
};

#endif
