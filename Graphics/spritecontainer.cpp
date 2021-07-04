#include "stdafx.h"
#include "spritecontainer.h"
#include "sprite.h"
#include <algorithm>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

using namespace Graphics;

SpriteContainer::SpriteContainer()
: LogContext("Graphics::SpriteContainer")
{
    LOG("Creating...");
}

SpriteContainer::~SpriteContainer()
{
    LOG("Deleting...");
}

void SpriteContainer::Prepend(SpritePtr sprite, bool allow_duplicates)
{
    if(allow_duplicates)
    {
        sprites_.push_front(sprite);
    }
    else
    {
        Sprites::iterator itor = std::find(sprites_.begin(), sprites_.end(), sprite);
        if(itor == sprites_.end())
        {
            sprites_.push_front(sprite);
        }
    }
}

void SpriteContainer::Append(SpritePtr sprite, bool allow_duplicates)
{
    if(allow_duplicates)
    {
        sprites_.push_back(sprite);
    }
    else
    {
        Sprites::iterator itor = std::find(sprites_.begin(), sprites_.end(), sprite);
        if(itor == sprites_.end())
        {
            sprites_.push_back(sprite);
        }
    }
}

void SpriteContainer::Remove(SpritePtr sprite)
{
    Sprites::iterator itor = std::find(sprites_.begin(), sprites_.end(), sprite);
    if(itor != sprites_.end())
    {
        if(sprite_focus_ == *itor)
        {
            sprite_focus_.reset();
        }
        sprites_.erase(itor);
    }
}

void SpriteContainer::Replace(SpritePtr sprite_old, SpritePtr sprite_new)
{
    Sprites::iterator itor = std::find(sprites_.begin(), sprites_.end(), sprite_old);
    if(itor != sprites_.end())
    {
        itor = sprites_.erase(itor);
        sprites_.insert(itor, sprite_new);
    }
}

void SpriteContainer::Clear()
{
    sprites_.clear();
    sprite_focus_.reset();
}

void SpriteContainer::Think(float time_delta)
{
    Sprites::iterator itor;
    for(itor = sprites_.begin(); itor != sprites_.end(); ++itor)
    {
        (*itor)->Think(time_delta);
    }
}

void SpriteContainer::Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus)
{
    Sprites::iterator itor;
    for(itor = sprites_.begin(); itor != sprites_.end(); ++itor)
    {
        (*itor)->Draw(time_delta, device, sprite_focus);
    }
}

bool SpriteContainer::IsEmpty() const
{
    return sprites_.empty();
}

bool SpriteContainer::IsPresent(SpritePtr sprite) const
{
    Sprites::const_iterator itor = std::find(sprites_.begin(), sprites_.end(), sprite);
    return (itor != sprites_.end());
}

void SpriteContainer::SetFocus(SpritePtr sprite)
{
    if(sprite_focus_)
    {
        if(sprite_focus_ != sprite)
        {
            sprite_focus_->OnLoseFocus(sprite, shared_from_this());
            sprite_focus_->GetSpriteEventHandler()->OnSpriteLoseFocus(sprite_focus_);

            if(sprite)
            {
                sprite->OnGetFocus(sprite_focus_, shared_from_this());
                sprite_focus_ = sprite;
                sprite_focus_->GetSpriteEventHandler()->OnSpriteGetFocus(sprite_focus_);
            }
            else
            {
                sprite_focus_ = sprite;
            }
        }
    }
    else
    {
        sprite_focus_ = sprite;
        if(sprite_focus_)
        {
            sprite_focus_->OnGetFocus(SpritePtr(), shared_from_this());
            sprite_focus_->GetSpriteEventHandler()->OnSpriteGetFocus(sprite_focus_);
        }
    }
}

void SpriteContainer::MoveFocusFirst()
{
    SetFocus(GetFirstFocusableSprite());
}

void SpriteContainer::MoveFocusLast()
{
    SetFocus(GetLastFocusableSprite());
}

void SpriteContainer::MoveFocusNext()
{
    if(!sprite_focus_)
    {
        if(sprites_.empty()) return;
        SetFocus(GetFirstFocusableSprite());
        return;
    }

    // Get an iterator that points to sprite with the focus.
    Sprites::iterator itor = std::find(sprites_.begin(), sprites_.end(), sprite_focus_);
    if(itor == sprites_.end())
    {
        if(sprites_.empty()) return;
        SetFocus(sprites_.front());
        return;
    }

    // Move to the next focusable sprite.
    bool done = false;
    while(!done)
    {
        ++itor;
        if(itor == sprites_.end())
        {
            itor = sprites_.begin();
        }
        if(*itor == sprite_focus_)
        {
            done = true;
        }
        else if((*itor)->IsFocusable() && (*itor)->IsEnabled())
        {
            SetFocus(*itor);
            done = true;
        }
    }
}

void SpriteContainer::MoveFocusPrev()
{
    if(!sprite_focus_)
    {
        if(sprites_.empty()) return;
        SetFocus(GetLastFocusableSprite());
        return;
    }

    // Get an iterator that points to sprite with the focus.
    Sprites::reverse_iterator itor = std::find(sprites_.rbegin(), sprites_.rend(), sprite_focus_);
    if(itor == sprites_.rend())
    {
        if(sprites_.empty()) return;
        SetFocus(sprites_.front());
        return;
    }

    // Move to the next focusable sprite.
    bool done = false;
    while(!done)
    {
        ++itor;
        if(itor == sprites_.rend())
        {
            itor = sprites_.rbegin();
        }
        if(*itor == sprite_focus_)
        {
            done = true;
        }
        else if((*itor)->IsFocusable() && (*itor)->IsEnabled())
        {
            SetFocus(*itor);
            done = true;
        }
    }
}

void SpriteContainer::SetCapture(SpritePtr sprite)
{
    if(sprite_capture_)
    {
        sprite_capture_->OnCaptureStolen(sprite, shared_from_this());
    }
    sprite_capture_ = sprite;
}

void SpriteContainer::ReleaseCapture()
{
    sprite_capture_.reset();
}

SpritePtr SpriteContainer::GetFirstFocusableSprite() const
{
    Sprites::const_iterator itor;
    for(itor = sprites_.begin(); itor != sprites_.end(); ++itor)
    {
        if((*itor)->IsFocusable() && (*itor)->IsEnabled()) return *itor;
    }
    return SpritePtr();
}

SpritePtr SpriteContainer::GetLastFocusableSprite() const
{
    Sprites::const_reverse_iterator itor;
    for(itor = sprites_.rbegin(); itor != sprites_.rend(); ++itor)
    {
        if((*itor)->IsFocusable() && (*itor)->IsEnabled()) return *itor;
    }
    return SpritePtr();
}
