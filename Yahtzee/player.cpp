#include "stdafx.h"
#include "player.h"

Player::Player(const std::string& name)
: name_(name)
, remote_(false)
{
}

Player::~Player()
{
}

void Player::SetName(const std::string& name)
{
    name_ = name;
}

void Player::SetRemote(bool remote)
{
    remote_ = remote;
}
