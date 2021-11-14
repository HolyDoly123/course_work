#include "graphscene.h"
#include "grapharrow.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>

GraphScene::GraphScene(DFA *dfa, QObject *parent)
    : QGraphicsScene(parent)
{
    myMode = MoveItem;
    myItemType = GraphVertex::Normal;
    line = nullptr;
    arrow = nullptr;
    initialVertex = nullptr;
    myVertexColor = Qt::cyan;
    myTextColor = Qt::black;
    myLineColor = Qt::black;

    myDfa = dfa;

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

    changeStateNameAct = new QAction(tr("Set name"), this);
    changeStateNameAct->setStatusTip(tr("Sets states name"));
    connect(changeStateNameAct, &QAction::triggered, this, &GraphScene::changeStateName);

    changeTransitionSignalAct = new QAction(tr("Set signal"), this);
    changeTransitionSignalAct->setStatusTip(tr("Sets transition's signal"));
    connect(changeTransitionSignalAct, &QAction::triggered, this, &GraphScene::changeTransitionSignal);

    setInitialVertexAct = new QAction(tr("Set initial"), this);
    setInitialVertexAct->setStatusTip(tr("Sets state initial"));
    connect(setInitialVertexAct, &QAction::triggered, this, &GraphScene::setInitialVertex);

    setFinalVertexAct = new QAction(tr("Set/unset final"), this);
    setFinalVertexAct->setStatusTip(tr("Sets state final"));
    connect(setFinalVertexAct, &QAction::triggered, this, &GraphScene::setFinalVertex);

    setTransitionOutputAct = new QAction(tr("Set output"), this);
    setTransitionOutputAct->setStatusTip(tr("Sets transitions output"));
    connect(setTransitionOutputAct, &QAction::triggered, this, &GraphScene::setTransitionOutput);

    setStateDescriptionAct = new QAction(tr("Set description"), this);
    setStateDescriptionAct->setStatusTip(tr("Sets state description"));
    connect(setStateDescriptionAct, &QAction::triggered, this, &GraphScene::setStateDescription);

    setTransitionDescriptionAct = new QAction(tr("Set description"), this);
    setTransitionDescriptionAct->setStatusTip(tr("Sets state description"));
    connect(setTransitionDescriptionAct, &QAction::triggered, this, &GraphScene::setTransitionDescription);
}


void GraphScene::createContextMenus()
{
    myItemMenu = new QMenu;
    myItemMenu->addAction(insertVertexAct);


    vertexContextMenu = new QMenu;
    vertexContextMenu->addAction(insertArrowAct);
    vertexContextMenu->addAction(changeStateNameAct);
    vertexContextMenu->addAction(setStateDescriptionAct);
    vertexContextMenu->addSeparator();
    vertexContextMenu->addAction(setInitialVertexAct);
    vertexContextMenu->addAction(setFinalVertexAct);
    vertexContextMenu->addSeparator();
    vertexContextMenu->addAction(deleteVertexAct);

    arrowContextMenu = new QMenu;
    arrowContextMenu->addAction(changeTransitionSignalAct);
    arrowContextMenu->addSeparator();
    arrowContextMenu->addAction(setTransitionOutputAct);
    arrowContextMenu->addAction(setTransitionDescriptionAct);
    arrowContextMenu->addSeparator();
    arrowContextMenu->addAction(deleteArrowAct);
}

void GraphScene::insertVertex()
{
    GraphVertex *item;
    current_state = myDfa->addState(QString());
    item = new GraphVertex(myItemType);
    item->setName(current_state->getName());
    item->setPen(QPen(myLineColor, 3));
    item->setBrush(myVertexColor);
    item->setZValue(1000);
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
            if (vertex == initialVertex)
            {
                initialVertex = nullptr;
            }
            vertex->removeArrows();
            myDfa->removeState(vertex->getName());
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
    arrow->setZValue(1.0);
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
            myDfa->getState(arrow->startItem()->getName())->removeTransition(arrow->getSignal());
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            this->update();
            delete item;
        }
    }
}

void GraphScene::changeStateName()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    if (selectedItems.count() == 1 && selectedItems.first()->type() == GraphVertex::Type)
        editVertex = qgraphicsitem_cast<GraphVertex *>(selectedItems.first());
    else
        return;
    bool ok;
    QString text = QInputDialog::getText(nullptr, tr("Change state name"),
                                         tr("Input unique state name"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
    {
        if(myDfa->getState(text) != nullptr)
        {
            QMessageBox::warning(nullptr, tr("Input Error"),
                                          "This state already exist");
            return;
        }
        myDfa->getState(editVertex->getName())->setName(text);
        editVertex->setName(text);
        editVertex = nullptr;
        this->update();
    }
}

void GraphScene::changeTransitionSignal()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    if (selectedItems.count() == 1 && selectedItems.first()->type() == GraphArrow::Type)
        editArrow = qgraphicsitem_cast<GraphArrow *>(selectedItems.first());
    else
        return;

    bool ok;
    QString text = QInputDialog::getText(nullptr, tr("Change transition signal"),
                                         tr("Input state-unique signal for transition"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
    {
        if(myDfa->getState(editArrow->startItem()->getName())->getTransition(text) != nullptr)
        {
            QMessageBox::warning(nullptr, tr("Input Error"),
                                          "This transition in starting state already exist");
            return;
        }
        myDfa->getState(editArrow->startItem()->getName())->getTransition(editArrow->getSignal())->setSignal(text);
        editArrow->setSignal(text);
        editArrow = nullptr;
        this->update();
    }
}

void GraphScene::setInitialVertex()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    if (selectedItems.count() == 1 && selectedItems.first()->type() == GraphVertex::Type)
    {
        GraphVertex* gv = qgraphicsitem_cast<GraphVertex *>(selectedItems.first());
        if (initialVertex == nullptr)
        {
            initialVertex = gv;
            myDfa->setInitial(initialVertex->getName());
            initialVertex->setInitial(true);
        }
        else
        {
            initialVertex->setInitial(false);
            initialVertex = gv;
            myDfa->setInitial(initialVertex->getName());
            initialVertex->setInitial(true);
        }
    }
    this->update();
}

void GraphScene::setFinalVertex()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    for (QGraphicsItem *item : qAsConst(selectedItems)) {
        if (item->type() == GraphVertex::Type){
            GraphVertex* gv = qgraphicsitem_cast<GraphVertex *>(selectedItems.first());
            if (gv->isFinal())
            {
                myDfa->getState(gv->getName())->setFinal(false);
                gv->setFinal(false);
            }
            else
            {
                myDfa->getState(gv->getName())->setFinal(true);
                gv->setFinal(true);
            }
        }
    }
    this->update();
}

void GraphScene::setTransitionOutput()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    if (selectedItems.count() == 1 && selectedItems.first()->type() == GraphArrow::Type)
        editArrow = qgraphicsitem_cast<GraphArrow *>(selectedItems.first());
    else
        return;
    bool ok;
    QString text = QInputDialog::getText(nullptr, tr("Set transition output"),
                                         tr("Input any string to serve as output"), QLineEdit::Normal,
                                         "", &ok);
    if (ok)
    {
        myDfa->getState(editArrow->startItem()->getName())->getTransition(editArrow->getSignal())->setOutput(text);
        editArrow->setOutput(text);
        editArrow = nullptr;
        this->update();
    }
}

void GraphScene::setStateDescription()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    if (selectedItems.count() == 1 && selectedItems.first()->type() == GraphVertex::Type)
        editVertex = qgraphicsitem_cast<GraphVertex *>(selectedItems.first());
    else
        return;
    bool ok;
    QString text = QInputDialog::getMultiLineText(nullptr, tr("Set state description"),
                                         tr("In-detail description of state"),
                                         myDfa->getState(editVertex->getName())->getDescription(), &ok);
    if (ok)
    {
        myDfa->getState(editVertex->getName())->setDescription(text);
        editVertex = nullptr;
        this->update();
    }
}

void GraphScene::setTransitionDescription()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    if (selectedItems.count() == 1 && selectedItems.first()->type() == GraphArrow::Type)
        editArrow = qgraphicsitem_cast<GraphArrow *>(selectedItems.first());
    else
        return;
    bool ok;
    QString text = QInputDialog::getMultiLineText(nullptr, tr("Set transition description"),
                                         tr("In-detail description of transition"),
                                         myDfa->getState(editArrow->startItem()->getName())->
                                                getTransition(editArrow->getSignal())->getDescription(), &ok);
    if (ok)
    {
        myDfa->getState(editArrow->startItem()->getName())->getTransition(editArrow->getSignal())->setDescription(text);
        editVertex = nullptr;
        this->update();
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
            endItems.first()->type() == GraphVertex::Type )
        {
            GraphVertex *startItem = qgraphicsitem_cast<GraphVertex *>(startItems.first());
            GraphVertex *endItem = qgraphicsitem_cast<GraphVertex *>(endItems.first());

            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setStartItem(startItem);
            arrow->setEndItem(endItem);
            arrow->setEndPoint(mouseEvent->scenePos());

            current_state = myDfa->getState(startItem->getName());
            Transition *t = current_state ->addTransition(QString());
            current_state->connectTo(myDfa->getState(endItem->getName()), t->getSignal());
            arrow->setSignal(t->getSignal());
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
        insertPos = contextMenuEvent->scenePos();
        if(selected.first()->type() == GraphVertex::Type)
        {
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
    views().first()->update();
}

void GraphScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(Qt::darkBlue));
    for (int x=0; x<=5000; x+=150)
        painter->drawLine(x,0,x,5000);

    for (int y=0; y<=5000; y+=150)
        painter->drawLine(0,y,5000,y);
}

bool GraphScene::isItemChange(int type) const
{
    const QList<QGraphicsItem *> items = selectedItems();
    const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}

void GraphScene::clear()
{
    QGraphicsScene::clear();
}

GraphScene::~GraphScene()
{
    delete myItemMenu;
    delete vertexContextMenu;
    delete arrowContextMenu;
}
