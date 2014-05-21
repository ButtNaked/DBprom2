#pragma once

#include "misc/Matrix.h"
#include <QVector>

#define cout qDebug()

class Normalization : public QObject
{
    Q_OBJECT

private:
    Matrix *pInMain;
    QVector<QVector<Matrix *> *> *graphs;

public:
    Normalization(Matrix*, QVector<QVector<Matrix *> *> *rGraphs);
    ~Normalization();
signals:

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

    bool arrayIsNull;
    bool looped;

public:
    Iteration(Matrix *pInMainM);
    ~Iteration();
    void showHeaderM();
    void secondNF();
    void thirdNF();
    void construction();
    void preparing();

    QVector<Matrix *> *getGraphs();
    Matrix* getRemains();
    bool getArrayIsNull();
    bool isLooped();
};

