#ifndef BOMB_HH
#define BOMB_HH

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

/**
 * @brief defines graphics object for a bomb in the game
 */

namespace Student {

class Bomb : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Bomb(QGraphicsItem *parent = nullptr);

private slots:
    void remove();
};

}
#endif // BOMB_HH
