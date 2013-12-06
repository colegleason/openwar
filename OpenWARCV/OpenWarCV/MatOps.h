#pragma once
#include "Mat.h"

namespace MatOps{

/**
*sum of squard difference (distance metric)
*starting from ax,ay in image a and bx,by in image b, with a width and height given
*indicies out of bounds results in MAX_DOUBLE
*/
double ssd(Mat& a, Mat& b, int x = 0, int y = 0);

/**
*singlular vaue decomposition, results are returned in destU, destS, and destV
*may pass in nil to disregaurd certian results
*/
void svd(Mat& a, Mat*destU, Mat*destS, Mat*destV);

/**
*performs matrix multiplication Result = AB
*/
void multi(Mat& a, Mat&b, Mat* result);

/**
*computes matrix transpose of a
*/ 
void transpose(Mat& a, Mat* result);

/**
*computes the sum of the elementwise multiplication of a with b centered at x,y of a
*/
double dot(Mat& a, Mat& b, int x, int y);

/**
*create an rxc identity matrix and stores it in result
*/
void identity(int r, int c, Mat* result);

/**
*function that computes a Gaussian distribution on a square matrix
*/
void GaussianDist(int size, double sigma, Mat* result);

/**
*power iteration method for computing eigenvectors
*/
void powerIteration(Mat* A, Mat* x, Mat* eigenv);

/**
*function for normalizing a vector
*/
void normalize(Mat* in, Mat* out);

/*
*calculate the inverse of a square matrix
*/
void inverse(Mat* in, Mat* out);

//TODO::add any conversion frunctions from Mat to other useful formats

};
