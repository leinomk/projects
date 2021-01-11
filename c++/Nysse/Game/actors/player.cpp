#include "player.hh"

namespace Student {

Player::Player() :
    isRemoved_(false)
{
    location_ = Interface::Location();
    location_.setXY(250, 250);
}

Player::~Player()
{

}

Interface::Location Player::giveLocation() const
{
    return location_;
}

void Player::move(Interface::Location loc)
{
    location_ = loc;
}

bool Player::isRemoved() const
{
    return isRemoved_;
}

void Player::remove()
{
    isRemoved_ = true;
}

}
