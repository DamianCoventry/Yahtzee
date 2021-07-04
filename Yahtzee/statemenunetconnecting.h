#ifndef INCLUDED_STATEMENUNETCONNECTING
#define INCLUDED_STATEMENUNETCONNECTING

#include "../Core/state.h"
#include "../Utility/logcontext.h"
#include "../network/ipaddress.h"
#include "yahtzeenetworkhandler.h"

class StateMenuNetConnecting
    : public Core::State
    , public YahtzeeNetworkHandler
{
public:
    StateMenuNetConnecting(Core::Engine* engine, const std::string& ip_address, const std::string& local_player_name);
    StateMenuNetConnecting(Core::Engine* engine, const std::string& ip_address, const std::string& local_player_name, Network::SocketPtr sock);
    ~StateMenuNetConnecting();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);

    void OnSpriteClicked(Graphics::SpritePtr sprite);
    void OnWindowActivated();
    void OnWindowDeactivated();

    // YahtzeeNetworkHandler
    void OnNetworkEventJoinAccepted(Network::SocketPtr sock, const YahtzeePacketJoinAccepted& yp);
    void OnNetworkEventJoinDeclined(Network::SocketPtr sock, const YahtzeePacketJoinDeclined& yp);

private:
    Network::IpAddress ip_address_;
    std::string local_player_name_;
    unsigned long last_send_time_;
    Network::SocketPtr sock_to_use_;
};

#endif
