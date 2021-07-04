#ifndef INCLUDED_STATEGAMEENDTURN
#define INCLUDED_STATEGAMEENDTURN

#include "../Core/state.h"
#include "../Utility/logcontext.h"
#include "yahtzeenetworkhandler.h"

class StateGameEndTurn
    : public Core::State
    , public YahtzeeNetworkHandler
{
public:
    StateGameEndTurn(Core::Engine* engine);
    ~StateGameEndTurn();
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
    void OnNetworkEventEndTurn(Network::SocketPtr sock, const YahtzeePacketEndTurn& yp);
    void OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp);
    void OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp);
    void OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp);
    void OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp);
    void OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp);
};

#endif
