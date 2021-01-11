#ifndef BONUSBAGITEM_H
#define BONUSBAGITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

/**
 * @brief defines a graphics object for bonus item in game
 */

namespace Student {


class BonusBagItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit BonusBagItem(QGraphicsItem *parent = nullptr);

    void setDir(char dir);
    bool move();

    int giveX();
    int giveY();
private:
    char dir_;
};
}

#endif // BONUSBAGITEM_H
