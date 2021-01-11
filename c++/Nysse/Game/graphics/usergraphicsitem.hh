#ifndef USERGRAPHICSITEM_HH
#define USERGRAPHICSITEM_HH

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

/**
 * @brief defines graphics object for the player's character on screen
 */

namespace Student {

class UserGraphicsItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit UserGraphicsItem(QGraphicsItem *parent = nullptr);

    void setDir(char dir);
    bool move();

    int giveX();
    int giveY();

private:
    char dir_;
};

}

#endif // USERGRAPHICSITEM_HH
