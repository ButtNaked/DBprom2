#ifndef CONNECTION_H
#define CONNECTION_H

#include <core/storage.h>

#include <QMessageBox>
#include <QDialog>
#include <QComboBox>
#include <QTableWidget>

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

    void on_cancelButton_clicked();

private:
    Ui::Connection *ui;
    Storage *storage;
    QVector<QVector<QString> > *attrTable;
    QVector<QVector<int> > *vMatrix;
QVector<QVector<QComboBox*>> ptrComboBoxes;
    QTableWidget *tw;

    void addSimpleConnection(const QString &currentNum, const QString &currentNum2) const;

signals:
    void updateMasterLabel();
};

#endif // CONNECTION_H
