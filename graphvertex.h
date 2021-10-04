#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H

#include <QGraphicsPixmapItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

class GraphArrow;

class GraphVertex : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };
    enum VertexType { Normal, UML };

    GraphVertex(VertexType diagramType, QMenu *contextMenu, QGraphicsItem *parent = nullptr);

    void removeArrow(GraphArrow *arrow);
    void removeArrows();
    VertexType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    void addArrow(GraphArrow *arrow);
    QPixmap image() const;
    int type() const override { return Type; }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    VertexType myDiagramType;
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QList<GraphArrow *> arrows;
};
#endif // GRAPHVERTEX_H
