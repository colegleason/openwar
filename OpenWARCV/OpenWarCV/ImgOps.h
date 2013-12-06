#pragma once
#include "Mat.h"

namespace ImgOps {
/**
*result(y,x) is dot product of filter and img centered at y,x
*/
void imfilter(Mat& img, Mat& filter, Mat* result);

/**
*resizes the image to size(img)*ratio, blurs if downsampling
*result stores new image matrix
*/
void imresize(Mat& img, float ratio, Mat* result);

/**
*creates an edge filter, either vertical or horizontal
*/
void edgeFilter(bool vertical, Mat* result);

};