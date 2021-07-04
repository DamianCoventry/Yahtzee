#include "stdafx.h"
#include "sprite.h"
#include "device.h"

using namespace Graphics;

Sprite::Sprite(SpriteEventHandler* event_handler)
: event_handler_(event_handler)
, enabled_(true)
{
}

Sprite::~Sprite()
{
}

void Sprite::Draw(float time_delta, DevicePtr device, SpritePtr sprite_focus)
{
}

bool Sprite::IsPointColliding(const Vector& point) const
{
    if(point.x_ < position_.x_ + rect_min_.x_) return false;
    if(point.x_ > position_.x_ + rect_max_.x_) return false;
    if(point.y_ < position_.y_ + rect_min_.y_) return false;
    if(point.y_ > position_.y_ + rect_max_.y_) return false;
    return true;
}
