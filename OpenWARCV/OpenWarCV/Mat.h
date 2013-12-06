#pragma once
#include <vector>

using namespace std;
class Mat {
public:
	typedef vector<double> Row;

private:
	vector<Row> rowData;

public:
	/**
	*create empty matrix
	*/
	Mat();

	/**
	*create a matrix with r rows and c columns
	*/
	Mat(int r, int c);

	/**
	*create matrix from 2d byte array
	*/
	Mat(char* b, int r, int c);

	/**
	*access in row, column format with []
	*/
	Row& Mat::operator[](int r); 

	/**
	*allow copy with =
	*/
	Mat& Mat::operator=(const Mat &rhs);

	/**
	*allow scalar subtraction operations to be applied to matrix
	*/
	Mat Mat::operator-(double val) const; 
	friend Mat operator-(double val, const Mat& mat);
	Mat Mat::operator-(const Mat& val) const; 

	/**
	*allow scalar addition to be applied to matrix
	*/
	Mat Mat::operator+(double num) const;
	friend Mat operator+(double num, const Mat& mat);
	Mat Mat::operator+(const Mat& val) const; 

	/**
	*allow scalar multiplication to be applied to matrix
	*/
	Mat Mat::operator*(double num) const;
	friend Mat operator*(double num, const Mat& mat);
	Mat Mat::operator*(const Mat& val) const;

	/**
	*get number of rows
	*/
	int rows() const;
	/**
	*get number of columns
	*/
	int cols() const;

	/**
	*resizes matrix to rxc
	*/
	void resize(int r, int c);

};
