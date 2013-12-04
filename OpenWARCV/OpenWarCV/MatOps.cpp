#pragma once
#include "pch.h"
#include "MatOps.h"
#include <cfloat>

/**
*sum of squard difference (distance metric)
*starting from ax,ay in image a and bx,by in image b, with a width and height given
*indicies out of bounds results in MAX_DOUBLE
*/
double Matops::ssd(Mat& a, Mat& b, int x, int y) {
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
void Matops::svd(Mat& a, Mat*destU, Mat*destS, Mat*destV) {

}

//TODO::add any conversion frunctions from Mat to other useful formats