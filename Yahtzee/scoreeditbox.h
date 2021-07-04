#ifndef INCLUDED_SCOREEDITBOX
#define INCLUDED_SCOREEDITBOX

#include "../Graphics/spriteeditbox.h"

class Yahtzee;

class ScoreEditBox :
    public Graphics::SpriteEditBox

{
public:
    ScoreEditBox(Graphics::SpriteEventHandler* event_handler, Yahtzee* yahtzee);

    void OnMouseEnter(Graphics::SpritePtr sprite_cursor, Graphics::SpriteContainerPtr sprites_clickable);
    void OnMouseLeave(Graphics::SpritePtr sprite_cursor, Graphics::SpriteContainerPtr sprites_clickable);

private:
    Yahtzee* yahtzee_;
};

#endif  // INCLUDED_SCOREEDITBOX
