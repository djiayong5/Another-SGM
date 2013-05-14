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
	DynamicDirectionRange(const Mat& left, const Mat& right, const MatND& costs,  const Size& size, const int& max_disp, 
		 const int& penalty1, const int& penalty2, const Mat minDisp, const Mat maxDisp) 
		: DynamicDirection(left,right, costs, size, max_disp, penalty1, penalty2) , minDisp(minDisp), maxDisp(maxDisp){ 
	}

	virtual MatND calculateL(const Point2i& direction){
		timer.start("Dynamic direction calculating");

		this->direction = direction;
		int sizes[3];
		sizes[0] = size.height;
		sizes[1] = size.width;
		sizes[2] = max_disp;

		l = MatND(3, sizes, CV_8U, Scalar(255));
		std::queue<Point2i> queue;
		for(int y = 0; y < size.height; ++y){
			for(int x = 0; x < size.width; ++x){
				const Point2i p(x,y);
				if(isStart(p)){
					queue.push(p);
				}
			}
		}
		while(!queue.empty()){
			Point2i p = queue.front();
			queue.pop();
			const Point2i prev = prevPoint(p);
			const uchar* l_iter_prev;
			if(isInside(prev)) 
				l_iter_prev = l.ptr<uchar>(prev.y,prev.x);
			const uchar* curr_iter = costs.ptr<uchar>(p.y,p.x);

			int min = 255;
			ushort fromDisp = *minDisp.ptr<ushort>(p.y, p.x);
			ushort toDisp = *maxDisp.ptr<ushort>(p.y, p.x);

			for(int i = fromDisp; i < toDisp; ++i){
				min = std::min(min, get(l_iter_prev, prev, i));
			}

			uchar* l_iter = l.ptr<uchar>(p.y,p.x);
			
			for(int d = fromDisp; d < toDisp; ++d){
				int curr = curr_iter[d]; 
				int a = get(l_iter_prev, prev,d);
				int b = get(l_iter_prev, prev,d - 1);
				int c = get(l_iter_prev, prev,d + 1);

				int bestValue = a;
				bestValue = std::min(bestValue, min + penalty2);
				bestValue = std::min(bestValue, b + penalty1);
				bestValue = std::min(bestValue, c + penalty1);
				bestValue = curr + bestValue - min;
				bestValue = std::min(bestValue, 255);

				l_iter[d] = bestValue; 
			}

			const Point2i next = nextPoint(p);
			if(isInside(next)){
				queue.push(next);
			}
		}
		timer.finish();
		return l;
	}

protected:
	/*
	int get(const uchar* l_iter, Point2i p, int d){
		if(!isInside(p)){
			return 255;
		}
		if(d < *minDisp.ptr<ushort>(p.y, p.x) || d >= *maxDisp.ptr<ushort>(p.y, p.x){
			return 255;
		}
		return l_iter[d];
	}
	*/
	const Mat minDisp;
	const Mat maxDisp;
};
