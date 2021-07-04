#include "stdafx.h"
#include "stategamewaitingforroll.h"
#include "../Graphics/vector.h"
#include "../Core/engine.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritelistctrl.h"
#include "../Graphics/spritelistbox.h"
#include "../graphics/spritebutton.h"
#include "yahtzee.h"
#include "stategamerolling.h"
#include "statemenumain.h"
#include "statemenuingamesettings.h"
#include "statemenunetgameended.h"
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "yahtzeepacket.h"

StateGameWaitingForRoll::StateGameWaitingForRoll(Core::Engine* engine)
: YahtzeeNetworkHandler("StateGameWaitingForRoll", engine)
, State(engine)
{
}

StateGameWaitingForRoll::~StateGameWaitingForRoll()
{
    LOG("Ended.");
}

void StateGameWaitingForRoll::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerPtr current_player = yahtzee->GetCurrentPlayer();

    yahtzee->GetButtonRoll()->Enable(true);
    yahtzee->GetEditBoxSubTotal()->Enable(false);
    yahtzee->GetEditBoxBonus()->Enable(false);
    yahtzee->GetEditBoxUpperTotal()->Enable(false);
    yahtzee->GetEditBoxLowerTotal()->Enable(false);
    yahtzee->GetEditBoxGrandTotal()->Enable(false);

    Graphics::SpriteListCtrl* list_ctrl = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlInGamePlayers().get());
    list_ctrl->SetCurSel(yahtzee->GetCurrentPlayerIndex());

    std::ostringstream oss;
    if(current_player->IsRemote())
    {
        oss << "Waiting for " << current_player->GetName() << " to throw the dice!";
    }
    else
    {
        oss << current_player->GetName() << ", throw the dice!";
    }
    Graphics::SpriteListBox* listbox = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxMessages().get());
    listbox->AddWrappedString(oss.str());
    listbox->ScrollToBottom();

    InsertCurrentPlayersScorecard();
}

void StateGameWaitingForRoll::OnEnd()
{
}

void StateGameWaitingForRoll::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerPtr current_player = yahtzee->GetCurrentPlayer();

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    if(!current_player->IsRemote())
    {
        clickable->Append(yahtzee->GetButtonRoll());
    }
    clickable->Append(yahtzee->GetButtonInGameSettings());
    clickable->Append(yahtzee->GetButtonEndGame());
    clickable->Append(yahtzee->GetEditBoxMessage());
    clickable->Append(yahtzee->GetButtonSend());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetButtonRoll());
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

    Graphics::SpriteButton* button = static_cast<Graphics::SpriteButton*>(yahtzee->GetButtonSend().get());
    button->SetPosition(Graphics::Vector(564.0f, 404.0f));

    if (yahtzee->IsPlayerTyping())
        clickable->SetFocus(yahtzee->GetEditBoxMessage());
    else
        clickable->MoveFocusFirst();
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateGameWaitingForRoll::DoFrameLogic(float time_delta)
{
}

void StateGameWaitingForRoll::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageGameBoard()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateGameWaitingForRoll::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetButtonRoll())
    {
        TransitionGameState(Core::StatePtr(new StateGameRolling(GetEngine())));
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
            PlayerPtr current_player = yahtzee->GetCurrentPlayer();

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

void StateGameWaitingForRoll::OnSpriteEnterPressed(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetEditBoxMessage())
    {
        OnSpriteClicked(yahtzee->GetButtonSend());
        Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
        clickable->SetFocus(yahtzee->GetEditBoxMessage());
    }
}

void StateGameWaitingForRoll::InsertCurrentPlayersScorecard()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Scorecard& scorecard = yahtzee->GetCurrentPlayer()->GetScorecard();

    Graphics::SpriteEditBox* editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxOnes().get());
    editbox->SetText(scorecard.GetOnes());
    editbox->Enable(scorecard.GetOnes().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxTwos().get());
    editbox->SetText(scorecard.GetTwos());
    editbox->Enable(scorecard.GetTwos().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxThrees().get());
    editbox->SetText(scorecard.GetThrees());
    editbox->Enable(scorecard.GetThrees().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFours().get());
    editbox->SetText(scorecard.GetFours());
    editbox->Enable(scorecard.GetFours().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFives().get());
    editbox->SetText(scorecard.GetFives());
    editbox->Enable(scorecard.GetFives().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxSixes().get());
    editbox->SetText(scorecard.GetSixes());
    editbox->Enable(scorecard.GetSixes().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxSubTotal().get());
    editbox->SetText(scorecard.GetSubTotal());

    if(scorecard.IsUpperComplete())
    {
        editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxBonus().get());
        editbox->SetText(scorecard.GetBonus());

        editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxUpperTotal().get());
        editbox->SetText(scorecard.GetUpperTotal());
    }
    else
    {
        editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxBonus().get());
        editbox->SetText(std::string());

        editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxUpperTotal().get());
        editbox->SetText(std::string());
    }

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxThreeOfAKind().get());
    editbox->SetText(scorecard.GetThreeOfAKind());
    editbox->Enable(scorecard.GetThreeOfAKind().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFourOfAKind().get());
    editbox->SetText(scorecard.GetFourOfAKind());
    editbox->Enable(scorecard.GetFourOfAKind().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFullHouse().get());
    editbox->SetText(scorecard.GetFullHouse());
    editbox->Enable(scorecard.GetFullHouse().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxSmallStraight().get());
    editbox->SetText(scorecard.GetSmallStraight());
    editbox->Enable(scorecard.GetSmallStraight().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxLargeStraight().get());
    editbox->SetText(scorecard.GetLargeStraight());
    editbox->Enable(scorecard.GetLargeStraight().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxYahtzee().get());
    editbox->SetText(scorecard.GetYahtzee());
    editbox->Enable(scorecard.GetYahtzee().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxChance().get());
    editbox->SetText(scorecard.GetChance());
    editbox->Enable(scorecard.GetChance().empty());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxLowerTotal().get());
    editbox->SetText(scorecard.GetLowerTotal());

    if(scorecard.IsEverythingComplete())
    {
        editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxGrandTotal().get());
        editbox->SetText(scorecard.GetGrandTotal());
    }
    else
    {
        editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxGrandTotal().get());
        editbox->SetText(std::string());
    }
}

void StateGameWaitingForRoll::OnNetworkEventThrowDice(Network::SocketPtr sock, const YahtzeePacketThrowDice& yp)
{
    int dice_values[5];
    dice_values[0] = yp.GetDiceValue(0);
    dice_values[1] = yp.GetDiceValue(1);
    dice_values[2] = yp.GetDiceValue(2);
    dice_values[3] = yp.GetDiceValue(3);
    dice_values[4] = yp.GetDiceValue(4);
    TransitionGameState(Core::StatePtr(new StateGameRolling(GetEngine(), dice_values)));
}

void StateGameWaitingForRoll::OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->AddMessage(yp.GetMessage());
}

void StateGameWaitingForRoll::OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetConnectionList()->clear();
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), yp.GetWhichPlayer(), yp.GetReason())));
}

void StateGameWaitingForRoll::OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetPlayerLocation(yp.GetWhichPlayer(), yp.GetActivated(), "Away");
}

void StateGameWaitingForRoll::OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetPlayerLocation(yp.GetWhichPlayer(), !yp.GetInSettings(), "Settings");
}

void StateGameWaitingForRoll::OnWindowActivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), true);
}

void StateGameWaitingForRoll::OnWindowDeactivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), false);
}

void StateGameWaitingForRoll::OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetConnectionList()->clear();
    TransitionGameState(Core::StatePtr());
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), yp.GetWhichPlayer(), "This player left the game")));
}
