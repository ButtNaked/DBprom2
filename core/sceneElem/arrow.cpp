#include "arrow.h"
#include <math.h>

#include <QPen>
#include <QPainter>
#include <QDebug>

const qreal Pi = 3.14;

Arrow::Arrow(MyTable *startItem, int StartItemElemNum, MyTable *endItem, int endItemElemNum, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    setZValue(1);
    myStartItem = startItem;
    myEndItem = endItem;
    myStartItemElemNum = StartItemElemNum;
    myEndItemElemNum = endItemElemNum;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    updatePosition();
}

QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void Arrow::updatePosition()
{
    const qreal scrollAreaWidthOffset = 30.0;

    QPointF pointStart = myStartItem->pos(), pointEnd = myEndItem->pos();
    pointStart.setY(pointStart.y()+ 10 + 17*myStartItemElemNum);
    pointEnd.setY(pointEnd.y()+ 10 + 17*myEndItemElemNum);

    qreal endOfStartItemX = pointStart.x()+myStartItem->size().width();
    qreal endOfEndItemX = pointEnd.x()+myEndItem->size().width();
    if (endOfStartItemX < pointEnd.x() ) {
        pointStart.rx() += myStartItem->size().width() - scrollAreaWidthOffset;
    } else if (endOfEndItemX < pointStart.x()){
        pointEnd.rx() += myEndItem->size().width() - scrollAreaWidthOffset;
    }


    QLineF line(pointStart, pointEnd);
    setLine(line);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 7;
    painter->setPen(myPen);
    painter->setBrush(myColor);

    QLineF myLine = line();
    double angle = ::acos(myLine.dx() / myLine.length());
    if (myLine.dy() >= 0)
        angle = (Pi * 2) - angle;

        QPointF arrowP1 = myLine.p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                        cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = myLine.p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                        cos(angle + Pi - Pi / 3) * arrowSize);
        arrowHead.clear();
        arrowHead << myLine.p1() << arrowP1 << arrowP2;
        painter->drawPolygon(arrowHead);
        painter->drawLine(myLine);
        QRectF myRect( QPointF ( myLine.p2().x() - 3, myLine.p2().y() - 3 ), QSizeF(6, 6));
        painter->drawEllipse(myRect);
}

