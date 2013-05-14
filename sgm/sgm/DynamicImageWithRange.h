#pragma once
#include <queue>

#include <opencv2/core/core.hpp>        
#include <opencv2/opencv.hpp>

#include "Timer.h"
#include "Properties.h"
#include "Utils.h"

using namespace cv;
using namespace std;

class DynamicDirectionRange{
public:
	DynamicDirectionRange(const Mat& left, const Mat& right, const MatND& costs,  const Size& size, const int& max_disp, 
		const Point2i& direction, const int& penalty1, const int& penalty2, const Mat minDisp, const Mat maxDisp) 
		: costs(costs), left(left), right(right), penalty1(penalty1), penalty2(penalty2), size(size), max_disp(max_disp), direction(direction), minDisp(minDisp), maxDisp(maxDisp){ 
	}

	MatND calculateL(){
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
			const uchar* l_iter;
			if(isInside(prev)) 
				l_iter = l.ptr<uchar>(prev.y,prev.x);
			const uchar* curr_iter = costs.ptr<uchar>(p.y,p.x);

			int min = 255;
			for(int i = *minDisp.ptr<ushort>(p.y, p.x); i < *maxDisp.ptr<ushort>(p.y, p.x); ++i){
				min = std::min(min, get(l_iter, prev, i));
			}

			//todo d = 0; d < max_disp; ++d. Maybe bug!!!!
			
			for(int d = *minDisp.ptr<ushort>(p.y, p.x); d < *maxDisp.ptr<ushort>(p.y, p.x); ++d){
				int curr = curr_iter[d]; 
				int a = get(l_iter, prev,d);
				int b = get(l_iter, prev,d - 1);
				int c = get(l_iter, prev,d + 1);

				//todo penalty2 update
				int bestValue = curr + std::min(std::min(a, min + penalty2), std::min(b, c) + penalty1) - min;
				bestValue = std::min(bestValue, 255);
				*l.ptr<uchar>(p.y,p.x,d) = bestValue; 
			}

			const Point2i next = nextPoint(p);
			if(isInside(next)){
				queue.push(next);
			}
		}
		return l;
	}

private:
	//unused now
	int getPenalty2(Point2i prev, Point2i p){
		if(!isInside(prev)){
			return penalty2;
		}
		int diff = std::abs(*left.ptr(p.y, p.x) - *left.ptr(prev.y, prev.x));
		return diff == 0 ? penalty2 : penalty2 / diff;
	}
	Point2i prevPoint(Point2i p){
		return p - direction;
	}
	Point2i nextPoint(Point2i p){
		return p + direction;
	}
	bool isInside(Point2i p){
		if(p.x < 0 || p.y < 0 || p.x >= size.width || p.y >= size.height ){
			return false;;
		}
		return true;
	}

	bool isStart(Point2i p){
		if(!isInside(prevPoint(p))){
			return true;
		}
		return false;
	}

	int get(const uchar* l_iter, Point2i p, int d){
		if(!isInside(p)){
			return 255;
		}
		if(d < *minDisp.ptr<ushort>(p.y, p.x) ||  d >= *maxDisp.ptr<ushort>(p.y, p.x)){
			return 255;
		}
		return l_iter[d];
		//return *l.ptr<uchar>(p.y,p.x, d);
	}

	MatND l;
	const Mat left;
	const Mat right;
	const MatND costs;
	const Size size;
	const int max_disp;
	const int penalty1;
	const int penalty2;
	const Point2i direction;
	const Mat minDisp;
	const Mat maxDisp;
};

class DynamicImageWithRange {
public:
	//calculate disparity map via sgm
	// a,b - costs
	Mat calculateDisparity(const Mat& left, const Mat& right, const MatND& costs, const Size size, const int max_disp, int penalty1, int penalty2, Mat minDisp, Mat maxDisp){
		Mat s(size, CV_16U);
		MatND l[DIRS];
		//std::cout << minDisp.size() << " " << maxDisp.size() << " " << size;
		for(int i = 0; i < DIRS; ++i){
			timer.start("Dynamic direction calculating");
			DynamicDirectionRange dd(left, right, costs, size, max_disp, Point2i(r_x[i], r_y[i]), penalty1, penalty2, minDisp, maxDisp);
			l[i] = dd.calculateL();
			timer.finish();
		}
		timer.start("Sum and best calculating");
		for(int y = 0; y < size.height; ++y){
			for(int x = 0; x < size.width; ++x){
				int minD;
				int min = 100000;
				for(int d = *minDisp.ptr<ushort>(y,x); d < *maxDisp.ptr<ushort>(y,x); ++d){
					int sum = 0;
					for(int i = 0; i < DIRS; ++i){
						sum += *l[i].ptr<uchar>(y,x,d);
					}
					if(min >= sum){
						min = sum;
						minD = d;
					}
				}
				*s.ptr<ushort>(y, x) = minD;
			}
		}
		timer.finish();
		return s;
	}
};