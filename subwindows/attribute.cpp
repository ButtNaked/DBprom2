#include "subwindows/attribute.h"
#include "ui_attribute.h"

Attribute::Attribute(QWidget *parent, Storage *rStorage) :
    QDialog(parent),
    ui(new Ui::Attribute),
    storage(rStorage)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Add attribute [DB prom]"));

    lw = ui->listWidget;
    attrTable = storage->getAttrTable();
    vMatrix = storage->getVMatrix();
    uniTable = storage->getUniTable();

    if (!attrTable->isEmpty())  {
        for (int i=0; i<attrTable->size(); i++) {
            lw->addItem((*attrTable)[i][1]);
        }
    }

    QRegExp exp("[a-zA-zа-яА-я_0-9]{1,35}");
    ui->lineEdit->setValidator(new QRegExpValidator(exp, this));

}

Attribute::~Attribute()
{
    delete ui;
}

bool Attribute::isOriginalName(const QString &text)
{
    for (int i = 0; i < attrTable->size(); ++i) {
        if ( (*attrTable)[i][1] == text)  {
            return false;
        }
    }

    return true;
}

void Attribute::on_addAttrButton_clicked()
{
    QString text = ui->lineEdit->text();
    if (!isOriginalName(text))  {
        QMessageBox::information(this, tr("Внимание!"), tr("Имя атрибута должно быть оригинальным."));
        return;
    }
    if (text.size() == 0)   {
        QMessageBox::information(this, tr("Внимание!"), tr("Введите имя атрибута."));
        return;
    }

    lw->addItem(text);
    ui->lineEdit->clear();
    lw->scrollToBottom();

    //добавление нового атрибута в attrTable
    QVector<QString> vec;
    int number = -1;

    if(!attrTable->isEmpty())   {
        int last = attrTable->size()-1;
        QString str = (*attrTable).at(last).at(0);
        number = str.toInt();
        number++;
    }
    else    number = 0;

    vec.append(QString::number(number));
    vec.append(text);
    attrTable->append(vec);

    //добавление нового атрибута в vMatrix
    QVector<int> iVec;
    iVec.append(number);
    vMatrix->append(iVec);
    int last = vMatrix->size()-1;
    for (int i = 0; i < vMatrix->size()-1; ++i) {
        (*vMatrix)[last] << 0;
    }

    (*vMatrix)[0] << number;
    for (int i = 1; i < vMatrix->size()-1; ++i) {
        (*vMatrix)[i] << 0;
    }

    //Данные были изменены
    storage->somethingChanged();

    //добавление нового атрибута в uniTable
    if (uniTable->isEmpty()) return;
    (*uniTable)[0].append(QString::number(number));

    for (int i = 1; i < uniTable->size() - 1; ++i) {
        (*uniTable)[i].append(tr("ПУСТО"));
    }
    (*uniTable)[uniTable->size() - 1].append("");
}

void Attribute::on_delAttrButton_clicked()
{
    if ( ui->listWidget->currentRow() == -1)    {
        QMessageBox::information(this, tr(""), tr("Для удаления сначала выберите нужный атрибут."));
        return;
    }

    auto result = QMessageBox::question(this, "", tr("Вы уверены что хотите удалить выбранный атрибут?"));
    if ( result == QMessageBox::No) return;

    QListWidgetItem *delItem = lw->takeItem(lw->row(lw->currentItem()));
    QString delItemText = delItem->text();

    //удаление из attrTable
    int delItemNum = -1;
    for (int i = 0; i < attrTable->size(); ++i) {
        if ( (*attrTable)[i][1] == delItemText )   {
            delItemNum = ((*attrTable)[i][0]).toInt();
            attrTable->remove(i);
            break;
        }
    }

    //удаление из vMatrix
    int delItemPosition = -1;
    for (int i = 1; i < vMatrix->size(); ++i) {
        if ( (*vMatrix)[i][0] == delItemNum)    {
            delItemPosition=i;
            vMatrix->remove(i);
            break;
        }
    }
    for (int i = 0; i < vMatrix->size(); ++i) {
            (*vMatrix)[i].remove(delItemPosition);
    }

    //Данные были изменены
    storage->somethingChanged();

    //Удаление из uniTable
    if (uniTable->isEmpty()) return;
    for (int i = 0; i < uniTable->at(0).size(); ++i) {
        if ( ((*uniTable)[0][i]).toInt() == delItemNum) {
            delItemPosition = i;
        }
    }
    for (int i = 0; i < uniTable->size(); ++i) {
        (*uniTable)[i].remove(delItemPosition);
    }
}


void Attribute::on_pushButton_2_clicked()
{
    close();
}
