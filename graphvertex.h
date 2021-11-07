#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QAction>
#include "state.h"

class GraphArrow;

class GraphVertex : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };
    enum VertexType { Normal, UML };

    GraphVertex(VertexType diagramType, QGraphicsItem *parent = nullptr);

    void removeArrow(GraphArrow *arrow);
    void removeArrows();

    void setName(const QString &text);
    const QString& getName() const{return name;}

    void setInitial(bool initial) { _initial = initial; }
    void setFinal(bool final) { _final = final; }
    bool isFinal() const{ return _final; }

    VertexType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    void addArrow(GraphArrow *arrow);
    int type() const override { return Type; }
    void deleteVertex();


protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    bool _initial;
    bool _final;
    VertexType myDiagramType;
    QPolygonF myPolygon;
    QList<GraphArrow *> arrows;
    QString name;
};
#endif // GRAPHVERTEX_H
