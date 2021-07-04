#ifndef INCLUDED_STATEGAMEWAITINGFORROLL
#define INCLUDED_STATEGAMEWAITINGFORROLL

#include "../Core/state.h"
#include "../Utility/logcontext.h"
#include "yahtzeenetworkhandler.h"

class StateGameWaitingForRoll
    : public Core::State
    , public YahtzeeNetworkHandler
{
public:
    StateGameWaitingForRoll(Core::Engine* engine);
    ~StateGameWaitingForRoll();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);

    void OnSpriteClicked(Graphics::SpritePtr sprite);
    void OnSpriteEnterPressed(Graphics::SpritePtr sprite);
    void OnWindowActivated();
    void OnWindowDeactivated();

    // YahtzeeNetworkHandler
    void OnNetworkEventThrowDice(Network::SocketPtr sock, const YahtzeePacketThrowDice& yp);
    void OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp);
    void OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp);
    void OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp);
    void OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp);
    void OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp);

private:
    void InsertCurrentPlayersScorecard();
};

#endif
