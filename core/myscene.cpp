#include "myscene.h"

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
        this->setSceneRect(0, 0, 731, (1+i/9)*411 );


    //Adding arrows on the scene and paint foreign items in table
    QIcon foreignIcon(":/pic/foreign.bmp");

        for (int i = 0; i < arrowTable.size(); ++i) {
            MyTable *start = list[ arrowTable[i][0] ];
            MyTable *end = list[ arrowTable[i][2] ];
            int startNum = arrowTable[i][1];
            int endNum = arrowTable[i][3];
            Arrow *newArrow = new Arrow(start, startNum, end, endNum);
            this->addItem(newArrow);
            start->addArrow(newArrow);
            end->addArrow(newArrow);

            QBrush foreignItemsBackground(QColor(255, 255, 0, 150));

            QListWidgetItem *item = start->content->item(arrowTable[i][1]);
            item->setBackground(foreignItemsBackground);
            item->setIcon(foreignIcon);
        }



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

                        int innerMaxX = graphs->at(m)->at(n)->getX();
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

    qDebug() << "ArrowTable";
    QString str;
    for (int i = 0; i < arrowTable.size(); i++)	{
        for (int j = 0; j < arrowTable.at(i).size(); j++)   {
            QString temp;
            str += temp.setNum(arrowTable[i][j]) += " ";
        }
        qDebug() << str;
        str.clear();
    }
    qDebug() << "\n";

























}
