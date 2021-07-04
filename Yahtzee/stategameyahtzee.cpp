#include "stdafx.h"
#include "stategameyahtzee.h"
#include "../Graphics/vector.h"
#include "../Graphics/spritelistbox.h"
#include "../Core/engine.h"
#include "../Core/timeoutmanager.h"
#include "yahtzee.h"
#include "statemenumain.h"
#include "stategamewaitingforscore.h"
#include "stategamewaitingforscoreorroll.h"
#include "statemenunetgameended.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Audio/sound.h"
#include "yahtzeepacket.h"

StateGameYahtzee::StateGameYahtzee(Core::Engine* engine)
: YahtzeeNetworkHandler("StateGameYahtzee", engine)
, State(engine)
{
}

StateGameYahtzee::~StateGameYahtzee()
{
    LOG("Ended.");
}

void StateGameYahtzee::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerPtr current_player = yahtzee->GetCurrentPlayer();

    if(!current_player->IsRemote())
    {
        Core::TimeoutManagerPtr timeout_manager = yahtzee->GetTimeoutManager();
        end_stategame_timeout_id_ = timeout_manager->AddTimeout(this, 2000);
    }

    yahtzee->GetYahtzeeSound()->Play();
}

void StateGameYahtzee::OnEnd()
{
}

void StateGameYahtzee::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
}

void StateGameYahtzee::DoFrameLogic(float time_delta)
{
}

void StateGameYahtzee::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageYahtzee()->Draw(time_delta, gfx, Graphics::Vector());
}

void StateGameYahtzee::OnTimeout(int timeout_id)
{
    if(timeout_id == end_stategame_timeout_id_)
    {
        DoTransition();
    }
}

void StateGameYahtzee::DoTransition()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    // Award a bonus yahtzee if applicable.
    PlayerPtr player = yahtzee->GetCurrentPlayer();
    bool bonus_yahtzee_awarded = player->GetScorecard().AwardBonusYahtzee(yahtzee->GetCurrentRoll());
    yahtzee->SetBonusYahtzee(bonus_yahtzee_awarded);

    BroadcastNetworkEventYahtzeeDone();

    if(yahtzee->IsFinalRoll())
    {
        TransitionGameState(Core::StatePtr(new StateGameWaitingForScore(GetEngine())));
    }
    else
    {
        TransitionGameState(Core::StatePtr(new StateGameWaitingForScoreOrRoll(GetEngine())));
    }
}

void StateGameYahtzee::OnNetworkEventYahtzeeDone(Network::SocketPtr sock, const YahtzeePacketYahtzeeDone& yp)
{
    DoTransition();
}

void StateGameYahtzee::OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->AddMessage(yp.GetMessage());
}

void StateGameYahtzee::OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetConnectionList()->clear();
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), yp.GetWhichPlayer(), yp.GetReason())));
}

void StateGameYahtzee::OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetPlayerLocation(yp.GetWhichPlayer(), yp.GetActivated(), "Away");
}

void StateGameYahtzee::OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetPlayerLocation(yp.GetWhichPlayer(), !yp.GetInSettings(), "Settings");
}

void StateGameYahtzee::OnWindowActivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), true);
}

void StateGameYahtzee::OnWindowDeactivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), false);
}

void StateGameYahtzee::OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetConnectionList()->clear();
    TransitionGameState(Core::StatePtr());
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), yp.GetWhichPlayer(), "This player left the game")));
}
