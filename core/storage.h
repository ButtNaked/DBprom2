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




class Storage : public QObject
{
    Q_OBJECT

private:
    bool upToDate = false;
    QVector<QVector<int> > *vMatrix;
    Matrix *matrix;
    QVector<QVector<QString> > *attrTable;
    QVector<QVector<Matrix*>*>* graphs;
    QVector<QVector<QString>> *uniTable;
    QVector<int> superKey;
    QString dbName;

    void createMatrix();
    bool checkingData();

public:
    Storage(QObject *parent = 0);
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
    bool isNormalizeUpdated() const;
    void setNormalizeChanged();
    void setNormalizeUpToDate();
    const QVector<int>& getSuperKey() const;
    void updateSuperKey();
    QString getSuperKeytoString() const;
    void setdbName(const QString &rdbName);
    const QString& getdbName() const;
    void clear();
signals:

};



#endif // STORAGE_H
