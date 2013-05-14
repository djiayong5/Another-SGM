
#pragma once

#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>        
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const int r_x[] = {0, 0, 1, -1, -1, -1,  1,  1};
const int r_y[] = {1, -1, 0, -1,  0,  1, -1,  1};

struct VecComparator {
	bool operator() (const Vec3i& l, const Vec3i& r) const {
		if(l[0] != r[0])
			return l[0] < r[0];
		if(l[1] != r[1])
			return l[1] < r[1];
		return l[2] < r[2];
	}
};

Mat scaleDisp(Mat in){
	int scale = 3;
	if(in.size().width < 400){
		scale = 6;
	}
	return in * scale;
}

Mat unscaleDisp(Mat in){
	int scale = 3;
	if(in.size().width < 400){
		scale = 6;
	}
	return in / scale;
}


/*
opencv_core244d.lib
opencv_highgui244d.lib
opencv_video244d.lib
opencv_ml244d.lib
opencv_legacy244d.lib
opencv_imgproc244d.lib


*/


