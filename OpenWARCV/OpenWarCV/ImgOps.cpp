#pragma once
#include "pch.h"
#include "math.h"
#include "MatOps.h"

/**
*result(y,x) is dot product of filter and img centered at y,x
*/
void imfilter(Mat& img, Mat& filter, Mat* result){
    int r,c;
    for (r = 0 ; r < img.rows() ; r++) {
         for (c = 0 ; c < img.cols() ; c++) {
              (*result)[r][c] = MatOps::dot(img, filter, r,c);
		 }
	}
}

/**
*resizes the image to size(img)*ratio, blurs if downsampling
*result stores new image matrix
*/
void imresize(Mat& img, float ratio, Mat* result){
    if ( !(ratio > 0.0f) )
        return;
    int r,c, pixel; // Iterators

	result->resize((img.rows() * ratio),(int)  (img.cols() * ratio));

    /* imresize can go in two directions, either an upsampled image or
        a downsampled image */
    for (r = 0 ; r < result->rows() ; r++)
        for (c = 0 ; c < result->cols() ; c++)
			(*result)[r][c] = img[ceil(r/ratio)][ceil(c/ratio)];

    Mat fil;
	MatOps::GaussianDist(5, 0.08, &fil); // Magic numbers

    /* If increasing size of image, we need to use a Laplacian Filter, otherwise we do Gaussian */
    if (ratio > 1.0) {
        fil = 1 - fil;
	}

	Mat temp = *result;
    imfilter(temp, fil, result); 
	
}

/**
*creates an edge filter, either vertical or horizontal
*/
void edgeFilter(bool vertical, Mat* result) {
	result->resize(3,3);
	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			(*result)[i][j] = 0;
		}
	}
	if(vertical) {
		(*result)[1][0] = 1;
		(*result)[1][2] = -1;
	} else {
		(*result)[0][1] = 1;
		(*result)[2][1] = -1;
	}
}
