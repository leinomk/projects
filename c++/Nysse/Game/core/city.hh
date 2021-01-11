#ifndef CITY_HH
#define CITY_HH

#include "../Course/CourseLib/interfaces/icity.hh"
#include "../Course/CourseLib/interfaces/iactor.hh"
#include "../Course/CourseLib/actors/nysse.hh"
#include "../Course/CourseLib/actors/passenger.hh"
#include "../Course/CourseLib/actors/stop.hh"
#include "./graphics/mainwindow.hh"
#include "./actors/player.hh"
#include "statistics.hh"
#include "./actors/bonusbag.h"

#include <list>
#include <memory>
#include <QTime>
#include <QTimer>

/**
 * @brief defines the city for the game
 */

namespace Student {


class City : public Interface::ICity, public QWidget
{

public:
    //ICity interface
    City();

    ~City();

    void setBackground(QImage& basicbackground, QImage& bigbackground);

    void setClock(QTime clock);

    void addStop(std::shared_ptr<Interface::IStop> stop);

    void startGame();

    void addActor(std::shared_ptr<Interface::IActor> newactor);

    void removeActor(std::shared_ptr<Interface::IActor> actor);

    void actorRemoved(std::shared_ptr<Interface::IActor> actor);

    bool findActor(std::shared_ptr<Interface::IActor> actor) const;

    void actorMoved(std::shared_ptr<Interface::IActor> actor);

    std::vector<std::shared_ptr<Interface::IActor>> getNearbyActors(Interface::Location loc) const;

    bool isGameOver() const;

    //Game window and event handler for moving the player on the screen
    Student::MainWindow* window;
    void keyPressEvent(QKeyEvent* event);

private:

    std::list< std::shared_ptr<Interface::IActor> > actors_;
    std::vector< std::shared_ptr<Interface::IStop> > stops_;

    std::shared_ptr<Student::Player> player_;
    std::shared_ptr<Student::Bonusbag> bonusBag_;


    // Current time
    QTime time_;

    //Maximum round length, in seconds
    int roundLength_ = 120;

    int roundHasLasted_ = 0;

    //Amount of bombs user can drop in a round
    int amOfBombs_ = 20;

    int bombsUsed_ = 0;

    bool gameStarted_;

    //Statistics class keeping track of events in game
    Statistics *ptr_;

};

}
#endif // CITY_HH
