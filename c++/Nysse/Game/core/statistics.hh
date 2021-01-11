#ifndef STATISTICS_HH
#define STATISTICS_HH

#include "../Course/CourseLib/interfaces/istatistics.hh"

/**
 * @brief defines Statistics class for keeping track of events in te game
 */

namespace Student {

class Statistics : public Interface::IStatistics
{
public:
    Statistics();
    ~Statistics();

    void morePassengers(int num);
    void nysseRemoved();
    void newNysse();
    void nysseLeft();
    int getNysses();
    int getPassengers();
    int getNyssesDestroyed();

private:
    // nysses_ and passengers_ have the current total amount of nysses and
    // passengers in game, respectively
    int nysses_;
    int passengers_;

    int nyssesDestroyed_;
};

}

#endif // STATISTICS_HH
