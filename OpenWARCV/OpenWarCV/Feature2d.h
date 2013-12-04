#pragma once
#include "Mat.h"

namespace Feature2d {

typedef struct Point {
	double x;
	double y;
};

typedef vector<Point> KeyPoints;
typedef vector<Mat> Descriptors;

/**
*uses RANSAC to compute map from a onto b with maximum number of points
*threadhold is the maximum distance between to valid matches
*H is the resulting 3x3 projective trasnformation
*num_matches is the number of points used to compute the homography
*/
void findHomographyRANSAC(KeyPoints& a, KeyPoints& b, double threshhold, Mat* H, int* num_matches);

/**
*helper function for computeHomographyRANSAC
*computes a homography from a onto b
*/
void findHomography(KeyPoints& a, KeyPoints& b, Mat* H);

/**
*matches the descriptors of a to the descriptors of b
*returns the indexes of a that match the index of b
*/
void match(Descriptors& a, Descriptors& b,  vector<int> * indexesA, vector<int> * indexesB);

/**
*performs harris cornder detections, and returns the key points of the image, and descriptors
*/
void harrisCorners(Mat& a, KeyPoints * keyspoints, Descriptors * decriptors);

/**
*applies H to the plane of a onto the plane of b, returns the new points in result
*/
void projectiveTransform(KeyPoints& a, KeyPoints& b, Mat& H, KeyPoints* result);

};