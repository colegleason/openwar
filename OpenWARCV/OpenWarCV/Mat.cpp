#pragma once
#include "pch.h"

/**
*create empty matrix
*/
Mat::Mat() {
	init(3,3);
}

/**
*create an rxc matrix
*/
Mat::Mat(int r, int c) {
	init(r,c);
}

/**
*create matrix from 2d byte array
*/
Mat::Mat(char* b, int r, int c) {
	init(r,c);
	int index = 0;
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){
			(*this)[r][c] = (double)(b[index]);
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

/**
*get number of rows
*/
int Mat::rows() {
	return this->rowData.size();
}

/**
*get number of columns
*/
int Mat::cols() {
	return this->rowData[0].size();
}

void Mat::init(int r, int c) {
	this->rowData.reserve(r);
	Mat m;
	for(int i=0; i<r; i++){
		rowData[i].reserve(c);
	}
}