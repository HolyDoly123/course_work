#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include "graphvertex.h"

#include <QGraphicsScene>
#include <QAction>
#include <QMenu>
#include <QGraphicsView>
#include <QLineEdit>
#include <QGraphicsProxyWidget>

#include "dfa.h"

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertVertex, InsertLine, InsertText, MoveItem };

    GraphScene(DFA *dfa, QObject *parent = nullptr);
    ~GraphScene();
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
    void clear();

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

private slots:
    void insertVertex();
    void deleteVertex();
    void insertArrow();
    void deleteArrow();

    void changeStateName();
    void changeTransitionSignal();

    void openStateEdit();
    void openTransitionEdit();
    void openOutputEdit();

    void setInitialVertex();
    void setFinalVertex();
    void setTransitionOutput();

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
    QColor myTextColor;
    QColor myVertexColor;
    QColor myLineColor;

    QPointF insertPos;

    QMenu *myItemMenu;
    QMenu *vertexContextMenu;
    QMenu *arrowContextMenu;

    DFA *myDfa;

    double current_scale = 1.0;
    State *current_state;

    QAction *insertVertexAct;
    QAction *deleteVertexAct;

    QAction *insertArrowAct;
    QAction *deleteArrowAct;

    QAction *openStateEditAct;
    QAction *openTransitionEditAct;

    QAction *setInitialVertexAct;
    QAction *setFinalVertexAct;

    QAction *setTransitionOutputAct;

    QLineEdit *lineEdit;
    QGraphicsProxyWidget *proxy;
    GraphVertex *editVertex;
    GraphArrow *editArrow;
    GraphVertex *initialVertex;
};

#endif // GRAPHSCENE_H
