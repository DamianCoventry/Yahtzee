#ifndef INCLUDED_STATEMENUINGAMESETTINGS
#define INCLUDED_STATEMENUINGAMESETTINGS

#include "../Core/state.h"
#include "yahtzeenetworkhandler.h"

class StateMenuInGameSettings
    : public Core::State
    , public YahtzeeNetworkHandler
{
public:
    StateMenuInGameSettings(Core::Engine* engine);
    ~StateMenuInGameSettings();
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
