#ifndef WORLD_H
#define WORLD_H
#include <QObject>
#include <QPointF>
#include <QTimer>
#include <vector>
#include <list>
#include "snake.h"

class World : public QObject
{
    Q_OBJECT
public:
    World(qreal left, qreal top, qreal width, qreal height);

    void updateCtrlDir(QPointF &rod, QPoint pos);

    bool moveOn();
    bool forceMoveSnake(Snake &snake);
    bool tryMoveSnake(Snake &snake);

    bool canMoveThisWay(Snake &snake, qreal newDir);

    bool snakeCrash(const Snake &snake, const QPointF &head) const;
    bool tryEatMeat(Snake &snake);

public:
    void paint(QPainter *painter);

protected slots:
    void placeMeat();

private:
    //get a new place to add a meat
    bool getRandomEmptyPos(QPointF &pos);
    bool isEmptyPos(const QPointF &pos);

    //draw controller
    void paintController(QPainter *painter);
    //draw meat
    void paintMeat(QPainter *painter);

private:
    QRectF             worldRect;

    std::vector<Snake> snakes;
    qreal              ctrlDir;
    qreal              ctrlLen;

    QTimer            *timer;
    std::list<QPointF> meat;

    //use brush and pen to paint meat
    QBrush meatBrush;
    QPen   meatPen;

    //use brush and pen to paint controller
    QBrush ctrlBrush;
    QPen   ctrlPen;
    QBrush dirBrush;
    QPen   dirPen;

public:
    static const int   meatRadius = 4;
    static const int   ctrlRadius = 30;
    static const int   dirRadius = 9;
};

#endif // WORLD_H
