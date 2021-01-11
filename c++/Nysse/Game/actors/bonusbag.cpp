#include "bonusbag.h"
#include <math.h>

namespace Student {



Bonusbag::Bonusbag() :

    isRemoved_(false)
{
    location_ = Interface::Location();
    int x = 30 + (rand() % static_cast<int>(470 - 30 + 1));
    int y = 30 + (rand() % static_cast<int>(470 - 30 + 1));
    location_.setXY(x, y);
}

Bonusbag::~Bonusbag()
{
    
}

Interface::Location Bonusbag::giveLocation() const
{
    return location_;
}

void Bonusbag::move(Interface::Location loc)
{
    location_ = loc;
}

bool Bonusbag::isRemoved() const
{
    return isRemoved_;
}

void Bonusbag::remove()
{
    isRemoved_ = true;
}

}
