#include "subwindows/attribute.h"
#include "ui_attribute.h"

Attribute::Attribute(QWidget *parent, Storage *rStorage) :
    QDialog(parent),
    ui(new Ui::Attribute),
    storage(rStorage)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Add attribute [DB prom]"));

    QListWidget *lw= ui->listWidget;
    QVector<QVector<QString> > *attrTable = storage->getAttrTable();

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
    QVector<QVector<QString> > *attrTable = storage->getAttrTable();

    for (int i = 0; i < attrTable->size(); ++i) {
        if ( (*attrTable)[i][1] == text)  {
            return false;
        }
    }


    return true;
}

void Attribute::on_addAttrButton_clicked() //TODO: Добавить проверку на разные имена атрибутов
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
    QVector<QVector<QString> > *attrTable = storage->getAttrTable();
    QVector<QVector<int> > *vMatrix = storage->getVMatrix();

    ui->listWidget->addItem(text);
    ui->lineEdit->clear();
    ui->listWidget->scrollToBottom();

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

    vec.push_back(QString::number(number));
    vec.push_back(text);

    attrTable->push_back(vec);

//    for (int i = 0; i < attrTable->size(); ++i) {
//        qDebug() << (*attrTable)[i][0] << (*attrTable)[i][1];
//    }
//    qDebug() << "_____________________";

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

void Attribute::on_delAttrButton_clicked()
{


    QVector<QVector<QString> > *attrTable = storage->getAttrTable();
    QVector<QVector<int> > *vMatrix = storage->getVMatrix();
    int number = -1;
    int actualN;
    QString text;

    if ( ui->listWidget->currentRow() == -1)    {
        QMessageBox::information(this, tr(""), tr("Для удаления сначала выберите нужный атрибут"));
        return;
    }
    QListWidgetItem *item = ui->listWidget->currentItem();
    text = item->text();
    ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));


    //удаление из attrTable

    for (int i = 0; i < attrTable->size(); ++i) {
        if ( (*attrTable)[i][1] == text )   {
            number = ((*attrTable)[i][0]).toInt();
            attrTable->remove(i);
            break; //!!!!
        }
    }

//        for (int i = 0; i < attrTable->size(); ++i) {
//            qDebug() << (*attrTable)[i][0] << (*attrTable)[i][1];
//        }
//        qDebug() << "_____________________";

    //удаление из vMatrix

    for (int i = 1; i < vMatrix->size(); ++i) {
        if ( (*vMatrix)[i][0] == number)    {
            actualN=i;
            vMatrix->remove(i);
            break;
        }
    }

    for (int i = 0; i < vMatrix->size(); ++i) {
            (*vMatrix)[i].remove(actualN);
    }

//        for (int i = 0; i < vMatrix->size(); ++i) {
//            QString str;
//            for (int j = 0; j < vMatrix->size(); ++j) {

//                int tempint = (*vMatrix)[i][j];
//                QString temp;
//                str += temp.setNum(tempint);

//            }
//            qDebug() << str << "\n";
//        }


}


void Attribute::on_saveButton_clicked()
{

    close();
}

void Attribute::on_pushButton_2_clicked()
{
    close();
}
