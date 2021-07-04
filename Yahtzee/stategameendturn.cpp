#include "stdafx.h"
#include "stategameendturn.h"
#include "../Graphics/vector.h"
#include "../Core/engine.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritelistbox.h"
#include "../Graphics/spritelistctrl.h"
#include "yahtzee.h"
#include "stategamewaitingforroll.h"
#include "statemenuenterhighscore.h"
#include "statemenugameover.h"
#include "statemenumain.h"
#include "statemenuingamesettings.h"
#include "statemenunetgameended.h"
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "yahtzeepacket.h"

StateGameEndTurn::StateGameEndTurn(Core::Engine* engine)
: YahtzeeNetworkHandler("StateGameEndTurn", engine)
, State(engine)
{
}

StateGameEndTurn::~StateGameEndTurn()
{
    LOG("Ended.");
}

void StateGameEndTurn::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerPtr current_player = yahtzee->GetCurrentPlayer();

    std::ostringstream oss;
    if(current_player->IsRemote())
    {
        oss << "Waiting for " << current_player->GetName() << " to click Continue";
    }
    else
    {
        oss << current_player->GetName() << ", click Continue to proceed";
    }
    Graphics::SpriteListBox* listbox = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxMessages().get());
    listbox->AddWrappedString(oss.str());
    listbox->ScrollToBottom();
}

void StateGameEndTurn::OnEnd()
{
}

void StateGameEndTurn::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerPtr current_player = yahtzee->GetCurrentPlayer();

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    if(!current_player->IsRemote())
    {
        clickable->Append(yahtzee->GetButtonContinue2());
    }
    clickable->Append(yahtzee->GetButtonInGameSettings());
    clickable->Append(yahtzee->GetButtonEndGame());
    clickable->Append(yahtzee->GetEditBoxMessage());
    clickable->Append(yahtzee->GetButtonSend());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetButtonContinue2());
    drawable->Append(yahtzee->GetButtonInGameSettings());
    drawable->Append(yahtzee->GetButtonEndGame());
    drawable->Append(yahtzee->GetStaticDice0());
    drawable->Append(yahtzee->GetStaticDice1());
    drawable->Append(yahtzee->GetStaticDice2());
    drawable->Append(yahtzee->GetStaticDice3());
    drawable->Append(yahtzee->GetStaticDice4());
    drawable->Append(yahtzee->GetEditBoxOnes());
    drawable->Append(yahtzee->GetEditBoxTwos());
    drawable->Append(yahtzee->GetEditBoxThrees());
    drawable->Append(yahtzee->GetEditBoxFours());
    drawable->Append(yahtzee->GetEditBoxFives());
    drawable->Append(yahtzee->GetEditBoxSixes());
    drawable->Append(yahtzee->GetEditBoxSubTotal());
    drawable->Append(yahtzee->GetEditBoxBonus());
    drawable->Append(yahtzee->GetEditBoxUpperTotal());
    drawable->Append(yahtzee->GetEditBoxThreeOfAKind());
    drawable->Append(yahtzee->GetEditBoxFourOfAKind());
    drawable->Append(yahtzee->GetEditBoxFullHouse());
    drawable->Append(yahtzee->GetEditBoxSmallStraight());
    drawable->Append(yahtzee->GetEditBoxLargeStraight());
    drawable->Append(yahtzee->GetEditBoxYahtzee());
    drawable->Append(yahtzee->GetEditBoxChance());
    drawable->Append(yahtzee->GetEditBoxLowerTotal());
    drawable->Append(yahtzee->GetEditBoxGrandTotal());
    drawable->Append(yahtzee->GetListCtrlInGamePlayers());
    drawable->Append(yahtzee->GetListBoxMessages());
    drawable->Append(yahtzee->GetEditBoxMessage());
    drawable->Append(yahtzee->GetButtonSend());

    if (yahtzee->IsPlayerTyping())
        clickable->SetFocus(yahtzee->GetEditBoxMessage());
    else
        clickable->MoveFocusFirst();
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateGameEndTurn::DoFrameLogic(float time_delta)
{
}

void StateGameEndTurn::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageGameBoard()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateGameEndTurn::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerPtr current_player = yahtzee->GetCurrentPlayer();

    if(sprite == yahtzee->GetButtonContinue2())
    {
        if(yahtzee->IsGameOver())
        {
            if(yahtzee->GetFirstHighscoringPlayer())
            {
                TransitionGameState(Core::StatePtr());
                TransitionMenuState(Core::StatePtr(new StateMenuEnterHighscore(GetEngine())));
            }
            else
            {
                TransitionGameState(Core::StatePtr());
                TransitionMenuState(Core::StatePtr(new StateMenuGameOver(GetEngine())));
            }
        }
        else
        {
            yahtzee->StartNewTurn();
            TransitionGameState(Core::StatePtr(new StateGameWaitingForRoll(GetEngine())));
        }
        BroadcastNetworkEventEndTurn();
    }
    else if(sprite == yahtzee->GetButtonInGameSettings())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuInGameSettings(GetEngine())));
    }
    else if(sprite == yahtzee->GetButtonEndGame())
    {
        if(yahtzee->IsNetworkGame())
        {
            yahtzee->EndNetworkGame(this);
        }
        else
        {
            TransitionGameState(Core::StatePtr());
            TransitionMenuState(Core::StatePtr(new StateMenuMain(GetEngine())));
        }
    }
    else if(sprite == yahtzee->GetButtonSend())
    {
        Graphics::SpriteEditBox* editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxMessage().get());
        if(!editbox->GetText().empty())
        {
            std::string message;
            Graphics::SpriteListBox* listbox = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxMessages().get());
            if(current_player->IsRemote())
            {
                message = yahtzee->GetLocalPlayer()->GetName() + ": " + editbox->GetText();
            }
            else
            {
                message = yahtzee->GetCurrentPlayer()->GetName() + ": " + editbox->GetText();
            }
            listbox->AddWrappedString(message, Graphics::Color(0.9f), Graphics::Color(0.368f, 0.368f, 0.368f));
            BroadcastNetworkEventMessage(message);

            listbox->ScrollToBottom();
            editbox->SetText(std::string());
        }
    }
}

void StateGameEndTurn::OnSpriteEnterPressed(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    OnSpriteClicked(yahtzee->GetButtonSend());
}

void StateGameEndTurn::OnNetworkEventEndTurn(Network::SocketPtr sock, const YahtzeePacketEndTurn& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(yahtzee->IsGameOver())
    {
        if(yahtzee->GetFirstHighscoringPlayer())
        {
            TransitionGameState(Core::StatePtr());
            TransitionMenuState(Core::StatePtr(new StateMenuEnterHighscore(GetEngine())));
        }
        else
        {
            TransitionGameState(Core::StatePtr());
            TransitionMenuState(Core::StatePtr(new StateMenuGameOver(GetEngine())));
        }
    }
    else
    {
        yahtzee->StartNewTurn();
        TransitionGameState(Core::StatePtr(new StateGameWaitingForRoll(GetEngine())));
    }
}

void StateGameEndTurn::OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->AddMessage(yp.GetMessage());
}

void StateGameEndTurn::OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetConnectionList()->clear();
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), yp.GetWhichPlayer(), yp.GetReason())));
}

void StateGameEndTurn::OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetPlayerLocation(yp.GetWhichPlayer(), yp.GetActivated(), "Away");
}

void StateGameEndTurn::OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetPlayerLocation(yp.GetWhichPlayer(), !yp.GetInSettings(), "Settings");
}

void StateGameEndTurn::OnWindowActivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), true);
}

void StateGameEndTurn::OnWindowDeactivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), false);
}

void StateGameEndTurn::OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetConnectionList()->clear();
    TransitionGameState(Core::StatePtr());
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), yp.GetWhichPlayer(), "This player left the game")));
}
