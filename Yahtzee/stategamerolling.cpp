#include "stdafx.h"
#include "stategamerolling.h"
#include "../Graphics/vector.h"
#include "../Core/engine.h"
#include "../Core/timeoutmanager.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritestatic.h"
#include "../Graphics/spritelistbox.h"
#include "../Graphics/imagelist.h"
#include "../Input/keyboard.h"
#include "yahtzee.h"
#include "statemenumain.h"
#include "stategamewaitingforscore.h"
#include "stategamewaitingforscoreorroll.h"
#include "stategameyahtzee.h"
#include "statemenuingamesettings.h"
#include "statemenunetgameended.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include <sstream>
#include "yahtzeepacket.h"

StateGameRolling::StateGameRolling(Core::Engine* engine, int* dice_values, bool yahtzee)
: YahtzeeNetworkHandler("StateGameRolling", engine)
, State(engine)
{
    net_controlled_ = (dice_values != NULL);
    if(net_controlled_)
    {
        memcpy(dice_values_, dice_values, sizeof(int)*6);
        yahtzee_ = yahtzee;
    }
}

StateGameRolling::~StateGameRolling()
{
    LOG("Ended.");
}

void StateGameRolling::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Roll& roll = yahtzee->GetCurrentRoll();

    yahtzee->GetButtonRoll()->Enable(false);

    Graphics::SpriteListBox* listbox = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxMessages().get());
    listbox->AddWrappedString("Rolling the dice...");
    listbox->ScrollToBottom();

    dice_animations_[0].state_      = DiceAnimation::DAS_FROM;
    dice_animations_[0].counter_    = 0.0f;
    dice_animations_[0].threshold_  = GenerateTimeIncrement();
    dice_animations_[1].state_      = DiceAnimation::DAS_FROM;
    dice_animations_[1].counter_    = 0.0f;
    dice_animations_[1].threshold_  = GenerateTimeIncrement();
    dice_animations_[2].state_      = DiceAnimation::DAS_FROM;
    dice_animations_[2].counter_    = 0.0f;
    dice_animations_[2].threshold_  = GenerateTimeIncrement();
    dice_animations_[3].state_      = DiceAnimation::DAS_FROM;
    dice_animations_[3].counter_    = 0.0f;
    dice_animations_[3].threshold_  = GenerateTimeIncrement();
    dice_animations_[4].state_      = DiceAnimation::DAS_FROM;
    dice_animations_[4].counter_    = 0.0f;
    dice_animations_[4].threshold_  = GenerateTimeIncrement();

    Graphics::SpriteStatic* sprite_static;

    if(!roll.GetHold(0))
    {
        sprite_static = static_cast<Graphics::SpriteStatic*>(yahtzee->GetStaticDice0().get());
        sprite_static->SetImageList(yahtzee->ChooseDiceFromImageList(roll.GetValue(0)));
        sprite_static->SetCurrentFrame(0);
    }
    if(!roll.GetHold(1))
    {
        sprite_static = static_cast<Graphics::SpriteStatic*>(yahtzee->GetStaticDice1().get());
        sprite_static->SetImageList(yahtzee->ChooseDiceFromImageList(roll.GetValue(1)));
        sprite_static->SetCurrentFrame(0);
    }
    if(!roll.GetHold(2))
    {
        sprite_static = static_cast<Graphics::SpriteStatic*>(yahtzee->GetStaticDice2().get());
        sprite_static->SetImageList(yahtzee->ChooseDiceFromImageList(roll.GetValue(2)));
        sprite_static->SetCurrentFrame(0);
    }
    if(!roll.GetHold(3))
    {
        sprite_static = static_cast<Graphics::SpriteStatic*>(yahtzee->GetStaticDice3().get());
        sprite_static->SetImageList(yahtzee->ChooseDiceFromImageList(roll.GetValue(3)));
        sprite_static->SetCurrentFrame(0);
    }
    if(!roll.GetHold(4))
    {
        sprite_static = static_cast<Graphics::SpriteStatic*>(yahtzee->GetStaticDice4().get());
        sprite_static->SetImageList(yahtzee->ChooseDiceFromImageList(roll.GetValue(4)));
        sprite_static->SetCurrentFrame(0);
    }

    if(net_controlled_)
    {
        yahtzee->ThrowFromNet(dice_values_);
    }
    else
    {
        // We throw the dice now.
        Input::DevicePtr inp_device = yahtzee->GetInputDevice();
        Input::KeyboardPtr kbd = inp_device->GetKeyboard();
        if(kbd->IsKeyHeld(DIK_LSHIFT) || kbd->IsKeyHeld(DIK_RSHIFT))        // How to cheat.. ;o)
        {
            yahtzee->ThrowAYahtzee();
        }
        else
        {
            yahtzee->ThrowDice();
        }

        Roll& roll = yahtzee->GetCurrentRoll();
        int dice_values[5];
        dice_values[0] = roll.GetValue(0); dice_values[1] = roll.GetValue(1);
        dice_values[2] = roll.GetValue(2); dice_values[3] = roll.GetValue(3);
        dice_values[4] = roll.GetValue(4);
        BroadcastNetworkEventThrowDice(dice_values);
    }

    yahtzee->GetShakeDiceSound()->Play();
}

void StateGameRolling::OnEnd()
{
}

void StateGameRolling::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
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

void StateGameRolling::DoFrameLogic(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    UpdateTimeout(time_delta, 0, yahtzee->GetStaticDice0());
    UpdateTimeout(time_delta, 1, yahtzee->GetStaticDice1());
    UpdateTimeout(time_delta, 2, yahtzee->GetStaticDice2());
    UpdateTimeout(time_delta, 3, yahtzee->GetStaticDice3());
    UpdateTimeout(time_delta, 4, yahtzee->GetStaticDice4());

    if(!net_controlled_)
    {
        // Check if all dice are done rolling.
        bool stategame_completed = true;
        Roll& roll = yahtzee->GetCurrentRoll();
        if(!roll.GetHold(0) && !dice_animations_[0].done_) stategame_completed = false;
        if(!roll.GetHold(1) && !dice_animations_[1].done_) stategame_completed = false;
        if(!roll.GetHold(2) && !dice_animations_[2].done_) stategame_completed = false;
        if(!roll.GetHold(3) && !dice_animations_[3].done_) stategame_completed = false;
        if(!roll.GetHold(4) && !dice_animations_[4].done_) stategame_completed = false;

        if(stategame_completed)
        {
            BroadcastNetworkEventThrowDone();

            if(roll.IsYahtzee())
            {
                TransitionGameState(Core::StatePtr(new StateGameYahtzee(GetEngine())));
            }
            else if(yahtzee->IsFinalRoll())
            {
                TransitionGameState(Core::StatePtr(new StateGameWaitingForScore(GetEngine())));
            }
            else
            {
                TransitionGameState(Core::StatePtr(new StateGameWaitingForScoreOrRoll(GetEngine())));
            }
        }
    }
}

void StateGameRolling::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageGameBoard()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateGameRolling::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerPtr current_player = yahtzee->GetCurrentPlayer();

    if(sprite == yahtzee->GetButtonInGameSettings())
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

void StateGameRolling::OnSpriteEnterPressed(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetEditBoxMessage())
    {
        OnSpriteClicked(yahtzee->GetButtonSend());
        Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
        clickable->SetFocus(yahtzee->GetEditBoxMessage());
    }
}

void StateGameRolling::UpdateTimeout(float time_delta, int dice, Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Roll& roll = yahtzee->GetCurrentRoll();
    if(roll.GetHold(dice) || dice_animations_[dice].done_)
    {
        return;
    }

    dice_animations_[dice].counter_ += time_delta;
    if(dice_animations_[dice].counter_ >= dice_animations_[dice].threshold_)
    {
        dice_animations_[dice].counter_    = 0.0f;
        dice_animations_[dice].threshold_  = GenerateTimeIncrement();

        Graphics::SpriteStatic* sprite_static = static_cast<Graphics::SpriteStatic*>(sprite.get());
        if(!sprite_static->IncrementCurrentFrame_Clamp())
        {
            // Then we've reached the end of the images in the imagelist.
            Graphics::ImageListPtr new_imagelist;
            switch(dice_animations_[dice].state_)
            {
            case DiceAnimation::DAS_FROM:
                new_imagelist = yahtzee->ChooseDiceSpinImageList(sprite_static->GetImageList());
                sprite_static->SetImageList(new_imagelist);
                sprite_static->SetCurrentFrame(0);
                dice_animations_[dice].state_ = DiceAnimation::DAS_SPIN;
                break;
            case DiceAnimation::DAS_SPIN:
                new_imagelist = yahtzee->SyncDiceToImageListToCurrentRoll(roll.GetValue(dice), sprite_static->GetImageList());
                sprite_static->SetImageList(new_imagelist);
                sprite_static->SetCurrentFrame(0);
                dice_animations_[dice].state_ = DiceAnimation::DAS_TO;
                break;
            case DiceAnimation::DAS_TO:
                dice_animations_[dice].done_ = true;
                break;
            }
        }
    }
}

float StateGameRolling::GenerateTimeIncrement() const
{
    return float(40 + ((rand()%20)-10))/1000.0f;
}

void StateGameRolling::SetFinalDiceImages()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    SetFinalDiceImage(0, yahtzee->GetStaticDice0());
    SetFinalDiceImage(1, yahtzee->GetStaticDice1());
    SetFinalDiceImage(2, yahtzee->GetStaticDice2());
    SetFinalDiceImage(3, yahtzee->GetStaticDice3());
    SetFinalDiceImage(4, yahtzee->GetStaticDice4());
}

void StateGameRolling::SetFinalDiceImage(int dice, Graphics::SpritePtr sprite)
{
    Graphics::SpriteStatic* sprite_static = static_cast<Graphics::SpriteStatic*>(sprite.get());

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    sprite_static->SetCurrentFrame(sprite_static->GetImageList()->GetImageCount()-1);
}

void StateGameRolling::OnNetworkEventThrowDone(Network::SocketPtr sock, const YahtzeePacketThrowDone& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    SetFinalDiceImages();

    Roll& roll = yahtzee->GetCurrentRoll();
    if(roll.IsYahtzee())
    {
        TransitionGameState(Core::StatePtr(new StateGameYahtzee(GetEngine())));
    }
    else if(yahtzee->IsFinalRoll())
    {
        TransitionGameState(Core::StatePtr(new StateGameWaitingForScore(GetEngine())));
    }
    else
    {
        TransitionGameState(Core::StatePtr(new StateGameWaitingForScoreOrRoll(GetEngine())));
    }
}

void StateGameRolling::OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->AddMessage(yp.GetMessage());
}

void StateGameRolling::OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetConnectionList()->clear();
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), yp.GetWhichPlayer(), yp.GetReason())));
}

void StateGameRolling::OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetPlayerLocation(yp.GetWhichPlayer(), yp.GetActivated(), "Away");
}

void StateGameRolling::OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetPlayerLocation(yp.GetWhichPlayer(), !yp.GetInSettings(), "Settings");
}

void StateGameRolling::OnWindowActivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), true);
}

void StateGameRolling::OnWindowDeactivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), false);
}

void StateGameRolling::OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->GetConnectionList()->clear();
    TransitionGameState(Core::StatePtr());
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), yp.GetWhichPlayer(), "This player left the game")));
}
