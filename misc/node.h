#ifndef NODE_H
#define NODE_H
#include <QString>
#include <QVector>
#include <QDebug>

enum NodeType {TEXT, DATE, NUMERIC, BOOL};

class Node
{
private:
    static int nodeCounter;
    int id;
    QString name;
    NodeType type=TEXT;
    QVector<int> masterD;
    QVector<int> slaveD;
    bool foreign = false;
    bool topest = false;
public:
    Node();
    Node(const QString &rName);
    ~Node();
    void setName(const QString &rName);
    const QString &getName() const;
    int getId() const;
    QVector<int> &getMasterD();
    QVector<int> &getSlaveD();
    bool isForeign() const;
    bool isTopest() const;
    void show() const;

};


#endif // NODE_H
