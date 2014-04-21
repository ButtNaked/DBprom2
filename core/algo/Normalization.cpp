#include "core/algo/Normalization.h"

myint Iteration::iteration_counter=0;


Iteration::Iteration(Matrix *pInMainM):
    pMainM(pInMainM)
{
    iteration_counter++;

    mainM_x=pInMainM->getX();
    mainM_y=pInMainM->getY();

    cout << "/////////////////////////////////////////////////" << "\n";
    cout << "//////////////////ИТЕРАЦИЯ НОМЕР " << iteration_counter << "///////////////" << "\n";
    cout << "/////////////////////////////////////////////////" << "\n";
    pMainM->show();
    preparing();
    secondNF();
    thirdNF();
    construction();

}

Iteration::~Iteration()
{
    delete pMainMcr;
    delete pSetsM;
    delete pSecondM;
    delete pSecondM2;
    delete pThirdM;

    delete []headerM;

}

void Iteration::showHeaderM()
{
    // Вывод на экран хедера
    cout << "\n";
    for (int i=0;i<headerM_x;i++)
        cout << headerM[i] << " ";
    cout << "\n";
    cout << "\n";
}


QVector<Matrix*>* Iteration::getGraphs()
{
    return vRes;
}


Matrix* Iteration::getRemains()
{
    if (arrayIsNull)
        return nullptr; //nullptr

    return pOutM;


}

bool Iteration::getArrayIsNull()
{
    return arrayIsNull;
}


Normalization::Normalization(Matrix* in, QVector<QVector<Matrix *> *> *rGraphs):
    pInMain(in)
{
	cout << "OKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOK" << "\n";

    Iteration *b;
    Matrix* pRes=pInMain;

    while (1)	{
        b=new Iteration(pRes);
        delete pRes;
        rGraphs->append( b->getGraphs());
        if (b->getArrayIsNull())	{
            delete b;
              return;
        }
        pRes=b->getRemains();
        delete b;

    }


}


Normalization::~Normalization()
{

}






