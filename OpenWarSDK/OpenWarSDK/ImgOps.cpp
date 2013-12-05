#pragma once
#include "pch.h"
#include "math.h"
#include <mat_ops.h>


/**
*result(y,x) is dot product of filter and img centered at y,x
*/
void imfilter(Mat& img, Mat& filter, Mat* result){
    if (img == NULL || filter == NULL)
        return;

    int r,c;
    for (r = 0 ; r < img.rows() ; r++)
         for (c = 0 ; c < img.cols() ; c++)
              result[r][c] = dot(img, filter, r,c);
}

/**
*resizes the image to size(img)*ratio, blurs if downsampling
*result stores new image matrix
*/
void imresize(Mat& img, float ratio, Mat* result){
    if ( !(ratio > 0.0f) )
        return;
    int r,c, pixel; // Iterators

    *result = new Mat((int) (img.rows() * ratio),(int)  (img.cols() * ratio));

    /* imresize can go in two directions, either an upsampled image or
        a downsampled image */
    for (r = 0 ; r < *result.rows() ; r++)
        for (c = 0 ; c < *result.cols() ; c++)
            *result[r][c] = img[round(r/ratio)][round(c/ratio)];

    Mat fil = GaussianDist(5, 0.08); // Magic numbers

    /* If increasing size of image, we need to use a Laplacian Filter, otherwise we do Gaussian */
    if (ratio > 1.0)
        fil = 1 - fil;
    
    imfilter(result, fil, &result); 
	
}
