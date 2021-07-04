#include "stdafx.h"
#include "statemenunethostinggame.h"
#include "../Graphics/vector.h"
#include "../Graphics/spritebutton.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritelistbox.h"
#include "../Core/engine.h"
#include "yahtzee.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "statemenunetgamesetup.h"
#include "stategamewaitingforroll.h"
#include "yahtzeepacket.h"

StateMenuNetHostingGame::StateMenuNetHostingGame(Core::Engine* engine, const std::string& local_player_name)
: YahtzeeNetworkHandler("StateMenuNetHostingGame", engine)
, State(engine)
, local_player_name_(local_player_name)
{
}

StateMenuNetHostingGame::~StateMenuNetHostingGame()
{
    LOG("Ended.");
}

void StateMenuNetHostingGame::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    yahtzee->SetHosting(true);

    Graphics::SpriteEditBox* edit_box = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxPlayer().get());
    edit_box->SetPosition(Graphics::Vector(145.0f, 360.0f));
    edit_box->SetText(std::string());

    Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetMessages().get());
    list_box->ResetContent();

    list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetPlayers().get());
    list_box->ResetContent();
    list_box->AddString(local_player_name_);

    Graphics::SpriteButton* button = static_cast<Graphics::SpriteButton*>(yahtzee->GetButtonStart().get());
    button->SetPosition(Graphics::Vector(545.0f, 141.0f));
    button->Enable(false);

    button = static_cast<Graphics::SpriteButton*>(yahtzee->GetButtonCancel().get());
    button->SetPosition(Graphics::Vector(545.0f, 181.0f));

    button = static_cast<Graphics::SpriteButton*>(yahtzee->GetButtonSend().get());
    button->SetPosition(Graphics::Vector(402.0f, 360.0f));

    last_broadcast_time_ = 0;
}

void StateMenuNetHostingGame::OnEnd()
{
}

void StateMenuNetHostingGame::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    clickable->Append(yahtzee->GetListBoxNetPlayers());
    clickable->Append(yahtzee->GetListBoxNetMessages());
    clickable->Append(yahtzee->GetButtonStart());
    clickable->Append(yahtzee->GetButtonCancel());
    clickable->Append(yahtzee->GetEditBoxPlayer());
    clickable->Append(yahtzee->GetButtonSend());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetStaticNetGameHeading2());
    drawable->Append(yahtzee->GetListBoxNetPlayers());
    drawable->Append(yahtzee->GetListBoxNetMessages());
    drawable->Append(yahtzee->GetButtonStart());
    drawable->Append(yahtzee->GetButtonCancel());
    drawable->Append(yahtzee->GetEditBoxPlayer());
    drawable->Append(yahtzee->GetButtonSend());

    clickable->SetFocus(yahtzee->GetEditBoxPlayer());
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuNetHostingGame::DoFrameLogic(float time_delta)
{
    unsigned long now = timeGetTime();
    if(last_broadcast_time_ + 1000 <= now)
    {
        last_broadcast_time_ = now;

        Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
        Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetPlayers().get());

        BroadcastGameAvailable(yahtzee->GetBroadcastAddresses(), local_player_name_, list_box->GetStringCount());
    }
}

void StateMenuNetHostingGame::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageNetGameBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuNetHostingGame::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    if(sprite == yahtzee->GetButtonStart())
    {
        Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetPlayers().get());
        int count = list_box->GetStringCount();
        if(count > 1)
        {
            BroadcastNetworkEventGameStarting();

            yahtzee->StartNetworkGame();
            TransitionMenuState(Core::StatePtr());
            TransitionGameState(Core::StatePtr(new StateGameWaitingForRoll(GetEngine())));
        }
    }
    else if(sprite == yahtzee->GetButtonCancel())
    {
        yahtzee->EndNetworkGame(this);
    }
    else if(sprite == yahtzee->GetButtonSend())
    {
        Graphics::SpriteEditBox* edit_box = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxPlayer().get());
        std::string text = edit_box->GetText();
        if(!text.empty())
        {
            std::string message = local_player_name_ + ": " + text;

            Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetMessages().get());
            list_box->AddString(message);
            list_box->ScrollToBottom();
            edit_box->SetText(std::string());

            BroadcastNetworkEventMessage(message);
        }
    }
}

void StateMenuNetHostingGame::OnSpriteEnterPressed(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetEditBoxPlayer())
    {
        OnSpriteClicked(yahtzee->GetButtonSend());
        Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
        clickable->SetFocus(yahtzee->GetEditBoxPlayer());
    }
}

void StateMenuNetHostingGame::OnNetworkEventJoinRequest(Network::SocketPtr sock, const YahtzeePacketJoinRequest& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerContainerPtr player_container = yahtzee->GetPlayerContainer();

    if(player_container->size() >= Yahtzee::MAX_PLAYERS)
    {
        SendNetworkEventJoinDeclined(yp.GetFromAddress(), "Player limit reached");
    }
    else
    {
        BroadcastNetworkEventPlayerJoined(int(player_container->size()), yp.GetPlayerName());

        Network::ConnectionPtr connection(new Network::Connection(yp.GetFromAddress(), sock));
        Network::ConnectionListPtr connection_list = yahtzee->GetConnectionList();
        connection_list->push_back(connection);

        PlayerPtr player(new Player(yp.GetPlayerName()));
        player->SetRemote(true);
        player_container->push_back(player);

        std::vector<std::string> player_names;
        PlayerContainer::const_iterator itor;
        for(itor = player_container->begin(); itor != player_container->end(); ++itor)
        {
            player_names.push_back((*itor)->GetName());
        }

        SendNetworkEventJoinAccepted(yp.GetFromAddress(), player_names, int(player_container->size()-1));

        Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetPlayers().get());
        list_box->AddString(yp.GetPlayerName());

        Graphics::SpriteButton* button = static_cast<Graphics::SpriteButton*>(yahtzee->GetButtonStart().get());
        button->Enable(list_box->GetStringCount() > 1);

        list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetMessages().get());
        list_box->AddString(yp.GetPlayerName() + " joined the game.");
    }
}

void StateMenuNetHostingGame::OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetMessages().get());
    list_box->AddString(yp.GetMessage());
    list_box->ScrollToBottom();
    yahtzee->GetMessageReceiveSound()->Play();
}

void StateMenuNetHostingGame::OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    PlayerContainerPtr player_container = yahtzee->GetPlayerContainer();
    if(yp.GetWhichPlayer() < int(player_container->size()))
    {
        PlayerContainer::iterator itor = player_container->begin();
        std::advance(itor, yp.GetWhichPlayer());

        std::ostringstream oss;
        if(!yp.GetActivated()) oss << "(Away) ";
        oss << (*itor)->GetName();

        Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetPlayers().get());
        list_box->SetString(yp.GetWhichPlayer(), oss.str());
    }
}

void StateMenuNetHostingGame::OnWindowActivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), true);
}

void StateMenuNetHostingGame::OnWindowDeactivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), false);
}

void StateMenuNetHostingGame::OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Network::ConnectionListPtr connection_list = yahtzee->GetConnectionList();
    // There's always one fewer connections than there is players, because the local player (the computer
    // hosting the game) does not have a connection.
    int conn_index = yp.GetWhichPlayer() - 1;
    if(conn_index >= 0 && conn_index < int(connection_list->size()))
    {
        // Remove the connection state
        Network::ConnectionList::iterator itor = connection_list->begin();
        std::advance(itor, conn_index);
        connection_list->erase(itor);
    }

    PlayerContainerPtr player_container = yahtzee->GetPlayerContainer();
    if(yp.GetWhichPlayer() >= 0 && yp.GetWhichPlayer() < int(player_container->size()))
    {
        // Remove the player state
        PlayerContainer::iterator itor = player_container->begin();
        std::advance(itor, yp.GetWhichPlayer());

        Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetMessages().get());
        list_box->AddString((*itor)->GetName() + " left the game.");

        player_container->erase(itor);

        // Update the GUI
        list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxNetPlayers().get());
        list_box->ResetContent();

        for(itor = player_container->begin(); itor != player_container->end(); ++itor)
        {
            list_box->AddString((*itor)->GetName());
        }

        Graphics::SpriteButton* button = static_cast<Graphics::SpriteButton*>(yahtzee->GetButtonStart().get());
        button->Enable(list_box->GetStringCount() > 1);
    }

    // Propagate this message to the other clients.
    //BroadcastNetworkEventPlayerLeft(yp.GetWhichPlayer());
}
