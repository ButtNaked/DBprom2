#ifndef DBNAMEDIALOG_H
#define DBNAMEDIALOG_H

#include "core/storage.h"

#include <QDialog>

namespace Ui {
class DBnameDialog;
}

class DBnameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBnameDialog(QWidget *parent = 0, Storage *_storage = 0);
    ~DBnameDialog();

private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::DBnameDialog *ui;
    Storage* storage;
};

#endif // DBNAMEDIALOG_H
