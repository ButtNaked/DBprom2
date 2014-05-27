#include "universalrelationtable.h"
#include "ui_universalrelationtable.h"

#include <QMessageBox>

UniversalRelationTable::UniversalRelationTable(QWidget *parent, Storage *rStorage) :
    QMainWindow(parent),
    ui(new Ui::UniversalRelationTable),
    storage(rStorage)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setCentralWidget(ui->tableWidget);
    this->setWindowTitle(tr("Универсальное отношение [DB prom]"));

    wt = ui->tableWidget;
    wt->setSortingEnabled(false);
    uniTable = storage->getUniTable();
    vMatrix = storage->getVMatrix();
    attrTable = storage->getAttrTable();

    this->fillTableWidget();

    connect (ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(updateTable(QTableWidgetItem*)));
}

UniversalRelationTable::~UniversalRelationTable()
{
    delete ui;
}

void UniversalRelationTable::fillTableWidget()
{
    wt->setColumnCount(attrTable->count());

    if (!uniTable->isEmpty())    {
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
}

void UniversalRelationTable::tuplesValidation()
{
    isAllTuplesValid = true;
    //Check is storage up to date
    if(!storage->isNormalizeUpdated()) {
        storage->startNormalization();
        //QMessageBox::information(this, tr(""), tr("Обновленно."));
    }

    //Making rule array, that consist all rules.
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

    //Calling checkRule method for every rule
    foreach (Rule rule, rulesList) {
        qDebug() << rule.keys << rule.attr;
        qDebug() << "******";
        checkRule(rule);
    }
    if (isAllTuplesValid) {
        QMessageBox::information(this, tr("Проверка корректности кортежей"), tr("Все кортежи корректны."));
    }
    else QMessageBox::information(this, tr("Проверка корректности кортежей"),
                                  tr("Найдены некорректные кортежи. Красным выделен несовпадающий атрибут кортежа, желтым - его ключ."));

}

void UniversalRelationTable::checkRule(Rule &rRule) //method cheking rule and paint tablewidget items
{
    //Conversion attributes index values in positional values
    Rule rule = rRule;
    for (int i = 0; i < uniTable->at(0).size(); ++i) {
        for (int q = 0; q < rRule.keys.size(); ++q) {
            if ( rRule.keys.at(q) == uniTable->at(0).at(i).toInt()) {
                rule.keys[q] = i;
            }
        }
        if ( uniTable->at(0).at(i).toInt() == rRule.attr) rule.attr = i;
    }

    // Gather all unique key-tuples in one QList
    QList<QString> uniqueKeyTuples;

    for (int i = 1; i < uniTable->size() -1 ; ++i) {
        QString keyTuple;
        for (int j = 0; j < rule.keys.size(); ++j) {
            keyTuple += (*uniTable)[i][rule.keys[j]];
        }
            if ( !(uniqueKeyTuples.contains( keyTuple )) )  {
                uniqueKeyTuples.append( keyTuple );
            }
    }

    //Calculating for every unique keyTuple his range of rows and store it in tupleRowsPos.
    //Next cheking this range for depend attibutes, if attribute tuples not equal, then
    //set tupleValidFlag = false.
    //This actions reapiting for every unique key-tuple in uniqueKeyTuples array.
    bool tupleValidFlag;

    for (int i = 0; i < uniqueKeyTuples.size(); ++i) {

        tupleValidFlag = true;
        QString dependAttrTuple;
        QList<int> tupleRowsPos;
        for (int j = 1; j < uniTable->size() -1 ; ++j) {

            QString keyTuple;
            for (int k = 0; k < rule.keys.size(); ++k) {
                keyTuple += (*uniTable)[j][rule.keys[k]];
            }

            if ( keyTuple == uniqueKeyTuples[i])   {
               tupleRowsPos.append(j - 1);
               if ( dependAttrTuple.isEmpty() ) dependAttrTuple = (*uniTable)[j][rule.attr];
               else if ( dependAttrTuple != (*uniTable)[j][rule.attr] ) {
                    tupleValidFlag = false;
               }
            }
        }

    // If flag is not true, paint appopriate tableWidget items
        //qDebug() << rule.keys << rule.attr;
        if (!tupleValidFlag)  {
            isAllTuplesValid = false;
            for (int n = 0; n < tupleRowsPos.size(); ++n) {
                QBrush redBackground(Qt::red);
                for (int q = 0; q < rule.keys.size(); ++q) {
                    QTableWidgetItem *item1 = wt->item(tupleRowsPos[n], rule.keys[q]);
                    item1->setBackground(QBrush(Qt::yellow));
                }
                QTableWidgetItem *item2 = wt->item(tupleRowsPos[n], rule.attr);
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
    //repaint all (almost) TableWidget items in white color
    for (int i = 0; i < wt->rowCount() - 1 ; ++i) {
        for (int j = 0; j < wt->columnCount(); ++j) {
            QTableWidgetItem *item = wt->item(i, j);
            item->setBackground(QBrush(Qt::white));
        }
    }

    this->tuplesValidation();
}
