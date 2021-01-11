#ifndef BONUSBAG_H
#define BONUSBAG_H

#include "../Course/CourseLib/interfaces/iactor.hh"
#include "../Course/CourseLib/core/location.hh"
/**
 * @brief defines a bonus item actor for the game, collecting which awards
 *        player with extra points
 */
namespace Student {

class Bonusbag : public Interface::IActor
{

public:
    //IActor interface
    Bonusbag();
    ~Bonusbag();

    Interface::Location giveLocation() const;

    void move(Interface::Location loc);

    bool isRemoved() const;

    void remove();

private:
    Interface::Location location_;
    bool isRemoved_;


};
}
#endif // BONUSBAG_H
