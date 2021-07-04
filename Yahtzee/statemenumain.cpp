#include "stdafx.h"
#include "statemenumain.h"
#include "../Graphics/vector.h"
#include "../Graphics/color.h"
#include "../Graphics/spriteeditbox.h"
#include "../Core/engine.h"
#include "yahtzee.h"
#include "statemenuviewhighscores.h"
#include "statemenuplayersetup.h"
#include "statemenusettings.h"
#include "statemenuabout.h"
#include "statemenunetgameentername.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "yahtzeepacket.h"

StateMenuMain::StateMenuMain(Core::Engine* engine)
: LogContext("StateMenuMain")
, State(engine)
{
}

StateMenuMain::~StateMenuMain()
{
    LOG("Ended.");
}

void StateMenuMain::OnBegin()
{
    LOG("Beginning...");

    ResetEditBoxes();
}

void StateMenuMain::OnEnd()
{
}

void StateMenuMain::SetupLists()
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

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetButtonMain());
    drawable->Append(yahtzee->GetButtonViewHighscores());
    drawable->Append(yahtzee->GetButtonSettings());
    drawable->Append(yahtzee->GetButtonAbout());
    drawable->Append(yahtzee->GetButtonExitApplication());
    drawable->Append(yahtzee->GetButtonLocalGame());
    drawable->Append(yahtzee->GetButtonNetworkGame());

    clickable->SetFocus(yahtzee->GetButtonMain());
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuMain::DoFrameLogic(float time_delta)
{
}

void StateMenuMain::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageMainBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuMain::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetButtonLocalGame())
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
}

void StateMenuMain::ResetEditBoxes()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteEditBox* editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxOnes().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxTwos().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxThrees().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFours().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFives().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxSixes().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxSubTotal().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxBonus().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxUpperTotal().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxThreeOfAKind().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFourOfAKind().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxFullHouse().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxSmallStraight().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxLargeStraight().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxYahtzee().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxChance().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxLowerTotal().get());
    editbox->SetText(std::string());
    editbox = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxGrandTotal().get());
    editbox->SetText(std::string());
}
