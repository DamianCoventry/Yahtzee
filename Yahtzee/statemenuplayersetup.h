#ifndef INCLUDED_STATEMENUPLAYERSETUP
#define INCLUDED_STATEMENUPLAYERSETUP

#include "../Core/state.h"
#include "../Utility/logcontext.h"

class StateMenuPlayerSetup :
    public Core::State,
    public Utility::LogContext
{
public:
    StateMenuPlayerSetup(Core::Engine* engine);
    ~StateMenuPlayerSetup();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);

    void OnSpriteClicked(Graphics::SpritePtr sprite);
    void OnSpriteEnterPressed(Graphics::SpritePtr sprite);
    void OnSpriteDoubleClicked(Graphics::SpritePtr sprite);
};

#endif
