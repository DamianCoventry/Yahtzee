#include "stdafx.h"
#include "statemenunetconnecting.h"
#include "../Graphics/vector.h"
#include "../Graphics/spritestatic.h"
#include "../Graphics/spritebutton.h"
#include "../Core/engine.h"
#include "yahtzee.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "statemenunetgamesetup.h"
#include "statemenunetgameended.h"
#include "statemenunetjoinedremote.h"
#include "yahtzeepacket.h"

StateMenuNetConnecting::StateMenuNetConnecting(Core::Engine* engine, const std::string& ip_address, const std::string& local_player_name)
: YahtzeeNetworkHandler("StateMenuNetConnecting", engine)
, State(engine)
, ip_address_(ip_address, engine->GetNetworkPort())
, local_player_name_(local_player_name)
{
}

StateMenuNetConnecting::StateMenuNetConnecting(Core::Engine* engine, const std::string& ip_address, const std::string& local_player_name, Network::SocketPtr sock)
: YahtzeeNetworkHandler("StateMenuNetConnecting", engine)
, State(engine)
, ip_address_(ip_address, engine->GetNetworkPort())
, local_player_name_(local_player_name)
, sock_to_use_(sock)
{
}

StateMenuNetConnecting::~StateMenuNetConnecting()
{
    LOG("Ended.");
}

void StateMenuNetConnecting::OnBegin()
{
    LOG("Beginning...");
    last_send_time_ = timeGetTime() - 2000;
}

void StateMenuNetConnecting::OnEnd()
{
}

void StateMenuNetConnecting::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    clickable->Append(yahtzee->GetStaticIpText());
    clickable->Append(yahtzee->GetButtonCancel());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetStaticIpText());
    drawable->Append(yahtzee->GetButtonCancel());

    Graphics::SpriteStatic* static_sprite = static_cast<Graphics::SpriteStatic*>(yahtzee->GetStaticIpText().get());
    static_sprite->SetPosition(Graphics::Vector(205.0f, 218.0f));
    static_sprite->SetBoundingRect(Graphics::Vector(), Graphics::Vector(230.0f, 36.0f));
    static_sprite->SetText(std::string("Contacting server ") + ip_address_.ToLongString());

    Graphics::SpriteButton* button = static_cast<Graphics::SpriteButton*>(yahtzee->GetButtonCancel().get());
    button->SetPosition(Graphics::Vector(366.0f, 258.0f));

    clickable->MoveFocusFirst();
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuNetConnecting::DoFrameLogic(float time_delta)
{
    unsigned long now = timeGetTime();
    if(last_send_time_ + 3000 <= now)
    {
        last_send_time_ = now;
        sock_to_use_ ?
            SendNetworkEventJoinRequest(ip_address_, local_player_name_, sock_to_use_) :
            SendNetworkEventJoinRequest(ip_address_, local_player_name_);
    }
}

void StateMenuNetConnecting::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageNetGameConnectingBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuNetConnecting::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    if(sprite == yahtzee->GetButtonCancel())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuNetGameSetup(GetEngine(), local_player_name_)));
    }
}

void StateMenuNetConnecting::OnNetworkEventJoinAccepted(Network::SocketPtr sock, const YahtzeePacketJoinAccepted& yp)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Network::ConnectionPtr connection(new Network::Connection(ip_address_, sock));
    yahtzee->GetConnectionList()->push_back(connection);

    PlayerContainerPtr player_container = yahtzee->GetPlayerContainer();
    player_container->clear();
    std::vector<std::string>::const_iterator itor;
    for(itor = yp.GetPlayerNames().begin(); itor != yp.GetPlayerNames().end(); ++itor)
    {
        PlayerPtr player(new Player(*itor));
        player->SetRemote(true);
        player_container->push_back(player);
    }
    yahtzee->SetLocalPlayer(yp.GetYourPlayerIndex());
    yahtzee->GetLocalPlayer()->SetRemote(false);

    TransitionMenuState(Core::StatePtr(new StateMenuNetJoinedRemote(GetEngine(), local_player_name_)));
}

void StateMenuNetConnecting::OnNetworkEventJoinDeclined(Network::SocketPtr sock, const YahtzeePacketJoinDeclined& yp)
{
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(GetEngine(), -1, yp.GetReason())));
}

void StateMenuNetConnecting::OnWindowActivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), true);
}

void StateMenuNetConnecting::OnWindowDeactivated()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    BroadcastNetworkEventActivateApp(yahtzee->GetLocalPlayerIndex(), false);
}
