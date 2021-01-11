#include "bonusbagitem.h"

namespace Student
{

BonusBagItem::BonusBagItem(QGraphicsItem *parent) :
                           QGraphicsPixmapItem(parent)

{
    setPixmap(QPixmap(":/images/bonusbag.png").scaled(QSize(30, 30)));

    this->setZValue(1);

}

bool BonusBagItem::move()
{
    if (dir_ == 'a'){
        if (pos().x() - 6 > 0) {
            setPos(x()-2,y());
            return true;
        }
    }
    else if (dir_ == 'd'){
        if (pos().x() + 1 < 405){
            setPos(x()+2,y());
            return true;
        }
    }
    else if (dir_ == 'w'){
        if (pos().y() - 6 > 0){
            setPos(x(),y()-2);
            return true;
        }
    }
    else if (dir_ == 's'){
        if (pos().y() + 1 < 405){
            setPos(x(),y()+2);
            return true;
        }
    }
    return false;
}

void BonusBagItem::setDir(char dir)
{
    // set different direction than input, so it is harder to drive bonusitem in
    // to a corner

    if (dir == 'a'){
        dir_ = 's';
    }
    else if (dir == 'd'){
        dir_ = 'w';
    }
    else if (dir == 'w'){
        dir_ = 'a';
    }
    else if (dir == 's'){
        dir_ = 'd';
    }
}

int BonusBagItem::giveX()
{
    return pos().x();
}

int BonusBagItem::giveY()
{
    return pos().y();
}

}



