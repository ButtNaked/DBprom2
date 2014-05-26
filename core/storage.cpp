#include "core/storage.h"

void Storage::createMatrix()
{
    int size = vMatrix->size();
    matrix = new Matrix(size, size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            (*matrix)[i][j]=(*vMatrix)[i][j];
        }
    }

//        for (int i = 0; i < vMatrix->size(); ++i) {
//            QString str;
//            for (int j = 0; j < vMatrix->size(); ++j) {

//                int tempint = (*vMatrix)[i][j];
//                QString temp;
//                str += temp.setNum(tempint);

//            }
//            qDebug() << str;
//        }

//    int size=14;
//    matrix = new Matrix(size, size);
//    int arr[14][14] = {
//            { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12},
//            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            { 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            { 4, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            { 5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
//            { 6, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
//            { 7, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            { 8, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            { 9, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            {10, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            {11, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            {12, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//        };

//    for (int i = 0; i < size; ++i) {
//        for (int j = 0; j < size; ++j) {
//            (*matrix)[i][j] = arr[i][j];
//        }
//    }

}

Storage::Storage(QObject *parent):
    QObject(parent)

{
    vMatrix = new QVector<QVector<int> > (1, QVector<int>(1, 0));
    attrTable = new QVector<QVector<QString> >;
    graphs = new QVector<QVector<Matrix *> *>;
    uniTable = new QVector<QVector<QString>>;

//    for(int i = 0; i < 10; i++)
//    {
//        QVector<QString> vec;
//        for(int j = 0; j < 2; j++)
//            vec.push_back(QString::number(i));
//        attrTable->push_back(vec);
//    }



}

Storage::~Storage()
{
    delete vMatrix;
    delete attrTable;

    for (int i = 0; i < graphs->size(); ++i) {
        QVector<Matrix *> *pTemp2;
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            Matrix *pTemp;
            pTemp = graphs->at(i)->at(j);
            delete pTemp;
        }
        pTemp2 = graphs->at(i);
        delete pTemp2;
    }

    delete graphs;

    delete uniTable;

}

QVector<QVector<int> > *Storage::getVMatrix()
{
    return vMatrix;
}

Matrix *Storage::getMatrix()
{
    return matrix;
}

QVector<QVector<QString> > *Storage::getAttrTable()
{
    return attrTable;
}

QVector<QVector<Matrix *> *> *Storage::getGraphs()
{
    return graphs;
}

void Storage::setAttrTable(QVector<QVector<QString> > &inAttrTable)
{
    delete attrTable;
    attrTable = new QVector<QVector<QString> >;
    (*attrTable) = inAttrTable;
}

void Storage::setVMatrix(QVector<QVector<int> > &inVMatrix)
{
    delete vMatrix;
    vMatrix = new QVector<QVector<int> >;
    (*vMatrix) = inVMatrix;
}

void Storage::setUniTable(QVector<QVector<QString> > &inUniTable)
{
    delete uniTable;
    uniTable = new QVector<QVector<QString> >;
    (*uniTable) = inUniTable;
}

void Storage::startNormalization()
{
    Iteration::resetIterationCounter();

    for (int i = 0; i < graphs->size(); ++i) {
        QVector<Matrix *> *pTemp2;
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            Matrix *pTemp;
            pTemp = graphs->at(i)->at(j);
            delete pTemp;
        }
        pTemp2 = graphs->at(i);
        delete pTemp2;
    }

    graphs->clear();

    if (!checkingData()) {
        qDebug() << "Checking is failed\n";
        return;
    }

    createMatrix();
    matrix->show();
    Normalization norm(matrix, graphs);
    Iteration::resetIterationCounter();

    qDebug() << "*******************************";
    for (int i = 0; i < graphs->size(); ++i) {
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            graphs->at(i)->at(j)->show();
        }
    }
    qDebug() << "*******************************";
}

const QString &Storage::getOutputText(QString &rOutText) const
{
    rOutText="<h3 align= \"center\">Cхема отношений:</h3>";

    for (int i = 0; i < graphs->size(); ++i) {
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            int x = graphs->at(i)->at(j)->getX();
            int y = graphs->at(i)->at(j)->getY();
            QString textHeader;
            QString textKeys;
            QString textElem;


            for (int l = 1; l < y; ++l) {
                int number = ( *graphs->at(i)->at(j) )[0][l];
                textHeader += getTextByNumber(number);
                if (l != y-1)
                    textHeader += "_";
            }

            for (int l = 1; l < y; ++l) {
                int number = ( *graphs->at(i)->at(j) )[0][l];
                textKeys += getTextByNumber(number);
                if (l != y-1)
                    textKeys += ", ";
            }

            for (int k = 1; k < x; ++k) {

                int number = ( *graphs->at(i)->at(j) )[k][0];
                textElem += getTextByNumber(number);
                if (k != x-1)
                        textElem += ", ";
            }

            rOutText += QString("%1 { <u>%2</u>, %3 }<br>").arg(textHeader).arg(textKeys).arg(textElem);


        }
    }


    return rOutText;
}

int Storage::getNumberByText(const QString &rText) const
{

    for (int i = 0; i < attrTable->size(); ++i) {
        if ( (*attrTable)[i][1] == rText)
            return (*attrTable)[i][0].toInt();
    }

    return -1;
}

QString Storage::getTextByNumber(int rNum) const
{
    QString tNum = QString::number(rNum);
    for (int i = 0; i < attrTable->size(); ++i) {
        if ( (*attrTable)[i][0] == tNum)
            return (*attrTable)[i][1];
    }
    return "Error";

}

QVector<QVector<QString> > *Storage::getUniTable() const
{
    return uniTable;
}

bool Storage::isNormalizeUpdated() const
{
    return upToDate;
}

void Storage::setNormalizeChanged()
{
    if (upToDate == true)
        upToDate = false;
}

void Storage::setNormalizeUpToDate()
{
    if (upToDate == false)
        upToDate = true;
}

const QString& Storage::getSuperKeyString()
{
    if (upToDate) return superKeyString;

    QVector<int> superKey;
    for (int i = 1; i < vMatrix->size(); ++i) {
        for (int j = 1; j < vMatrix->size(); ++j) {
            if ( vMatrix->at(i).at(j) == 1) break;
            else if ( (vMatrix->at(i).at(j) == 0) && ( j == vMatrix->size()-1 ) )   {
                superKey.append( vMatrix->at(i).at(0) );
            }
        }
    }

   superKeyString.clear();
   for (int q = 0; q < superKey.size(); ++q) {
       for (int i = 0; i < attrTable->size(); ++i) {
           if ( (*attrTable)[i][0] == QString::number(superKey[q]) )   {
                superKeyString += (*attrTable)[i][1];
                superKeyString += " ";
           }
       }
   }


   superKeyString = QString(tr("Ключ универсального отношения: ")) + superKeyString;

   return superKeyString;
}

void Storage::setdbName(const QString &rdbName)
{
    dbName = rdbName;
}

const QString& Storage::getdbName() const
{
    return dbName;
}

void Storage::clear()
{
    attrTable->clear();
    vMatrix->clear();
    vMatrix->append(QVector<int>(1, 0));
    uniTable->clear();
    upToDate=false;
    dbName.clear();
    qDebug() << "Storage has been cleared";
}

bool Storage::checkingData()
{
    if (attrTable->isEmpty() || vMatrix->isEmpty()) {
        qDebug() << "Storage::checkingData, Data is not valid.";
        return false;
    }

    if (vMatrix->size() <= 2)   {
        qDebug() << "Матрица пуста или состоит из одного атрибута.";
        return false;
    }

    int limit = vMatrix->size();
    bool connectionNotExist = true;
    for (int i = 1; i < limit; ++i) {
        for (int j = 1; j < limit; ++j) {
            if( (*vMatrix)[i][j] == 1) connectionNotExist = false;
        }
    }

    if (connectionNotExist) {
        return false;
    }

    return true;
}







