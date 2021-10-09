#include "grapharrow.h"
#include "graphvertex.h"

#include <QPainter>
#include <QPen>
#include <QtMath>

//! [0]
GraphArrow::GraphArrow(GraphVertex *startItem, GraphVertex *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), myStartItem(startItem), myEndItem(endItem)
{
    setPen(QPen(myColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

GraphArrow::GraphArrow(QPointF startPoint, QPointF endPoint, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), myStartPoint(startPoint), myEndPoint(endPoint)
{
    myStartItem = nullptr;
    myEndItem = nullptr;
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(QPen(myColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF GraphArrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;
    if (!arrowHead.empty())
    {
        return arrowHead.boundingRect();
    }
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath GraphArrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void GraphArrow::updatePosition()
{
    setLine(QLineF(myStartPoint, myEndPoint));
}

void GraphArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *)
{

    QPen myPen = pen();
    myPen.setColor(myColor);
    painter->setPen(myPen);
    painter->setBrush(myColor);
    QLineF centerLine(myStartPoint, myEndPoint);
    QPainterPath path;
    double angle = std::atan2(-line().dy(), line().dx());
    if (myStartItem && myEndItem)
    {
        painter->setBrush(Qt::NoBrush);
        path.moveTo(centerLine.p1());

        if (myStartItem->pos().x() < myEndItem->pos().x())
        {
            myStartPoint = myStartItem->pos()+myStartItem->boundingRect().center();
            myEndPoint = myEndItem->pos()+myEndItem->boundingRect().center() + QPointF(0, myEndItem->boundingRect().height()/2);
            centerLine.setP2(myEndPoint);
            centerLine.setP1(myStartPoint);
            path.cubicTo(centerLine.p1()+QPointF(0,50),centerLine.center()+QPointF(0,200), centerLine.p2());
            angle = M_PI / 3;
        }
        else
        {
            myStartPoint = myStartItem->pos()+myStartItem->boundingRect().center();
            myEndPoint = myEndItem->pos()+myEndItem->boundingRect().center() - QPointF(0, myEndItem->boundingRect().height()/2);
            centerLine.setP2(myEndPoint);
            centerLine.setP1(myStartPoint);
            path.cubicTo(centerLine.p1()-QPointF(0,50),centerLine.center()-QPointF(0,200), centerLine.p2());
            angle = M_PI + M_PI / 3;
        }
        painter->drawPath(path);
    }
    else
    {
        setLine(centerLine);
        painter->drawLine(line());
    }



    QPointF arrowP1 = myEndPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                    cos(angle - M_PI / 3) * arrowSize);
    QPointF arrowP2 = myEndPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                    cos(angle - M_PI + M_PI / 3) * arrowSize);

    arrowHead.clear();
    arrowHead << myEndPoint << arrowP1 << arrowP2;

    painter->setBrush(myColor);
    painter->drawPolygon(arrowHead);

    painter->drawText(centerLine.center()+QPointF(0,50), "1/0");

    if (isSelected())
    {
        painter->setPen(QPen(Qt::blue, 1));
        painter->setBrush(Qt::blue);
        painter->drawEllipse(myEndPoint, 5, 5);
    }

}
