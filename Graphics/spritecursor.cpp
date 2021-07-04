#include "stdafx.h"
#include "spritecursor.h"
#include "spritecontainer.h"
#include "imagelist.h"
#include "device.h"
#include "color.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

using namespace Graphics;

SpriteCursor::SpriteCursor(SpriteEventHandler* event_handler)
: LogContext("Graphics::SpriteCursor")
, Sprite(event_handler)
, animate_counter_(0.0f)
, animate_threshold_(1.0f/15.0f)
, direction_(true)
, direction_change_(float(1500+((rand()%1000)-500)))
, direction_counter_(0.0f)
{
    LOG("Creating...");
}

SpriteCursor::~SpriteCursor()
{
    LOG("Deleting...");
}

void SpriteCursor::Think(float time_delta)
{
    if(image_list_)
    {
        animate_counter_ += time_delta;
        if(animate_counter_ >= animate_threshold_)
        {
            animate_counter_ = 0.0f;
            direction_ ? image_list_->IncCurrentFrame() : image_list_->DecCurrentFrame();
        }

        direction_counter_ += time_delta;
        if(direction_counter_ >= direction_change_)
        {
            direction_counter_ = 0.0f;
            direction_change_ = float(1500+((rand()%1000)-500));
            direction_ = !direction_;
        }
    }
}

void SpriteCursor::Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus)
{
    if(image_list_)
    {
        image_list_->Draw(time_delta, device, GetPosition());
    }
}
