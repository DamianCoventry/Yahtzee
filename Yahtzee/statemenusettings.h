#ifndef INCLUDED_STATEMENUSETTINGS
#define INCLUDED_STATEMENUSETTINGS

#include "../Core/state.h"
#include "../Utility/logcontext.h"

class StateMenuSettings :
    public Core::State,
    public Utility::LogContext
{
public:
    StateMenuSettings(Core::Engine* engine);
    ~StateMenuSettings();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);
    void OnSpriteClicked(Graphics::SpritePtr sprite);
    void OnSpriteSliderMoved(Graphics::SpritePtr sprite);
    void OnSpriteSliderReleased(Graphics::SpritePtr sprite);
};

#endif
