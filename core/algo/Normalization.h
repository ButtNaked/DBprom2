#pragma once

#include "misc/Matrix.h"

#include <QVector>
#include <QWidget>

#define cout qDebug()

class Normalization : public QWidget
{
    Q_OBJECT

private:
    Matrix *pInMain;
    QVector<QVector<Matrix *> *> *graphs;

public:
    Normalization(QWidget *parent, Matrix*, QVector<QVector<Matrix *> *> *rGraphs);
    ~Normalization();
signals:
    void loopedMatrix();

};

class Iteration
{
    static myint iteration_counter;
    myint mainM_x, mainM_y; Matrix *pMainM;
    myint mainMcr_x; Matrix *pMainMcr;
    myint headerM_x, *headerM;
    myint setsM_x, setsM_y; Matrix *pSetsM;
    myint secondM_x, secondM_y; Matrix *pSecondM;
    myint secondM2_x, secondM2_y; Matrix *pSecondM2;
    myint thirdM_x, thirdM_y; Matrix *pThirdM;
    myint outM_x, outM_y; Matrix *pOutM;
    QVector<Matrix *> *vRes;
    bool arrayIsNull = 0;
    bool looped;

public:
    Iteration(Matrix *pInMainM);
    ~Iteration();
    void showHeaderM();
    void secondNF();
    void thirdNF();
    void construction();
    void preparing();
    static void resetIterationCounter();
    QVector<Matrix *> *getGraphs();
    Matrix* getRemains();
    bool getArrayIsNull();
    bool isLooped();
};

