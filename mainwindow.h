#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <core/storage.h>
#include <subwindows/attribute.h>
#include <subwindows/connection.h>
#include <core/algo/Normalization.h>
#include <QTextDocument>
#include <QGraphicsItem>
#include <QListWidget>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showOutput();
    void saveFile();

private slots:
    void on_addAttrButton_clicked();

    void on_addConButton_clicked();

    void on_normButton_clicked();

    void on_pushButton_clicked();

    void on_actionSave_File_triggered();

    void on_actionOpen_File_triggered();

    void on_actionSave_ass_triggered();

    void on_actionAbout_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    Storage *storage;
    QGraphicsScene *scene;
    QString fileName;
};

#endif // MAINWINDOW_H
