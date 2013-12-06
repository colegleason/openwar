#pragma once
#include "pch.h"
#include "Mat.h"

/**
*create empty matrix
*/
Mat::Mat() {
	resize(3,3);
}

/**
*create an rxc matrix
*/
Mat::Mat(int r, int c) {
	resize(r,c);

	for(int i = 0 ; i < r ; ++i)
		for(int j = 0 ; j < c ; ++i)
			(*this)[i][j] = 0;
}

/**
*create matrix from 2d byte array
*/
Mat::Mat(char* b, int r, int c) {
	resize(r,c);
	int index = 0;
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){
			(*this)[i][j] = (double)(b[index]);
			index++;
		}
	}
}

/**
*access in row, column format with []
*/
Mat::Row& Mat::operator[](int r) {
	return this->rowData[r];
}

Mat& Mat::operator=(const Mat &rhs)
{
	if (this == &rhs) {
        return *this;
	}

	this->rowData = rhs.rowData;
	return *this;
}

/**
*allow scalar subtraction operations to be applied to matrix
*/
Mat Mat::operator-(double val) const {
	Mat ret = Mat(this->rows(), this->cols());
	for(int i=0; i<this->rows(); i++) {
		for(int j=0; j<this->cols(); j++) {
			ret[i][j] = this->rowData[i][j] - val;
		}
	}
	return ret;
}
static Mat operator-(double val, const Mat& mat) {
	Mat ret = Mat(mat.rows(), mat.cols());
	for(int i=0; i<mat.rows(); i++) {
		for(int j=0; j<mat.cols(); j++) {
			ret[i][j] = val - mat.rowData[i][j];
		}
	}
	return ret;
}
Mat Mat::operator-(const Mat& mat) const {
	if(mat.rows() != this->rows() || mat.cols() != this->cols()) {
		throw "operator- only applies to matricies of the same size";
	}
	Mat ret = Mat(mat.rows(), mat.cols());
	for(int i=0; i<mat.rows(); i++) {
		for(int j=0; j<mat.cols(); j++) {
			ret[i][j] = this->rowData[i][j] - mat.rowData[i][j];
		}
	}
	return ret;
}

/**
*allow scalar addition to be applied to matrix
*/
Mat Mat::operator+(double val) const {
	Mat ret = Mat(this->rows(), this->cols());
	for(int i=0; i<this->rows(); i++) {
		for(int j=0; j<this->cols(); j++) {
			ret[i][j] = this->rowData[i][j] + val;
		}
	}
	return ret;
}
static Mat operator+(double num, const Mat& mat) {
	return mat+num;
}
Mat Mat::operator+(const Mat& mat) const {
	if(mat.rows() != this->rows() || mat.cols() != this->cols()) {
		throw "operator+ only applies to matricies of the same size";
	}
	Mat ret = Mat(mat.rows(), mat.cols());
	for(int i=0; i<mat.rows(); i++) {
		for(int j=0; j<mat.cols(); j++) {
			ret[i][j] = this->rowData[i][j] + mat.rowData[i][j];
		}
	}
	return ret;
}

/**
*allow scalar multiplication to be applied to matrix
*/
Mat Mat::operator*(double val) const {
	Mat ret = Mat(this->rows(), this->cols());
	for(int i=0; i<this->rows(); i++) {
		for(int j=0; j<this->cols(); j++) {
			ret[i][j] = this->rowData[i][j] * val;
		}
	}
	return ret;
}
Mat operator*(double num, const Mat& mat) {
	return mat*num;
}
Mat Mat::operator*(const Mat& mat) const {
	if(mat.rows() != this->rows() || mat.cols() != this->cols()) {
		throw "operator* only applies to matricies of the same size";
	}
	Mat ret = Mat(this->rows(), this->cols());
	for(int i=0; i<this->rows(); i++) {
		for(int j=0; j<this->cols(); j++) {
			ret[i][j] = this->rowData[i][j] * mat.rowData[i][j];
		}
	}
	return ret;
}


/**
*get number of rows
*/
int Mat::rows() const {
	return this->rowData.size();
}

/**
*get number of columns
*/
int Mat::cols() const{
	return this->rowData[0].size();
}

void Mat::resize(int r, int c) {
	this->rowData.resize(r);
	Mat m;
	for(int i=0; i<r; i++){
		rowData[i].resize(c);
	}
}

