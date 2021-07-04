#ifndef INCLUDED_STATEGAMEROLLING
#define INCLUDED_STATEGAMEROLLING

#include "../Core/state.h"
#include "../Utility/logcontext.h"
#include "yahtzeenetworkhandler.h"

class StateGameRolling
    : public Core::State
    , public YahtzeeNetworkHandler
{
public:
    StateGameRolling(Core::Engine* engine, int* dice_values = NULL, bool yahtzee = false);
    ~StateGameRolling();
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
    void OnNetworkEventThrowDone(Network::SocketPtr sock, const YahtzeePacketThrowDone& yp);
    void OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp);
    void OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp);
    void OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp);
    void OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp);
    void OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp);

private:
    void UpdateTimeout(float time_delta, int dice, Graphics::SpritePtr sprite);
    void ShowHolds(Graphics::DevicePtr device);
    float GenerateTimeIncrement() const;

    void SetFinalDiceImages();
    void SetFinalDiceImage(int dice, Graphics::SpritePtr sprite);

private:
    bool net_controlled_;
    int dice_values_[5];
    bool yahtzee_;

    struct DiceAnimation
    {
        DiceAnimation() : counter_(0.0f), threshold_(0.0f), done_(false) {}
        float counter_;
        float threshold_;
        enum State { DAS_FROM, DAS_SPIN, DAS_TO };
        State state_;
        bool done_;
    };
    DiceAnimation dice_animations_[5];
};

#endif
