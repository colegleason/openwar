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
	*get number of rows
	*/
	int rows();
	/**
	*get number of columns
	*/
	int cols();

	/**
	*resizes matrix to rxc
	*/
	void resize(int r, int c);
};
