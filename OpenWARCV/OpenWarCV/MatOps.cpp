#pragma once
#include "pch.h"
#include "MatOps.h"

/**
*sum of squard difference (distance metric)
*starting from ax,ay in image a and bx,by in image b, with a width and height given
*indicies out of bounds results in MAX_DOUBLE
*/
double Matops::ssd(Mat& a, Mat& b, int x, int y) {
	return 0;
}

/**
*singlular vaue decomposition, results are returned in destU, destS, and destV
*may pass in nil to disregaurd certian results
*/
void Matops::svd(Mat& a, Mat*destU, Mat*destS, Mat*destV) {

}

//TODO::add any conversion frunctions from Mat to other useful formats