#pragma once
#include <queue>

#include <opencv2/core/core.hpp>        
#include <opencv2/opencv.hpp>

#include "Timer.h"
#include "Properties.h"
#include "Utils.h"

using namespace cv;
using namespace std;

class DynamicDirectionRange : public DynamicDirection {
public:
	DynamicDirectionRange(const Mat& left, const Mat& right, const CostCalculator& costs,  const Size& size, const int& max_disp, 
		 const int& penalty1, const int& penalty2, const Mat& minDisp, const Mat& maxDisp) 
		: DynamicDirection(left,right, costs, size, max_disp, penalty1, penalty2) , minDisp(minDisp), maxDisp(maxDisp){ 
	}
protected:
	virtual int getFromDisp(const Point2i& p){
		return *minDisp.ptr<ushort>(p.y, p.x);
	}
	virtual int getToDisp(const Point2i& p){
		return *maxDisp.ptr<ushort>(p.y, p.x);
	}

	const Mat minDisp;
	const Mat maxDisp;
};


