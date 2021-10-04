#include "graphscene.h"
#include "grapharrow.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QMenu>

GraphScene::GraphScene(QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = nullptr;//new QMenu(this);
    myMode = MoveItem;
    myItemType = GraphVertex::Normal;
    line = nullptr;
    textItem = nullptr;
    myVertexColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}
//! [0]

//! [1]
void GraphScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(GraphArrow::Type)) {
        GraphArrow *item = qgraphicsitem_cast<GraphArrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}
//! [1]

//! [2]
void GraphScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(GraphText::Type)) {
        GraphText *item = qgraphicsitem_cast<GraphText *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}
//! [2]

//! [3]
void GraphScene::setVertexColor(const QColor &color)
{
    myVertexColor = color;
    if (isItemChange(GraphVertex::Type)) {
        GraphVertex *item = qgraphicsitem_cast<GraphVertex *>(selectedItems().first());
        item->setBrush(myVertexColor);
    }
}
//! [3]

//! [4]
void GraphScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(GraphText::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<GraphText *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]

void GraphScene::setMode(Mode mode)
{
    myMode = mode;
}

void GraphScene::setVertexType(GraphVertex::VertexType type)
{
    myItemType = type;
}

void GraphScene::editorLostFocus(GraphText *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    GraphVertex *item;
    switch (myMode) {
        case InsertVertex:
            item = new GraphVertex(myItemType, myItemMenu);
            item->setBrush(myVertexColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            break;

        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;

        case InsertText:
            textItem = new GraphText();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, &GraphText::lostFocus,
                    this, &GraphScene::editorLostFocus);
            connect(textItem, &GraphText::selectedChange,
                    this, &GraphScene::itemSelected);
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
//! [8] //! [9]
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}
//! [10]

//! [11]
void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != nullptr && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
//! [11] //! [12]

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == GraphVertex::Type &&
            endItems.first()->type() == GraphVertex::Type &&
            startItems.first() != endItems.first())
        {
            GraphVertex *startItem = qgraphicsitem_cast<GraphVertex *>(startItems.first());
            GraphVertex *endItem = qgraphicsitem_cast<GraphVertex *>(endItems.first());
            GraphArrow *arrow = new GraphArrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    menu.addAction(newAct);
    menu.addAction(openAct);
    menu.exec(contextMenuEvent->scenePos());
}

bool GraphScene::isItemChange(int type) const
{
    const QList<QGraphicsItem *> items = selectedItems();
    const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}
