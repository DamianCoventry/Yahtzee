#include "stdafx.h"
#include "statemenuabout.h"
#include "../Graphics/vector.h"
#include "../Core/engine.h"
#include "../Graphics/spritelistctrl.h"
#include "yahtzee.h"
#include "statemenumain.h"
#include "statemenuabout.h"
#include "statemenusettings.h"
#include "statemenuplayersetup.h"
#include "statemenuviewhighscores.h"
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iomanip>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "yahtzeepacket.h"

StateMenuAbout::StateMenuAbout(Core::Engine* engine)
: LogContext("StateMenuAbout")
, State(engine)
{
}

StateMenuAbout::~StateMenuAbout()
{
    LOG("Ended.");
}

void StateMenuAbout::OnBegin()
{
    LOG("Beginning...");
}

void StateMenuAbout::OnEnd()
{
}

void StateMenuAbout::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    clickable->Append(yahtzee->GetButtonMain());
    clickable->Append(yahtzee->GetButtonViewHighscores());
    clickable->Append(yahtzee->GetButtonSettings());
    clickable->Append(yahtzee->GetButtonAbout());
    clickable->Append(yahtzee->GetButtonExitApplication());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetButtonMain());
    drawable->Append(yahtzee->GetButtonViewHighscores());
    drawable->Append(yahtzee->GetButtonSettings());
    drawable->Append(yahtzee->GetButtonAbout());
    drawable->Append(yahtzee->GetButtonExitApplication());

    clickable->SetFocus(yahtzee->GetButtonAbout());
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuAbout::DoFrameLogic(float time_delta)
{
}

void StateMenuAbout::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageAboutBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuAbout::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetButtonMain())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuMain(GetEngine())));
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
