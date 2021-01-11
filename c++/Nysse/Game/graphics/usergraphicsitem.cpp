#include "usergraphicsitem.hh"

namespace Student {

UserGraphicsItem::UserGraphicsItem(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/images/player.png").scaled(QSize(90, 90)));

    this->setZValue(2);
    setTransformOriginPoint(53,43);

}

bool UserGraphicsItem::move()
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

void UserGraphicsItem::setDir(char dir)
{
    dir_ = dir;

    if (dir_ == 'a'){
        setRotation(180);
    }
    else if (dir_ == 'd'){
        setRotation(0);
    }
    else if (dir_ == 'w'){
        setRotation(270);
    }
    else if (dir_ == 's'){
        setRotation(90);
    }
}

int UserGraphicsItem::giveX()
{
    return pos().x();
}

int UserGraphicsItem::giveY()
{
    return pos().y();
}

}
