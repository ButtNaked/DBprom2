#include "newdialog.h"
#include "ui_newdialog.h"

NewDialog::NewDialog(QWidget *parent, MainWindow *_mainwindow, Storage *rStorage) :
    QDialog(parent),
    ui(new Ui::NewDialog),
    storage(rStorage),
    mainwindow(_mainwindow)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle(tr("Создайте новый файл или откройте существующий"));

}

NewDialog::~NewDialog()
{
    delete ui;
}



void NewDialog::on_pushButton_OK_clicked()
{
    mainwindow->reset();
    storage->setdbName(ui->lineEdit->text());
    mainwindow->on_actionSave_ass_triggered();
    this->close();
}

void NewDialog::on_pushButton_Exit_clicked()
{
    this->close();
    mainwindow->close();
}

void NewDialog::on_pushButton_OpenFIle_clicked()
{
    mainwindow->on_actionOpen_File_triggered();
    this->close();
}
