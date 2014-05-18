#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QDialog>
#include <core/storage.h>
#include <QMessageBox>
#include <QRegExp>

namespace Ui {
class Attribute;
}

class Attribute : public QDialog
{
    Q_OBJECT

public:
    explicit Attribute(QWidget *parent = 0, Storage *rStorage = 0);
    ~Attribute();
    bool isOriginalName(const QString &text);

private slots:
    void on_addAttrButton_clicked();

    void on_delAttrButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Attribute *ui;
    Storage *storage;
    QListWidget *lw;
    QVector<QVector<QString> > *attrTable;
    QVector<QVector<int> > *vMatrix;
    QVector<QVector<QString>> *uniTable;
};

#endif // ATTRIBUTE_H
