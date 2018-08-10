#include "snake.h"
#include "utility.h"
#include <QPainter>

QRectF Snake::worldRect = QRectF();

Snake::Snake(int key, const QColor &color, const QPointF &head, qreal dir) :
    id(key), color(color), headDir(dir), brush(color), pen(color)
{
    body.push_front(head);
    int tailX = head.x() - bodyRadius*2*qCos(headDir);
    int tailY = head.y() - bodyRadius*2*qSin(headDir);
    body.push_back(QPointF(0.8*head.x() + 0.2*tailX, 0.8*head.y() + 0.2*tailY));
    body.push_back(QPointF(0.6*head.x() + 0.4*tailX, 0.6*head.y() + 0.4*tailY));
    body.push_back(QPointF(0.4*head.x() + 0.6*tailX, 0.4*head.y() + 0.6*tailY));
    body.push_back(QPointF(0.2*head.x() + 0.8*tailX, 0.2*head.y() + 0.8*tailY));
    body.push_back(QPointF(tailX, tailY));
}

void Snake::setWorldRect(const QPointF &topLeft, const QSizeF &size)
{
    worldRect.setTopLeft(topLeft);
    worldRect.setSize(size);
}

void Snake::setWorldRect(const QRectF &rect)
{
    worldRect.setRect(rect.left(), rect.top(), rect.width(), rect.height());
}

void Snake::setWorldRect(qreal x, qreal y, qreal width, qreal height)
{
    worldRect.setRect(x, y, width, height);
}

bool Snake::crashIntoWall(const QPointF &head)
{
    static QRectF safeRange(worldRect.x() + bodyRadius, worldRect.y() + bodyRadius, worldRect.width() - 2*bodyRadius, worldRect.height() - 2*bodyRadius);
    return !pointInRect(head, safeRange);
}

bool Snake::crashIntoSnake(const QPointF &head, const Snake &snake)
{
    static int overlapDistanceSquared = overlapDistance*overlapDistance;
    for (auto itr = snake.body.begin(); itr != snake.body.end(); ++itr) {
        if (distanceSquared(head, *itr) <= overlapDistanceSquared)
            return true;
    }
    return false;
}

qreal Snake::getCurDir() const
{
    return headDir;
}

void Snake::setDir(qreal dir)
{
    headDir = dir;
}

const QPointF &Snake::getHead() const
{
    return body.front();
}

QPointF Snake::imagineMove(qreal newDir)
{
    const QPointF &head = body.front();
    int headX = head.x() + bodyRadius*2*qCos(newDir);
    int headY = head.y() + bodyRadius*2*qSin(newDir);
    return QPointF(headX, headY);
}

bool Snake::sameSnake(const Snake &other) const
{
    return id == other.id;
}

bool Snake::canEat(const QPointF &meat) const
{
    static int eatDistanceSquare = eatDistance*eatDistance;
    return distanceSquared(body.front(), meat) <= eatDistanceSquare;
}

void Snake::eatMeat()
{
    const QPointF &oldHead = body.front();
    int headX = oldHead.x() + bodyRadius*2*qCos(headDir);
    int headY = oldHead.y() + bodyRadius*2*qSin(headDir);
    body.push_front(QPointF(0.8*oldHead.x() + 0.2*headX, 0.8*oldHead.y() + 0.2*headY));
    body.push_front(QPointF(0.6*oldHead.x() + 0.4*headX, 0.6*oldHead.y() + 0.4*headY));
    body.push_front(QPointF(0.4*oldHead.x() + 0.6*headX, 0.4*oldHead.y() + 0.6*headY));
    body.push_front(QPointF(0.2*oldHead.x() + 0.8*headX, 0.2*oldHead.y() + 0.8*headY));
    body.push_front(QPointF(headX, headY));
}

void Snake::moveOn()
{
    const QPointF &oldHead = body.front();

    int dx = bodyRadius*2*qCos(headDir);
    int dy = bodyRadius*2*qSin(headDir);

    //there should be four body between old head and new head to fill the gap
    body.push_front(QPointF(oldHead.x() + 0.2*dx, oldHead.y() + 0.2*dy));
    body.push_front(QPointF(oldHead.x() + 0.4*dx, oldHead.y() + 0.4*dy));
    body.push_front(QPointF(oldHead.x() + 0.6*dx, oldHead.y() + 0.6*dy));
    body.push_front(QPointF(oldHead.x() + 0.8*dx, oldHead.y() + 0.8*dy));
    body.push_front(QPointF(oldHead.x() + dx, oldHead.y() + dy));

    //as add five body in the front, we need remove five body in the tail
    body.pop_back();
    body.pop_back();
    body.pop_back();
    body.pop_back();
    body.pop_back();
}

void Snake::paint(QPainter *painter)
{
    static QPointF headPolygon[7] = {QPointF(bodyRadius, 0),
                                     QPointF(bodyRadius*0.8, bodyRadius*0.6),
                                     QPointF(0, bodyRadius),
                                     QPointF(-bodyRadius, bodyRadius),
                                     QPointF(-bodyRadius, -bodyRadius),
                                     QPointF(0, -bodyRadius),
                                     QPointF(bodyRadius*0.8, -bodyRadius*0.6)};

    static QPointF tailPolygon[3] = {QPointF(0.2*bodyRadius, bodyRadius),
                                     QPointF(0.2*bodyRadius, -bodyRadius),
                                     QPointF(-3*bodyRadius, 0)};


    painter->setBrush(brush);
    painter->setPen(pen);
    for (auto itr = body.begin(); itr != body.end(); ++itr) {
        if (itr == body.begin()) {
            //head special
            qDebug("head angle is %g\n", headDir);
            painter->save();
            painter->setBrush(QBrush(QColor(rand())));
            painter->translate(itr->x(), itr->y());
            painter->rotate(headDir * 180 / PI);
            painter->drawConvexPolygon(headPolygon, 7);
            painter->restore();
            continue;
        }
        else if (itr == body.end()-1) {
            //tail special
            auto itr_prev = itr-1;
            qreal dir = getAngle(*itr, *itr_prev);
            qDebug("tail angle is %g\n", dir);
            painter->save();
            painter->translate(itr->x(), itr->y());
            painter->rotate(dir * 180 / PI);
            painter->drawConvexPolygon(tailPolygon, 3);
            painter->restore();
            continue;
        }

        //body
        painter->drawEllipse(*itr, bodyRadius, bodyRadius);
    }
}


