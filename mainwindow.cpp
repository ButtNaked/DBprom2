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

void MainWindow::showNormText()
{
    QString outText;
    QVector<QVector<Matrix*>*>* graphs = storage->getGraphs();

//    for (int i = 0; i < graphs->size(); ++i) {
//        for (int j = 0; j < graphs->at(i)->size(); ++j) {
//            int x = graphs->at(i)->at(j)->getX() - 1;
//            int y = graphs->at(i)->at(j)->getY() - 1;
//            outText += "Результирующие отношения:\n";

//            outText += "{"
//            for (int k = 0; k < total; ++k) {
//                QString str1;
//                str( *graphs->at(i)->at(j) )[][];
//            }










//        }
//    }







    outText="<u>123 </u>";

    QTextDocument *doc = new QTextDocument(outText, this);
    qDebug() << doc->toHtml();
    ui->textEdit->setDocument(doc);
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
    this->showNormText();

}

void MainWindow::on_pushButton_clicked()
{
    close();
}
