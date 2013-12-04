#pragma once
#include "pch.h"


/**
*sum of squard difference (distance metric)
*starting from ax,ay in image a and bx,by in image b, with a width and height given
*indicies out of bounds results in MAX_DOUBLE
*/
double ssd(Mat& a, Mat& b, int x = 0, int y = 0) {

}

/**
*singlular vaue decomposition, results are returned in destU, destS, and destV
*may pass in nil to disregaurd certian results
*/
void svd(Mat& a, Mat*destU, Mat*destS, Mat*destV) {

}

//TODO::add any conversion frunctions from Mat to other useful formats