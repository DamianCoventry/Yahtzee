#ifndef INCLUDED_STATEMENUGAMEOVER
#define INCLUDED_STATEMENUGAMEOVER

#include "../Core/state.h"
#include "../Utility/logcontext.h"

class StateMenuGameOver :
    public Core::State,
    public Utility::LogContext
{
public:
    StateMenuGameOver(Core::Engine* engine);
    ~StateMenuGameOver();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);
    void OnSpriteClicked(Graphics::SpritePtr sprite);
private:
};

#endif
