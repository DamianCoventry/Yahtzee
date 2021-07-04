#include "stdafx.h"
#include "statemenuenterhighscore.h"
#include "../Graphics/vector.h"
#include "../Core/engine.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritelistctrl.h"
#include "yahtzee.h"
#include "statemenugameover.h"
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Audio/sound.h"
#include "yahtzeepacket.h"

StateMenuEnterHighscore::StateMenuEnterHighscore(Core::Engine* engine)
: LogContext("StateMenuEnterHighscore")
, State(engine)
{
}

StateMenuEnterHighscore::~StateMenuEnterHighscore()
{
    LOG("Ended.");
}

void StateMenuEnterHighscore::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteListCtrl* listctrl = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlHighscores().get());
    listctrl->SetPosition(Graphics::Vector(10.0f, 120.0f));

    current_player_ = yahtzee->GetFirstHighscoringPlayer();
    if(!current_player_)
    {
        throw std::logic_error("No player has a highscore, shouldn't have got here.");
    }

    HighscoresPtr highscores = yahtzee->GetHighscores();
    current_player_row_ = highscores->Insert(current_player_->GetName(), current_player_->GetScorecard().GetGrandTotal_i());
    InsertHighscores();

    Graphics::SpriteEditBox* edit_box = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxHighscore().get());
    edit_box->SetText(current_player_->GetName());

    yahtzee->GetApplauseSound()->Play();
}

void StateMenuEnterHighscore::OnEnd()
{
}

void StateMenuEnterHighscore::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    clickable->Append(yahtzee->GetListCtrlHighscores());
    clickable->Append(yahtzee->GetEditBoxHighscore());
    clickable->Append(yahtzee->GetButtonSaveHighscore());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetListCtrlHighscores());
    drawable->Append(yahtzee->GetEditBoxHighscore());
    drawable->Append(yahtzee->GetButtonSaveHighscore());

    clickable->SetFocus(yahtzee->GetEditBoxHighscore());
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuEnterHighscore::DoFrameLogic(float time_delta)
{
}

void StateMenuEnterHighscore::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageEnterScoreBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuEnterHighscore::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee*                    yahtzee     = static_cast<Yahtzee*>(GetEngine());
    Graphics::SpriteEditBox*    edit_box    = static_cast<Graphics::SpriteEditBox*>(yahtzee->GetEditBoxHighscore().get());
    Graphics::SpriteListCtrl*   list_ctrl   = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlHighscores().get());
    HighscoresPtr               highscores  = yahtzee->GetHighscores();

    if(sprite == yahtzee->GetButtonSaveHighscore())
    {
        if(!edit_box->GetText().empty())
        {
            // Set the name in the highscore table and change the GUI to paint this
            // highscore's row in black.
            highscores->SetName(current_player_row_, edit_box->GetText());

            // If there's another player with a highscore insert their score, otherwise
            // replace the Save and Cancel buttons with an OK button.
            current_player_ = yahtzee->GetNextHighscoringPlayer(current_player_);
            if(current_player_)
            {
                current_player_row_ = highscores->Insert(current_player_->GetName(), current_player_->GetScorecard().GetGrandTotal_i());
                edit_box->SetText(current_player_->GetName());
                yahtzee->GetClickable()->SetFocus(yahtzee->GetEditBoxHighscore());
            }
            else
            {
                current_player_row_ = -1;

                Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
                clickable->Clear();
                clickable->Append(yahtzee->GetListCtrlHighscores());
                clickable->Append(yahtzee->GetButtonContinue());

                Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
                drawable->Clear();
                drawable->Append(yahtzee->GetListCtrlHighscores());
                drawable->Append(yahtzee->GetButtonContinue());

                clickable->SetFocus(yahtzee->GetListCtrlHighscores());
            }

            InsertHighscores();
        }
    }
    else if(sprite == yahtzee->GetButtonContinue())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuGameOver(GetEngine())));
    }
}

void StateMenuEnterHighscore::OnSpriteEnterPressed(Graphics::SpritePtr sprite)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetEditBoxHighscore())
    {
        OnSpriteClicked(yahtzee->GetButtonSaveHighscore());
        Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
        clickable->SetFocus(yahtzee->GetEditBoxHighscore());
    }
}

void StateMenuEnterHighscore::InsertHighscores()
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

        row == current_player_row_ ?
            list_ctrl->SetRowColor(row, Graphics::Color(1.0f, 0.0f, 0.0f)) :
            list_ctrl->SetRowColor(row, Graphics::Color(0.0f, 0.0f, 0.0f));
    }
}
