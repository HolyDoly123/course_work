#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QAction>

class GraphArrow;

class GraphVertex : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };
    enum VertexType { Normal, UML };

    GraphVertex(VertexType diagramType, QGraphicsItem *parent = nullptr);

    void removeArrow(GraphArrow *arrow);
    void removeArrows();

    VertexType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    void addArrow(GraphArrow *arrow);
    QPixmap image() const;
    int type() const override { return Type; }
    void deleteVertex();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    VertexType myDiagramType;
    QPolygonF myPolygon;
    QList<GraphArrow *> arrows;
};
#endif // GRAPHVERTEX_H
