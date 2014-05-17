#include "myscene.h"
#include <misc/Matrix.h>
#include <core/algo/Normalization.h>

MyScene::MyScene(Storage *rStorage, QObject *parent) :
    QGraphicsScene(parent)
{
    storage = rStorage;
    this->setBackgroundBrush(QBrush(QColor(240, 248, 255, 255))); // colour AliceBlue

}

void MyScene::fillScene()
{
    graphs = storage->getGraphs();

    this->clear();
    list.clear();
    this->makeArrowTable();

    for (int i = 0; i < graphs->size(); ++i) {
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            int x = graphs->at(i)->at(j)->getX();
            int y = graphs->at(i)->at(j)->getY();
            QString textHeader = tr("Т: ");
            QString textKeys;
            QString textElem;
            QListWidget *newTable = new QListWidget();
            QListWidgetItem *newItem;
            QIcon icon1(":/pic/key.bmp");
            QBrush keyItemsBackground(QColor(0, 255, 127, 150));

            for (int l = 1; l < y; ++l) {
                int number = ( *graphs->at(i)->at(j) )[0][l];
                textHeader += storage->getTextByNumber(number);
                if (l != y-1)
                    textHeader += "_";


            }
            newTable->setWindowTitle(textHeader);


            for (int l = 1; l < y; ++l) {
                int number = ( *graphs->at(i)->at(j) )[0][l];
                textKeys = storage->getTextByNumber(number);
//                if (l != y-1)
//                    textKeys += " + ";
                newItem = new QListWidgetItem(textKeys);
                newItem->setBackground(keyItemsBackground);
                newItem->setIcon(icon1);
                newTable->addItem(newItem);
            }



            for (int k = 1; k < x; ++k) {

                int number = ( *graphs->at(i)->at(j) )[k][0];
                textElem = storage->getTextByNumber(number);
                newItem = new QListWidgetItem(textElem);
                newTable->addItem(newItem);
            }

//            int count = newTable->count();
//            newTable->setGeometry(40, 40, 200, 40+count*16);
            MyTable *newMyTable = new MyTable(newTable);
            list.append(newMyTable);
        }
    }

    // Adding tables on the scene

    int i = 0;
    for (int verticalMod = 0, maxRowCount = 2; i < list.size(); ++i) {

        this->addItem(list.at(i));

        int count = list.at(i)->content->count();
        if (count > maxRowCount)
            maxRowCount=count;

        switch (i/3) {
        case 0:
            break;
        case 1:
            verticalMod = 200;
            break;
        case 2:
            verticalMod = 300;
        default:
            verticalMod = 400;
            break;
        }

        list.at(i)->content->setGeometry(30+(i%3)*240, 40+verticalMod, 200, 45+count*15);
    }
        //this->setSceneRect(0, 0, 731, (1+i/9)*411 );


    //Adding arrows on the scene and paint foreign items in table
    QIcon foreignIcon(":/pic/foreign.bmp");

    for (int i = 0; i < arrowTable.size(); ++i) {
        MyTable *start = list[ arrowTable[i][0] ];
        MyTable *end = list[ arrowTable[i][2] ];
        int startNum = arrowTable[i][1];
        int endNum = arrowTable[i][3];
        Arrow *newArrow = new Arrow(start, startNum, end, endNum);
        newArrow->setColor(Qt::blue);
        this->addItem(newArrow);
        start->addArrow(newArrow);
        end->addArrow(newArrow);
        if (arrowTable[i][4])   {
            QBrush foreignItemsBackground(QColor(255, 255, 0, 150));
            newArrow->setColor(Qt::black);
            QListWidgetItem *item = start->content->item(arrowTable[i][1]);
            item->setBackground(foreignItemsBackground);
            item->setIcon(foreignIcon);
        }
    }
    this->clearFocus();
    this->clearSelection();

}

void MyScene::makeArrowTable()
{
    arrowTable.clear();
    int startTableCounter  = 0;

    for (int i = 0; i < graphs->size(); ++i) {
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            int maxX = graphs->at(i)->at(j)->getX();
            int maxY = graphs->at(i)->at(j)->getY();

            for (int f = 1; f < maxX; ++f) {
                int startNum = (*graphs->at(i)->at(j))[f][0];

                int endTableCounter = 0;
                for (int m = 0; m < graphs->size(); ++m) {
                    for (int n = 0; n < graphs->at(m)->size(); ++n) {

                        //int innerMaxX = graphs->at(m)->at(n)->getX();
                        int innerMaxY = graphs->at(m)->at(n)->getY();

                        for (int k = 1; k < innerMaxY; ++k) {

                            int endNum = (*graphs->at(m)->at(n))[0][k];

                            if (startTableCounter == endTableCounter) break;
                            if (startNum == endNum) {
                                QVector<int> vector;
                                vector.append(startTableCounter);
                                vector.append( f - 1 + ( maxY - 1) );
                                vector.append(endTableCounter);
                                vector.append(k - 1);
                                vector.append(1);
                                arrowTable.append(vector);
                            }

                        }

                        endTableCounter++;
                    }
                }

            }
            startTableCounter++;
        }
    }

//////////////////////////////////////////////////
/// key to key relation arrows
//////////////////////////////////////////////////

    struct KeyAttrElem
    {
        int number;
        QSet<int> set;
        bool operator()(const KeyAttrElem& a, const KeyAttrElem& b) const
        {
          return a.set.size() > b.set.size();
        }
    };

    QVector<KeyAttrElem> keyAttributes;

    int keyConnectionSize = graphs->at(0)->size() + 1;
    Matrix *keyCon = new Matrix(keyConnectionSize, keyConnectionSize);
    keyCon->fillWithNumber(0);
    for (int i = 1; i < keyConnectionSize; ++i) {
        (*keyCon)[i][0] = i-1;
        (*keyCon)[0][i] = i-1;
    }

    for (int j = 0; j < graphs->at(0)->size(); ++j) {        
        int maxY = graphs->at(0)->at(j)->getY();
        KeyAttrElem newElem;
        newElem.number = j;
        for (int k = 1; k < maxY; ++k) {
            newElem.set << (*graphs->at(0)->at(j))[0][k];
        }
        keyAttributes.append(newElem);
    }

    qSort(keyAttributes.begin(), keyAttributes.end(), //Sort with lambda functor
          [](const KeyAttrElem& a, const KeyAttrElem& b){return a.set.size() > b.set.size(); } );

//    foreach (KeyAttrElem elem, keyAttributes) {
//        qDebug() << elem.number << elem.set.toList();
//    }


    QVector<QVector<int> > keyAttrTemp;

    int i = 0;
    while ( keyAttributes[i].set.size() != 1 ) {
        for (int j = 0; j < keyAttributes.size(); ++j) {
            if ( i == j ) continue;
            else if (keyAttributes[i].set.contains(keyAttributes[j].set) )   {
                QVector<int> newVec;
                newVec.append(keyAttributes[i].number);
                newVec.append(keyAttributes[j].number);
                keyAttrTemp.append(newVec);
            }

        }
        i++;
    }


    foreach (QVector<int> vec, keyAttrTemp) {
        (*keyCon)[ vec[1]+1 ][ vec[0]+1 ] = 1;
    }
    //keyCon->show();
    QVector<QVector<Matrix*>*> *out = new QVector<QVector<Matrix *> *>;
    Normalization norm(keyCon, out);

//    qDebug() << "*******test************************";
//    for (int i = 0; i < out->size(); ++i) {
//        for (int j = 0; j < out->at(i)->size(); ++j) {
//            out->at(i)->at(j)->show();
//        }
//    }
//    qDebug() << "*******************************";

    keyAttrTemp.clear();
    for (int i = 0; i < out->size(); ++i) {
        for (int j = 0; j < out->at(i)->size(); ++j) {
            int x = out->at(i)->at(j)->getX();
            int y = out->at(i)->at(j)->getY();
            for (int q = 1; q < x; ++q) {
                for (int p = 1; p < y; ++p) {
                    QVector<int> newVec;
                    newVec.append( (*out->at(i)->at(j))[0][p] );
                    newVec.append( (*out->at(i)->at(j))[q][0] );
                    keyAttrTemp.append(newVec);
                }
            }
        }
    }



//    qDebug() << "ketAttrTemp";
//    QString str;
//    for (int i = 0; i < keyAttrTemp.size(); i++)	{
//        for (int j = 0; j < keyAttrTemp.at(i).size(); j++)   {
//            QString temp;
//            str += temp.setNum(keyAttrTemp[i][j]) += " ";
//        }
//        qDebug() << str;
//        str.clear();
//    }
//    qDebug() << "\n";


    for (int i = 0; i < keyAttrTemp.size(); ++i) {
        int matrixStart = keyAttrTemp[i][0];
        int matrixEnd = keyAttrTemp[i][1];
        int maxStartY = graphs->at(0)->at(matrixStart)->getY();
        int maxEndY = graphs->at(0)->at(matrixEnd)->getY();

        for (int m = 1; m < maxStartY; ++m)    {
            int startNum = (*graphs->at(0)->at(matrixStart))[0][m];
            for (int n = 1; n < maxEndY; ++n)    {
                int endNum = (*graphs->at(0)->at(matrixEnd))[0][n];
                if ( startNum == endNum )   {
                    QVector<int> vector;
                    vector.append(matrixStart);
                    vector.append( m -1 );
                    vector.append(matrixEnd);
                    vector.append( n - 1);
                    vector.append(0);
                    arrowTable.append(vector);
                }
            }
        }
    }


//    qDebug() << "ArrowTable";
//    QString str;
//    for (int i = 0; i < arrowTable.size(); i++)	{
//        for (int j = 0; j < arrowTable.at(i).size(); j++)   {
//            QString temp;
//            str += temp.setNum(arrowTable[i][j]) += " ";
//        }
//        qDebug() << str;
//        str.clear();
//    }
//    qDebug() << "\n";

}
