#ifndef INCLUDED_STATEGAMEWAITINGFORSCOREORROLL
#define INCLUDED_STATEGAMEWAITINGFORSCOREORROLL

#include "../Core/state.h"
#include "../Utility/logcontext.h"
#include "yahtzeenetworkhandler.h"

class StateGameWaitingForScoreOrRoll
    : public Core::State
    , public YahtzeeNetworkHandler
{
public:
    StateGameWaitingForScoreOrRoll(Core::Engine* engine);
    ~StateGameWaitingForScoreOrRoll();
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
    void OnNetworkEventHoldDice(Network::SocketPtr sock, const YahtzeePacketHoldDice& yp);
    void OnNetworkEventScoreTurn(Network::SocketPtr sock, const YahtzeePacketScoreTurn& yp);
    void OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp);
    void OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp);
    void OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp);
    void OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp);
    void OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp);

private:
    void HandlePostAwardPoints();
    void ShowHolds(Graphics::DevicePtr device);
    void UpdateScores();
};

#endif
