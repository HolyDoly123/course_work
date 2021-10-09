#include "graphscene.h"
#include "grapharrow.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QMenu>


//TODO: arrow arc(variants), text, adjusting text

GraphScene::GraphScene(QObject *parent)
    : QGraphicsScene(parent)
{
    myMode = MoveItem;
    myItemType = GraphVertex::Normal;
    line = nullptr;
    arrow = nullptr;
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

    vertexContextMenu = new QMenu;
    vertexContextMenu->addAction(deleteVertexAct);
    vertexContextMenu->addSeparator();
    vertexContextMenu->addAction(insertArrowAct);

    arrowContextMenu = new QMenu;
    arrowContextMenu->addAction(deleteArrowAct);
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
            vertex->removeArrows();
            this->removeItem(item);
            this->update();
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
    arrow->setZValue(-1000.0);
    addItem(arrow);
    arrow->updatePosition();
    this->update();
}

void GraphScene::deleteArrow()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    for (QGraphicsItem *item : qAsConst(selectedItems)) {
        if (item->type() == GraphArrow::Type) {
            this->removeItem(item);
            GraphArrow *arrow = qgraphicsitem_cast<GraphArrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            this->update();
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

void GraphScene::setTextColor(const QColor &color)
{
    myTextColor = color;

}

void GraphScene::setVertexColor(const QColor &color)
{
    myVertexColor = color;
    if (isItemChange(GraphVertex::Type)) {
        GraphVertex *item = qgraphicsitem_cast<GraphVertex *>(selectedItems().first());
        item->setBrush(myVertexColor);
    }
}

void GraphScene::setFont(const QFont &font)
{
    myFont = font;
}

void GraphScene::setMode(Mode mode)
{
    myMode = mode;
}

void GraphScene::setVertexType(GraphVertex::VertexType type)
{
    myItemType = type;
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    if (myMode == InsertLine) {
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
            arrow->setEndPoint(mouseEvent->scenePos());
        }
        else
        {
            removeItem(arrow);
            delete arrow;
        }
        this->update();
        arrow = nullptr;
        myMode = MoveItem;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

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
    current_scale = current_scale *scale;
    views().first()->scale(scale, scale);
    this->update();
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
