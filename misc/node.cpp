#include "misc/node.h"

int Node::nodeCounter = 0;

Node::Node()
{
    id = nodeCounter++;
}

Node::Node(const QString &rName):
    name(rName)
{
    id = nodeCounter++;
}

Node::~Node()
{

}


void Node::setName(const QString &rName)
{
    name=rName;
}

const QString &Node::getName() const
{
    return name;
}

int Node::getId() const
{
    return id;
}

QVector<int> &Node::getMasterD()
{
    return masterD;
}

QVector<int> &Node::getSlaveD()
{
    return slaveD;
}

bool Node::isForeign() const
{
 return foreign;
}

bool Node::isTopest() const
{
    return topest;
}

void Node::show() const
{
    QString strM, strS;
    for (int i = 0; i < masterD.size(); ++i) {
        strM += QString::number(masterD[i])+= " ";
    }

    for (int i = 0; i < slaveD.size(); ++i) {
        strS += QString::number(slaveD[i]) += " ";
    }

    qDebug() << "***";
    qDebug() << "Имя " << name;
    qDebug() << "id : " << id;
    qDebug() << "Зависят от него : " << strM;
    qDebug() << "Зависит от них : " << strS;
    qDebug() << "***" << "\n";
}
