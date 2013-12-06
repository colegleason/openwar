#pragma once
#include "pch.h"
#include "MatOps.h"
#include <cfloat>
#include <cmath>

#define PI	3.14159265359

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
*singlular value decomposition, results are returned in destU, destS, and destV
*may pass in nil to disregaurd certian results
*/
void MatOps::svd(Mat& a, Mat*destV) {



    // Find eigenvalues if there a is squared or has more rows than cols. 
    if (a.cols() > a.rows()) 
          return;

    if(destV == null)
    {
        destV = new Mat(9,9);
    }    

    Mat eigen = new Mat(a.cols(),1);
    Mat temp = a; 
    Mat temp2;

    for (c = 0 ; c < a.cols() ; c++){
  
        Mat x = Mat(temp.rows(),1);
        for(r = 0 ; r < temp.rows() - 1 ; r++){
            x(r,0) = 0;
        }
        x(temp.rows(),0) = 1; 
        powerIteration(temp,x,&eigen);
	deflate(temp,*temp2);
        temp = temp2;
        dest[c][c] = eigen[1][1]; 	        
     }     
	
       

}

void powerIteration(Mat* A, Mat* x, Mat* eigenv)
{
	//have the initial value
	Mat* x_o = x;

	x = A * x;

	//compute the difference
	Mat* diff = x - x_o;

	double maxval = normalize(diff)
	int count = 0;

	while(maxval > 1e-6 && count < 1000)
	{
		x_o = x;
		x = A * x;
		x = x / normalize(x);
		diff = x - xo
		maxval = normalize(diff);
		count++;
	}
}
/*
* deflate:
* Reduces the matrix by eliminating the first row and column
* input: 
* a is the original matrix to be deflated
* output
* dest is the deflated matrix
*/
void MatOps::deflate(Mat&a, Mat*dest)
{

    for(int r = 1 ; r < a.rows() ; r++){
        for(int c = 1 ; c < a.cols() ; c++){
            *(dest[r-1][c-1] = a[r][c];
        }
    }
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
void MatOps::multi(Mat& a, Mat&b, Mat* result) {
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

void GaussianDist(int size, double sigma, Mat * result)
{
	//parameter check
	if(size == 0 || sigma == 0)
		return;

	//we dont want an even matrix
	if(size % 2 == 0)
		size++;

	result->resize(size, size);
	int u = size >> 1; //mean

	//P(x) = (1/(sigma * sqrt(2pi))) * exp(-(x-u)^2/(2*sigma))
	for(int x = 0 ; x < size ; ++x)
	{
		for(int y = 0 ; y < size ; ++y)
		{
			(*result)[x][y] = (1/(sigma * sqrt(2 * PI))) * exp(-((x-u)*(x-u))/(2*sigma));
			(*result)[x][y] *= (1/(sigma * sqrt(2 * PI))) * exp(-((y-u)*(y-u))/(2*sigma));
		}
	}
	
}


/* normalize takes a Mat* with one row only (a vector)
*/
double normalize(Mat* in)
{
	//this is not a vector
	if(in.rows() != 1)
		return 0.0;
	
	double sum = 0.0;

	//add the square of all the values in the vector
	for(int i = 0 ; i < in.cols() ; ++i)
	{
		sum += (in[0][i] * in[0][i]);
	}
		
	//take the sqrt and divide all
	sum = 1/sqrt(sum);

	return sum;
}

void inverse(Mat* in, Mat* out)
{
	if(in.rows() != in.cols())
		return;

	out->resize(in.rows(), in.cols());

	//make our out matrix an identity
	identity(in.rows(), in.cols(), out);

	for(int i = 0 ; i < out.rows() ; ++i)
	{
		double factor = in[i][i];
		out /= factor;
	
		for(int j = 0 ; j < out.rows() ; ++j)
		{
			if(i == j)	continue;
			double coef = in[j][i];
			out[j] -= coef * out[i];
		}
	}
}


//TODO::add any conversion frunctions from Mat to other useful formats
