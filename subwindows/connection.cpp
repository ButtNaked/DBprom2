#include "subwindows/connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent, Storage *rStorage) :
    QDialog(parent),
    ui(new Ui::Connection),
    storage(rStorage)
{
    ui->setupUi(this);

    QVector<QVector<QString> > *attrTable = storage->getAttrTable();
    QVector<QVector<int> > *vMatrix = storage->getVMatrix();
    QTableWidget *tw = ui->tableWidget;
    QComboBox *cb = ui->comboBox;
    QComboBox *cb2 = ui->comboBox_2;

    tw->setColumnCount(2);
    tw->setRowCount(0);
    tw->setHorizontalHeaderLabels(QStringList() << "Основной" << "Зависимый");

    QStringList tempStl;
    for (int i=0; i<attrTable->count(); i++)    {
        tempStl.append( (*attrTable)[i][1] );
    }

    cb->addItems(tempStl);
    cb2->addItems(tempStl);

    for (int i = 1; i < vMatrix->size(); ++i) {
        for (int j = 1; j < vMatrix->size(); ++j) {
            if ( (*vMatrix)[i][j] == 1) {
                int numberX=(*vMatrix)[i][0];
                int numberY=(*vMatrix)[0][j];
                QString strM;
                QString strS;
                for (int m = 0; m < attrTable->size(); ++m) {
                    if ( numberX == ((*attrTable)[m][0]).toInt() )   {
                        strS=(*attrTable)[m][1];
                    }
                    if ( numberY == ((*attrTable)[m][0]).toInt() )   {
                        strM=(*attrTable)[m][1];
                    }
                }
                QTableWidgetItem *item1 = new QTableWidgetItem(strM);
                QTableWidgetItem *item2 = new QTableWidgetItem(strS);

                tw->insertRow(tw->rowCount());

                tw->setItem(tw->rowCount()-1, 0, item1);
                tw->setItem(tw->rowCount()-1, 1, item2);
            }
        }
    }

    tw->sortItems(0);

}

Connection::~Connection()
{
    delete ui;
}

void Connection::on_addConButton_clicked()
{
    QVector<QVector<QString> > *attrTable = storage->getAttrTable();
    QVector<QVector<int> > *vMatrix = storage->getVMatrix();
    QComboBox *cb = ui->comboBox;
    QComboBox *cb2 = ui->comboBox_2;
    QTableWidget *tw = ui->tableWidget;

    int currentNum = cb->currentIndex();
    int currentNum2 = cb2->currentIndex();

    QString strM = (*attrTable)[currentNum][1];
    QString strS = (*attrTable)[currentNum2][1];

    if (strM==strS) {
        QMessageBox::information(this, "", "Атрибуты для устанавления связи должны быть разными!");
        return;
    }

    QTableWidgetItem *item1 = new QTableWidgetItem(strM);
    QTableWidgetItem *item2 = new QTableWidgetItem(strS);

    tw->insertRow(tw->rowCount());

    tw->setItem(tw->rowCount()-1, 0, item1);
    tw->setItem(tw->rowCount()-1, 1, item2);

    //Добавление связей в vMatrix

    int numberX = -1, numberY = -1;

    for (int i = 0; i < attrTable->size(); ++i) {
        if (strM == (*attrTable)[i][1])   {
            numberY=( (*attrTable)[i][0]).toInt()+1;
        }
        if (strS == (*attrTable)[i][1])   {
            numberX=( (*attrTable)[i][0]).toInt()+1;
        }

    }

    (*vMatrix)[numberX][numberY]= 1;


//    for (int i = 0; i < vMatrix->size(); ++i) {
//        QString str;
//        for (int j = 0; j < vMatrix->size(); ++j) {

//            int tempint = (*vMatrix)[i][j];
//            QString temp;
//            str += temp.setNum(tempint);

//        }
//        qDebug() << str << "\n";
//    }


}

void Connection::on_deleteConButton_clicked()
{
    QVector<QVector<QString> > *attrTable = storage->getAttrTable();
    QVector<QVector<int> > *vMatrix = storage->getVMatrix();
    QTableWidget *tw = ui->tableWidget;

    int iRow = tw->currentRow();
    QTableWidgetItem *item1 = tw->takeItem(iRow,0);
    QTableWidgetItem *item2 = tw->takeItem(iRow,1);
    tw->removeRow(iRow);

    QString strM = item1->text();
    QString strS = item2->text();

    int numberX, numberY;

    for (int i = 0; i < attrTable->size(); ++i) {
        if (strM == (*attrTable)[i][1])   {
            numberY=( (*attrTable)[i][0]).toInt()+1;
        }
        if (strS == (*attrTable)[i][1])   {
            numberX=( (*attrTable)[i][0]).toInt()+1;
        }

    }

    (*vMatrix)[numberX][numberY]= 0;




}

void Connection::on_acceptButton_clicked()
{
    close();
}

void Connection::on_cancelButton_clicked()
{
    close();
}
