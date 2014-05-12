#include "mytable.h"
#include "arrow.h"

MyTable::MyTable(QListWidget *rListWidget) :
    content(rListWidget)
{
    setZValue(0);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWidget(rListWidget);
}

void MyTable::addArrow(Arrow *newArrow)
{
    arrowList.append(newArrow);
}

void MyTable::moveEvent(QGraphicsSceneMoveEvent *moveEvent)
{
    Q_UNUSED(moveEvent);
    foreach (Arrow *arrow, arrowList) {
        arrow->updatePosition();
    }
}

void MyTable::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
    foreach (Arrow *arrow, arrowList) {
        arrow->updatePosition();
    }
}

void MyTable::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    setZValue(2);
    foreach (Arrow *arrow, arrowList) {
        arrow->setZValue(3);
    }
}

void MyTable::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    setZValue(0);
    foreach (Arrow *arrow, arrowList) {
        arrow->setZValue(1);
    }
}
