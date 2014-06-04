#include "subwindows/connection.h"
#include "ui_connection.h"
#include "mainwindow.h"


Connection::Connection(QWidget *parent, Storage *rStorage) :
    QDialog(parent),
    ui(new Ui::Connection),
    storage(rStorage)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle(tr("Добавление связи [DB prom]"));

    attrTable = storage->getAttrTable();
    vMatrix = storage->getVMatrix();
    tw = ui->tableWidget;
    tw->setAlternatingRowColors(true);

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
    tw->setColumnWidth(0, 297);
    tw->setColumnWidth(1, 147);
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
        bool isAttributeDependable = false;
        for (int j = 1; j < vMatrix->size(); ++j)   {
            if ((*vMatrix)[i][j] == 1) isAttributeDependable = true;
         }
        if (isAttributeDependable)  {
            QString strM;
            QString strS;
            QVariantList list;

            strS = storage->getTextByNumber( (*vMatrix)[i][0] );

            for (int j = 1; j < vMatrix->size(); ++j) {
                if ((*vMatrix)[i][j] == 1) {
                    strM += storage->getTextByNumber((*vMatrix)[0][j]) + " ";
                    list.append(QVariant((*vMatrix)[0][j]));
                }
            }

            QTableWidgetItem *item1 = new QTableWidgetItem(strM);
            item1->setData(Qt::UserRole, QVariant(list));
            QTableWidgetItem *item2 = new QTableWidgetItem(strS);
            item2->setData(Qt::UserRole, QVariant((*vMatrix)[i][0]));

            tw->insertRow(tw->rowCount());
            tw->setRowHeight(tw->rowCount()-1, 50);
            tw->setItem(tw->rowCount()-1, 0, item1);
            tw->setItem(tw->rowCount()-1, 1, item2);
        }
    }


    tw->sortByColumn(1, Qt::AscendingOrder);
    tw->resizeRowsToContents();

    connect(this, SIGNAL(updateMasterLabel()), parent, SLOT(updateSuperKeyLabel()));
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

    //Вызов функции заполнения для каждой пары атрибутов.
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if ( difficultConnection[0][i] != empty &&  difficultConnection[1][j] != empty )  {
              addSimpleConnection(difficultConnection[0][i], difficultConnection[1][j]);
            }
        }
    }

    //Заполнение TableWidget
    for (int i = 0; i < 4; ++i) {
        if (difficultConnection[1][i] != empty ) {
            for (int j = 0; j < tw->rowCount(); ++j) {
                    if ( tw->item(j, 1)->data(Qt::UserRole).value<int>() == difficultConnection[1][i].toInt() )   {
                    tw->removeRow(j);
                }
            }

            QString strM;
            QString strS;
            QVariantList list;
            strS = storage->getTextByNumber(difficultConnection[1][i].toInt());

            int currentNum = -1;
            for (int j = 1; j < vMatrix->size(); ++j) {
                if ( (*vMatrix)[j][0] == difficultConnection[1][i].toInt()) currentNum = j;
            }

            for (int j = 1; j < vMatrix->size(); ++j) {
                if ( (*vMatrix)[currentNum][j] == 1)    {
                    strM += storage->getTextByNumber( (*vMatrix)[0][j]) + " ";
                    list.append(QVariant( (*vMatrix)[0][j] ));
                }
            }

            QTableWidgetItem *itemM = new QTableWidgetItem(strM);
            itemM->setData(Qt::UserRole, QVariant(list));
            QTableWidgetItem *itemS = new QTableWidgetItem(strS);
            itemS->setData(Qt::UserRole, QVariant(difficultConnection[1][i].toInt()));

            tw->insertRow(tw->rowCount());
            tw->setRowHeight(tw->rowCount()-1, 50);
            tw->setItem(tw->rowCount()-1, 0, itemM);
            tw->setItem(tw->rowCount()-1, 1, itemS);
        }
    }

    tw->resizeRowsToContents();
    tw->sortByColumn(1, Qt::AscendingOrder);

    storage->setNormalizeChanged();
}

void Connection::on_deleteConButton_clicked()
{
    int curRow = tw->currentRow();
    if (curRow == -1) {
        QMessageBox::information(this, "", tr("Для удаления сначала выберите нужную связь."));
        return;
    }

    auto result = QMessageBox::question(this, "", tr("Удалить выбранную связь?"));
    if ( result == QMessageBox::No) return;

    QTableWidgetItem *itemM = tw->takeItem(curRow,0);
    QTableWidgetItem *itemS = tw->takeItem(curRow,1);
    tw->removeRow(curRow);

    int index_X = itemS->data(Qt::UserRole).value<int>();
    QVariantList vList = itemM->data(Qt::UserRole).value<QVariantList>();
    for (int i = 0; i < vList.size(); ++i) {
        this->deleteSimpleConnection(index_X, vList[i].value<int>());
    }

    storage->setNormalizeChanged();
}

void Connection::on_cancelButton_clicked()
{
    emit updateMasterLabel();
    close();
}

void Connection::addSimpleConnection(const QString& currentNum, const QString& currentNum2) const
{
    QString strM, strS;
    for (int i = 0; i < attrTable->size(); ++i) {
        if ( currentNum == (*attrTable)[i][0] ) {
            strM = (*attrTable)[i][1];
            break;
        }
    }
    for (int i = 0; i < attrTable->size(); ++i) {
        if ( currentNum2 == (*attrTable)[i][0] ) {
            strS = (*attrTable)[i][1];
            break;
        }
    }

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

void Connection::deleteSimpleConnection(int index_X, int index_Y)
{
    int numberX = -1;
    int numberY = -1;

    for (int i = 1; i < vMatrix->size(); ++i) {
        if (index_X == (*vMatrix)[i][0]) numberX = i;
        if (index_Y == (*vMatrix)[0][i]) numberY = i;
    }

    (*vMatrix)[numberX][numberY]= 0;
}

void Connection::on_resetComBoxesButton_clicked()
{
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i == 1 && j == 0)   ptrComboBoxes[i][j]->setCurrentIndex(1);
            else    ptrComboBoxes[i][j]->setCurrentIndex(0);
        }
    }
}
