#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <core/storage.h>
#include <subwindows/attribute.h>
#include <subwindows/connection.h>
#include <core/algo/Normalization.h>
#include <QTextDocument>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showOutputText();

private slots:
    void on_addAttrButton_clicked();

    void on_addConButton_clicked();

    void on_normButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Storage *storage;
};

#endif // MAINWINDOW_H
