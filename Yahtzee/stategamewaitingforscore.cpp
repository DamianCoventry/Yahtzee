#include "stdafx.h"
#include "stategamewaitingforscore.h"
#include "../Graphics/vector.h"
#include "../Core/engine.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritelistctrl.h"
#include "../Graphics/spritelistbox.h"
#include "yahtzee.h"
#include "stategameendturn.h"
#include "statemenumain.h"
#include "statemenuenterhighscore.h"
#include "statemenugameover.h"
#include "statemenuingamesettings.h"
#include "stategamewaitingforroll.h"
#include "statemenunetgameended.h"
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "yahtzeepacket.h"

StateGameWaitingForScore::StateGameWaitingForScore(Core::Engine* engine)
: YahtzeeNetworkHandler("StateGameWaitingForScore", engine)
, State(engine)
{
}

StateGameWaitingForScore::~StateGameWaitingForScore()
{
    LOG("Ended.");
}

void StateGameWaitingForScore::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerPtr current_player = yahtzee->GetCurrentPlayer();
    Roll& roll = yahtzee->GetCurrentRoll();

    yahtzee->GetButtonRoll()->Enable(false);

    Graphics::SpriteListBox* listbox = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxMessages().get());
    std::ostringstream oss;

    // Special case for bonus yahtzees.
    if(yahtzee->IsBonusYahtzee())
    {
        listbox->AddWrappedString("Bonus Yahtzee!", Graphics::Color(1.0f, 1.0f, 0.0f), Graphics::Color(1.0f, 0.0f, 0.0f));

        bool rule_applies = false;
        int value = yahtzee->GetCurrentRoll().GetValue(0);
        switch(value)
        {
        case 1: rule_applies = current_player->GetScorecard().GetOnes().empty(); break;
        case 2: rule_applies = current_player->GetScorecard().GetTwos().empty(); break;
        case 3: rule_applies = current_player->GetScorecard().GetThrees().empty(); break;
        case 4: rule_applies = current_player->GetScorecard().GetFours().empty(); break;
        case 5: rule_applies = current_player->GetScorecard().GetFives().empty(); break;
        case 6: rule_applies = current_player->GetScorecard().GetSixes().empty(); break;
        }

        if(rule_applies)
        {
            yahtzee->GetEditBoxOnes()->Enable(value == 1);
            yahtzee->GetEditBoxTwos()->Enable(value == 2);
            yahtzee->GetEditBoxThrees()->Enable(value == 3);
            yahtzee->GetEditBoxFours()->Enable(value == 4);
            yahtzee->GetEditBoxFives()->Enable(value == 5);
            yahtzee->GetEditBoxSixes()->Enable(value == 6);
            yahtzee->GetEditBoxThreeOfAKind()->Enable(false);
            yahtzee->GetEditBoxFourOfAKind()->Enable(false);
            yahtzee->GetEditBoxFullHouse()->Enable(false);
            yahtzee->GetEditBoxSmallStraight()->Enable(false);
            yahtzee->GetEditBoxLargeStraight()->Enable(false);
            yahtzee->GetEditBoxYahtzee()->Enable(false);
            yahtzee->GetEditBoxChance()->Enable(false);
            if(current_player->IsRemote())
            {
                oss << "Waiting for " << current_player->GetName() << " to score this roll.";
            }
            else
            {
                oss << current_player->GetName() << ", you must choose " << value << "s for this roll.";
            }
        }
        else
        {
            if(current_player->IsRemote())
            {
                oss << "Waiting for " << current_player->GetName() << " to score this roll.";
            }
            else
            {
                oss << current_player->GetName() << ", you may choose any score box for this roll.";
            }
        }
    }
    else
    {
        if(current_player->IsRemote())
        {
            oss << "Waiting for " << current_player->GetName() << " to score this roll.";
        }
        else
        {
            oss << current_player->GetName() << ", choose a score box for this roll.";
        }
    }

    listbox->AddWrappedString(oss.str());
    listbox->ScrollToBottom();
}

void StateGameWaitingForScore::OnEnd()
{
}

void StateGameWaitingForScore::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerPtr current_player = yahtzee->GetCurrentPlayer();

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    clickable->Append(yahtzee->GetButtonInGameSettings());
    clickable->Append(yahtzee->GetButtonEndGame());
    if(!current_player->IsRemote())
    {
        clickable->Append(yahtzee->GetEditBoxOnes());
        clickable->Append(yahtzee->GetEditBoxTwos());
        clickable->Append(yahtzee->GetEditBoxThrees());
        clickable->Append(yahtzee->GetEditBoxFours());
        clickable->Append(yahtzee->GetEditBoxFives());
        clickable->Append(yahtzee->GetEditBoxSixes());
        clickable->Append(yahtzee->GetEditBoxThreeOfAKind());
        clickable->Append(yahtzee->GetEditBoxFourOfAKind());
        clickable->Append(yahtzee->GetEditBoxFullHouse());
        clickable->Append(yahtzee->GetEditBoxSmallStraight());
        clickable->Append(yahtzee->GetEditBoxLargeStraight());
        clickable->Append(yahtzee->GetEditBoxYahtzee());
        clickable->Append(yahtzee->GetEditBoxChance());
    }
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

    Roll& roll = yahtzee->GetCurrentRoll();
    if(roll.GetHold(0))
    {
        drawable->Append(yahtzee->GetDice0Held());
    }
    if(roll.GetHold(1))
    {
        drawable->Append(yahtzee->GetDice1Held());
    }
    if(roll.GetHold(2))
    {
        drawable->Append(yahtzee->GetDice2Held());
    }
    if(roll.GetHold(3))
    {
        drawable->Append(yahtzee->GetDice3Held());
    }
    if(roll.GetHold(4))
    {
        drawable->Append(yahtzee->GetDice4Held());
    }

    if (yahtzee->IsPlayerTyping())
        clickable->SetFocus(yahtzee->GetEditBoxMessage());
    else
        clickable->MoveFocusFirst();
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateGameWaitingForScore::DoFrameLogic(float time_delta)
{
}

void StateGameWaitingForScore::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageGameBoard()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateGameWaitingForScore::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee*    yahtzee         = static_cast<Yahtzee*>(GetEngine());
    PlayerPtr   current_player  = yahtzee->GetCurrentPlayer();
    Scorecard&  scorecard       = current_player->GetScorecard();
    Roll&       roll            = yahtzee->GetCurrentRoll();
    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    Graphics::SpriteEditBox* editbox;

    if(sprite == yahtzee->GetEditBoxOnes())
    {
        if(scorecard.AwardOnes(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxOnes().get());
            editbox->SetText(scorecard.GetOnes());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetOnes()), YahtzeeNetworkHandler::SC_ONES);
        }
    }
    else if(sprite == yahtzee->GetEditBoxTwos())
    {
        if(scorecard.AwardTwos(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxTwos().get());
            editbox->SetText(scorecard.GetTwos());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetTwos()), YahtzeeNetworkHandler::SC_TWOS);
        }
    }
    else if(sprite == yahtzee->GetEditBoxThrees())
    {
        if(scorecard.AwardThrees(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxThrees().get());
            editbox->SetText(scorecard.GetThrees());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetThrees()), YahtzeeNetworkHandler::SC_THREES);
        }
    }
    else if(sprite == yahtzee->GetEditBoxFours())
    {
        if(scorecard.AwardFours(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFours().get());
            editbox->SetText(scorecard.GetFours());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetFours()), YahtzeeNetworkHandler::SC_FOURS);
        }
    }
    else if(sprite == yahtzee->GetEditBoxFives())
    {
        if(scorecard.AwardFives(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFives().get());
            editbox->SetText(scorecard.GetFives());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetFives()), YahtzeeNetworkHandler::SC_FIVES);
        }
    }
    else if(sprite == yahtzee->GetEditBoxSixes())
    {
        if(scorecard.AwardSixes(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxSixes().get());
            editbox->SetText(scorecard.GetSixes());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetSixes()), YahtzeeNetworkHandler::SC_SIXES);
        }
    }
    else if(sprite == yahtzee->GetEditBoxThreeOfAKind())
    {
        if(scorecard.AwardThreeOfAKind(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxThreeOfAKind().get());
            editbox->SetText(scorecard.GetThreeOfAKind());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetThreeOfAKind()), YahtzeeNetworkHandler::SC_THREE_OF_A_KIND);
        }
    }
    else if(sprite == yahtzee->GetEditBoxFourOfAKind())
    {
        if(scorecard.AwardFourOfAKind(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFourOfAKind().get());
            editbox->SetText(scorecard.GetFourOfAKind());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetFourOfAKind()), YahtzeeNetworkHandler::SC_FOUR_OF_A_KIND);
        }
    }
    else if(sprite == yahtzee->GetEditBoxFullHouse())
    {
        if(scorecard.AwardFullHouse(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFullHouse().get());
            editbox->SetText(scorecard.GetFullHouse());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetFullHouse()), YahtzeeNetworkHandler::SC_FULL_HOUSE);
        }
    }
    else if(sprite == yahtzee->GetEditBoxSmallStraight())
    {
        if(scorecard.AwardSmallStraight(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxSmallStraight().get());
            editbox->SetText(scorecard.GetSmallStraight());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetSmallStraight()), YahtzeeNetworkHandler::SC_SMALL_STRAIGHT);
        }
    }
    else if(sprite == yahtzee->GetEditBoxLargeStraight())
    {
        if(scorecard.AwardLargeStraight(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxLargeStraight().get());
            editbox->SetText(scorecard.GetLargeStraight());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetLargeStraight()), YahtzeeNetworkHandler::SC_LARGE_STRAIGHT);
        }
    }
    else if(sprite == yahtzee->GetEditBoxYahtzee())
    {
        if(scorecard.AwardYahtzee(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxYahtzee().get());
            editbox->SetText(scorecard.GetYahtzee());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetYahtzee()), YahtzeeNetworkHandler::SC_YAHTZEE);
        }
    }
    else if(sprite == yahtzee->GetEditBoxChance())
    {
        if(scorecard.AwardChance(roll))
        {
            editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxChance().get());
            editbox->SetText(scorecard.GetChance());
            editbox->Enable(false);
            HandlePostAwardPoints();
            BroadcastNetworkEventScoreTurn(boost::lexical_cast<int>(scorecard.GetChance()), YahtzeeNetworkHandler::SC_CHANCE);
        }
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

void StateGameWaitingForScore::OnSpriteEnterPressed(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetEditBoxMessage())
    {
        OnSpriteClicked(yahtzee->GetButtonSend());
        Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
        clickable->SetFocus(yahtzee->GetEditBoxMessage());
    }
}

void StateGameWaitingForScore::HandlePostAwardPoints()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Scorecard& scorecard = yahtzee->GetCurrentPlayer()->GetScorecard();

    Graphics::SpriteEditBox* editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxSubTotal().get());
    editbox->SetText(scorecard.GetSubTotal());

    if(scorecard.IsUpperComplete())
    {
        editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxBonus().get());
        editbox->SetText(scorecard.GetBonus());

        editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxUpperTotal().get());
        editbox->SetText(scorecard.GetUpperTotal());
    }

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxYahtzee().get());
    editbox->SetText(scorecard.GetYahtzee());

    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxLowerTotal().get());
    editbox->SetText(scorecard.GetLowerTotal());

    UpdateScores();

    if(scorecard.IsEverythingComplete())
    {
        editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxGrandTotal().get());
        editbox->SetText(scorecard.GetGrandTotal());
    }

    if(yahtzee->GetPlayerContainer()->size() > 1)
    {
        TransitionGameState(Core::StatePtr(new StateGameEndTurn(GetEngine())));
    }
    else
    {
        if(yahtzee->IsGameOver())
        {
            TransitionGameState(Core::StatePtr(new StateGameEndTurn(GetEngine())));
        }
        else
        {
            yahtzee->StartNewTurn();
            TransitionGameState(Core::StatePtr(new StateGameWaitingForRoll(GetEngine())));
        }
    }
}

void StateGameWaitingForScore::UpdateScores()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::SpriteListCtrl* list_ctrl = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlInGamePlayers().get());
    PlayerContainerPtr players = yahtzee->GetPlayerContainer();
    PlayerContainer::iterator player;
    int row = 0;
    for(player = players->begin(); player != players->end(); ++player, ++row)
    {
        list_ctrl->SetString(row, 1, (*player)->GetScorecard().GetGrandTotal());
    }
}

void StateGameWaitingForScore::OnNetworkEventScoreTurn(Network::SocketPtr sock, const YahtzeePacketScoreTurn& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetEditBoxClickSound()->Play();
    switch(yp.GetScoreCategory())
    {
    case SC_ONES:
        OnSpriteClicked(yahtzee->GetEditBoxOnes());
        break;
    case SC_TWOS:
        OnSpriteClicked(yahtzee->GetEditBoxTwos());
        break;
    case SC_THREES:
        OnSpriteClicked(yahtzee->GetEditBoxThrees());
        break;
    case SC_FOURS:
        OnSpriteClicked(yahtzee->GetEditBoxFours());
        break;
    case SC_FIVES:
        OnSpriteClicked(yahtzee->GetEditBoxFives());
        break;
    case SC_SIXES:
        OnSpriteClicked(yahtzee->GetEditBoxSixes());
        break;
    case SC_THREE_OF_A_KIND:
        OnSpriteClicked(yahtzee->GetEditBoxThreeOfAKind());
        break;
    case SC_FOUR_OF_A_KIND:
        OnSpriteClicked(yahtzee->GetEditBoxFourOfAKind());
        break;
    case SC_FULL_HOUSE:
        OnSpriteClicked(yahtzee->GetEditBoxFullHouse());
        break;
    case SC_SMALL_STRAIGHT:
        OnSpriteClicked(yahtzee->GetEditBoxSmallStraight());
        break;
    case SC_LARGE_STRAIGHT:
        OnSpriteClicked(yahtzee->GetEditBoxLargeStraight());
        break;
    case SC_YAHTZEE:
        OnSpriteClicked(yahtzee->GetEditBoxYahtzee());
        break;
    case SC_CHANCE:
        OnSpriteClicked(yahtzee->GetEditBoxChance());
        break;
    }
}

void StateGameWaitingForScore::OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->AddMessage(yp.GetMessage());
}

void StateGameWaitingForScore::OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetConnectionList()->clear();
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), yp.GetWhichPlayer(), yp.GetReason())));
}

void StateGameWaitingForScore::OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetPlayerLocation(yp.GetWhichPlayer(), yp.GetActivated(), "Away");
}

void StateGameWaitingForScore::OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetPlayerLocation(yp.GetWhichPlayer(), !yp.GetInSettings(), "Settings");
}

void StateGameWaitingForScore::OnWindowActivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), true);
}

void StateGameWaitingForScore::OnWindowDeactivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), false);
}

void StateGameWaitingForScore::OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetConnectionList()->clear();
    TransitionGameState(Core::StatePtr());
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), yp.GetWhichPlayer(), "This player left the game")));
}
