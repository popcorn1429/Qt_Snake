#include "world.h"
#include "utility.h"
#include <QPainter>

World::World(qreal left, qreal top, qreal width, qreal height) :
    worldRect(left, top, width, height),
    meatBrush(QColor(200, 188, 103)),
    meatPen(QColor(189,100,220)),
    ctrlBrush(QColor(255,0,0)),
    ctrlPen(QColor(0,255,255)),
    dirBrush(QColor(0,255,0)),
    dirPen(QColor(255,0,255))
{
    Snake::setWorldRect(worldRect);
    snakes.push_back(Snake(0, QColor(223,100,88),QPointF(left + width/8, top + height/5),0.0));
    //snakes.push_back(Snake(1, QColor(100,150,100),QPoint(left + width/8, top + height*2/5),0.0));
    //snakes.push_back(Snake(2, QColor(99,200,148),QPoint(left + width/8, top + height*3/5),0.0));
    //snakes.push_back(Snake(3, QColor(200,150,100),QPoint(left + width/8, top + height*4/5),0.0));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &World::placeMeat);
    timer->start(50);
}

void World::updateCtrlDir(QPointF &rod, QPoint pos)
{
    ctrlDir = getAngle(rod, pos);

    //static qreal mouseDistanceSquared = World::ctrlRadius*World::ctrlRadius;
    ctrlLen = distance(pos, rod);
    if ((ctrlRadius-dirRadius) < ctrlLen) {
        rod.setX(pos.x()-(ctrlRadius-dirRadius)*qCos(ctrlDir));
        rod.setY(pos.y()-(ctrlRadius-dirRadius)*qSin(ctrlDir));
    }
}

bool World::moveOn()
{
    //the first snake controled by user, it moves by user
    Snake &firstSnake = snakes[0];
    firstSnake.setDir(ctrlDir);
    if (!forceMoveSnake(firstSnake)) {
        return false;
    }

    //other snakes would move by itself(randomly)
    for (int i = 1; i < snakes.size(); ++i) {
        Snake &snake = snakes[i];

        if (!tryMoveSnake(snake)) {
            return false;
        }
    }
    return true;
}

bool World::forceMoveSnake(Snake &snake)
{
    //eat a meat to grow or just move on
    if (!tryEatMeat(snake))
        snake.moveOn();

    //1 crash with wall
    if (Snake::crashIntoWall(snake.getHead())) {
        return false;
    }
    //2 crash with other snake
    if (snakeCrash(snake, snake.getHead())) {
        return false;
    }
    return true;
}

bool World::tryMoveSnake(Snake &snake)
{
    //1 eat a meet, good!
    if (tryEatMeat(snake))
        return true;

    //2 move (maybe current direction, maybe change)
    qreal curDir = snake.getCurDir();
    bool canMoveCurDir = canMoveThisWay(snake, curDir); //whether current dir is safe

    //if it's safe but this snake want change a new direction(30%)
    //or it's not safe this direction.
    //then change direction (try 5 times)
    if ((canMoveCurDir && qrand()%100 < 30) || !canMoveCurDir) {
        for (int i = 0; i < 5; ++i) {
            qreal tryDir = curDir + (qrand()%100-50)*1.0/50;
            if (canMoveThisWay(snake, tryDir)) {
                snake.setDir(tryDir);
                snake.moveOn();
                return true;
            }
        }
    }

    //no direction change, use original direction and move on (if it's safe) or just crash.
    if (canMoveCurDir) {
        snake.moveOn();
        return true;
    }
    return false;
}

bool World::canMoveThisWay(Snake &snake, qreal newDir)
{
    //it's not a real move, just imagine the snake would move ahead and judge it's safe or not
    //if it's safe, ok, return true (still not move really)
    //if it's not safe, fine , return false (not move really, just a try, won't crash)
    QPointF nextHeadPos = snake.imagineMove(newDir);
    if (Snake::crashIntoWall(nextHeadPos))
        return false;

    if (snakeCrash(snake, nextHeadPos)) {
        return false;
    }
    return true;
}

bool World::snakeCrash(const Snake &snake, const QPointF &head) const
{
    for (int j = 0; j < snakes.size(); ++j) {
        if (snake.sameSnake(snakes[j]))
            continue;

        if (Snake::crashIntoSnake(head, snakes[j])) {
            return true;
        }
    }
    return false;
}

bool World::tryEatMeat(Snake &snake)
{
    for (auto itr = meat.begin(); itr != meat.end(); ++itr) {
        if (snake.canEat(*itr)) {
            snake.eatMeat();
            meat.erase(itr); //this meat will be eaten,so remove it from world.
            return true;
        }
    }

    return false;
}

void World::paint(QPainter *painter)
{
    //1 draw controller
    paintController(painter);

    //2 draw meat
    paintMeat(painter);

    //3 draw snakes
    for (int i = 0; i < snakes.size(); ++i) {
        snakes[i].paint(painter);
    }
}

void World::placeMeat()
{
    if (meat.size() < 5) {
        while (true) {
            QPointF newPos;
            if (getRandomEmptyPos(newPos)) {
                meat.push_back(newPos);
                return;
            }
        }
    }
}

bool World::getRandomEmptyPos(QPointF &pos)
{
    pos.setX(qrand() % int(worldRect.width()*0.8) + worldRect.x() + 0.1*worldRect.width());
    pos.setY(qrand() % int(worldRect.height()*0.8) + worldRect.y() + 0.1*worldRect.height());

    return isEmptyPos(pos);
}

bool World::isEmptyPos(const QPointF &pos)
{
    //dont be too close to each meat
    static int meatDistanceSquared = 4 * meatRadius * meatRadius;
    for (auto itr = meat.begin(); itr != meat.end(); ++itr) {
        if (distanceSquared(pos, *itr) < meatDistanceSquared)
            return false;
    }

    //dont be too close to snake head
    static int snakeDistanceSquared = 4 * Snake::bodyRadius * Snake::bodyRadius;
    for (int i = 0; i < snakes.size(); ++i) {
        if (distanceSquared(pos, snakes[i].getHead()) < snakeDistanceSquared)
            return false;
    }
    return true;
}

void World::paintController(QPainter *painter)
{
    painter->save();
    painter->translate(worldRect.x() + ctrlRadius + 2, worldRect.y()+worldRect.height()-ctrlRadius - 2);
    painter->rotate(ctrlDir * 180 / PI);
    painter->setBrush(ctrlBrush);
    painter->setPen(ctrlPen);
    painter->drawEllipse(-ctrlRadius, -ctrlRadius, ctrlRadius*2, ctrlRadius*2);
    painter->setBrush(dirBrush);
    painter->setPen(dirPen);
    painter->drawEllipse(ctrlLen-dirRadius, dirRadius, dirRadius*2, dirRadius*2);
    painter->restore();
}

void World::paintMeat(QPainter *painter)
{
    painter->setBrush(meatBrush);
    painter->setPen(meatPen);
    for (auto itr = meat.begin(); itr != meat.end(); ++itr) {
        painter->drawEllipse(*itr, meatRadius, meatRadius);
    }
}
