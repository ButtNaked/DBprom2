#ifndef STORAGE_H
#define STORAGE_H

#include <misc/Matrix.h>
#include <core/algo/Normalization.h>

#include <QObject>
#include <QDebug>
#include <QGraphicsItem>
#include <QFont>
#include <QBrush>
#include <QList>
#include <QWidget>
#include <QListWidget>
#include <QByteArray>
#include <QMessageBox>

class Storage : public QWidget
{
    Q_OBJECT

private:
    bool upToDate = false;
    QVector<QVector<int> > *vMatrix;
    Matrix *matrix;
    QVector<QVector<QString> > *attrTable;
    QVector<QVector<Matrix*>*>* graphs;
    QVector<QVector<QString>> *uniTable;
    QString superKeyString;
    QString dbName;

    void createMatrix();
    bool checkingData();

public:
    Storage(QWidget *parent = 0);
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
    const QString getSuperKeyString();
    void setdbName(const QString &rdbName);
    const QString& getdbName() const;
    void clear();

signals:
    void loopedMatrix();
};



#endif // STORAGE_H
