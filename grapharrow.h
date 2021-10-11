#ifndef GRAPHARROW_H
#define GRAPHARROW_H


#include <QGraphicsLineItem>
#include <transition.h>

class GraphVertex;

class GraphArrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    GraphArrow(GraphVertex *startItem, GraphVertex *endItem,
          QGraphicsItem *parent = nullptr);

    GraphArrow(QPointF startPoint, QPointF endPoint,
               QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor &color) { myColor = color; }
    GraphVertex *startItem() const { return myStartItem; }
    GraphVertex *endItem() const { return myEndItem; }

    void setStartItem(GraphVertex *startItem){ myStartItem = startItem; }
    void setEndItem(GraphVertex *endItem){ myEndItem = endItem; }

    QPointF startPoint() const { return myStartPoint; }
    QPointF endPoint() const { return myEndPoint; }

    void setStartPoint(QPointF startPoint) { myStartPoint = startPoint; }
    void setEndPoint(QPointF endPoint) { myEndPoint = endPoint; }

    void setSignal(const QString &text);
    const QString & getSignal() const{ return signal;};
    void setOutput(const QString &text) { output = text; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    static constexpr qreal arrowSize = 20;
    QPointF myStartPoint;
    QPointF myEndPoint;

    GraphVertex *myStartItem;
    GraphVertex *myEndItem;

    QPolygonF arrowHead;
    QColor myColor = Qt::black;

    QString signal;
    QString output = QString();
};
#endif // GRAPHARROW_H
