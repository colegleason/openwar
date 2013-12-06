#pragma once
#include "pch.h"
#include "Feature2d.h"
#include "MatOps.h"

using namespace MatOps;

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
void Feature2d::harrisCorners(Mat& a, KeyPoints * keyspoints, Descriptors * decriptors){

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
