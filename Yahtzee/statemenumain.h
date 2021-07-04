#ifndef INCLUDED_STATEMENUMAIN
#define INCLUDED_STATEMENUMAIN

#include "../Core/state.h"
#include "../Utility/logcontext.h"

class StateMenuMain :
    public Core::State,
    public Utility::LogContext
{
public:
    StateMenuMain(Core::Engine* engine);
    ~StateMenuMain();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);
    void OnSpriteClicked(Graphics::SpritePtr sprite);
private:
    void ResetEditBoxes();
};

#endif
