#include "utility.h"

const qreal PI = 3.1415926535898;

qreal getAngle(const QPointF &pos1, const QPointF &pos2) {
    qreal dx = pos2.x()-pos1.x();
    qreal dy = pos2.y()-pos1.y();

    qreal dir = 0;
    if (dx <= 0.0001 && -0.0001 <= dx) {
        dir = (0 < dy) ? PI/2 : 3*PI/2;
    }
    else if (dy == 0) {
        dir = (0 < dx) ? 0 : PI;
    }
    else {
        dir = qAtan(dy*1.0/dx);
        if (dx < 0) {
            dir += PI;
        }
        else {
            dir += (dir < 0) ? 2*PI : 0;
        }
    }

    //qDebug("pos1(%d, %d) pos2(%d,%d) -> angle %g.\n", pos1.x(), pos1.y(), pos2.x(), pos2.y(), dir);
    return dir;
}


int distanceSquared(const QPoint &first, const QPoint &second)
{
    return ((first.x()-second.x())*(first.x()-second.x())
            + (first.y()-second.y())*(first.y()-second.y()));
}

qreal distanceSquared(const QPointF &first, const QPointF &second)
{
    return ((first.x()-second.x())*(first.x()-second.x())
            + (first.y()-second.y())*(first.y()-second.y()));
}

bool pointInRect(const QPoint &pos, const QRect &range)
{
    return (pos.x() < range.x() + range.width()) && (range.x() < pos.x())
            && (pos.y() < range.y() + range.height()) && (range.y() < pos.y());
}

bool pointInRect(const QPointF &pos, const QRect &range)
{
    return (pos.x() < range.x() + range.width()) && (range.x() < pos.x())
            && (pos.y() < range.y() + range.height()) && (range.y() < pos.y());
}

bool pointInRect(const QPointF &pos, const QRectF &range)
{
    return (pos.x() < range.x() + range.width()) && (range.x() < pos.x())
            && (pos.y() < range.y() + range.height()) && (range.y() < pos.y());
}

qreal distance(const QPointF &first, const QPointF &second)
{
    return sqrt(distanceSquared(first, second));
}
