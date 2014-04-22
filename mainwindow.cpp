#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    storage = new Storage();
    ui->textEdit->setReadOnly(1);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete storage;
}

void MainWindow::showOutputText()
{
    QString outText;
    ui->textEdit->setHtml(storage->getOutputText(outText));
}

void MainWindow::on_addAttrButton_clicked()
{
    Attribute *wa = new Attribute(this, storage);
    wa->show();
}

void MainWindow::on_addConButton_clicked()
{
    Connection *wc = new Connection(this, storage);
    wc->show();
}

void MainWindow::on_normButton_clicked()
{
    storage->startNormalization();
    this->showOutputText();

}

void MainWindow::on_pushButton_clicked()
{
    close();
}
