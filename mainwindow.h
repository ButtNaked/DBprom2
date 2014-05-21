#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <core/storage.h>
#include <subwindows/attribute.h>
#include <subwindows/connection.h>
#include <core/algo/Normalization.h>
#include "core/myscene.h"
#include <subwindows/universalrelationtable.h>
#include <subwindows/dbnamedialog.h>
#include <subwindows/newdialog.h>

#include <QMainWindow>
#include <QTextDocument>
#include <QGraphicsItem>
#include <QListWidget>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QSettings>


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
    void reset();

public slots:
    void on_actionOpen_File_triggered();

    void on_actionSave_ass_triggered();

private slots:
    void on_addAttrButton_clicked();

    void on_addConButton_clicked();

    void on_normButton_clicked();

    void on_pushButton_clicked();

    void on_actionSave_File_triggered();  

    void on_actionAbout_triggered();

    void on_actionExit_triggered();

    void on_uniRelButton_clicked();

    void on_action_triggered();

    void on_actionNew_data_base_scheme_triggered();

private:
    const QString version = "0.85";
    Ui::MainWindow *ui;
    Storage *storage;
    MyScene *scene;
    QString filePath;
};

#endif // MAINWINDOW_H

