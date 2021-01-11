#include "statistics.hh"

namespace Student {

Statistics::Statistics() : nysses_(0),
    passengers_(0), nyssesDestroyed_(0)
{

}

Statistics::~Statistics()
{

}

void Statistics::morePassengers(int num)
{
    passengers_ += num;
}

void Statistics::nysseRemoved()
{
    nyssesDestroyed_ += 1;

}

void Statistics::newNysse()
{
    nysses_ += 1;
}

void Statistics::nysseLeft()
{
    nysses_ -= 1;
}

int Statistics::getNysses()
{
    return nysses_;
}

int Statistics::getPassengers()
{
    return passengers_;
}

int Statistics::getNyssesDestroyed()
{
    return nyssesDestroyed_;
}

}
