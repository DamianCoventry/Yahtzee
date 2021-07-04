#ifndef INCLUDED_STATEMENUNETHOSTINGGAME
#define INCLUDED_STATEMENUNETHOSTINGGAME

#include "../Core/state.h"
#include "../Utility/logcontext.h"
#include "yahtzeenetworkhandler.h"

class StateMenuNetHostingGame
    : public Core::State
    , public YahtzeeNetworkHandler
{
public:
    StateMenuNetHostingGame(Core::Engine* engine, const std::string& local_player_name);
    ~StateMenuNetHostingGame();
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
    void OnNetworkEventJoinRequest(Network::SocketPtr sock, const YahtzeePacketJoinRequest& yp);
    void OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp);
    void OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp);
    void OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp);

private:
    unsigned long last_broadcast_time_;
    std::string local_player_name_;
};

#endif
