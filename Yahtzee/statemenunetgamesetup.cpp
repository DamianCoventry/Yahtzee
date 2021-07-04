#include "stdafx.h"
#include "statemenunetgamesetup.h"
#include "../Graphics/vector.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritelistctrl.h"
#include "../Core/engine.h"
#include "statemenumain.h"
#include "statemenuplayersetup.h"
#include "statemenuviewhighscores.h"
#include "statemenusettings.h"
#include "statemenuabout.h"
#include "statemenunetconnecting.h"
#include "statemenunethostinggame.h"
#include "statemenunetgameentername.h"
#include "yahtzee.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "yahtzeepacket.h"

StateMenuNetGameSetup::StateMenuNetGameSetup(Core::Engine* engine, const std::string& local_player_name)
: YahtzeeNetworkHandler("StateMenuNetGameSetup", engine)
, State(engine)
, local_player_name_(local_player_name)
{
}

StateMenuNetGameSetup::~StateMenuNetGameSetup()
{
    LOG("Ended.");
}

void StateMenuNetGameSetup::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteEditBox* edit_box = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxPlayer().get());
    edit_box->SetPosition(Graphics::Vector(160.0f, 364.0f));
    edit_box->SetText(std::string());

    Graphics::SpriteListCtrl* list_ctrl = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlNetGames().get());
    list_ctrl->DeleteAllColumns();
    list_ctrl->DeleteAllRows();

    list_ctrl->AddColumn("IP Address", 140);
    list_ctrl->AddColumn("Player Name", 140);
    list_ctrl->AddColumn("Num Players", 110);

    PlayerPtr player(new Player(local_player_name_));
    player->SetRemote(false);
    PlayerContainerPtr player_container = yahtzee->GetPlayerContainer();
    player_container->clear();
    player_container->push_back(player);
    yahtzee->SetLocalPlayer(0);

    received_broadcasts_.clear();
}

void StateMenuNetGameSetup::OnEnd()
{
}

void StateMenuNetGameSetup::SetupLists()
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
    clickable->Append(yahtzee->GetStaticNetGameInfo1());
    clickable->Append(yahtzee->GetListCtrlNetGames());
    clickable->Append(yahtzee->GetButtonJoinSelectedNetGame());
    clickable->Append(yahtzee->GetButtonCreateNetGame());
    clickable->Append(yahtzee->GetStaticNetGameInfo2());
    clickable->Append(yahtzee->GetEditBoxPlayer());
    clickable->Append(yahtzee->GetButtonJoinSpecificNetGame());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetButtonMain());
    drawable->Append(yahtzee->GetButtonViewHighscores());
    drawable->Append(yahtzee->GetButtonSettings());
    drawable->Append(yahtzee->GetButtonAbout());
    drawable->Append(yahtzee->GetButtonExitApplication());
    drawable->Append(yahtzee->GetButtonLocalGame());
    drawable->Append(yahtzee->GetButtonNetworkGame());
    drawable->Append(yahtzee->GetStaticNetGameInfo1());
    drawable->Append(yahtzee->GetListCtrlNetGames());
    drawable->Append(yahtzee->GetButtonJoinSelectedNetGame());
    drawable->Append(yahtzee->GetButtonCreateNetGame());
    drawable->Append(yahtzee->GetStaticNetGameInfo2());
    drawable->Append(yahtzee->GetEditBoxPlayer());
    drawable->Append(yahtzee->GetButtonJoinSpecificNetGame());

    clickable->SetFocus(yahtzee->GetButtonNetworkGame());
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuNetGameSetup::DoFrameLogic(float time_delta)
{
    RemoveOldBroadcasts();
}

void StateMenuNetGameSetup::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageNetGameSetupBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuNetGameSetup::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

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
    else if(sprite == yahtzee->GetButtonJoinSelectedNetGame())
    {
        Graphics::SpriteListCtrl* list_ctrl = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlNetGames().get());
        int cur_sel = list_ctrl->GetCurSel();
        if(list_ctrl >= 0)
        {
            std::string ip_address = list_ctrl->GetString(cur_sel, 0);
            if(!ip_address.empty())
            {
                ReceivedBroadcasts::iterator itor = received_broadcasts_.find(ip_address);
                if(itor != received_broadcasts_.end())
                {
                    TransitionMenuState(Core::StatePtr(new StateMenuNetConnecting(GetEngine(), ip_address,
                        local_player_name_, itor->second.sock_)));
                }
                else
                {
                    TransitionMenuState(Core::StatePtr(new StateMenuNetConnecting(GetEngine(), ip_address,
                        local_player_name_)));
                }
            }
        }
    }
    else if(sprite == yahtzee->GetButtonCreateNetGame())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuNetHostingGame(GetEngine(), local_player_name_)));
    }
    else if(sprite == yahtzee->GetButtonJoinSpecificNetGame())
    {
        Graphics::SpriteEditBox* edit_box = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxPlayer().get());
        std::string ip_address = edit_box->GetText();
        if(!ip_address.empty())
        {
            TransitionMenuState(Core::StatePtr(new StateMenuNetConnecting(GetEngine(), ip_address, local_player_name_)));
        }
    }
}

void StateMenuNetGameSetup::OnSpriteEnterPressed(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetEditBoxPlayer())
    {
        OnSpriteClicked(yahtzee->GetButtonJoinSpecificNetGame());
        Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
        clickable->SetFocus(yahtzee->GetEditBoxPlayer());
    }
}

void StateMenuNetGameSetup::OnSpriteDoubleClicked(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetListCtrlNetGames())
    {
        OnSpriteClicked(yahtzee->GetButtonJoinSelectedNetGame());
    }
}

void StateMenuNetGameSetup::OnNetworkBroadcastGameAvailable(Network::SocketPtr socket, const YahtzeePacketBroadcastGameAvailable& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    // Don't insert the address if it's a local address
    if(yahtzee->IsLocalAddress(yp.GetFromAddress()))
    {
        return;
    }

    unsigned long now = timeGetTime();

    Graphics::SpriteListCtrl* list_ctrl = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlNetGames().get());
    int row_index = list_ctrl->FindRow(yp.GetFromAddress().ToString());
    if(row_index < 0)
    {
        row_index = list_ctrl->AddRow(yp.GetFromAddress().ToString());
    }
    list_ctrl->SetString(row_index, 1, yp.GetPlayerName());
    list_ctrl->SetString(row_index, 2, boost::lexical_cast<std::string>(yp.GetNumPlayers()));

    ReceivedBroadcasts::iterator itor = received_broadcasts_.find(yp.GetFromAddress().ToString());
    if(itor == received_broadcasts_.end())
    {
        BroadcastInfo info;
        info.last_received_time_    = now;
        info.sock_                  = socket;
        received_broadcasts_[yp.GetFromAddress().ToString()] = info;
    }
    else
    {
        itor->second.last_received_time_    = now;
        itor->second.sock_                  = socket;
    }
}

void StateMenuNetGameSetup::RemoveOldBroadcasts()
{
    unsigned long now = timeGetTime();

    ReceivedBroadcasts::iterator itor = received_broadcasts_.begin();
    while(itor != received_broadcasts_.end())
    {
        if(itor->second.last_received_time_ + 5000 <= now)
        {
            // Remove the row from the GUI if it's there.
            Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
            Graphics::SpriteListCtrl* list_ctrl = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlNetGames().get());
            int row_index = list_ctrl->FindRow(itor->first);
            if(row_index >= 0)
            {
                list_ctrl->RemoveRow(row_index);
            }

            // Remove the state info.
            received_broadcasts_.erase(itor);
            itor = received_broadcasts_.begin();
        }
        else
        {
            ++itor;
        }
    }
}
