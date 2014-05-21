#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include "core/storage.h"
#include "mainwindow.h"

#include <QDialog>

class MainWindow;
namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDialog(QWidget *parent = 0, MainWindow * _mainwindow = 0, Storage *rStorage = 0);
    ~NewDialog();

private slots:


    void on_pushButton_OK_clicked();

    void on_pushButton_Exit_clicked();

    void on_pushButton_OpenFIle_clicked();

private:
    Ui::NewDialog *ui;
    Storage *storage;
    MainWindow *mainwindow;

};

#endif // NEWDIALOG_H
