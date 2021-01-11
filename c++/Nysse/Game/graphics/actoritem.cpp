#include "actoritem.hh"

namespace Student {

ActorItem::ActorItem(int x, int y, int type, QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent), x_(x), y_(y), type_(type)
{
    setPos(mapToParent(x_, y_ ));

    if (type_ == 1) {
        setPixmap(QPixmap(":/images/bus.png").scaled(QSize(2 * WIDTH, HEIGHT)));
    }
    else if (type_ == 2) {
        setPixmap(QPixmap(":/images/stop.png").scaled(QSize(WIDTH, HEIGHT)));
    }

}

ActorItem::~ActorItem()
{

}

void ActorItem::setCoord(int x, int y)
{
    setX( x );
    setY( y );
}

}
