#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

#include "mytable.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Arrow(MyTable *startItem, int StartItemElemNum , MyTable *endItem,
      int endItemElemNum, QGraphicsItem *parent = 0);

    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color) { myColor = color; }
    MyTable *startItem() const { return myStartItem; }
    MyTable *endItem() const { return myEndItem; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    int myStartItemElemNum;
    int myEndItemElemNum;
    MyTable *myStartItem;
    MyTable *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
};

#endif // ARROW_H
