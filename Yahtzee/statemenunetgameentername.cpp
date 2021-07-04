#include "stdafx.h"
#include "statemenunetgameentername.h"
#include "../Graphics/vector.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritebutton.h"
#include "../Graphics/spritestatic.h"
#include "../Core/engine.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "yahtzee.h"
#include "statemenunetgamesetup.h"
#include "statemenumain.h"
#include "statemenuplayersetup.h"
#include "statemenunetgameentername.h"
#include "statemenuviewhighscores.h"
#include "statemenusettings.h"
#include "statemenuabout.h"
#include "yahtzeepacket.h"

StateMenuNetGameEnterName::StateMenuNetGameEnterName(Core::Engine* engine)
: LogContext("StateMenuNetGameEnterName")
, State(engine)
{
}

StateMenuNetGameEnterName::~StateMenuNetGameEnterName()
{
    LOG("Ended.");
}

void StateMenuNetGameEnterName::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteStatic* sprite_static = static_cast<Graphics::SpriteStatic*>(yahtzee->GetStaticIpText().get());
    sprite_static->SetPosition(Graphics::Vector(160.0f, 150.0f));
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(300.0f, 30.0f));
    sprite_static->SetText("Please enter your name:");

    Graphics::SpriteEditBox* edit_box = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxPlayer().get());
    edit_box->SetPosition(Graphics::Vector(170.0f, 190.0f));
    edit_box->SetText(std::string());

    Graphics::SpriteButton* button = static_cast<Graphics::SpriteButton*>(yahtzee->GetButtonOK().get());
    button->SetPosition(Graphics::Vector(440.0f, 190.0f));
}

void StateMenuNetGameEnterName::OnEnd()
{
}

void StateMenuNetGameEnterName::SetupLists()
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
    clickable->Append(yahtzee->GetStaticIpText());
    clickable->Append(yahtzee->GetEditBoxPlayer());
    clickable->Append(yahtzee->GetButtonOK());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetButtonMain());
    drawable->Append(yahtzee->GetButtonViewHighscores());
    drawable->Append(yahtzee->GetButtonSettings());
    drawable->Append(yahtzee->GetButtonAbout());
    drawable->Append(yahtzee->GetButtonExitApplication());
    drawable->Append(yahtzee->GetButtonLocalGame());
    drawable->Append(yahtzee->GetButtonNetworkGame());
    drawable->Append(yahtzee->GetStaticIpText());
    drawable->Append(yahtzee->GetEditBoxPlayer());
    drawable->Append(yahtzee->GetButtonOK());

    clickable->SetFocus(yahtzee->GetEditBoxPlayer());
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuNetGameEnterName::DoFrameLogic(float time_delta)
{
}

void StateMenuNetGameEnterName::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageNetGameSetupBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuNetGameEnterName::OnSpriteClicked(Graphics::SpritePtr sprite)
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
    else if(sprite == yahtzee->GetButtonOK())
    {
        Graphics::SpriteEditBox* edit_box = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxPlayer().get());
        std::string text = edit_box->GetText();
        if(!text.empty())
        {
            TransitionMenuState(Core::StatePtr(new StateMenuNetGameSetup(GetEngine(), text)));
        }
    }
}

void StateMenuNetGameEnterName::OnSpriteEnterPressed(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetEditBoxPlayer())
    {
        OnSpriteClicked(yahtzee->GetButtonOK());
        Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
        clickable->SetFocus(yahtzee->GetEditBoxPlayer());
    }
}
