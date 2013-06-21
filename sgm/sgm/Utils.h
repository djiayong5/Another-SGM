
#pragma once

#include <stdarg.h>
#include <cstdio>
#include <string>

#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>        
#include <opencv2/opencv.hpp>
;
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

std::string fmt(const std::string fmt, ...) {
    int size = 100;
    std::string str;
    va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }
    return str;
}

Mat addCols(const Mat& m, size_t sz){
	Mat tm(m.rows, m.cols + sz, m.type());
	m.copyTo(tm(Rect(Point(sz, 0), m.size())));
	return tm;
}
/*
opencv_core244d.lib
opencv_highgui244d.lib
opencv_video244d.lib
opencv_ml244d.lib
opencv_legacy244d.lib
opencv_imgproc244d.lib


*/


/*

*/