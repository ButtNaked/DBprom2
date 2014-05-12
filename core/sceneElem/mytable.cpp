#include "mytable.h"
#include "arrow.h"

MyTable::MyTable(QListWidget *rListWidget) :
    content(rListWidget)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWidget(rListWidget);
}

void MyTable::addArrow(Arrow *newArrow)
{
    arrowList.append(newArrow);
}

void MyTable::moveEvent(QGraphicsSceneMoveEvent *moveEvent)
{
    foreach (Arrow *arrow, arrowList) {
        arrow->updatePosition();
    }
}

void MyTable::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    foreach (Arrow *arrow, arrowList) {
        arrow->updatePosition();
    }
}
