#pragma once
#include <cstdint>

#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>        
#include <opencv2/opencv.hpp>

#include "Timer.h"

using namespace cv;

Mat calculateCensus(const Mat& img, const Size& block){
	timer.start("Census calculating");
	const Size size = img.size();
	uint64_t default = 0;
	Mat temp(size, CV_64F);
	uint64_t census = 0;
	bool bit = 0;
	int m = block.height;
	int n = block.width;
	if( m % 2 == 0 || n % 2 == 0){
		std::cout << "Bad block windows size for census\n";
		throw "Error";
	}
	const Size imgSize = img.size();
	for (int y = 0; y < imgSize.height; y++){ 
		const uchar* xyr = img.ptr<uchar>(y);
        for(int x = 0; x < imgSize.width; x++){ 
            if(y >= m/2 && y < imgSize.height - m/2 && x >= n/2 && x < imgSize.width - n/2){ 
                census = 0; 
                for (int i = y - m/2; i <= y + m/2; i++){ 
					const uchar* ijr = img.ptr<uchar>(i);
                    for (int j = x - n/2; j <= x + n/2; j++){ 
                        if( i != y || j != x){ 
                            if( ijr[j] < xyr[x] ) 
                                bit = 1; 
                            else
                                bit = 0; 
                            census = (census << 1) + bit; 
                        } 
                    } 
                } 
                *temp.ptr<uint64_t>(y,x) = census; 
            } else { 
                *temp.ptr<uint64_t>(y,x) = default; 
            } 
        } 
    }
	timer.finish();
	return temp;
}


uchar hammingDistance(const uint64_t  i, const uint64_t  j){
	uint64_t xor = i ^ j;
	uchar dist = 0;
	while(xor != 0 ){
		++dist;
		xor = xor & (xor - 1);
	}
	return dist;
}

//use k = +- 1 for (left to right) cost or (right to left) cost
MatND calculateCensusCost(const Mat& censusLeft, const Mat& censusRight, const int max_disp, const int k){
	timer.start("Costs census calculating");
	int size[3];
	size[0] = censusLeft.size().height;
	size[1] = censusLeft.size().width;
	size[2] = max_disp;

	MatND c(3, size, CV_8U);
	for(int y = 0; y < size[0]; ++y){
		const uint64_t* iterRight = censusRight.ptr<uint64_t>(y);
		const uint64_t* iterLeft =  censusLeft.ptr<uint64_t>(y);

		for(int x = 0; x < size[1]; ++x){
			for(int d = 0; d < size[2] && x + k * d < size[1] && x + k * d >= 0; ++d){
				// if image was done from the left - pixel will be on right side
				const uint64_t r = iterRight[x];
				const uint64_t l = iterLeft[x + k * d];
				*c.ptr<uchar>(y,x,d) = hammingDistance(l, r);
			}

		}
	}
	timer.finish();
	return c;
}

//5 30
MatND calculateSegment(const Mat& src, int sp, int sr){
	timer.start("Segment calculating");
	Mat dest = src.clone();
	cv::pyrMeanShiftFiltering(src, dest, sp, sr);
	timer.finish();
	return dest;
}

MatND calculateSegmentCost(const Mat& segmentLeft, const Mat& segmentRight, const int max_disp, const int k){
	timer.start("Costs segment calculating");
	int size[3];
	size[0] = segmentLeft.size().height;
	size[1] = segmentLeft.size().width;
	size[2] = max_disp;

	MatND c(3, size, CV_8U);
	for(int y = 0; y < size[0]; ++y){
		for(int x = 0; x < size[1]; ++x){
			for(int d = 0; d < size[2] && x + k * d < size[1] && x + k * d >= 0; ++d){
				// if image was done from the left - pixel will be on right side
				const uchar r = *segmentRight.ptr<uchar>(y,x);
				const uchar l = *segmentLeft.ptr<uchar>(y,x + k * d);
				*c.ptr<uchar>(y,x,d) = std::abs(l - r);
			}

		}
	}
	timer.finish();
	return c;
}
