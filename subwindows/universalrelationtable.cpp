#include "universalrelationtable.h"
#include "ui_universalrelationtable.h"

UniversalRelationTable::UniversalRelationTable(QWidget *parent, Storage *rStorage) :
    QMainWindow(parent),
    ui(new Ui::UniversalRelationTable),
    storage(rStorage)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->tableWidget);

    wt = ui->tableWidget;
    wt->setSortingEnabled(false);
    uniTable = storage->getUniTable();
    vMatrix = storage->getVMatrix();


    wt->setColumnCount(storage->getAttrTable()->count());

    for (int i = 0; i < wt->columnCount(); ++i) {
        QTableWidgetItem *item = new QTableWidgetItem(storage->getAttrTable()->at(i).at(1));
        wt->setHorizontalHeaderItem(i, item);
    }

    if (!uniTable->isEmpty())    {
//        qDebug() << "not empty";
        wt->setRowCount(uniTable->size());

        for (int i = 0; i < wt->rowCount(); ++i) {
            for (int j = 0; j < wt->columnCount(); ++j) {
                QTableWidgetItem *item = new QTableWidgetItem(uniTable->at(i).at(j));
                wt->setItem(i, j, item);
            }
        }
    }
    else    {
//        qDebug() << "empty";
        wt->setRowCount(1);
        QVector<QString> vector(storage->getAttrTable()->count(), QString(""));
        uniTable->append(vector);
    }













    connect (ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(updateTable(QTableWidgetItem*)));
}

UniversalRelationTable::~UniversalRelationTable()
{
    delete ui;
}

void UniversalRelationTable::validation()
{
//    QBrush redBackground(Qt::red);
//    rItem->setBackground(redBackground);

    //SuperKeyFinding
    QVector<int> superKey;
    QVector<int> superKeyPos;
    for (int i = 1; i < vMatrix->size(); ++i) {
        for (int j = 1; j < vMatrix->size(); ++j) {
            if ( vMatrix->at(i).at(j) == 1) break;
            else if ( (vMatrix->at(i).at(j) == 0) && ( j == vMatrix->size()-1 ) )   {
                superKey.append( vMatrix->at(i).at(0) );
                superKeyPos.append(i);
            }
        }
    }


    QVector<QVector<int> > position;
    for (int i = 0; i < superKeyPos.size(); ++i) {
        for (int j = 1; j < vMatrix->size(); ++j) {
            if ( (*vMatrix)[j][superKeyPos[i]] == 1)   {
                QVector<int> vec;
                vec.append(superKeyPos[i] -1);
                vec.append(j -1);
                position.append(vec);
            }
        }
    }


//    for (int i = 0; i < position.size(); ++i) {
//        qDebug() << position[i];
//    }

    for (int i = 0; i < position.size(); ++i) {
        checkTuples(position[i][0], position[i][1]);
     }
}

void UniversalRelationTable::checkTuples(int key, int attr)
{
    QList<QString> keyNames;

    for (int i = 0; i < uniTable->size() -1 ; ++i) {
            if ( !(keyNames.contains( (*uniTable)[i][key] )) )  {
                keyNames.append( (*uniTable)[i][key] );
            }
    }

//    qDebug() << keyNames;

    bool isTupleValid;

    for (int i = 0; i < keyNames.size(); ++i) {
        isTupleValid = true;
        QString sample;
        QList<int> tupleRows;
        for (int j = 0; j < uniTable->size() -1 ; ++j) {
            if ( (*uniTable)[j][key] == keyNames[i])   {
               tupleRows.append(j);
               if ( sample.isEmpty() ) sample = (*uniTable)[j][attr];
               else if ( sample != (*uniTable)[j][attr] ) {
                    isTupleValid = false;
               }

            }
        }

        if (!isTupleValid)  {
            for (int n = 0; n < tupleRows.size(); ++n) {
                QBrush redBackground(Qt::red);
                QTableWidgetItem *item1 = wt->item(tupleRows[n], key);
                QTableWidgetItem *item2 = wt->item(tupleRows[n], attr);

                item1->setBackground(QBrush(Qt::yellow));
                item2->setBackground(redBackground);

            }
        }
   }


}

void UniversalRelationTable::updateTable(QTableWidgetItem* rItem)
{
    int x  = wt->currentRow(), y = wt->currentColumn();
    (*uniTable)[x][y] = rItem->text();

    if (wt->rowCount()-1 == x)  {
        wt->setRowCount(wt->rowCount()+1);
        QVector<QString> vector(storage->getAttrTable()->count(), QString(""));
        uniTable->append(vector);
    }



//    for (int i = 0; i < uniTable->size(); ++i) {
//        QString str;
//        for (int j = 0; j < uniTable->at(i).size(); ++j) {
//            str += uniTable->at(i).at(j);
//        }
//        qDebug() << str;
//    }


}




void UniversalRelationTable::on_actionCheck_triggered()
{
    qDebug() << wt->rowCount() << wt->columnCount();
    for (int i = 0; i < wt->rowCount(); ++i) {
        for (int j = 0; j < wt->columnCount(); ++j) {
            QTableWidgetItem *item = wt->item(i, j);
            item->setBackground(QBrush(Qt::white));
        }
    }

    this->validation();
}
