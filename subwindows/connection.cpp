#include "subwindows/connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent, Storage *rStorage) :
    QDialog(parent),
    ui(new Ui::Connection),
    storage(rStorage)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Добавление связи [DB prom]"));

    attrTable = storage->getAttrTable();
    vMatrix = storage->getVMatrix();
    tw = ui->tableWidget;

    QVector<QComboBox*> vecM;
    vecM.append(ui->comboBox_M1);
    vecM.append(ui->comboBox_M2);
    vecM.append(ui->comboBox_M3);
    vecM.append(ui->comboBox_M4);
    QVector<QComboBox*> vecS;
    vecS.append(ui->comboBox_S1);
    vecS.append(ui->comboBox_S2);
    vecS.append(ui->comboBox_S3);
    vecS.append(ui->comboBox_S4);
    ptrComboBoxes.append(vecM);
    ptrComboBoxes.append(vecS);

    tw->setColumnCount(2);
    tw->setColumnWidth(0, 210);
    tw->setColumnWidth(1, 210);
    tw->setRowCount(0);
    tw->setHorizontalHeaderLabels(QStringList() << tr("Основной") << tr("Зависимый"));

    QString itemStr(tr(""));
    for (int q = 0; q < 2; ++q) {
        for (int p = 1; p < 4; ++p) {
            ptrComboBoxes[q][p]->addItem(itemStr);
        }
    }

    for (int i=0; i<attrTable->count(); i++)    {
        QString itemStr = (*attrTable)[i][1];
        QVariant itemNum = (*attrTable)[i][0];
        for (int q = 0; q < 2; ++q) {
            for (int p = 0; p < 4; ++p) {
                ptrComboBoxes[q][p]->addItem(itemStr, itemNum);
            }
        }
    }

    ptrComboBoxes[1][0]->setCurrentIndex(1);

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

    tw->sortItems(1);
}

Connection::~Connection()
{
    delete ui;
}

void Connection::on_addConButton_clicked()
{
    QVector<QVector<QString> > difficultConnection;
    QString empty("empty");

    for (int i = 0; i < 2; ++i) {
        difficultConnection.append(QVector<QString>());
        for (int j = 0; j < 4; ++j) {
            if ( ptrComboBoxes[i][j]->currentData().isValid())  {
                QString currentNum = ptrComboBoxes[i][j]->currentData().value<QString>();
                difficultConnection[i].append(currentNum);
            }
            else {
                difficultConnection[i].append(empty);
            }
        }
    }

//    for (int i = 0; i < 2; ++i) {
//        QString temp;
//        for (int j = 0; j < 4; ++j) {
//            temp += difficultConnection[i][j];
//        }
//        qDebug() << temp << " ";
//    }
    //Проверка на совпадение атрибутов
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (difficultConnection[i][j] == empty) continue;
            for (int q = 0; q < 2; ++q) {
                for (int p = 0; p < 4; ++p) {
                    if (difficultConnection[q][p] == empty) continue;
                    else if (i == q && j == p) continue;
                    else if (difficultConnection[i][j] == difficultConnection[q][p]) {
                        QMessageBox::information(this, "", tr("Атрибуты для устанавления связи должны быть разными!"));
                        return;
                    }
                }
            }
        }
    }
    //Вызов функции заполнения для каждого пары атрибутов.
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if ( difficultConnection[0][i] != empty &&  difficultConnection[1][j] != empty )  {
              addSimpleConnection(difficultConnection[0][i], difficultConnection[1][j]);
              //qDebug() << difficultConnection[0][i] << difficultConnection[1][j];
            }
        }
    }

    //tw->scrollToBottom();
    tw->sortByColumn(1);

    storage->setNormalizeChanged();
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

    storage->setNormalizeChanged();
}

void Connection::on_cancelButton_clicked()
{
    close();
}

void Connection::addSimpleConnection(const QString& currentNum, const QString& currentNum2) const
{
    QString strM, strS;
    for (int i = 0; i < attrTable->size(); ++i) {
        if ( currentNum == (*attrTable)[i][0] ) {
            strM = (*attrTable)[i][1];
            //qDebug()<<"strM"<< strM;
            break;
        }
    }
    for (int i = 0; i < attrTable->size(); ++i) {
        if ( currentNum2 == (*attrTable)[i][0] ) {
            strS = (*attrTable)[i][1];
            //qDebug()<<"strS"<< strS;
            break;
        }
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
}
