#include "dbnamedialog.h"
#include "ui_dbnamedialog.h"

DBnameDialog::DBnameDialog(QWidget *parent, Storage *_storage) :
    QDialog(parent),
    ui(new Ui::DBnameDialog),
    storage(_storage)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle(tr("Введите название базы данных"));
    ui->lineEdit->setText(storage->getdbName());
    ui->lineEdit->selectAll();

    connect (this, SIGNAL(dbNameChanged()), parent, SLOT(updateWindowTitle()));
}

DBnameDialog::~DBnameDialog()
{
    delete ui;
}

void DBnameDialog::on_pushButton_OK_clicked()
{
    storage->setdbName(ui->lineEdit->text());
    emit dbNameChanged();
    close();
}

void DBnameDialog::on_pushButton_Cancel_clicked()
{
    close();
}
