#ifndef UTILITY_H
#define UTILITY_H
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QtMath>

extern const qreal PI;

//angle would be in range [0, 2*PI)
qreal getAngle(const QPointF &pos1, const QPointF &pos2);

//distance squared
int distanceSquared(const QPoint &first, const QPoint &second);
qreal distanceSquared(const QPointF &first, const QPointF &second);

//distance
qreal distance(const QPointF &first, const QPointF &second);

bool pointInRect(const QPoint &pos, const QRect &range);
bool pointInRect(const QPointF &pos, const QRect &range);
bool pointInRect(const QPointF &pos, const QRectF &range);

#endif // UTILITY_H
