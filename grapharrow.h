#ifndef GRAPHARROW_H
#define GRAPHARROW_H


#include <QGraphicsLineItem>

class GraphVertex;

class GraphArrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    GraphArrow(GraphVertex *startItem, GraphVertex *endItem,
          QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor &color) { myColor = color; }
    GraphVertex *startItem() const { return myStartItem; }
    GraphVertex *endItem() const { return myEndItem; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    GraphVertex *myStartItem;
    GraphVertex *myEndItem;
    QPolygonF arrowHead;
    QColor myColor = Qt::black;
};
#endif // GRAPHARROW_H
