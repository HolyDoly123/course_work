#include "graphvertex.h"
#include "grapharrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

GraphVertex::GraphVertex(VertexType diagramType, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), myDiagramType(diagramType)
{
    QPainterPath path;
    _initial = false;
    _final = false;
    switch (myDiagramType) {
        case Normal:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            myPolygon = path.toFillPolygon();
            break;
        case UML:
            myPolygon << QPointF(-100, 0) << QPointF(0, 100)
                      << QPointF(100, 0) << QPointF(0, -100)
                      << QPointF(-100, 0);
            break;
        default:
            myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
                      << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);
            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void GraphVertex::setName(const QString &text)
{
    name = text;
}

void GraphVertex::removeArrow(GraphArrow *arrow)
{
    arrows.removeAll(arrow);
}

void GraphVertex::removeArrows()
{
    // need a copy here since removeArrow() will
    // modify the arrows container
    const auto arrowsCopy = arrows;
    for (GraphArrow *arrow : arrowsCopy) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void GraphVertex::addArrow(GraphArrow *arrow)
{
    arrows.append(arrow);
}

QVariant GraphVertex::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        for (GraphArrow *arrow : qAsConst(arrows))
            arrow->update();
    }

    return value;
}

void GraphVertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *s,
                  QWidget *w)
{
    QGraphicsPolygonItem::paint(painter, s, w);
    if(_initial)
    {
        painter->setBrush(QBrush(Qt::black));
        painter->drawEllipse(QPointF(25, 50), 10,10);
        painter->drawLine(25, 50, 50, 50);
    }
    if(_final)
    {
        painter->setBrush(QBrush(Qt::cyan));
        QPainterPath path;
        path.moveTo(195, 50);
        path.arcTo(145, 5, 50, 50, 0, 90);
        path.arcTo(55, 5, 50, 50, 90, 90);
        path.arcTo(55, 45, 50, 50, 180, 90);
        path.arcTo(145, 45, 50, 50, 270, 90);
        path.lineTo(195, 50);

        painter->drawPath(path);

        /*
        painter->drawLine(200, 50, 225,50);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(QPointF(225, 50), 10,10);
        painter->setBrush(QBrush(Qt::black));
        painter->drawEllipse(QPointF(225, 50), 5, 5);*/
    }

    painter->drawText(QPointF(110-name.size()*1.5, 25), name);
}
