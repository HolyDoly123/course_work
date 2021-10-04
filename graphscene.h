#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include "graphvertex.h"
#include "graphtext.h"

#include <QGraphicsScene>

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertVertex, InsertLine, InsertText, MoveItem };

    GraphScene(QObject *parent = nullptr);
    QFont font() const { return myFont; }
    QColor textColor() const { return myTextColor; }
    QColor vertexColor() const { return myVertexColor; }
    QColor lineColor() const { return myLineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setVertexColor(const QColor &color);
    void setFont(const QFont &font);

public slots:
    void setMode(Mode mode);
    void setVertexType(GraphVertex::VertexType type);
    void editorLostFocus(GraphText *item);

signals:
    void itemInserted(GraphVertex *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent) override;

private:
    bool isItemChange(int type) const;

    GraphVertex::VertexType myItemType;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QFont myFont;
    GraphText *textItem;
    QColor myTextColor;
    QColor myVertexColor;
    QColor myLineColor;
};

#endif // GRAPHSCENE_H
