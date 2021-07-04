#ifndef INCLUDED_STATEMENUENTERHIGHSCORE
#define INCLUDED_STATEMENUENTERHIGHSCORE

#include "../Core/state.h"
#include "../Utility/logcontext.h"

class Player;
typedef boost::shared_ptr<Player> PlayerPtr;

class StateMenuEnterHighscore :
    public Core::State,
    public Utility::LogContext
{
public:
    StateMenuEnterHighscore(Core::Engine* engine);
    ~StateMenuEnterHighscore();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);
    void OnSpriteClicked(Graphics::SpritePtr sprite);
    void OnSpriteEnterPressed(Graphics::SpritePtr sprite);

private:
    void InsertHighscores();

private:
    PlayerPtr current_player_;
    int current_player_row_;
};

#endif
