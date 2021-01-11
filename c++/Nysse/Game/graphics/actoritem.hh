#ifndef ACTORITEM_HH
#define ACTORITEM_HH

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

/**
 * @brief defineds graphics object for nysse and stop actors in game
 */

namespace Student {

const int WIDTH = 20;
const int HEIGHT = 20;

class ActorItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    ActorItem(int x, int y, int type = 0, QGraphicsItem *parent = nullptr);
    virtual ~ActorItem();

    void setCoord(int x, int y);

private:
    int x_;
    int y_;
    int type_;
};

}

#endif // ACTORITEM_HH
