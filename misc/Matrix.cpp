#include "misc/Matrix.h"
#include <math.h>

Matrix::Matrix(myint rX, myint rY)
{
    x=rX;
    y=rY;
	arr= new myint* [x];     
	for (myint i = 0; i < x; i++)
		arr[i] = new myint [y];	
}


Matrix::~Matrix()
{
	for (myint i = 0; i < x; i++)
		delete []arr[i];
	delete []arr;	
}


myint** Matrix::getPtr()
{
	return arr;
}


void Matrix::show()
{
    qDebug() << "\n";
    QString str;
	for (int i=0; i<x;i++)	{
        for (int j=0;j<y;j++)   {
            QString temp;
            str += temp.setNum(arr[i][j]) += " ";
        }
        qDebug() << str;
        str.clear();
	}
    qDebug() << "\n";
}


void Matrix::fillBinarySequence()
{
	for (int i=0; i<x;i++)	{
		for (int j=0;j<y;j++)
			arr[i][j]=1;
	}
	for (myint j=0, mod=0, ch=0; j<y; j++)	{
		ch=(myint)pow(2,j+1);
		mod=0;
		while (ch>0)	{
			for (int i=0;i<(x-mod);i=i+(myint)pow(2,j+1)) arr[i+mod][j]=0;
			mod++;
			ch=ch-2;
		}
	}

}

void Matrix::fillWithNumber(myint num)
{
	for (int i=0; i<x;i++)	{
		for (int j=0;j<y;j++)
			arr[i][j]=num;
	}
}

void Matrix::addIndexRowCol(myint **MainMcr_ptr)
{
    arr[0][0]=0;
	for (int i=1; i<x; i++)	
		arr[i][0]=MainMcr_ptr[i][0];

	for (int j=1; j<y; j++)	
		arr[0][j]=j-1;
}

myint Matrix::getX()
{
	return x;
}

myint Matrix::getY()
{
	return y;
}


void Matrix::deleteNullCols()
{
	myint mod_y=0;
	for (int j=1; j<y; j++)
		for (int i=1; i<x; i++)	{
			if (arr[i][j]==1)	break;
			if (arr[i][j]==0 && i==x-1) mod_y++;
		}

	if (mod_y==0)	return;
	//ЦИКЛ УДАЛЕНИЯ НУЛЕВЫХ СТОЛБЦОВ
	
	myint new_y=y-mod_y;
	Matrix arrTmp(x, new_y);

	for (int j=1, mod=0; j<y; j++)	{
		int sum1=0;
		for (int i=1; i<x; i++)	{
			sum1=sum1+arr[i][j];
		}
		if (sum1>0)	{
			for (int i2=0; i2<x; i2++)	{
			arrTmp[i2][j-mod]=arr[i2][j];
			}
		}
		else mod++;
		sum1=0;
	}
	for (int i=0; i<x; i++)
		arrTmp[i][0]=arr[i][0];

	for (myint i = 0; i < x; i++)
		delete []arr[i];
	delete []arr;

	arr= new myint* [x];
	for (myint i = 0; i < x; i++)
		arr[i] = new myint [new_y];	

	for (int i = 0; i<x; i++)      
		for (int j = 0; j<new_y; j++) arr[i][j]=arrTmp[i][j];

	y=new_y;
}


Matrix& Matrix::operator=(Matrix& in)
{	
	for (int i=0; i<in.getX();i++)	
		for (int j=0;j<in.getY();j++)
			(*this)[i][j]=in[i][j];

	return *this;
}

bool Matrix::operator==(Matrix& rh)
{
    if (this->x != rh.x) return false;

    int max = this->x;

    for (int i = 1; i < max; ++i) {
        for (int j = 1; j < max; ++j) {
            if ((*this)[i][j] != rh[i][j]) return false;
        }
    }

    return true;
}
