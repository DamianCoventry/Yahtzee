#ifndef INCLUDED_STATEMENUNETGAMESETUP
#define INCLUDED_STATEMENUNETGAMESETUP

#include "../Core/state.h"
#include "../Utility/logcontext.h"
#include "yahtzeenetworkhandler.h"
#include <map>

class StateMenuNetGameSetup
    : public Core::State
    , public YahtzeeNetworkHandler
{
public:
    StateMenuNetGameSetup(Core::Engine* engine, const std::string& local_player_name);
    ~StateMenuNetGameSetup();
    void OnBegin();
    void OnEnd();
    void SetupLists();
    void DoFrameLogic(float time_delta);
    void DrawFrame(float time_delta);

    void OnSpriteClicked(Graphics::SpritePtr sprite);
    void OnSpriteEnterPressed(Graphics::SpritePtr sprite);
    void OnSpriteDoubleClicked(Graphics::SpritePtr sprite);

    // YahtzeeNetworkHandler
    void OnNetworkBroadcastGameAvailable(Network::SocketPtr socket, const YahtzeePacketBroadcastGameAvailable& yp);

private:
    void RemoveOldBroadcasts();

private:
    struct BroadcastInfo
    {
        unsigned long last_received_time_;
        Network::SocketPtr sock_;
    };
    typedef std::map<std::string, BroadcastInfo> ReceivedBroadcasts;
    ReceivedBroadcasts received_broadcasts_;
    std::string local_player_name_;
};

#endif
