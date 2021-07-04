#ifndef INCLUDED_STATEMENUNETGAMEENTERNAME
#define INCLUDED_STATEMENUNETGAMEENTERNAME

#include "../Core/state.h"
#include "../Utility/logcontext.h"

class StateMenuNetGameEnterName :
    public Core::State,
    public Utility::LogContext
{
public:
    StateMenuNetGameEnterName(Core::Engine* engine);
    ~StateMenuNetGameEnterName();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);
    void OnSpriteClicked(Graphics::SpritePtr sprite);
    void OnSpriteEnterPressed(Graphics::SpritePtr sprite);
};

#endif
