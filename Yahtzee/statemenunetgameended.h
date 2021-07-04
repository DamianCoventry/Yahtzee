#ifndef INCLUDED_STATEMENUNETGAMEENDED
#define INCLUDED_STATEMENUNETGAMEENDED

#include "../Core/state.h"
#include "../Utility/logcontext.h"

class Player;
typedef boost::shared_ptr<Player> PlayerPtr;

class StateMenuNetGameEnded :
    public Core::State,
    public Utility::LogContext
{
public:
    StateMenuNetGameEnded(Core::Engine* engine, int which_player, const std::string& reason);
    ~StateMenuNetGameEnded();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);
    void OnSpriteClicked(Graphics::SpritePtr sprite);
private:
    int which_player_;
    std::string reason_;
};

#endif
