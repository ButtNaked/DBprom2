#ifndef UNIVERSALRELATIONTABLE_H
#define UNIVERSALRELATIONTABLE_H

#include <QMainWindow>
#include <core/storage.h>
#include <QTableWidget>

// Structure to keep rules based on attributes realation
struct Rule
{
    QVector<int> keys;  // key attributes
    int attr;           // depend attribute
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

    void fillTableWidget();     //fill table widget with items
    void tuplesValidation();    //create set of rules for future tuples validation
    void checkRule(Rule& rRule);//check rules and paint items, if check dont passed

private slots:
    void updateTable(QTableWidgetItem* rItem);// update tableWidget and uniTable
    void on_actionCheck_triggered();          //initiate tuples validation process
};

#endif // UNIVERSALRELATIONTABLE_H
