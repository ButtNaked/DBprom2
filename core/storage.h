#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>
#include <misc/Matrix.h>
#include <core/algo/Normalization.h>
#include <misc/node.h>
#include <QDebug>


class Storage
{
private:
    QVector<QVector<int> > *vMatrix;
    Matrix *matrix;
    QVector<QVector<QString> > *attrTable;
    QVector<QVector<Matrix*>*>* graphs;
    void equalMatrix();


public:
    Storage();
    ~Storage();
    QVector<QVector<int> > *getVMatrix();
    Matrix *getMatrix();
    QVector<QVector<QString> > *getAttrTable();
    QVector<QVector<Matrix *> *> *getGraphs();
    void startNormalization();


};



#endif // STORAGE_H
