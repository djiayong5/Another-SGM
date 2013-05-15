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

Timer imageProcessTimer;

/*Mat minDisp(size, CV_8U, Scalar(0));
Mat maxDisp(size, CV_8U, Scalar(max_disp));
Mat dispL = DynamicImageWithRange().calculateDisparity(right, left, costsLeft, size, max_disp, penalty1, penalty2, minDisp, maxDisp);*/

//for(int pen1 = 20; pen1 < 30; ++pen1){
//		for(int pen2 = pen1 + 12; pen2 <  pen1 + 30; pen2 += 2){
//			penalty1 = pen1;
//			penalty2 = pen2;
//			test();
//			temp2();
//			logger.flush();
//
//		}
//	}

Mat simpleProcess(const Mat& left, const Mat& right){
	std::ostringstream stringStream;
	stringStream << "Processing image with size " << left.size();
	imageProcessTimer.start(stringStream.str());

	const Size size = left.size();

	//CostCalculator& costs = SimpleCostCalculator(right, left, block, max_disp, -1);
	CostCalculator& costs = RuntimeCostCalculator(right, left, block, max_disp, -1);
	DynamicDirection dd(right, left, costs , size, max_disp, penalty1, penalty2);
	const Mat dispL = DynamicImage(max_disp).calculateDisparity(size , dd);

	Mat disp = dispL;
	imageProcessTimer.finish();
	return disp;
}

Mat processWithRange(const Mat& left, const Mat& right, Mat& minDisp, Mat& maxDisp){
	std::ostringstream stringStream;
	stringStream << "Processing image with size " << left.size();
	imageProcessTimer.start(stringStream.str());

	const Size size = left.size();
	
	//CostCalculator& costs = SimpleCostCalculator(right, left, block, max_disp, -1);
	CostCalculator& costs = RuntimeCostCalculator(right, left, block, max_disp, -1);

	ushort maxDispRange = rangeAlgType == FAST ? 2 * delta : max_disp; //todo change to real calculating

	DynamicDirectionRange dd(right, left, costs, size, maxDispRange, penalty1, penalty2, minDisp, maxDisp); 
	DynamicImageWithRange dynamicImageRange(maxDispRange, minDisp, maxDisp);
	
	const Mat dispL = dynamicImageRange.calculateDisparity(size, dd);
	//stereoMedianBlur(dispL, dispR);
	Mat disp = dispL;
	imageProcessTimer.finish();
	return disp;
}

Mat addCols(const Mat& m, size_t sz){
	Mat tm(m.rows, m.cols + sz, m.type());
	m.copyTo(tm(Rect(Point(sz, 0), m.size())));
	return tm;
}
Mat processWithRange(const Mat& left, const Mat& right){
	timer.start("Calculating estimated disparity");
	int sad = 3;
	StereoSGBM sgbm( 0, (max_disp / 16 + 1) * 16, sad, 8 * sad * sad, 32 * sad * sad, 0, 0, 0, 0, 0, true);
	Mat hackedLeft = addCols(left, max_disp);
	Mat hackedRight = addCols(right, max_disp);
	Mat disp(hackedLeft.size(), CV_16S);

	sgbm(hackedLeft , hackedRight, disp);

	Mat realDisp(left.size(), CV_16U);
	for(int y = 0; y < realDisp.size().height; ++y){
		for(int x = 0; x < realDisp.size().width; ++x){
			*realDisp.ptr<ushort>(y ,x) = (*disp.ptr<short>(y,x + max_disp) / 16);
		}
	}	


	Mat minDisp = realDisp.clone();
	Mat maxDisp = realDisp.clone();
	for(int y = 0; y < realDisp.size().height; ++y){
		for(int x = 0; x < realDisp.size().width; ++x){
			ushort d = *realDisp.ptr<ushort>(y,x);
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
			if(rangeAlgType == FAST){
				//max_disp = 2 * delta;
			}
		}
	}
	//imwrite("X:\\Dropbox\\SGM\\dataset\\test\\disps\\f.png", realDisp * 256 * 16);
	/*
	imwrite("X:\\Dropbox\\SGM\\dataset\\test\\disps\\a.png", minDisp * 256);
	imwrite("X:\\Dropbox\\SGM\\dataset\\test\\disps\\b.png", maxDisp * 256);
	*/
	timer.finish();
	return processWithRange(left,right, minDisp, maxDisp);
}
