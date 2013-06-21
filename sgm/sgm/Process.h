#pragma once

#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>        
#include <opencv2/opencv.hpp>

#include "Timer.h"
#include "Cost.h"
#include "DynamicImage.h"
#include "DynamicImageWithRange.h"
#include "Utils.h"
#include "Properties.h"
#include "PostProcessing.h"

Timer processTimer;

void calEstimation(const Mat& estDisp, Mat& minDisp, Mat& maxDisp){
	minDisp = estDisp.clone();
	maxDisp = estDisp.clone();
	for(int y = 0; y < estDisp.size().height; ++y){
		for(int x = 0; x < estDisp.size().width; ++x){
			ushort d = *estDisp.ptr<ushort>(y,x);
			if(d == 0 || d == (ushort) -1){
				if(rangeAlgType == QUALITY ){
					*minDisp.ptr<ushort>(y,x) = 0;
					*maxDisp.ptr<ushort>(y,x) = max_disp;
				} else {
					*minDisp.ptr<ushort>(y,x) = x == 0 ? 0 : *minDisp.ptr<ushort>(y,x - 1);
					*maxDisp.ptr<ushort>(y,x) = x == 0 ? 0 : *maxDisp.ptr<ushort>(y,x - 1);	
				}
			} else {
				*minDisp.ptr<ushort>(y,x) = std::max(0, d - delta);
				*maxDisp.ptr<ushort>(y,x) = std::min(max_disp, d + delta);
			}
		}
	}
}

class StereoCorrespondence {
public:
	virtual Mat compute(const Mat& left, const Mat& right) = 0;	

	virtual std::string getName() const = 0;
};

class SGM : public StereoCorrespondence {
public:
	virtual Mat compute(const Mat& left, const Mat& right) override {
		std::ostringstream stringStream;
		stringStream << "Processing image with size " << left.size();
		processTimer.start(stringStream.str());

		const Size size = left.size();

		CostCalculator& costs = SimpleCostCalculator(right, left, block, max_disp, -1);
		//CostCalculator& costs = RuntimeCostCalculator(right, left, block, max_disp, -1);

		DynamicDirection dd(right, left, costs , size, max_disp, penalty1, penalty2);
		const Mat dispL = DynamicImage(max_disp).calculateDisparity(size , dd);

		Mat disp = dispL;
		processTimer.finish();
		return disp;
	}
	virtual std::string getName() const {
		return "SGM";
	}

};

class RangeSgm : public StereoCorrespondence {
public:
	RangeSgm(StereoCorrespondence& estAlg) : estAlg(estAlg){}

	virtual Mat compute(const Mat& left, const Mat& right) override {
		const Size size = left.size();
		ushort maxDispRange = rangeAlgType == FAST ? 2 * delta : max_disp; //todo change to real calculating
		const Mat estDisp = estAlg.compute(left, right);

		Mat minDisp;
		Mat maxDisp;
		calEstimation(estDisp, minDisp, maxDisp);

		std::ostringstream stringStream;
		stringStream << "Processing image with size " << left.size();
		processTimer.start(stringStream.str());

		//CostCalculator& costs = SimpleCostCalculator(right, left, block, max_disp, -1);
		CostCalculator& costs = RuntimeCostCalculator(right, left, block, max_disp, -1);

		//CostCalculator& costs = RangeCostCalculator(right, left, block, maxDispRange, -1, minDisp, maxDisp);

		DynamicDirectionRange dd(right, left, costs, size, maxDispRange, penalty1, penalty2, minDisp, maxDisp); 
		DynamicImageWithRange dynamicImageRange(maxDispRange, minDisp, maxDisp);

		const Mat dispL = dynamicImageRange.calculateDisparity(size, dd);
		//stereoMedianBlur(dispL, dispR);
		Mat disp = dispL;
		processTimer.finish();
		return disp;
	}

	virtual std::string getName() const {
		return fmt("RANGE by %s", estAlg.getName().c_str());
	}
private:
	StereoCorrespondence& estAlg;
};

Timer estimatedTimer;
class Sgbm : public StereoCorrespondence {
	virtual Mat compute(const Mat& left, const Mat& right) override {
		estimatedTimer.start("Calculating estimated disparity");
		int sad = 3;
		Mat hackedLeft = addCols(left, max_disp);
		Mat hackedRight = addCols(right, max_disp);
		Mat disp(hackedLeft.size(), CV_16S);
		StereoSGBM sgbm( 0, (max_disp / 16 + 1) * 16, sad, 8 * sad * sad, 32 * sad * sad, 0, 0, 0, 0, 0, true);
		StereoBM sbm(StereoBM::BASIC_PRESET, (max_disp / 16 + 1) * 16);

		sgbm(hackedLeft , hackedRight, disp);

		Mat realDisp(left.size(), CV_16U);
		for(int y = 0; y < realDisp.size().height; ++y){
			for(int x = 0; x < realDisp.size().width; ++x){
				*realDisp.ptr<ushort>(y ,x) = (*disp.ptr<short>(y,x + max_disp) / 16);
			}
		}	
		estimatedTimer.finish();
		return realDisp;
	}
	virtual std::string getName() const {
		return "SGBM";
	}

};

class BM : public StereoCorrespondence {
	virtual Mat compute(const Mat& left, const Mat& right) override {
		estimatedTimer.start("Calculating estimated disparity");
		int sad = 3;
		Mat hackedLeft = addCols(left, max_disp);
		Mat hackedRight = addCols(right, max_disp);
		Mat disp(hackedLeft.size(), CV_16S);
		StereoBM sbm(StereoBM::BASIC_PRESET, (max_disp / 16 + 1) * 16);

		sbm(hackedLeft , hackedRight, disp);

		Mat realDisp(left.size(), CV_16U);
		for(int y = 0; y < realDisp.size().height; ++y){
			for(int x = 0; x < realDisp.size().width; ++x){
				*realDisp.ptr<ushort>(y ,x) = (*disp.ptr<short>(y,x + max_disp) / 16);
			}
		}	
		estimatedTimer.finish();
		return realDisp;
	}
	virtual std::string getName() const {
		return "BM";
	}
};

