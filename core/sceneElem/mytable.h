#ifndef MYTABLE_H
#define MYTABLE_H


#include <QGraphicsProxyWidget>
#include <QListWidget>
#include <QPointF>
#include <QObject>

class Arrow;

class MyTable : public QGraphicsProxyWidget
{
public:
    MyTable(QListWidget *rListWidget);
    void addArrow(Arrow *newArrow);
    QListWidget *content;

private:
    QList<Arrow *> arrowList;

protected:
    void moveEvent(QGraphicsSceneMoveEvent *moveEvent);
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
};

#endif // MYTABLE_H
