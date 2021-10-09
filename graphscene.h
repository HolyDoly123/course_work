#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include "graphvertex.h"
#include "graphtext.h"

#include <QGraphicsScene>
#include <QAction>
#include <QMenu>
#include <QGraphicsView>

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertVertex, InsertLine, InsertText, MoveItem };

    GraphScene(QObject *parent = nullptr);
    ~GraphScene();
    QFont font() const { return myFont; }
    QColor textColor() const { return myTextColor; }
    QColor vertexColor() const { return myVertexColor; }
    QColor lineColor() const { return myLineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setVertexColor(const QColor &color);
    void setFont(const QFont &font);


    void insertVertex();
    void deleteVertex();
    void insertArrow();
    void deleteArrow();

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
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) override;

private:
    bool isItemChange(int type) const;
    void createActions();
    void createContextMenus();

    GraphVertex::VertexType myItemType;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    GraphArrow *arrow;
    QFont myFont;
    GraphText *textItem;
    QColor myTextColor;
    QColor myVertexColor;
    QColor myLineColor;

    QPointF insertPos;

    QMenu *myItemMenu;
    QMenu *vertexContextMenu;
    QMenu *arrowContextMenu;

    double current_scale = 1.0;

    QAction *insertVertexAct;
    QAction *deleteVertexAct;

    QAction *insertArrowAct;
    QAction *deleteArrowAct;
};

#endif // GRAPHSCENE_H
