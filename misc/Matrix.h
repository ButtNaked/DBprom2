#pragma once

#include <QDebug>

typedef int myint; // здесь был unsigned int

class Matrix
{
private:
    myint x, y;
    myint **arr;

public:
/////////////////////////////////////
    class CRow {
        friend class Matrix;
    public:
        myint& operator[](myint col)
        {
            return parent.arr[row][col];
        }
    private:
        CRow(Matrix &parent_, myint row_) : 
            parent(parent_),
            row(row_)
        {}

        Matrix& parent;
        myint row;
    };
/////////////////////////////////////


    CRow operator[](myint row)
    {
        return CRow(*this, row);
    }

	Matrix& operator=(Matrix&);
    Matrix(myint rX, myint rY);
	~Matrix();
	myint** getPtr();
	void show();
	void initializeByteS();
	void deleteNullRows();
	void deleteNullCols();
	void deleteKeyRows(myint x, myint y, myint *header);
	void fillBinarySequence();
	void fillWithNumber(myint num);
	void addIndexRowCol(myint**);
    myint getX();
    myint getY();
    bool operator==(Matrix& rh);

};
