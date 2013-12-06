#pragma once
#include "pch.h"
#include "Feature2d.h"
#include "MatOps.h"
#include "ImgOps.h"
#include <algorithm>

using namespace MatOps;

bool Feature2d::compAscX(const Point& p, const Point& p2) {
	return p.x < p2.x;
}

bool Feature2d::compDescX(const Point& p, const Point& p2) {
	return !compAscX(p, p2);
}

bool Feature2d::compAscY(const Point& p, const Point& p2) {
	return p.y < p2.y;
}

bool Feature2d::compDescY(const Point& p, const Point& p2) {
	return !compAscY(p, p2);
}

/**
*uses RANSAC to compute map from a onto b with maximum number of points
*threadhold is the maximum distance between to valid matches
*H is the resulting 3x3 projective trasnformation
*num_matches is the number of points used to compute the homography
*/
void Feature2d::findHomographyRANSAC(KeyPoints& a, KeyPoints& b, double threshhold, Mat* H, int* num_matches){
	int maxcount = 0;
	Mat tempH;
	for(int i=0; i<100; i++) {
		KeyPoints p;
		KeyPoints p2;
		for(int j=0; j<5; j++) {
			int ind = floor(rand()*a.size());
			p.push_back(a[ind]);
			p2.push_back(b[ind]);
			findHomography(p, p2, &tempH);
		}
		KeyPoints a2;
		projectiveTransform(a, tempH, &a2);
		int count = 0;
		for(int j=0; j<a2.size(); j++) {
			double dist = pow(a2[j].x - b[j].x, 2) + pow(a2[j].y - b[j].y, 2);
			if(dist < 10) {
				count++;
			}
		}

		if(count > maxcount) {
			*H = tempH;
			maxcount = count;
		}
	}
	*num_matches = maxcount;
}

/**
*helper function for computeHomographyRANSAC
*computes a homography from a onto b
*/
void Feature2d::findHomography(KeyPoints& a, KeyPoints& b, Mat* H){
	//calculate x,y means for a,b
	double max = 0;
	double mbx = 0;
	double may = 0;
	double mby = 0;
	for(int i=0; i<a.size(); i++) {
		max += a[i].x;
		may += a[i].y;
		mbx += b[i].x;
		mby += b[i].y;
	}
	max /= a.size();
	may /= a.size();
	mbx /= b.size();
	mby /= b.size();

	//calculate x,y std dev for a,b
	double sax = 0;
	double sbx = 0;
	double say = 0;
	double sby = 0;
	for(int i=0; i<a.size(); i++) {
		sax += pow(a[i].x-max, 2);
		say += pow(a[i].y-may, 2);
		sbx += pow(b[i].x-mbx, 2);
		sby += pow(b[i].y-mby, 2);
	}
	sax = sqrt(sax/(a.size()-1));
	say = sqrt(say/(a.size()-1));
	sbx = sqrt(sbx/(b.size()-1));
	sby = sqrt(sby/(b.size()-1));

	//create
	Mat T1 = Mat(3,3);
	T1[0][0] = 1/sax; T1[0][1] = 0; T1[0][2] = -max/sax;
	T1[1][0] = 0; T1[1][1] = 1/say; T1[1][2] = -may/say;
	T1[2][0] = 0; T1[2][1] = 0; T1[2][2] = 1;
	
	Mat T2 = Mat(3,3);
	T2[0][0] = 1/sbx; T2[0][1] = 0; T2[0][2] = -mbx/sbx;
	T2[1][0] = 0; T2[1][1] = 1/sby; T2[1][2] = -mby/sby;
	T2[2][0] = 0; T2[0][1] = 0; T2[0][2] = 1;

	KeyPoints a2;
	projectiveTransform(a, T1, &a2);
	KeyPoints b2;
	projectiveTransform(b, T2, &b2);

	Mat A = Mat(2*a2.size(), 9);
	for(int i=0; i<a2.size(); i++) {
		A[i][0] = -b2[i].x; A[i][1] = -b2[i].y; A[i][2] = -1; 
		A[i][3] = 0; A[i][4] = 0; A[i][5] = 0;
		A[i][6] = b2[i].x*a2[i].x; A[i][7] = b2[i].y*a2[i].x; A[i][8] = a2[i].x;
		int j = i+a2.size();
		A[j][0] = 0; A[j][1] = 0; A[j][2] = 0; 
		A[j][3] = -b2[i].x; A[j][4] = -b2[i].y; A[j][5] = -1;
		A[j][6] = b2[i].x*a2[i].y; A[j][7] = b2[i].y*a2[i].y; A[j][8] = a2[i].y;
	}

	Mat V;
	svd(A, NULL, NULL, &V);
	Mat H1 = Mat(3,3);
	int col = V.cols()-1;
	int row = 0;
	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			H1[i][j] = V[row][col];
			row++;
		}
	}

	Mat T;
	MatOps::inverse(T1, &T);
	Mat H2;
	MatOps::multi(T, H1, &H2);
	MatOps::multi(H2, T2, H);

}

/**
*matches the descriptors of a to the descriptors of b
*returns the indexes of a that match the index of b
*/
void Feature2d::match(Descriptors& a, Descriptors& b, double tol,  vector<int> * indexesA, vector<int> * indexesB){
	for(int i=0; i<a.size(); i++){
		double min = 0;
		int minindex = 0;
		double min2 = 0;
		for(int j=0; i<b.size(); j++) {
			double dist = ssd(a[i], b[j]);
			if (dist < min) {
				min2 = min;
				min = dist;
				minindex = j;
			} else if (dist < min2){
				min2 = dist;
			}
		}
		if(min < tol*min2) {
			indexesA->push_back(i);
			indexesB->push_back(minindex);
		}
	}
}

/**
*performs harris cornder detections, and returns the key points of the image, and descriptors
*/
void Feature2d::harrisCorners(Mat& im, double alpha, int N, KeyPoints * keypoints, Descriptors * decriptors){

Mat har;
{
	//ensure the stack is freed after computing harris responses
	Mat gfil, efil, imblur, Ix, Iy, Ixx, Iyy, Ixy, Ixxyy;

	GaussianDist(7, 1, &gfil); //smoothing filter
	ImgOps::imfilter(im, gfil, &imblur); // smooth image
	//compute horizontal gradient
	ImgOps::edgeFilter(false, &efil);
	ImgOps::imfilter(imblur, efil, &Ix);
	//compute vertical gradient
	ImgOps::edgeFilter(true, &efil);
	ImgOps::imfilter(imblur, efil, &Ix);
	ImgOps::imfilter(Ix*Ix, gfil, &Ixx); // compute smoothed x-gradient sq
	ImgOps::imfilter(Iy*Iy, gfil, &Iyy); // compute smoothed y-gradient sq
	ImgOps::imfilter(Ix*Iy, gfil, &Ixy); 
	Ixxyy = Ixx+Iyy;
	har = (Ixx * Iyy) - (Ixy * Ixy) - alpha*(Ixxyy * Ixxyy); // cornerness
}

//ensure local variables are freed as soon as possible
{
	vector<Point> tempKey;
	vector<Point> values;
	// get local maxima within 7x7 window
	for(int i=7; i<im.rows()-7; i+=7) {
		for(int j=7; j<im.cols()-7; j+=7) {
			int max = 0;
			int indy = 0;
			int indx = 0;
			for(int y=0; y<7; y++) {
				for(int x=0; x<7; x++) {
					if(har[y][x] > max) {
						max = har[y][x];
						indy = y;
						indx = x;
					}
				}
			}
			tempKey.push_back(Point(indx, indy));
			values.push_back(Point(max, values.size()));
		}
	}

	//sort values by response
	std::sort(values.begin(), values.end(), &Feature2d::compDescX);
	//copy key points in sorted order
	keypoints->resize(values.size());
	for(int i=0; i<values.size(); i++) {
		(*keypoints)[i] = tempKey[values[i].y]; 
	}

}

if(N < keypoints->size()) {
	keypoints->resize(N);
}
//extract descriptors
decriptors->resize(keypoints->size());
for(int i=0; i<keypoints->size(); i++) {
	Point& p = (*keypoints)[i];
	crop(im, p.x-7, p.y-7, p.x+7, p.x+7, &(*decriptors)[i]);
}

}

/**
*applies H to the plane of a onto the plane of b, returns the new points in result
*/
void Feature2d::projectiveTransform(KeyPoints& a, Mat& H, KeyPoints* result){
	result->resize(a.size());
	for(int i=0; i<a.size(); i++) {
		Point p = a[i];
		Mat mat = Mat(3, 1);
		mat[0][0] = p.x;
		mat[1][0] = p.y;
		mat[2][0] = 1;
		Mat ret;
		multi(H, mat, &ret);
		(*result)[i].x = ret[0][0]/ret[2][0];
		(*result)[i].y = ret[1][0]/ret[2][0];
	}
}

/**
* given a set of key points and a list of indexes, extract the key points with the given indexes
*/
void Feature2d::filter(const KeyPoints& a, vector<int>& indexes, KeyPoints* result) {
	result->resize(indexes.size());
	for(int i=0; i<indexes.size(); i++) {
		(*result)[i] = a[indexes[i]];
	}
}

/**
*given a matrix and a set of key points, extract the matrix containing key points
*/
void Feature2d::crop(Mat& mat, const KeyPoints& kp, Mat* result) {
	int x  = mat.cols();
	int y = mat.rows();
	int x2 = 0;
	int y2 = 0;
	for(int i=0; i<kp.size(); i++) {
		x = floor(min(x, kp[i].x));
		y = floor(min(y, kp[i].y));
		x2 = ceil(max(x2, kp[i].x));
		y2 = ceil(min(y2, kp[i].y));
	}
	x = max(0, x-8);
	y = max(0, y-8);
	x2 = min(mat.cols(), x2+8);
	y2 = min(mat.rows(), y2+8);
	crop(mat, x, y, x2, y2, result);
}

/**
*given a matrix and a set of bounds, extract the matrix within the bounds
*/
void Feature2d::crop(Mat& mat, int x, int y, int x2, int y2, Mat* result) {
	result->resize(y2-y, x2-x);
	for(int i=0; i<y2-y; i++) {
		for(int j=0; j<x2-x; j++) {
			(*result)[i][j] = mat[i+y][j+x];
		}
	}
}
