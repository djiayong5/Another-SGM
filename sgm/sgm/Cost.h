#pragma once
#include <cstdint>

#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>        
#include <opencv2/opencv.hpp>

#include "Timer.h"

using namespace cv;

class CostCalculator {
public:
	CostCalculator(const Mat& left, const Mat& right, const Size& block) {
		this->censusLeft = calculateCensus(left, block);
		this->censusRight = calculateCensus(right, block);
	}

	virtual uchar getCost( const int& y, const int& x, const int& d) const = 0;
	virtual const uchar* getCosts( const int& y, const int& x) const = 0;

	virtual const uchar* getCosts(const int& y, const int& x, const int& fromDisp, const int& toDisp) const {
		return getCosts(y,x);
	}
protected:
	Mat censusLeft;
	Mat censusRight;

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

	uchar hammingDistance(const uint64_t  i, const uint64_t  j) const{
		uint64_t xor = i ^ j;
		uchar dist = 0;
		while(xor != 0 ){
			++dist;
			xor = xor & (xor - 1);
		}
		return dist;
	}
};

class SimpleCostCalculator : public CostCalculator {
public:
	SimpleCostCalculator(const Mat& left, const Mat& right, const Size& block, const int max_disp, const int k) : CostCalculator(left, right, block) {
		calculateCensusCost(censusLeft, censusRight, max_disp, k);	
	}

	virtual uchar getCost(const int& y, const int& x, const int& d) const {
		return *costs.ptr<uchar>(y,x,d);		
	}

	virtual const uchar* getCosts(const int& y, const int& x) const {
		return costs.ptr<uchar>(y,x);
	}

	
protected:
	//use k = +- 1 for (left to right) cost or (right to left) cost
	void calculateCensusCost(const Mat& censusLeft, const Mat& censusRight, const int max_disp, const int k){
		timer.start("Costs census calculating");
		int size[3];
		size[0] = censusLeft.size().height;
		size[1] = censusLeft.size().width;
		size[2] = max_disp;
		costs = MatND(3, size, CV_8U);
		for(int y = 0; y < size[0]; ++y){
			const uint64_t* iterRight = censusRight.ptr<uint64_t>(y);
			const uint64_t* iterLeft =  censusLeft.ptr<uint64_t>(y);

			for(int x = 0; x < size[1]; ++x){
				for(int d = 0; d < size[2] && x + k * d < size[1] && x + k * d >= 0; ++d){
					// if image was done from the left - pixel will be on right side
					const uint64_t r = iterRight[x];
					const uint64_t l = iterLeft[x + k * d];
					*costs.ptr<uchar>(y,x,d) = hammingDistance(l, r);
				}

			}
		}
		timer.finish();
	}

	MatND costs;
};


//yes, it's fucking shit
namespace Temp {
	Mat temp;
}

class RuntimeCostCalculator : public CostCalculator {
public:
	RuntimeCostCalculator(const Mat& left, const Mat& right, const Size& block, const int& max_disp, const int k) : CostCalculator(left, right, block), k(k), max_disp(max_disp) {
		width = left.size().width;	
		Temp::temp = Mat(max_disp, 1, CV_8U);
	}

	virtual uchar getCost(const int& y,const int& x,const int& d) const {
		if(x + k * d < 0 || x + k * d >= width){
			return 255;
		}
		return hammingDistance(*censusLeft.ptr<uint64_t>(y,x + k * d), *censusRight.ptr<uint64_t>(y, x));		
	}

	virtual const uchar* getCosts(const int& y, const int& x) const {
		return getCosts(y,x, 0, max_disp);
		
	}

	virtual const uchar* getCosts(const int& y, const int& x, const int& fromDisp, const int& toDisp) const {
		const uint64_t cr = *censusRight.ptr<uint64_t>(y, x);
		const uint64_t* left_iter = censusLeft.ptr<uint64_t>(y); 
		uchar* ret_iter = Temp::temp.ptr<uchar>(0);
		for(int d = fromDisp; d < toDisp; ++d){
			if(x + k * d < 0 || x + k * d >= width){
				ret_iter[d] = 255;
			} else {
				ret_iter[d] = hammingDistance(left_iter[x + k * d], cr);	
			}
		}
		return ret_iter;
	}
	int width;
	const int k;
	const int max_disp;
};


class RangeCostCalculator : public CostCalculator {
public:
	RangeCostCalculator(const Mat& left, const Mat& right, const Size& block, const int max_disp, const int k, const Mat& minDisp, const Mat& maxDisp) 
		: CostCalculator(left, right, block), minDisp(minDisp), maxDisp(maxDisp) {
		calculateCensusCost(censusLeft, censusRight, max_disp, k);	
	}

	virtual const uchar* getCosts(const int& y, const int& x) const {
		return costs.ptr<uchar>(y,x);
	}

	virtual uchar getCost( const int& y, const int& x, const int& d) const {
		return *costs.ptr<uchar>(y,x,d - *minDisp.ptr<uchar>(y,x));		
	}

	virtual const uchar* getCosts(const int& y, const int& x, const int& fromDisp, const int& toDisp) const {
		return costs.ptr<uchar>(y,x);	
	}
	
protected:
	//use k = +- 1 for (left to right) cost or (right to left) cost
	void calculateCensusCost(const Mat& censusLeft, const Mat& censusRight, const int max_disp, const int k){
		timer.start("Costs census calculating");
		int size[3];
		size[0] = censusLeft.size().height;
		size[1] = censusLeft.size().width;
		size[2] = max_disp;
		costs = MatND(3, size, CV_8U);
		for(int y = 0; y < size[0]; ++y){
			const uint64_t* iterRight = censusRight.ptr<uint64_t>(y);
			const uint64_t* iterLeft =  censusLeft.ptr<uint64_t>(y);

			for(int x = 0; x < size[1]; ++x){
				uchar fromDisp = *minDisp.ptr<uchar>(y,x);
				uchar toDisp = *maxDisp.ptr<uchar>(y,x); 
				for(int d = fromDisp ; d < toDisp && x + k * d < size[1] && x + k * d >= 0; ++d){
					// if image was done from the left - pixel will be on right side
					const uint64_t r = iterRight[x];
					const uint64_t l = iterLeft[x + k * d];
					*costs.ptr<uchar>(y,x,d - fromDisp) = hammingDistance(l, r);
				}

			}
		}
		timer.finish();
	}

	const Mat& minDisp;
	const Mat& maxDisp;
	MatND costs;
};
