#ifndef SNAKE_H
#define SNAKE_H
#include <QPointF>
#include <QPoint>
#include <QColor>
#include <QRectF>
#include <QBrush>
#include <QPen>
#include <deque>

class Snake
{
public:
    static void setWorldRect(const QPointF &topLeft, const QSizeF &size);
    static void setWorldRect(const QRectF &rect);
    static void setWorldRect(qreal x, qreal y, qreal width, qreal height);

    static bool crashIntoWall(const QPointF &head);//whether this head would crash into wall
    static bool crashIntoSnake(const QPointF &head, const Snake &snake);//whether this head would crash into another snake

public:
    Snake(int key, const QColor &color, const QPointF &head, qreal dir);

    qreal getCurDir() const;
    void setDir(qreal dir);

    const QPointF &getHead() const;

    QPointF imagineMove(qreal newDir); //not real move, just imagine if move, return the next head position

    bool sameSnake(const Snake &other) const; //if "this" snake is the same one with "other" snake

    bool canEat(const QPointF &meat) const; //whether this snake would eat a piece of meat

    void eatMeat(); //eat and grow
    void moveOn(); //update headDir and move forward

    void paint(QPainter *painter); //paint this snake

private:
    int    id;   //this id would be the key of a unique snake
    QColor color;
    qreal  headDir; //[0,2*pi)

    std::deque<QPointF> body;

    //use brush and pen to paint snake (head, body, tail ...)
    QBrush brush;
    QPen   pen;

public:
    static const int bodyRadius = 4;    //size of body
    static const int eatDistance = 8;   //meat can be eaten

    static const int overlapDistance = 6;   //snake body overlap

    static QRectF worldRect;
};

#endif // SNAKE_H
