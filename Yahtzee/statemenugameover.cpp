#include "stdafx.h"
#include "statemenugameover.h"
#include "../Graphics/vector.h"
#include "../Graphics/spritelistctrl.h"
#include "../Core/engine.h"
#include "yahtzee.h"
#include "statemenumain.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "yahtzeepacket.h"

StateMenuGameOver::StateMenuGameOver(Core::Engine* engine)
: LogContext("StateMenuGameOver")
, State(engine)
{
}

StateMenuGameOver::~StateMenuGameOver()
{
    LOG("Ended.");
}

void StateMenuGameOver::OnBegin()
{
    LOG("Beginning...");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    yahtzee->GetButtonOK()->SetPosition(Graphics::Vector(460.0f, 145.0f));

    Graphics::SpriteListCtrl* list_ctrl = static_cast<Graphics::SpriteListCtrl*>(yahtzee->GetListCtrlSummary().get());
    list_ctrl->DeleteAllColumns();
    list_ctrl->DeleteAllRows();
    list_ctrl->AddColumn("Name", 160);
    list_ctrl->AddColumn("Score", 96);

    PlayerContainerPtr players = yahtzee->GetPlayerContainer();
    PlayerContainer::iterator player;
    for(player = players->begin(); player != players->end(); ++player)
    {
        int row_index = list_ctrl->AddRow((*player)->GetName());
        list_ctrl->SetString(row_index, 1, (*player)->GetScorecard().GetGrandTotal());
    }

    // Sort based on the score column.
    list_ctrl->Sort(1, false);
}

void StateMenuGameOver::OnEnd()
{
}

void StateMenuGameOver::SetupLists()
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());

    Graphics::SpriteContainerPtr clickable = yahtzee->GetClickable();
    clickable->Clear();
    clickable->Append(yahtzee->GetListCtrlSummary());
    clickable->Append(yahtzee->GetButtonOK());

    Graphics::SpriteContainerPtr drawable = yahtzee->GetDrawable();
    drawable->Clear();
    drawable->Append(yahtzee->GetListCtrlSummary());
    drawable->Append(yahtzee->GetButtonOK());

    clickable->MoveFocusFirst();
    yahtzee->OnWindowMouseMove(yahtzee->GetSpriteCursor()->GetPosition());
}

void StateMenuGameOver::DoFrameLogic(float time_delta)
{
}

void StateMenuGameOver::DrawFrame(float time_delta)
{
    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    Graphics::DevicePtr gfx = yahtzee->GetGraphicsDevice();
    Graphics::ViewportManagerPtr vpm = yahtzee->GetViewportManager();

    vpm->Get(yahtzee->GetViewportID())->Set();
    gfx->Set2D();

    yahtzee->GetImageGameOverBackground()->Draw(time_delta, gfx, Graphics::Vector());
    yahtzee->GetDrawable()->Draw(time_delta, gfx, yahtzee->GetClickable()->GetFocus());
}

void StateMenuGameOver::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    LOG("Dispatching mouse click");

    Yahtzee* yahtzee = static_cast<Yahtzee*>(GetEngine());
    if(sprite == yahtzee->GetButtonOK())
    {
        TransitionMenuState(Core::StatePtr(new StateMenuMain(GetEngine())));
    }
}
