#ifndef INCLUDED_STATEMENUNETJOINEDREMOTE
#define INCLUDED_STATEMENUNETJOINEDREMOTE

#include "../Core/state.h"
#include "../Utility/logcontext.h"
#include "yahtzeenetworkhandler.h"

class StateMenuNetJoinedRemote
    : public Core::State
    , public YahtzeeNetworkHandler
{
public:
    StateMenuNetJoinedRemote(Core::Engine* engine, const std::string& local_player_name);
    ~StateMenuNetJoinedRemote();
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
    void OnNetworkEventGameStarting(Network::SocketPtr sock, const YahtzeePacketGameStarting& yp);
    void OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp);
    void OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp);
    void OnNetworkEventPlayerJoined(Network::SocketPtr sock, const YahtzeePacketPlayerJoined& yp);
    void OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp);
    void OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp);

private:
    std::string local_player_name_;
};

#endif
