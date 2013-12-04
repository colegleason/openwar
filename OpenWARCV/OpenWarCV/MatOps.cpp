#pragma once
#include "pch.h"
#include "MatOps.h"
#include <cfloat>

/**
*sum of squard difference (distance metric)
*starting from ax,ay in image a and bx,by in image b, with a width and height given
*indicies out of bounds results in MAX_DOUBLE
*/
double MatOps::ssd(Mat& a, Mat& b, int x, int y) {
    if ( x < 0 || y < 0 || x + b.cols() > a.cols()|| y + b.rows() > a.rows())
        return DBL_MAX;
    
    /* Declaring iterators for SDD */
    int rb, cb;
    double distance = 0.0; 

    for (rb = 0 ; rb < b.rows() ; rb++) {
        for (cb = 0 ; cb < b.cols() ; cb++) {
            distance += a[y+rb][x+cb] * b[rb][cb];
        }
	}

     return distance;  
}

/**
*singlular vaue decomposition, results are returned in destU, destS, and destV
*may pass in nil to disregaurd certian results
*/
void MatOps::svd(Mat& a, Mat*destU, Mat*destS, Mat*destV) {

}

/**
*performs matrix multiplication Result = AB
*/
void MatOps::mulit(Mat& a, Mat&b, Mat* result) {
	if(a.cols()  != b.rows()) {
		//error
		return;
	}
	result->resize(a.rows(), b.cols());
	for(int i=0; i<a.rows(); i++){
		for(int j=0; j<b.cols(); j++){
			double sum = 0;
			for(int k=0; k<a.cols(); k++) {
				sum += a[i][k] * b[k][j];
			}
			(*result)[i][j] = sum;
		}
	}
}

/**
*computes matrix transpose of a
*/ 
void MatOps::transpose(Mat& a, Mat* result) {
	result->resize(a.cols(), a.rows());
	for(int i=0; i<a.rows(); i++) {
		for(int j=0; j<a.cols(); j++) {
			(*result)[j][i] = a[i][j];
		}
	}
}

/**
*computes the sum of the elementwise multiplication of a with b centered at x,y of a
*/
double MatOps::dot(Mat& a, Mat& b, int x, int y) {
	int dx = (b.cols()-1)/2;
	int dy = (b.rows()-1)/2;
	int minx = max(0, x-dx-1);
	int miny = max(0, y-dy-1);

	int mini = 0 + (y > dy) ? 0 : -1*dy;
	int minj = 0 + (x > dx) ? 0 : -1*dx;
	int height = min(b.rows() - mini, a.rows() - miny);
	int width = min(b.cols() - minj, a.cols() - minx);

	double sum = 0;
	for(int i=0; i<height; i++) {
		for(int j=0; i<width; j++) {
			sum += a[miny+i][minx+j] * b[mini+i][minj+j];
		}
	}

	return sum;
}

/**
*create an rxc identity matrix and stores it in result
*/
void identity(int r, int c, Mat* result) {
	result->resize(r, c);
	for(int i=0; i<r; i++) {
		for(int j=0; i<c; j++) {
			(*result)[i][j] = (i == j) ? 1 : 0;
		}
	}
}

//TODO::add any conversion frunctions from Mat to other useful formats