#ifndef CONNECTION_H
#define CONNECTION_H

#include <QDialog>
#include <core/storage.h>
#include <QMessageBox>

namespace Ui {
class Connection;
}

class Connection : public QDialog
{
    Q_OBJECT

public:
    explicit Connection(QWidget *parent = 0, Storage *rStorage = 0);
    ~Connection();

private slots:
    void on_addConButton_clicked();

    void on_deleteConButton_clicked();

    void on_acceptButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::Connection *ui;
    Storage *storage;
};

#endif // CONNECTION_H
