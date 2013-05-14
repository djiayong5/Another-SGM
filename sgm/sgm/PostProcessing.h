#pragma once
#include <queue>

#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>        
#include <opencv2/opencv.hpp>

#include "Timer.h"
#include "Properties.h"
#include "Utils.h"

void stereoMedianBlur(Mat dispL, Mat dispR){
	timer.start("Median bluring for 2 images");
	Mat dispRNew = dispR.clone();
	Mat dispLNew = dispL.clone();
	medianBlur(dispRNew, dispR, 3);
	medianBlur(dispLNew, dispL, 3);
	timer.finish();
}

Mat uniquenessConstraint(Mat dispL, Mat dispR){
	timer.start("Uniqueness constraint");
	Size size = dispL.size();
	Mat disp(size, CV_8U, Scalar(0));
	for(int y = 0; y < size.height; ++y){
		for(int x = 0; x < size.width; ++x){
			int dLeft = *dispL.ptr(y, x); 
			//x - dLeft should be always positive or zero
			*disp.ptr(y,x) = dLeft;	
			if(std::abs(dLeft - *dispR.ptr(y, x - dLeft)) > 1){ //it's strange
				//std::cout << "x " << x << " y " << y << " dLeft " << dLeft << " dRight " << (int) *dispR.ptr(y, x - dLeft) << "\n";
				*disp.ptr(y,x) = 0;	
			}
		}
	}
	timer.finish();
	return disp;
}