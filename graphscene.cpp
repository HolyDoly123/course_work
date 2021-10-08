#include "graphscene.h"
#include "grapharrow.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QMenu>


//TODO: arrow paint, arrow delete, arrow press event
GraphScene::GraphScene(QObject *parent)
    : QGraphicsScene(parent)
{
    myMode = MoveItem;
    myItemType = GraphVertex::Normal;
    line = nullptr;
    textItem = nullptr;
    myVertexColor = Qt::gray;
    myTextColor = Qt::black;
    myLineColor = Qt::black;

    createActions();
    createContextMenus();
}

void GraphScene::createActions()
{
    insertVertexAct = new QAction(tr("Insert vertex"), this);
    insertVertexAct->setStatusTip(tr("Create a new vertex"));
    connect(insertVertexAct, &QAction::triggered, this, &GraphScene::insertVertex);

    deleteVertexAct = new QAction(tr("Delete vertex"), this);
    deleteVertexAct->setStatusTip(tr("Delete vertex"));
    connect(deleteVertexAct, &QAction::triggered, this, &GraphScene::deleteVertex);

    insertArrowAct = new QAction(tr("Insert arrow"), this);
    insertArrowAct->setStatusTip(tr("Create a new arrow"));
    connect(insertArrowAct, &QAction::triggered, this, &GraphScene::insertArrow);

    deleteArrowAct = new QAction(tr("Delete arrow"), this);
    deleteArrowAct->setStatusTip(tr("Delete arrow"));
    connect(deleteArrowAct, &QAction::triggered, this, &GraphScene::deleteArrow);
}

void GraphScene::createContextMenus()
{
    myItemMenu = new QMenu;
    myItemMenu->addAction(insertVertexAct);
    myItemMenu->addSeparator();
    myItemMenu->addAction(insertArrowAct);

    vertexContextMenu = new QMenu;
    vertexContextMenu->addAction(deleteVertexAct);
    vertexContextMenu->addSeparator();
    vertexContextMenu->addAction(insertArrowAct);

    arrowContextMenu = new QMenu;
    arrowContextMenu->addAction(deleteArrowAct);
}

void GraphScene::connectVertexArrow(GraphVertex *vertex, GraphArrow *arrow)
{

}

void GraphScene::unconnectVertexArrow(GraphVertex *vertex, GraphArrow *arrow)
{

}

void GraphScene::insertVertex()
{
    GraphVertex *item;
    item = new GraphVertex(myItemType);
    item->setPen(QPen(myLineColor, 3));
    item->setBrush(myVertexColor);
    addItem(item);
    item->setPos(insertPos);
    emit itemInserted(item);
}

void GraphScene::deleteVertex()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    for (QGraphicsItem *item : qAsConst(selectedItems)) {
        if (item->type() == GraphVertex::Type){
            GraphVertex *vertex = qgraphicsitem_cast<GraphVertex *>(item);
            for(GraphArrow* arrow : vertex->getArrows())
            {
                if(arrow->startItem() == vertex)
                {
                    arrow->setStartItem(nullptr);
                }
                if(arrow->endItem() == vertex)
                {
                    arrow->setEndItem(nullptr);
                }
            }
            this->removeItem(item);
            delete item;
        }
    }
}

void GraphScene::insertArrow()
{
    myMode = InsertLine;
    views().first()->setMouseTracking(true);
    arrow = new GraphArrow(insertPos, insertPos);
    arrow->setColor(myLineColor);
    arrow->setZValue(1000.0);
    addItem(arrow);
    arrow->updatePosition();
}

void GraphScene::deleteArrow()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    for (QGraphicsItem *item : qAsConst(selectedItems)) {
        if (item->type() == GraphArrow::Type) {
            this->removeItem(item);
            GraphArrow *arrow = qgraphicsitem_cast<GraphArrow *>(item);
            if(arrow->startItem()) arrow->startItem()->removeArrow(arrow);
            if(arrow->endItem()) arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }
}

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
    switch (myMode) {
        case InsertLine:
        {
            views().first()->setMouseTracking(false);
            if (!arrow)
            {
                myMode = MoveItem;
                QGraphicsScene::mousePressEvent(mouseEvent);
            }

            QList<QGraphicsItem *> startItems = items(arrow->startPoint());
            if (startItems.count() && startItems.first() == arrow)
                startItems.removeFirst();
            QList<QGraphicsItem *> endItems = items(arrow->endPoint());
            if (endItems.count() && endItems.first() == arrow)
                endItems.removeFirst();

            if (startItems.count() > 0 && endItems.count() > 0 &&
                startItems.first()->type() == GraphVertex::Type &&
                endItems.first()->type() == GraphVertex::Type &&
                startItems.first() != endItems.first())
            {
                GraphVertex *startItem = qgraphicsitem_cast<GraphVertex *>(startItems.first());
                GraphVertex *endItem = qgraphicsitem_cast<GraphVertex *>(endItems.first());

                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setStartItem(startItem);
                arrow->setEndItem(endItem);
            }
            arrow->setEndPoint(mouseEvent->scenePos());
            myMode = MoveItem;
            break;
        }
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
        default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && arrow != nullptr) {
        arrow->setEndPoint(mouseEvent->scenePos());
        this->update();
    } else if (myMode == MoveItem) {
        this->update();
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}
//! [10]

//! [11]
void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    QList<QGraphicsItem *> selected = this->selectedItems();
    if(!selected.empty())
    {
        if(selected.first()->type() == GraphVertex::Type)
        {
            insertPos = contextMenuEvent->scenePos();
            vertexContextMenu->exec(contextMenuEvent->screenPos());
            QGraphicsScene::contextMenuEvent(contextMenuEvent);
        }
        else if(selected.first()->type() == GraphArrow::Type)
        {
            arrowContextMenu->exec(contextMenuEvent->screenPos());
            QGraphicsScene::contextMenuEvent(contextMenuEvent);
        }
        return;
    }
    insertPos = contextMenuEvent->scenePos();
    myItemMenu->exec(contextMenuEvent->screenPos());
    QGraphicsScene::contextMenuEvent(contextMenuEvent);
}

void GraphScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
    double scale = wheelEvent->delta();
    if (scale > 0)
    {
        if (current_scale > 2.5)
            return;
        scale = 1.25;
    }
    else
    {
        if (current_scale < 0.4)
            return;
        scale = 0.8;
    }
    current_scale = current_scale * scale;
    views().first()->scale(scale, scale);
}

bool GraphScene::isItemChange(int type) const
{
    const QList<QGraphicsItem *> items = selectedItems();
    const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}

GraphScene::~GraphScene()
{
    delete myItemMenu;
    delete vertexContextMenu;
    delete arrowContextMenu;
}
