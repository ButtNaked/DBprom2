#include "subwindows/connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent, Storage *rStorage) :
    QDialog(parent),
    ui(new Ui::Connection),
    storage(rStorage)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Add connection [DB prom]"));

    attrTable = storage->getAttrTable();
    vMatrix = storage->getVMatrix();
    tw = ui->tableWidget;
    cb = ui->comboBox;
    cb2 = ui->comboBox_2;

    tw->setColumnCount(2);
    tw->setRowCount(0);
    tw->setHorizontalHeaderLabels(QStringList() << tr("Основной") << tr("Зависимый"));

    QStringList tempStl;
    for (int i=0; i<attrTable->count(); i++)    {
        tempStl.append( (*attrTable)[i][1] );
    }

    cb->addItems(tempStl);
    cb2->addItems(tempStl);
    cb2->setCurrentIndex(1);

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
    int currentNum = cb->currentIndex();
    int currentNum2 = cb2->currentIndex();

    QString strM = (*attrTable)[currentNum][1];
    QString strS = (*attrTable)[currentNum2][1];

    if (strM==strS) {
        QMessageBox::information(this, "", tr("Атрибуты для устанавления связи должны быть разными!"));
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
            numberY=( (*attrTable)[i][0]).toInt();
        }
        if (strS == (*attrTable)[i][1])   {
            numberX=( (*attrTable)[i][0]).toInt();
        }

    }

    int coordX = -1;
    int coordY = -1;
    for (int i = 1; i < vMatrix->size(); ++i) {
        if (numberX == vMatrix->at(i).at(0))    coordX = i;
        if (numberY == vMatrix->at(0).at(i))    coordY = i;
    }

    if ( ( coordX != -1 ) && ( coordY != -1 ) )  {
        (*vMatrix)[coordX][coordY]= 1;
    }   else qDebug() << "vMatrix write error...";



//    for (int i = 0; i < vMatrix->size(); ++i) {
//        QString str;
//        for (int j = 0; j < vMatrix->size(); ++j) {

//            int tempint = (*vMatrix)[i][j];
//            QString temp;
//            str += temp.setNum(tempint);

//        }
//        qDebug() << str << "\n";
//    }

//    for (int i = 0; i < attrTable->size(); ++i) {
//        qDebug() << (*attrTable)[i][0] << (*attrTable)[i][1];
//    }
//    qDebug() << "_____________________";

    storage->somethingChanged();
}

void Connection::on_deleteConButton_clicked()
{
    int curRow = tw->currentRow();
    if (curRow == -1) {
        QMessageBox::information(this, "", tr("Для удаления сначала выберите нужную связь."));
        return;
    }
    QTableWidgetItem *item1 = tw->takeItem(curRow,0);
    QTableWidgetItem *item2 = tw->takeItem(curRow,1);
    tw->removeRow(curRow);

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

    storage->somethingChanged();
}

void Connection::on_acceptButton_clicked()
{
    close();
}

void Connection::on_cancelButton_clicked()
{
    close();
}
