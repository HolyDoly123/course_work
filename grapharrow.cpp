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
    setPen(QPen(myColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

GraphArrow::GraphArrow(QPointF startPoint, QPointF endPoint, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), myStartPoint(startPoint), myEndPoint(endPoint)
{
    myStartItem = nullptr;
    myEndItem = nullptr;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
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
    QPainterPath path;
    if (myStartItem && myEndItem)
    {
        centerLine.setP2(myEndItem->pos()+myEndItem->boundingRect().center());
        centerLine.setP1(myStartItem->pos()+myStartItem->boundingRect().center());
        QPolygonF endPolygon = myEndItem->polygon();
        QPointF p1 = endPolygon.first() + myEndItem->pos();
        QPointF intersectPoint;
        for (int i = 1; i < endPolygon.count(); ++i) {
            QPointF p2 = endPolygon.at(i) + myEndItem->pos();
            QLineF polyLine = QLineF(p1, p2);
            QLineF::IntersectionType intersectionType =
                polyLine.intersects(centerLine, &intersectPoint);
            if (intersectionType == QLineF::BoundedIntersection)
                break;
            p1 = p2;
        }
        myStartPoint = myStartItem->pos()+myStartItem->boundingRect().center();
        myEndPoint = intersectPoint;
        centerLine.setP2(intersectPoint);
        centerLine.setP1(myStartPoint);

        painter->setBrush(Qt::white);
        path.moveTo(centerLine.p1());
        path.setFillRule(Qt::WindingFill);

        if (myStartItem->pos().x() < myEndItem->pos().x())
        {
            path.cubicTo(centerLine.p1()+QPointF(0,50),centerLine.center()+QPointF(0,200), centerLine.p2());
        }
        else
        {
            path.cubicTo(centerLine.p1()-QPointF(0,50),centerLine.center()-QPointF(0,200), centerLine.p2());
        }
        painter->drawPath(path);
    }
    else
    {
        setLine(centerLine);
        painter->drawLine(line());
    }


    double angle = std::atan2(-line().dy(), line().dx());

    QPointF arrowP1 = line().p2() + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                    cos(angle - M_PI / 3) * arrowSize);
    QPointF arrowP2 = line().p2() + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                    cos(angle - M_PI + M_PI / 3) * arrowSize);

    arrowHead.clear();
    arrowHead << line().p2() << arrowP1 << arrowP2;

    painter->setPen(QPen(Qt::blue, 10));
    painter->setBrush(Qt::blue);
    painter->drawEllipse(myEndPoint, 10, 10);
    painter->setBrush(myColor);
    //painter->drawPolygon(arrowHead);
    if (isSelected()) {

        painter->setPen(QPen(Qt::darkBlue, 1, Qt::DashLine));
        /*QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);*/
        painter->setBrush(Qt::white);
        QPainterPath myPath = path;
        myPath.translate(0, 4.0);
        painter->drawPath(myPath);
        myPath.translate(0,-8.0);
        painter->drawPath(myPath);
    }
}
