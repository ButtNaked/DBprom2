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

void UniversalRelationTable::updateTable(QTableWidgetItem* rItem)
{
    int x  = wt->currentRow(), y = wt->currentColumn();
    (*uniTable)[x][y] = rItem->text();

    if (wt->rowCount()-1 == x)  {
        wt->setRowCount(wt->rowCount()+1);
        QVector<QString> vector(storage->getAttrTable()->count(), QString(""));
        uniTable->append(vector);
    }

}



