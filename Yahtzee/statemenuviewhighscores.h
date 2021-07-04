#ifndef INCLUDED_STATEMENUVIEWHIGHSCORES
#define INCLUDED_STATEMENUVIEWHIGHSCORES

#include "../Core/state.h"
#include "../Utility/logcontext.h"

class StateMenuViewHighscores :
    public Core::State,
    public Utility::LogContext
{
public:
    StateMenuViewHighscores(Core::Engine* engine);
    ~StateMenuViewHighscores();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);
    void OnSpriteClicked(Graphics::SpritePtr sprite);
private:
    void InsertHighscores();
};

#endif
