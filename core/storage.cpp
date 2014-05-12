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

Storage::Storage()
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

void Storage::startNormalization()
{
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

    createMatrix();

    Normalization norm(matrix, graphs);

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
    rOutText="<h3 align= \"center\">Результирующие отношения:</h3>";

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


QList<QListWidget *> *Storage::getWidgetTableList() const
{

    QList<QListWidget *> *list = new QList<QListWidget *>;



    //item1->setTextAlignment(Qt::AlignHCenter);



    for (int i = 0; i < graphs->size(); ++i) {
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            int x = graphs->at(i)->at(j)->getX();
            int y = graphs->at(i)->at(j)->getY();
            QString textHeader = "Таблица: ";
            QString textKeys;
            QString textElem;
            QListWidget *newTable = new QListWidget();
            QListWidgetItem *newItem;
            QIcon icon1(":/pic/key.bmp");

            for (int l = 1; l < y; ++l) {
                int number = ( *graphs->at(i)->at(j) )[0][l];
                textHeader += getTextByNumber(number);
                if (l != y-1)
                    textHeader += "_";


            }
            newTable->setWindowTitle(textHeader);


            for (int l = 1; l < y; ++l) {
                int number = ( *graphs->at(i)->at(j) )[0][l];
                textKeys = getTextByNumber(number);
//                if (l != y-1)
//                    textKeys += " + ";
                newItem = new QListWidgetItem(textKeys);
                newItem->setIcon(icon1);
                newTable->addItem(newItem);
            }



            for (int k = 1; k < x; ++k) {

                int number = ( *graphs->at(i)->at(j) )[k][0];
                textElem = getTextByNumber(number);
                newItem = new QListWidgetItem(textElem);
                newTable->addItem(newItem);
            }

//            int count = newTable->count();
//            newTable->setGeometry(40, 40, 200, 40+count*16);
            list->append(newTable);


        }
    }




    return list;
}

QVector<QVector<QString> > *Storage::getUniTable() const
{
    return uniTable;
}







