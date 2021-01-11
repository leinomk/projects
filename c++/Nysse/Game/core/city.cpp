#include "city.hh"

#include <algorithm>
#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <QKeyEvent>

namespace Student {

City::City() :
    actors_(),
    stops_(),
    time_(),
    gameStarted_(false)
{

    window = new Student::MainWindow();

    QImage basicbackground(":/offlinedata/offlinedata/kartta_pieni_500x500.png");
    QImage bigbackground(":/offlinedata/offlinedata/kartta_iso_1095x592.png");

    setBackground(basicbackground, bigbackground);


    City::grabKeyboard();
    ptr_ = new Statistics;

}

City::~City()
{

}


void City::setBackground(QImage &basicbackground, QImage& bigbackground)
{
    window->setPicture(basicbackground);
}

void City::setClock(QTime clock)
{
    time_.setHMS(clock.hour(), clock.minute(), clock.second());
    if ( gameStarted_ ) {
        ++roundHasLasted_;
        if (roundHasLasted_ >= roundLength_){
            gameStarted_ = false;
        }
    }

}

void City::addStop(std::shared_ptr<Interface::IStop> stop)
{
    window->addStop(stop);
    stops_.push_back(stop);
}

void City::startGame()
{
    player_ = std::make_shared<Student::Player>();
    window->addPlayer(player_);
    bonusBag_ = std::make_shared<Student::Bonusbag>();
    window->addBonusbag(bonusBag_);

    window->show();
    gameStarted_ = true;
}

void City::addActor(std::shared_ptr<Interface::IActor> newactor)
{
    if ( std::dynamic_pointer_cast<CourseSide::Nysse>(newactor) ){
        window->addActor(newactor);
        ptr_->newNysse();
    }
    else {
        ptr_->morePassengers(1);
    }
    actors_.push_back(newactor);
}

void City::removeActor(std::shared_ptr<Interface::IActor> actor)
{
    if ( std::dynamic_pointer_cast<CourseSide::Nysse>(actor) ){
        window->removeActor(actor);
        ptr_->nysseLeft();
    }
    actors_.remove(actor);
}

void City::actorRemoved(std::shared_ptr<Interface::IActor> actor)
{
    if ( std::dynamic_pointer_cast<CourseSide::Nysse>(actor) ){
        window->removeActor(actor);
        ptr_->nysseRemoved();
        window->addPoints(10);
    } else {
        window->addPoints(2);
    }
}

bool City::findActor(std::shared_ptr<Interface::IActor> actor) const
{
    auto it = std::find(actors_.begin(), actors_.end(), actor);

    return it != actors_.end();
}

void City::actorMoved(std::shared_ptr<Interface::IActor> actor)
{
    if ( std::dynamic_pointer_cast<CourseSide::Nysse>(actor) ){
        window->updateCoords(actor);
    }
}

void City::keyPressEvent(QKeyEvent *event)
{
    if (gameStarted_){
        if (event->key() == Qt::Key_A){
            window->changeDirection('a');
        }
        else if (event->key() == Qt::Key_D){
            window->changeDirection('d');
        }
        else if (event->key() == Qt::Key_W){
            window->changeDirection('w');
        }
        else if (event->key() == Qt::Key_S){
            window->changeDirection('s');
        }
        else if (event->key() == Qt::Key_Space){
            window->addBomb();
            ++bombsUsed_;

            std::vector<std::shared_ptr<Interface::IActor>> nearbyActors;
            nearbyActors = getNearbyActors(player_->giveLocation());

            for (std::shared_ptr<Interface::IActor> actor : nearbyActors) {
                actor->remove();
                actorRemoved(actor);
            }

            if (bombsUsed_ >= amOfBombs_){
                gameStarted_ = false;
            }
        }
    }
}

std::vector<std::shared_ptr<Interface::IActor> > City::getNearbyActors(Interface::Location loc) const
{
    std::vector<std::shared_ptr<Interface::IActor>> nearbyActors;

    for (auto const& actorIter : actors_) {
        if ( actorIter->giveLocation().isClose(loc)) {
            nearbyActors.push_back(actorIter);
        }
    }

    return nearbyActors;
}

bool City::isGameOver() const
{
    if ( roundHasLasted_ >= roundLength_ || bombsUsed_ >= amOfBombs_ ) {
        if (window->isBonusCollected) {
            window->addPoints(100);
        }
        window->roundOver();
        return 1;
    }

    return 0;
}

}
