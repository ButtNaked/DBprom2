#include "core/storage.h"

void Storage::createMatrix()
{
    int size = vMatrix->size();
    matrix = new Matrix(size, size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            (*matrix)[i][j]=(*vMatrix)[i][j];
        }
    }
}

Storage::Storage(QWidget *parent):
    QWidget(parent)

{
    vMatrix = new QVector<QVector<int> > (1, QVector<int>(1, 0));
    attrTable = new QVector<QVector<QString> >;
    graphs = new QVector<QVector<Matrix *> *>;
    uniTable = new QVector<QVector<QString>>;
}

Storage::~Storage()
{
    delete vMatrix;
    delete attrTable;

    for (int i = 0; i < graphs->size(); ++i) {
        QVector<Matrix *> *pTemp2;
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            Matrix *pTemp;
            pTemp = graphs->at(i)->at(j);
            delete pTemp;
        }
        pTemp2 = graphs->at(i);
        delete pTemp2;
    }

    delete graphs;
    delete uniTable;
}

QVector<QVector<int> > *Storage::getVMatrix()
{
    return vMatrix;
}

Matrix *Storage::getMatrix()
{
    return matrix;
}

QVector<QVector<QString> > *Storage::getAttrTable()
{
    return attrTable;
}

QVector<QVector<Matrix *> *> *Storage::getGraphs()
{
    return graphs;
}

void Storage::setAttrTable(QVector<QVector<QString> > &inAttrTable)
{
    delete attrTable;
    attrTable = new QVector<QVector<QString> >;
    (*attrTable) = inAttrTable;
}

void Storage::setVMatrix(QVector<QVector<int> > &inVMatrix)
{
    delete vMatrix;
    vMatrix = new QVector<QVector<int> >;
    (*vMatrix) = inVMatrix;
}

void Storage::setUniTable(QVector<QVector<QString> > &inUniTable)
{
    delete uniTable;
    uniTable = new QVector<QVector<QString> >;
    (*uniTable) = inUniTable;
}

void Storage::startNormalization()
{
    Iteration::resetIterationCounter();

    for (int i = 0; i < graphs->size(); ++i) {
        QVector<Matrix *> *pTemp2;
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            Matrix *pTemp;
            pTemp = graphs->at(i)->at(j);
            delete pTemp;
        }
        pTemp2 = graphs->at(i);
        delete pTemp2;
    }

    graphs->clear();

    if (!checkingData()) {
        qDebug() << "Check is failed\n";
        return;
    }

    createMatrix();
    matrix->show();
    Normalization norm(this, matrix, graphs);
    Iteration::resetIterationCounter();

    qDebug() << "*******************************";
    for (int i = 0; i < graphs->size(); ++i) {
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            graphs->at(i)->at(j)->show();
        }
    }
    qDebug() << "*******************************";
}

const QString &Storage::getOutputText(QString &rOutText) const
{
    rOutText="<h3 align= \"center\">Cхемы отношений:</h3>";

    for (int i = 0; i < graphs->size(); ++i) {
        for (int j = 0; j < graphs->at(i)->size(); ++j) {
            int x = graphs->at(i)->at(j)->getX();
            int y = graphs->at(i)->at(j)->getY();
            QString textHeader;
            QString textKeys;
            QString textElem;


            for (int l = 1; l < y; ++l) {
                int number = ( *graphs->at(i)->at(j) )[0][l];
                textHeader += getTextByNumber(number);
                if (l != y-1)
                    textHeader += "_";
            }

            for (int l = 1; l < y; ++l) {
                int number = ( *graphs->at(i)->at(j) )[0][l];
                textKeys += getTextByNumber(number);
                if (l != y-1)
                    textKeys += ", ";
            }

            for (int k = 1; k < x; ++k) {

                int number = ( *graphs->at(i)->at(j) )[k][0];
                textElem += getTextByNumber(number);
                if (k != x-1)
                        textElem += ", ";
            }

            rOutText += QString("%1 { <u>%2</u>, %3 }<br>").arg(textHeader).arg(textKeys).arg(textElem);
        }
    }

    return rOutText;
}

int Storage::getNumberByText(const QString &rText) const
{

    for (int i = 0; i < attrTable->size(); ++i) {
        if ( (*attrTable)[i][1] == rText)
            return (*attrTable)[i][0].toInt();
    }

    return -1;
}

QString Storage::getTextByNumber(int rNum) const
{
    QString tNum = QString::number(rNum);
    for (int i = 0; i < attrTable->size(); ++i) {
        if ( (*attrTable)[i][0] == tNum)
            return (*attrTable)[i][1];
    }

    return "Error";
}

QVector<QVector<QString> > *Storage::getUniTable() const
{
    return uniTable;
}

bool Storage::isNormalizeUpdated() const
{
    return upToDate;
}

void Storage::setNormalizeChanged()
{
    if (upToDate == true)
        upToDate = false;
}

void Storage::setNormalizeUpToDate()
{
    if (upToDate == false)
        upToDate = true;
}

const QString Storage::getSuperKeyString()
{
    if (upToDate) return superKeyString;

    QVector<int> superKey;
    for (int i = 1; i < vMatrix->size(); ++i) {
        for (int j = 1; j < vMatrix->size(); ++j) {
            if ( vMatrix->at(i).at(j) == 1) break;
            else if ( (vMatrix->at(i).at(j) == 0) && ( j == vMatrix->size()-1 ) )   {
                superKey.append( vMatrix->at(i).at(0) );
            }
        }
    }

   superKeyString.clear();
   for (int q = 0; q < superKey.size(); ++q) {
       for (int i = 0; i < attrTable->size(); ++i) {
           if ( (*attrTable)[i][0] == QString::number(superKey[q]) )   {
                superKeyString += (*attrTable)[i][1];
                superKeyString += " ";
           }
       }
   }

   if (superKeyString.isEmpty()) return "Введите связи между атрибутами для отображения ключа универсального отношения.";

   superKeyString = QString(tr("Ключ универсального отношения: ")) + superKeyString;

   return superKeyString;
}

void Storage::setdbName(const QString &rdbName)
{
    dbName = rdbName;
}

const QString& Storage::getdbName() const
{
    return dbName;
}

void Storage::clear()
{
    attrTable->clear();
    vMatrix->clear();
    vMatrix->append(QVector<int>(1, 0));
    uniTable->clear();
    upToDate=false;
    dbName.clear();
    qDebug() << "Storage has been cleared";
}

bool Storage::checkingData()
{
    if (attrTable->isEmpty() || vMatrix->isEmpty()) {
        qDebug() << "Storage::checkingData, Data is not valid.";
        QMessageBox::warning(this, tr("Ошибка"),
                             tr("Файл пуст. Введите атрибуты и установите связи между ними."));
        return false;
    }

    if (vMatrix->size() <= 2)   {
        qDebug() << "Матрица пуста или состоит из одного атрибута.";
        QMessageBox::warning(this, tr("Ошибка"),
                             tr("Матрица пуста или состоит из одного атрибута."));
        return false;
    }

    int limit = vMatrix->size();
    bool connectionNotExist = true;
    for (int i = 1; i < limit; ++i) {
        for (int j = 1; j < limit; ++j) {
            if( (*vMatrix)[i][j] == 1) connectionNotExist = false;
        }
    }

    if (connectionNotExist) {
        QMessageBox::warning(this, tr("Ошибка"),
                             tr("Не установленны связи между атрибутами базы данных."));
        return false;
    }

    return true;
}







