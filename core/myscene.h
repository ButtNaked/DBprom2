#ifndef MYSCENE_H
#define MYSCENE_H

#include "core/sceneElem/arrow.h"
#include "core/sceneElem/mytable.h"
#include "core/storage.h"

#include <QGraphicsScene>

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(Storage *rStorage, QObject *parent = 0);
    void fillScene();

private:
    Storage *storage;
    QVector<QVector<Matrix*>*>* graphs;
    QVector<QVector<int>> arrowTable;
    QList<MyTable *> list;

    void makeArrowTable();
signals:

public slots:

};

#endif // MYSCENE_H
