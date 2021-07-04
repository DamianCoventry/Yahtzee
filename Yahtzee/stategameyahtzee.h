#ifndef INCLUDED_STATEGAMEYAHTZEE
#define INCLUDED_STATEGAMEYAHTZEE

#include "../Core/state.h"
#include "../Utility/logcontext.h"
#include "yahtzeenetworkhandler.h"

class StateGameYahtzee
    : public Core::State
    , public YahtzeeNetworkHandler
{
public:
    StateGameYahtzee(Core::Engine* engine);
    ~StateGameYahtzee();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);

    void OnTimeout(int timeout_id);
    void OnWindowActivated();
    void OnWindowDeactivated();

    // YahtzeeNetworkHandler
    void OnNetworkEventYahtzeeDone(Network::SocketPtr sock, const YahtzeePacketYahtzeeDone& yp);
    void OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp);
    void OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp);
    void OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp);
    void OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp);
    void OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp);

private:
    void DoTransition();

private:
    int end_stategame_timeout_id_;
};

#endif
