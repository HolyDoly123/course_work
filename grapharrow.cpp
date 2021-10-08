#include "grapharrow.h"
#include "graphvertex.h"

#include <QPainter>
#include <QPen>
#include <QtMath>

//! [0]
GraphArrow::GraphArrow(GraphVertex *startItem, GraphVertex *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), myStartItem(startItem), myEndItem(endItem)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setPen(QPen(myColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

GraphArrow::GraphArrow(QPointF startPoint, QPointF endPoint, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), myStartPoint(startPoint), myEndPoint(endPoint)
{
    myStartItem = nullptr;
    myEndItem = nullptr;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setPen(QPen(myColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF GraphArrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath GraphArrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}
//! [2]

//! [3]
void GraphArrow::updatePosition()
{
    setLine(QLineF(myStartPoint, myEndPoint));
}
//! [3]

//! [4]
void GraphArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *)
{

    QPen myPen = pen();
    myPen.setColor(myColor);
    painter->setPen(myPen);
    painter->setBrush(myColor);
    QLineF centerLine(myStartPoint, myEndPoint);
    /*if (myStartItem && myEndItem)
    {
        QLineF centerLine(myStartItem->pos(), myEndItem->pos());
        QPolygonF endPolygon = myEndItem->polygon();
        QPointF p2 = endPolygon.first() + myEndItem->pos();
        QPointF intersectPoint;
        for (int i = 1; i < endPolygon.count(); ++i) {
            QPointF p1 = endPolygon.at(i) + myEndItem->pos();
            QLineF polyLine = QLineF(p1, p2);
            QLineF::IntersectionType intersectionType =
                polyLine.intersects(centerLine, &intersectPoint);
            if (intersectionType == QLineF::BoundedIntersection)
                break;
            p2 = p1;
        }

        setLine(QLineF(myStartItem->pos() +myStartItem->boundingRect().center(), intersectPoint));
    }*/
    if (myStartItem)
    {
        /*myStartPoint = myStartItem->pos();
        centerLine.setP1(myStartPoint);
        QPolygonF startPolygon = myStartItem->polygon();
        QPointF p1 = startPolygon.first() + myStartItem->pos();
        QPointF intersectPoint;
        for (int i = 1; i < startPolygon.count(); ++i) {
            QPointF p2 = startPolygon.at(i) + myStartItem->pos();
            qDebug() << p2;
            QLineF polyLine = QLineF(p1, p2);
            QLineF::IntersectionType intersectionType =
                polyLine.intersects(centerLine, &intersectPoint);
            if (intersectionType == QLineF::BoundedIntersection)
                break;
            p1 = p2;
        }
        myStartPoint = p1;*/
        QPointF min = myStartItem->pos()+myStartItem->boundingRect().center();// myStartItem->polygon().first() + myStartItem->pos();
        qreal min_dist = (myStartPoint.x()-myEndPoint.x()) * (myStartPoint.x()-myEndPoint.x()) + (myStartPoint.y()-myEndPoint.y()) * (myStartPoint.y()-myEndPoint.y());
        for (auto p1 : myStartItem->polygon())
        {
            p1 += myStartItem->pos();
            qreal dist = (p1.x()-myEndPoint.x()) * (p1.x()-myEndPoint.x()) + (p1.y()-myEndPoint.y()) * (p1.y()-myEndPoint.y());
            if (dist < min_dist)
            {
                min = p1;
                min_dist = dist;
            }
        }
        myStartPoint = min;
    }
    if (myEndItem)
    {
        QPointF min = myEndItem->pos()+myEndItem->boundingRect().center();// myStartItem->polygon().first() + myStartItem->pos();
        qreal min_dist = (myEndPoint.x()-myStartPoint.x()) * (myEndPoint.x()-myStartPoint.x()) + (myEndPoint.y()-myStartPoint.y()) * (myEndPoint.y()-myStartPoint.y());
        for (auto p1 : myEndItem->polygon())
        {
            p1 += myEndItem->pos();
            qreal dist = (p1.x()-myStartPoint.x()) * (p1.x()-myStartPoint.x()) + (p1.y()-myStartPoint.y()) * (p1.y()-myStartPoint.y());
            if (dist < min_dist)
            {
                min = p1;
                min_dist = dist;
            }
        }
        myEndPoint = min;
    }
    else setLine(centerLine);
    //centerLine.setP1(myStartPoint);
    //centerLine.setP2(myEndPoint);

    double angle = std::atan2(-line().dy(), line().dx());

    QPointF arrowP1 = line().p2() + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                    cos(angle - M_PI / 3) * arrowSize);
    QPointF arrowP2 = line().p2() + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                    cos(angle - M_PI + M_PI / 3) * arrowSize);

    arrowHead.clear();
    arrowHead << line().p2() << arrowP1 << arrowP2;
//! [6] //! [7]
    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    if (isSelected()) {
        painter->setPen(QPen(myColor, 2, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
