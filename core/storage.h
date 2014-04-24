#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>
#include <misc/Matrix.h>
#include <core/algo/Normalization.h>
#include <misc/node.h>
#include <QDebug>
#include <QGraphicsItem>
#include <QFont>
#include <QBrush>
#include <QList>
#include <QWidget>
#include <QListWidget>





class Storage
{
private:
    QVector<QVector<int> > *vMatrix;
    Matrix *matrix;
    QVector<QVector<QString> > *attrTable;
    QVector<QVector<Matrix*>*>* graphs;
    void createMatrix();


public:
    Storage();
    ~Storage();
    QVector<QVector<int> > *getVMatrix();
    Matrix *getMatrix();
    QVector<QVector<QString> > *getAttrTable();
    QVector<QVector<Matrix *> *> *getGraphs();
    void startNormalization();
    const QString &getOutputText(QString & rOutText) const;
    int getNumberByText(const QString & rText) const;
    QString getTextByNumber(int rNum) const;
    QList<QListWidget *> *getWidgetTableList() const;


};



#endif // STORAGE_H
