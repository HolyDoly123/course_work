#ifndef GRAPHTEXT_H
#define GRAPHTEXT_H


#include <QGraphicsTextItem>

class GraphText : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };

    GraphText(QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }

signals:
    void lostFocus(GraphText *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // GRAPHTEXT_H
