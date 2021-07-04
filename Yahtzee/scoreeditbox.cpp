#include "stdafx.h"
#include "scoreeditbox.h"
#include "yahtzee.h"

ScoreEditBox::ScoreEditBox(Graphics::SpriteEventHandler* event_handler, Yahtzee* yahtzee)
: SpriteEditBox(event_handler)
, yahtzee_(yahtzee)
{
}

void ScoreEditBox::OnMouseEnter(Graphics::SpritePtr sprite_cursor, Graphics::SpriteContainerPtr sprites_clickable)
{
    if(IsEnabled())
    {
        yahtzee_->SetCursor(yahtzee_->GetCursorPlaceScore());
    }
}

void ScoreEditBox::OnMouseLeave(Graphics::SpritePtr sprite_cursor, Graphics::SpriteContainerPtr sprites_clickable)
{
    yahtzee_->SetCursor(yahtzee_->GetCursorNormal());
}
