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
#include <QByteArray>




class Storage
{
private:
    QVector<QVector<int> > *vMatrix;
    Matrix *matrix;
    QVector<QVector<QString> > *attrTable;
    QVector<QVector<Matrix*>*>* graphs;
    void createMatrix();
    QVector<QVector<QString>> *uniTable;


public:
    Storage();
    ~Storage();
    QVector<QVector<int> > *getVMatrix();
    Matrix *getMatrix();
    QVector<QVector<QString> > *getAttrTable();
    QVector<QVector<Matrix *> *> *getGraphs();
    void setAttrTable(QVector<QVector<QString> > &inAttrTable);
    void setVMatrix(QVector<QVector<int> > &inVMatrix);
    void setUniTable(QVector<QVector<QString>> &inUniTable);
    void startNormalization();
    const QString &getOutputText(QString & rOutText) const;
    int getNumberByText(const QString & rText) const;
    QString getTextByNumber(int rNum) const;
    QList<QListWidget *> *getWidgetTableList() const;
    QVector<QVector<QString> > *getUniTable() const;




};



#endif // STORAGE_H
