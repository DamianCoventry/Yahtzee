#include "stdafx.h"
#include "statemenuplayersetup.h"
#include "../Graphics/vector.h"
#include "../Core/engine.h"
#include "../Graphics/spritelistbox.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritebutton.h"
#include "yahtzee.h"
#include "statemenumain.h"
#include "statemenuviewhighscores.h"
#include "statemenuplayersetup.h"
#include "statemenusettings.h"
#include "stategamewaitingforroll.h"
#include "statemenuabout.h"
#include "statemenunetgameentername.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "yahtzeepacket.h"

StateMenuPlayerSetup::StateMenuPlayerSetup(Core::Engine* engine)
: LogContext("StateMenuPlayerSetup")
, State(engine)
{
}

StateMenuPlayerSetup::~StateMenuPlayerSetup()
{
    LOG("Ended.");
}

void StateMenuPlayerSetup::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxPlayers().get());
    list_box->SetPosition(Graphics::Vector(165.0f, 160.0f));
    list_box->ResetContent();
    list_box->Enable(true);

    PlayerContainerPtr player_container = yahtzee->GetPlayerContainer();
    PlayerContainer::iterator player;
    for(player = player_container->begin(); player != player_container->end(); ++player)
    {
        list_box->AddString((*player)->GetName());
    }

    Graphics::SpriteEditBox* edit_box = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxPlayer().get());
    edit_box->SetPosition(Graphics::Vector(165.0f, 120.0f));
    edit_box->SetText(std::string());

    Graphics::SpriteButton* button = static_cast<Graphics::SpriteButton*>(yahtzee->GetButtonStart().get());
    button->SetPosition(Graphics::Vector(430.0f, 330.0f));

    yahtzee->GetButtonAddPlayer()->Enable(int(player_container->size()) < Yahtzee::MAX_PLAYERS);
    yahtzee->GetButtonEditPlayer()->Enable();
    yahtzee->GetButtonRemovePlayer()->Enable();
    yahtzee->GetButtonClearPlayers()->Enable();
    yahtzee->GetButtonStart()->Enable(list_box->GetStringCount() > 0);
}

void StateMenuPlayerSetup::OnEnd()
{
}

void StateMenuPlayerSetup::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    clickable->Append(yahtzee->GetButtonMain());
    clickable->Append(yahtzee->GetButtonViewHighscores());
    clickable->Append(yahtzee->GetButtonSettings());
    clickable->Append(yahtzee->GetButtonAbout());
    clickable->Append(yahtzee->GetButtonExitApplication());
    clickable->Append(yahtzee->GetButtonLocalGame());
    clickable->Append(yahtzee->GetButtonNetworkGame());
    clickable->Append(yahtzee->GetEditBoxPlayer());
    clickable->Append(yahtzee->GetButtonAddPlayer());
    clickable->Append(yahtzee->GetListBoxPlayers());
    clickable->Append(yahtzee->GetButtonEditPlayer());
    clickable->Append(yahtzee->GetButtonRemovePlayer());
    clickable->Append(yahtzee->GetButtonClearPlayers());
    clickable->Append(yahtzee->GetButtonStart());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetButtonMain());
    drawable->Append(yahtzee->GetButtonViewHighscores());
    drawable->Append(yahtzee->GetButtonSettings());
    drawable->Append(yahtzee->GetButtonAbout());
    drawable->Append(yahtzee->GetButtonExitApplication());
    drawable->Append(yahtzee->GetButtonLocalGame());
    drawable->Append(yahtzee->GetButtonNetworkGame());
    drawable->Append(yahtzee->GetEditBoxPlayer());
    drawable->Append(yahtzee->GetButtonAddPlayer());
    drawable->Append(yahtzee->GetListBoxPlayers());
    drawable->Append(yahtzee->GetButtonEditPlayer());
    drawable->Append(yahtzee->GetButtonRemovePlayer());
    drawable->Append(yahtzee->GetButtonClearPlayers());
    drawable->Append(yahtzee->GetButtonStart());

    clickable->SetFocus(yahtzee->GetEditBoxPlayer());
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuPlayerSetup::DoFrameLogic(float time_delta)
{
}

void StateMenuPlayerSetup::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImagePlayerSetupBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuPlayerSetup::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxPlayers().get());
    Graphics::SpriteEditBox* edit_box = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxPlayer().get());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();

    if(sprite == yahtzee->GetButtonMain())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuMain(GetEngine())));
    }
    else if(sprite == yahtzee->GetButtonLocalGame())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuPlayerSetup(GetEngine())));
    }
    else if(sprite == yahtzee->GetButtonNetworkGame())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnterName(GetEngine())));
    }
    else if(sprite == yahtzee->GetButtonAddPlayer())
    {
        if(!edit_box->GetText().empty())
        {
            list_box->AddString(edit_box->GetText());
            edit_box->SetText(std::string());
            yahtzee->GetButtonStart()->Enable(true);
            yahtzee->GetButtonAddPlayer()->Enable(list_box->GetStringCount() < Yahtzee::MAX_PLAYERS);
            yahtzee->GetButtonStart()->Enable(list_box->GetStringCount() > 0);
        }
    }
    else if(sprite == yahtzee->GetButtonUpdatePlayer())
    {
        if(!edit_box->GetText().empty())
        {
            int cur_sel = list_box->GetCurSel();
            if(cur_sel != -1)
            {
                list_box->SetString(cur_sel, edit_box->GetText());
            }
            edit_box->SetText(std::string());
            clickable->Replace(yahtzee->GetButtonUpdatePlayer(), yahtzee->GetButtonAddPlayer());
            drawable->Replace(yahtzee->GetButtonUpdatePlayer(), yahtzee->GetButtonAddPlayer());
            if(list_box->GetStringCount() < Yahtzee::MAX_PLAYERS)
            {
                yahtzee->GetButtonAddPlayer()->Enable();
                clickable->SetFocus(yahtzee->GetButtonAddPlayer());
            }
            else
            {
                yahtzee->GetButtonAddPlayer()->Enable(false);
            }
            yahtzee->GetListBoxPlayers()->Enable();
            yahtzee->GetButtonEditPlayer()->Enable();
            yahtzee->GetButtonRemovePlayer()->Enable();
            yahtzee->GetButtonClearPlayers()->Enable();
            yahtzee->GetButtonStart()->Enable(list_box->GetStringCount() > 0);
        }
    }
    else if(sprite == yahtzee->GetButtonEditPlayer())
    {
        int cur_sel = list_box->GetCurSel();
        if(cur_sel != -1)
        {
            edit_box->SetText(list_box->GetString(cur_sel));
            clickable->SetFocus(yahtzee->GetEditBoxPlayer());
            clickable->Replace(yahtzee->GetButtonAddPlayer(), yahtzee->GetButtonUpdatePlayer());
            drawable->Replace(yahtzee->GetButtonAddPlayer(), yahtzee->GetButtonUpdatePlayer());
            yahtzee->GetListBoxPlayers()->Enable(false);
            yahtzee->GetButtonEditPlayer()->Enable(false);
            yahtzee->GetButtonRemovePlayer()->Enable(false);
            yahtzee->GetButtonClearPlayers()->Enable(false);
            yahtzee->GetButtonStart()->Enable(false);
        }
    }
    else if(sprite == yahtzee->GetButtonRemovePlayer())
    {
        int cur_sel = list_box->GetCurSel();
        if(cur_sel != -1)
        {
            list_box->RemoveString(cur_sel);
            yahtzee->GetButtonStart()->Enable(list_box->GetStringCount() > 0);
            yahtzee->GetButtonAddPlayer()->Enable(list_box->GetStringCount() < Yahtzee::MAX_PLAYERS);
        }
    }
    else if(sprite == yahtzee->GetButtonClearPlayers())
    {
        list_box->ResetContent();
        yahtzee->GetButtonStart()->Enable(false);
        yahtzee->GetButtonAddPlayer()->Enable();
    }
    else if(sprite == yahtzee->GetButtonStart())
    {
        int count = list_box->GetStringCount();
        if(count > 0)
        {
            PlayerContainerPtr player_container = yahtzee->GetPlayerContainer();
            player_container->clear();

            for(int i = 0; i < count; i++)
            {
                player_container->push_back(PlayerPtr(new Player(list_box->GetString(i))));
            }

            yahtzee->StartLocalGame();
            TransitionMenuState(Core::StatePtr());
            TransitionGameState(Core::StatePtr(new StateGameWaitingForRoll(GetEngine())));
        }
    }
    else if(sprite == yahtzee->GetButtonViewHighscores())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuViewHighscores(GetEngine())));
    }
    else if(sprite == yahtzee->GetButtonSettings())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuSettings(GetEngine())));
    }
    else if(sprite == yahtzee->GetButtonAbout())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuAbout(GetEngine())));
    }
    else if(sprite == yahtzee->GetButtonExitApplication())
    {
        PostQuitMessage(0);
    }
}

void StateMenuPlayerSetup::OnSpriteEnterPressed(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetEditBoxPlayer())
    {
        // Either add or update the player name
        Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
        if(clickable->IsPresent(yahtzee->GetButtonUpdatePlayer()))
        {
            OnSpriteClicked(yahtzee->GetButtonUpdatePlayer());
        }
        else
        {
            OnSpriteClicked(yahtzee->GetButtonAddPlayer());
        }

        clickable->SetFocus(yahtzee->GetEditBoxPlayer());
    }
}

void StateMenuPlayerSetup::OnSpriteDoubleClicked(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetListBoxPlayers())
    {
        OnSpriteClicked(yahtzee->GetButtonEditPlayer());
    }
}
