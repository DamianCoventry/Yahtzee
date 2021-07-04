#include "stdafx.h"
#include "statemenunetgameended.h"
#include "../Graphics/vector.h"
#include "../Core/engine.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritelistbox.h"
#include "yahtzee.h"
#include "statemenumain.h"
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Audio/sound.h"
#include "yahtzeepacket.h"

StateMenuNetGameEnded::StateMenuNetGameEnded(Core::Engine* engine, int which_player, const std::string& reason)
: LogContext("StateMenuNetGameEnded")
, State(engine)
, which_player_(which_player)
, reason_(reason)
{
}

StateMenuNetGameEnded::~StateMenuNetGameEnded()
{
    LOG("Ended.");
}

void StateMenuNetGameEnded::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::SpriteEditBox* edit_box = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxPlayer().get());
    edit_box->SetPosition(Graphics::Vector(132.0f, 153.0f));

    PlayerContainerPtr player_container = yahtzee->GetPlayerContainer();
    if(which_player_ >= 0 && which_player_ < int(player_container->size()))
    {
        PlayerContainer::iterator player = player_container->begin();
        std::advance(player, which_player_);
        edit_box->SetText((*player)->GetName());
    }
    else
    {
        edit_box->SetText("Unknown");
    }

    Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(yahtzee->GetListBoxPlayers().get());
    list_box->SetPosition(Graphics::Vector(132.0f, 188.0f));
    list_box->ResetContent();
    list_box->Enable(true);
    list_box->AddWrappedString(reason_);
}

void StateMenuNetGameEnded::OnEnd()
{
}

void StateMenuNetGameEnded::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    clickable->Append(yahtzee->GetButtonContinue());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetEditBoxPlayer());
    drawable->Append(yahtzee->GetListBoxPlayers());
    drawable->Append(yahtzee->GetButtonContinue());

    clickable->MoveFocusFirst();
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuNetGameEnded::DoFrameLogic(float time_delta)
{
}

void StateMenuNetGameEnded::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageNetGameEndBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuNetGameEnded::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetButtonContinue())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuMain(GetEngine())));
    }
}
