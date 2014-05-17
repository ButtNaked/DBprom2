#ifndef UNIVERSALRELATIONTABLE_H
#define UNIVERSALRELATIONTABLE_H

#include <QMainWindow>
#include <core/storage.h>
#include <QTableWidget>

struct Rule
{
    QVector<int> keys;
    int attr;
};

namespace Ui {
class UniversalRelationTable;
}

class UniversalRelationTable : public QMainWindow
{
    Q_OBJECT

public:
    explicit UniversalRelationTable(QWidget *parent = 0, Storage *rStorage = 0);
    ~UniversalRelationTable();

private:
    Ui::UniversalRelationTable *ui;
    Storage *storage;
    QTableWidget *wt;
    QVector<QVector<QString>> *uniTable;
    QVector<QVector<int> > *vMatrix;
    QVector<QVector<QString> > *attrTable;

    void validation();
    void checkRule(Rule& rRule);

private slots:
    void updateTable(QTableWidgetItem *rItem);
    void on_actionCheck_triggered();
};

#endif // UNIVERSALRELATIONTABLE_H
