#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Main window [DB prom]"));

    storage = new Storage();
    ui->textEdit->setReadOnly(1);

    scene = new QGraphicsScene(QRectF(0, 0, 731, 411));
    ui->graphicsView->setScene(scene);

    scene->setBackgroundBrush(QBrush(Qt::lightGray));

    QSettings *settings = new QSettings("settings.conf",QSettings::IniFormat, this);


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

    int i = 0;
    for (int verticalMod = 0, maxRowCount = 2; i < list->size(); ++i) {

        QListWidget *lw = list->at(i);
        scene->addWidget(lw, Qt::Window | Qt::WindowTitleHint);

        int count = lw->count();
        if (count > maxRowCount)
            maxRowCount=count;

        switch (i/3) {
        case 0:
            break;
        case 1:
            verticalMod = 200;
            break;
        case 2:
            verticalMod = 300;
        default:
            verticalMod = 400;
            break;
        }

        lw->setGeometry(30+(i%3)*240, 40+verticalMod, 200, 45+count*15);
    }
    scene->setSceneRect(0, 0, 731, (1+i/9)*411 );

}

void MainWindow::saveFile()
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << *(storage->getAttrTable()) << *(storage->getVMatrix());
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

void MainWindow::on_actionSave_File_triggered()
{
    saveFile();
    qDebug() << fileName << "saved.";

}

void MainWindow::on_actionOpen_File_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.dbp)"));

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    QVector<QVector<QString> > outAttrTable;
    QVector<QVector<int> > outVMatrix;
    in >> outAttrTable >> outVMatrix;
    storage->setAttrTable(outAttrTable);
    storage->setVMatrix(outVMatrix);
    qDebug() << fileName << "opened.";

    storage->getUniTable()->clear();
}

void MainWindow::on_actionSave_ass_triggered()
{
    fileName = QFileDialog::getSaveFileName(this, tr("Save file"), "untitled.dbp", tr("DB prom files (*.dbp)"));
    saveFile();
    qDebug() << fileName << "saved ass...";
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("О программе..."), tr("Программа нормализации реляционных баз данных. 2014. Масленников Роман. E-mail: m3angreen@gmail.com"));
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_uniRelButton_clicked()
{
    UniversalRelationTable *wu = new UniversalRelationTable(this, storage);
    wu->show();
}
