#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "bomb.hh"

#include <QDebug>
#include <QKeyEvent>
#include <QLCDNumber>
#include <QMediaPlayer>

const int PADDING = 10;

namespace Student {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gameView->setFixedSize(width_, width_);
    ui->centralwidget->setFixedSize(width_ + ui->endButton->width() + PADDING, height_ + PADDING);

    startDialog = new Student::StartDialog();
    connect(startDialog, &Student::StartDialog::beginClicked,
            this, &MainWindow::initGameDiff);

    //disable scroll bars
    ui->gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->endButton->move(width_ + PADDING , PADDING);
    ui->pointsLabel->move(width_ + 2 * PADDING, 5 * PADDING);
    ui->pointsView->move(width_ + PADDING, (7 * PADDING));

    ui->textView->move(PADDING + 145, PADDING + 150);
    ui->textView->hide();

    points_ = 0;
    ui->pointsView->display(points_);

    map = new QGraphicsScene(this);
    ui->gameView->setScene(map);
    map->setSceneRect(0,0,width_,height_);

    resize(minimumSizeHint());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, map, &QGraphicsScene::advance);

    music_ = new QMediaPlayer();
    music_->setMedia(QUrl("qrc:/sounds/Intense DuBstep song.mp3"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSize(int w, int h)
{
    width_ = w;
    height_ = h;
}

void MainWindow::setTick(int t)
{
    tick_ = t;
}

void MainWindow::addActor(std::shared_ptr<Interface::IActor> actor)
{
    int locX = actor->giveLocation().giveX();
    int locY = actor->giveLocation().giveY();
    Student::ActorItem* nActor = new Student::ActorItem(locX, 500 - locY, 1);
    actors_[actor] = nActor;
    map->addItem(nActor);
    last_ = nActor;
}

void MainWindow::addStop(std::shared_ptr<Interface::IStop> stop)
{
    int locX = stop->getLocation().giveX();
    int locY = stop->getLocation().giveY();
    Student::ActorItem* nActor = new Student::ActorItem(locX, 500 - locY, 2);
    stops_[stop] = nActor;
    map->addItem(nActor);
}


void MainWindow::addPlayer(std::shared_ptr<Player> player)
{
    player_ = player;
    int locX = player->giveLocation().giveX() - 53;
    int locY = player->giveLocation().giveY() - 43;
    playerItem_ = new Student::UserGraphicsItem();
    playerItem_->setPos(locX, locY);
    playerItem_->setFlag(QGraphicsItem::ItemIsFocusable);
    playerItem_->setFocus();
    map->addItem(playerItem_);
    connect(timer, &QTimer::timeout, this, &MainWindow::movePlayer);
}

void MainWindow::addBonusbag(std::shared_ptr<Bonusbag> bonusBag)
{
    bonusBag_ = bonusBag;
    int locX = bonusBag->giveLocation().giveX() - 15;
    int locY = bonusBag->giveLocation().giveY() - 15;
    bonusBagItem_ = new Student::BonusBagItem();
    bonusBagItem_->setPos(locX, locY);
    map->addItem(bonusBagItem_);
}


void MainWindow::removeActor(std::shared_ptr<Interface::IActor> actorToRm)
{
    if (actors_.find(actorToRm) != actors_.end()){
        map->removeItem(actors_.at(actorToRm));
        actors_.erase(actorToRm);
    }
}

void MainWindow::updateCoords(std::shared_ptr<Interface::IActor> actor)
{
    int locX = actor->giveLocation().giveX();
    int locY = actor->giveLocation().giveY();
    actors_[actor]->setCoord(locX - 20, 480 - locY);
}

void MainWindow::setPicture(QImage &img)
{
    map->setBackgroundBrush(img);
}

void MainWindow::changeDirection(char direction)
{
    if (direction == 'a'){
        playerItem_->setDir('a');
        bonusBagItem_->setDir('a');
    }
    else if (direction == 'd'){
        playerItem_->setDir('d');
        bonusBagItem_->setDir('d');
    }
    else if (direction == 'w'){
        playerItem_->setDir('w');
        bonusBagItem_->setDir('w');
    }
    else if (direction == 's'){
        playerItem_->setDir('s');
        bonusBagItem_->setDir('s');
    }
}

void MainWindow::addBomb()
{
    Student::Bomb* bomb = new Student::Bomb();
    bomb->setPos(playerItem_->giveX() + 36, playerItem_->giveY() + 26);
    map->addItem(bomb);
}

void MainWindow::addPoints(int points)
{
    points_ += points;

    ui->pointsView->display(points_);
}

void MainWindow::roundOver()
{
    timer->stop();
    ui->textView->setText("GAME OVER");
    ui->textView->show();
}

}


void Student::MainWindow::movePlayer()
{
    if(playerItem_->move()){
        Interface::Location newLoc = player_->giveLocation();
        newLoc.setXY(playerItem_->giveX() + 53, 500 - playerItem_->giveY() - 43);
        player_->move(newLoc);
    }

    if (!isBonusCollected){
        // move bonus item to a new location, and then check if user is close
        // enough to collect it
        if(bonusBagItem_->move()){
            Interface::Location newLoc = bonusBag_->giveLocation();
            newLoc.setXY(bonusBagItem_->giveX() + 15, 500 - bonusBagItem_->giveY() - 15);
            bonusBag_->move(newLoc);
        }
        if (abs(bonusBagItem_->giveX()-playerItem_->giveX()) <= 6
                and abs(bonusBagItem_->giveY()-playerItem_->giveY()) <= 6) {
            isBonusCollected = true;
            map->removeItem(bonusBagItem_);
            delete bonusBagItem_;
            }

    }
}

void Student::MainWindow::initGameDiff(int userInput)
{
    int difficultyFactor = 5 / userInput;
    tick_ = 10 * difficultyFactor;
    timer->start(tick_);
    music_->play();

}
