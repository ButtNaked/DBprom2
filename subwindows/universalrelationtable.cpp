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
    attrTable = storage->getAttrTable();


    wt->setColumnCount(attrTable->count());

    if (!uniTable->isEmpty())    {
//        qDebug() << "not empty";

        for (int i = 0; i < wt->columnCount(); ++i) {
            QString headerTextItem = uniTable->at(0).at(i);
            QTableWidgetItem *item = new QTableWidgetItem(storage->getTextByNumber(headerTextItem.toInt()));
            wt->setHorizontalHeaderItem(i, item);
        }
        wt->setRowCount( uniTable->count() - 1 );

        for (int i = 1; i < uniTable->count(); ++i) {
            for (int j = 0; j < uniTable->at(i).count(); ++j) {
                QTableWidgetItem *item = new QTableWidgetItem(uniTable->at(i).at(j));
                wt->setItem(i - 1, j, item);
            }
        }
    }
    else    {
//        qDebug() << "empty";

        for (int i = 0; i < wt->columnCount(); ++i) {
            QTableWidgetItem *item = new QTableWidgetItem(attrTable->at(i).at(1));
            wt->setHorizontalHeaderItem(i, item);
        }
        wt->setRowCount(1);

        QVector<QString> header;
        foreach (QVector<QString> vec, (*attrTable)) {
            header.append(vec[0]);
        }
        uniTable->append(header);
        QVector<QString> vector(attrTable->count(), QString(""));
        uniTable->append(vector);
    }













    connect (ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(updateTable(QTableWidgetItem*)));
}

UniversalRelationTable::~UniversalRelationTable()
{
    delete ui;
}

struct Rule
{
    QVector<int> keys;
    int attr;
};

void UniversalRelationTable::validation()
{
//    QBrush redBackground(Qt::red);
//    rItem->setBackground(redBackground);
    if(!storage->isUpToDate()) {qDebug() << "Need updated normalization"; return;}

    //SuperKeyFinding
//    QVector<int> superKey;
//    QVector<int> superKeyPos;
//    for (int i = 1; i < vMatrix->size(); ++i) {
//        for (int j = 1; j < vMatrix->size(); ++j) {
//            if ( vMatrix->at(i).at(j) == 1) break;
//            else if ( (vMatrix->at(i).at(j) == 0) && ( j == vMatrix->size()-1 ) )   {
//                superKey.append( vMatrix->at(i).at(0) );
//                superKeyPos.append(i);
//            }
//        }
//    }

    QList<Rule> rulesList;
    QVector<QVector<Matrix*>*>* graphs = storage->getGraphs();
    for (int i = 0; i < graphs->size(); ++i) {
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            int maxX = graphs->at(i)->at(j)->getX();
            int maxY = graphs->at(i)->at(j)->getY();
            for (int q = 1; q < maxX; ++q) {
                Rule newRule;
                newRule.attr = (*graphs->at(i)->at(j))[q][0];
                for (int p = 1; p < maxY; ++p) {
                    newRule.keys.append((*graphs->at(i)->at(j))[0][p]);
                }
                rulesList.append(newRule);
            }

        }
    }

    foreach (Rule rule, rulesList) {
//        qDebug() << rule.keys << rule.attr;
        foreach (int key, rule.keys) {
            checkRule(key, rule.attr);
        }
    }

}

void UniversalRelationTable::checkRule(int key, int attr)
{
    int keyPos = -1;
    int attrPos = -1;
    for (int i = 0; i < uniTable->at(0).size(); ++i) {
        if ( uniTable->at(0).at(i).toInt() == key) keyPos = i;
        if ( uniTable->at(0).at(i).toInt() == attr) attrPos = i;
    }

    if (  (keyPos == -1) || (attrPos == -1))    {
        qDebug() << "Check rule Error";
        return;
    }

    QList<QString> keyNames;

    for (int i = 1; i < uniTable->size() -1 ; ++i) {
            if ( !(keyNames.contains( (*uniTable)[i][keyPos] )) )  {
                keyNames.append( (*uniTable)[i][keyPos] );
            }
    }

////    qDebug() << keyNames;

    bool isTupleValid;

    for (int i = 0; i < keyNames.size(); ++i) {
        isTupleValid = true;
        QString sample;
        QList<int> tupleRows;
        for (int j = 1; j < uniTable->size() -1 ; ++j) {
            if ( (*uniTable)[j][keyPos] == keyNames[i])   {
               tupleRows.append(j - 1);
               if ( sample.isEmpty() ) sample = (*uniTable)[j][attrPos];
               else if ( sample != (*uniTable)[j][attrPos] ) {
                    isTupleValid = false;
               }

            }
        }

        if (!isTupleValid)  {
            for (int n = 0; n < tupleRows.size(); ++n) {
                QBrush redBackground(Qt::red);
                QTableWidgetItem *item1 = wt->item(tupleRows[n], keyPos);
                QTableWidgetItem *item2 = wt->item(tupleRows[n], attrPos);

                item1->setBackground(QBrush(Qt::yellow));
                item2->setBackground(redBackground);

            }
        }
   }


}

void UniversalRelationTable::updateTable(QTableWidgetItem* rItem)
{
    int row  = rItem->row(), column = rItem->column();
    (*uniTable)[row + 1][column] = rItem->text();

    if ( (wt->rowCount() - 1) == row)  {
        wt->insertRow(wt->rowCount());
        QVector<QString> vector(storage->getAttrTable()->count(), QString(""));
        uniTable->append(vector);
    }

//    qDebug() << "UniTable";
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
//    qDebug() << wt->rowCount() << wt->columnCount();
    for (int i = 0; i < wt->rowCount() - 1 ; ++i) {
        for (int j = 0; j < wt->columnCount(); ++j) {
            QTableWidgetItem *item = wt->item(i, j);
            item->setBackground(QBrush(Qt::white));
        }
    }

    this->validation();
}
