#include "stdafx.h"
#include "statemenuviewhighscores.h"
#include "../Graphics/vector.h"
#include "../Core/engine.h"
#include "../Graphics/spritelistctrl.h"
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

StateMenuViewHighscores::StateMenuViewHighscores(Core::Engine* engine)
: LogContext("StateMenuViewHighscores")
, State(engine)
{
}

StateMenuViewHighscores::~StateMenuViewHighscores()
{
    LOG("Ended.");
}

void StateMenuViewHighscores::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteListCtrl* listctrl = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlHighscores().get());
    listctrl->SetPosition(Graphics::Vector(10.0f, 140.0f));

    InsertHighscores();
}

void StateMenuViewHighscores::OnEnd()
{
}

void StateMenuViewHighscores::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    clickable->Append(yahtzee->GetButtonMain());
    clickable->Append(yahtzee->GetButtonViewHighscores());
    clickable->Append(yahtzee->GetButtonSettings());
    clickable->Append(yahtzee->GetButtonAbout());
    clickable->Append(yahtzee->GetButtonExitApplication());
    clickable->Append(yahtzee->GetListCtrlHighscores());
    clickable->Append(yahtzee->GetButtonResetHighscores());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetButtonMain());
    drawable->Append(yahtzee->GetButtonViewHighscores());
    drawable->Append(yahtzee->GetButtonSettings());
    drawable->Append(yahtzee->GetButtonAbout());
    drawable->Append(yahtzee->GetButtonExitApplication());
    drawable->Append(yahtzee->GetListCtrlHighscores());
    drawable->Append(yahtzee->GetButtonResetHighscores());

    clickable->SetFocus(yahtzee->GetButtonViewHighscores());
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuViewHighscores::DoFrameLogic(float time_delta)
{
}

void StateMenuViewHighscores::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageHighscoresBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuViewHighscores::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetButtonResetHighscores())
    {
        HighscoresPtr highscores = yahtzee->GetHighscores();
        highscores->Reset();
        InsertHighscores();
    }
    else if(sprite == yahtzee->GetButtonMain())
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

void StateMenuViewHighscores::InsertHighscores()
{
    Yahtzee*                    yahtzee     = static_cast<Yahtzee*>(GetEngine());
    Graphics::SpriteListCtrl*   list_ctrl   = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlHighscores().get());

    list_ctrl->DeleteAllColumns();
    list_ctrl->DeleteAllRows();

    list_ctrl->AddColumn("Name", 170);
    list_ctrl->AddColumn("Score", 100);
    list_ctrl->AddColumn("Date/Time", 200);

    const std::string months[12] =
    { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

    HighscoresPtr highscores = yahtzee->GetHighscores();
    for(int count = 0; count < Highscores::NUM_HIGHSCORES; count++)
    {
        int row = list_ctrl->AddRow(highscores->GetName(count));
        list_ctrl->SetString(row, 1, boost::lexical_cast<std::string>(highscores->GetScore(count)));

        SYSTEMTIME date_time = highscores->GetDateTime(count);
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << date_time.wHour << ":"
            << std::setw(2) << std::setfill('0') << date_time.wMinute << ":"
            << std::setw(2) << std::setfill('0') << date_time.wSecond << ", "
            << date_time.wDay << " " << months[date_time.wMonth-1] << " " << date_time.wYear;
        list_ctrl->SetString(row, 2, oss.str());

        list_ctrl->SetRowColor(row, Graphics::Color(0.0f, 0.0f, 0.0f));
    }
}
