#include "stdafx.h"
#include "statemenusettings.h"
#include "../Graphics/vector.h"
#include "../Core/engine.h"
#include "../Graphics/spritelistctrl.h"
#include "../Graphics/spritecheckbox.h"
#include "../Graphics/spriteslider.h"
#include "yahtzee.h"
#include "statemenumain.h"
#include "statemenuviewhighscores.h"
#include "statemenuplayersetup.h"
#include "statemenusettings.h"
#include "statemenuabout.h"
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iomanip>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "yahtzeepacket.h"

StateMenuSettings::StateMenuSettings(Core::Engine* engine)
: LogContext("StateMenuSettings")
, State(engine)
{
}

StateMenuSettings::~StateMenuSettings()
{
    LOG("Ended.");
}

void StateMenuSettings::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Utility::HwConfig* hw_config = yahtzee->GetHwConfig();
    Graphics::SpriteCheckBox* checkbox = static_cast<Graphics::SpriteCheckBox*>(yahtzee->GetCheckBoxFullscreenEnabled().get());
    checkbox->SetChecked(hw_config->IsFullscreen() ? Graphics::SpriteCheckBox::CHECKED : Graphics::SpriteCheckBox::UNCHECKED);

    checkbox = static_cast<Graphics::SpriteCheckBox*>(yahtzee->GetCheckBoxAudioEnabled().get());
    checkbox->SetChecked(hw_config->IsAudioEnabled() ? Graphics::SpriteCheckBox::CHECKED : Graphics::SpriteCheckBox::UNCHECKED);

    yahtzee->GetButtonApply()->Enable(false);
    yahtzee->GetSliderAudioVolume()->Enable(hw_config->IsAudioEnabled());
    yahtzee->GetButtonTest()->Enable(hw_config->IsAudioEnabled());

    Graphics::SpriteSlider* slider = static_cast<Graphics::SpriteSlider*>(yahtzee->GetSliderAudioVolume().get());
    slider->SetRange(0, 100);
    slider->SetPos(hw_config->GetVolume());
}

void StateMenuSettings::OnEnd()
{
}

void StateMenuSettings::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    clickable->Append(yahtzee->GetButtonMain());
    clickable->Append(yahtzee->GetButtonViewHighscores());
    clickable->Append(yahtzee->GetButtonSettings());
    clickable->Append(yahtzee->GetButtonAbout());
    clickable->Append(yahtzee->GetButtonExitApplication());
    clickable->Append(yahtzee->GetCheckBoxFullscreenEnabled());
    clickable->Append(yahtzee->GetButtonApply());
    clickable->Append(yahtzee->GetCheckBoxAudioEnabled());
    clickable->Append(yahtzee->GetSliderAudioVolume());
    clickable->Append(yahtzee->GetButtonTest());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetButtonMain());
    drawable->Append(yahtzee->GetButtonViewHighscores());
    drawable->Append(yahtzee->GetButtonSettings());
    drawable->Append(yahtzee->GetButtonAbout());
    drawable->Append(yahtzee->GetButtonExitApplication());
    drawable->Append(yahtzee->GetCheckBoxFullscreenEnabled());
    drawable->Append(yahtzee->GetButtonApply());
    drawable->Append(yahtzee->GetCheckBoxAudioEnabled());
    drawable->Append(yahtzee->GetSliderAudioVolume());
    drawable->Append(yahtzee->GetButtonTest());

    clickable->SetFocus(yahtzee->GetButtonSettings());
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuSettings::DoFrameLogic(float time_delta)
{
}

void StateMenuSettings::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageSettingsBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuSettings::OnSpriteClicked(Graphics::SpritePtr sprite)
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
    else if(sprite == yahtzee->GetCheckBoxFullscreenEnabled())
    {
        Graphics::SpriteCheckBox* checkbox = static_cast<Graphics::SpriteCheckBox*>(yahtzee->GetCheckBoxFullscreenEnabled().get());
        Utility::HwConfig* hw_config = yahtzee->GetHwConfig();
        yahtzee->GetButtonApply()->Enable(hw_config->IsFullscreen() != checkbox->IsChecked());
        hw_config->SetFullscreen(checkbox->IsChecked());
        LOG("Screen mode changed successfully");
    }
    else if(sprite == yahtzee->GetButtonApply())
    {
        Graphics::SpriteCheckBox* checkbox = static_cast<Graphics::SpriteCheckBox*>(yahtzee->GetCheckBoxFullscreenEnabled().get());
        checkbox->IsChecked() ? yahtzee->SetFullscreen() : yahtzee->SetWindowed();
    }
    else if(sprite == yahtzee->GetButtonTest())
    {
        // Do nothing!
    }
    else if(sprite == yahtzee->GetCheckBoxAudioEnabled())
    {
        Graphics::SpriteCheckBox* checkbox = static_cast<Graphics::SpriteCheckBox*>(yahtzee->GetCheckBoxAudioEnabled().get());
        Utility::HwConfig* hw_config = yahtzee->GetHwConfig();
        hw_config->SetAudioEnabled(checkbox->IsChecked());

        yahtzee->GetSliderAudioVolume()->Enable(hw_config->IsAudioEnabled());
        yahtzee->GetButtonTest()->Enable(hw_config->IsAudioEnabled());
        yahtzee->GetAudioDevice()->Enable(hw_config->IsAudioEnabled());
    }
}

void StateMenuSettings::OnSpriteSliderMoved(Graphics::SpritePtr sprite)
{
    LOG("Dispatching slider move");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetSliderAudioVolume())
    {
        Graphics::SpriteSlider* slider = static_cast<Graphics::SpriteSlider*>(yahtzee->GetSliderAudioVolume().get());
        Utility::HwConfig* hw_config = yahtzee->GetHwConfig();
        hw_config->SetVolume(slider->GetPos());
        yahtzee->SetVolume(slider->GetPos());
    }
}

void StateMenuSettings::OnSpriteSliderReleased(Graphics::SpritePtr sprite)
{
    LOG("Dispatching slider released");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetSliderAudioVolume())
    {
        Graphics::SpriteSlider* slider = static_cast<Graphics::SpriteSlider*>(yahtzee->GetSliderAudioVolume().get());
        Utility::HwConfig* hw_config = yahtzee->GetHwConfig();
        hw_config->SetVolume(slider->GetPos());
        yahtzee->SetVolume(slider->GetPos());
    }
}
