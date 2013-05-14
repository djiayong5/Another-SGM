#pragma once
#include <queue>
#include <fstream>

#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>        
#include <opencv2/opencv.hpp>

#include "Timer.h"
#include "Properties.h"
#include "Utils.h"

;
using namespace cv;
using namespace std;

std::ofstream log_disp("img_disp.txt");


class DynamicDirection{
public:
	DynamicDirection(const Mat& left, const Mat& right, const MatND& costs,  const Size& size, const int& max_disp, const int& penalty1, const int& penalty2) 
		: costs(costs), left(left), right(right), penalty1(penalty1), penalty2(penalty2), size(size), max_disp(max_disp) {}

	virtual MatND calculateL(const Point2i& direction){

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

		timer.start("Dynamic direction calculating2");
		while(!queue.empty()){
			Point2i p = queue.front();
			queue.pop();
			const Point2i prev = prevPoint(p);
			const uchar* l_iter_prev;
			if(isInside(prev)) 
				l_iter_prev = l.ptr<uchar>(prev.y,prev.x);
			const uchar* curr_iter = costs.ptr<uchar>(p.y,p.x);

			int min = 255;



			for(int i = 0; i < max_disp; ++i){
				min = std::min(min, get(l_iter_prev, prev, i));
			}

			uchar* l_iter = l.ptr<uchar>(p.y,p.x);

			for(int d = 0; d < max_disp; ++d){
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

				l_iter[d] = bestValue; //todo
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
		return !isInside(prevPoint(p));
	}

	int get(const uchar* l_iter, Point2i p, int d){
		if(!isInside(p)){
			return 255;
		}
		if(d < 0 ||  d >= max_disp){
			return 255;
		}
		return l_iter[d];

		//return *l.ptr<uchar>(p.y,p.x,d);
	}

	MatND l;
	const Mat left;
	const Mat right;
	const MatND costs;
	const Size size;
	const int max_disp;
	const int penalty1;
	const int penalty2;
	Point2i direction;
};


class DynamicImage {
public:

	//calculate disparity map via sgm
	// a,b - costs
	Mat calculateDisparity(const Size& size, const int max_disp, DynamicDirection& dd){
		Mat s(size, CV_16U);
		int sizes[3];
		sizes[0] = size.height;
		sizes[1] = size.width;
		sizes[2] = max_disp;
		MatND sum(3, sizes, CV_16U, Scalar(0));
		for(int i = 0; i < DIRS; ++i){
			MatND l = dd.calculateL(Point2i(r_x[i], r_y[i]));
			for(int y = 0; y < size.height; ++y){
				for(int x = 0; x < size.width; ++x){
					ushort* sum_iter = sum.ptr<ushort>(y,x);
					const uchar* l_iter = l.ptr<uchar>(y,x);
					for(int d = 0; d < max_disp; ++d){
						sum_iter[d] += l_iter[d];
					}
				}
			}
		}

		timer.start("Sum and best calculating");
		for(int y = 0; y < size.height; ++y){
			for(int x = 0; x < size.width; ++x){
				int minD = -1;
				int min = 10000;
				for(int d = 0; d < max_disp; ++d){
					if(min > *sum.ptr<ushort>(y,x,d)){
						min = *sum.ptr<ushort>(y,x,d);
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