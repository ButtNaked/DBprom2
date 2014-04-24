#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    storage = new Storage();
    ui->textEdit->setReadOnly(1);

    scene = new QGraphicsScene(QRectF(0, 0, 731, 411));
    ui->graphicsView->setScene(scene);


    scene->setBackgroundBrush(QBrush(Qt::lightGray));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete storage;
}

void MainWindow::showOutput()
{
    scene->clear();
    QString outText;
    ui->textEdit->setHtml(storage->getOutputText(outText));


    QList<QListWidget *> *list = storage->getWidgetTableList();


    for (int i = 0, verticalMod = 0, maxRowCount = 2; i < list->size(); ++i) {

        QListWidget *lw = list->at(i);
        scene->addWidget(lw, Qt::Window | Qt::WindowTitleHint);

        int count = lw->count();
        if (count > maxRowCount)
            maxRowCount=count;

        switch (i/1) {
        case 1:
            verticalMod = 200;
            break;
        case 2:
            verticalMod = 300;
        default:
            break;
        }

        lw->setGeometry(40+i*230, 40+verticalMod, 200, 45+count*15);
    }


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
    this->showOutput();

}

void MainWindow::on_pushButton_clicked()
{
    close();
}
