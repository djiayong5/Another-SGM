
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
#include "Process.h"

using namespace cv;
using namespace std;


#define SIMPLE 1
#define RANGE 2

void test(int type){
	const string path("X:\\Dropbox\\SGM\\dataset\\test\\");
	const string files[] = {"tsukuba", "venus", "cones",  
							"teddy", "Aloe", "Cloth4", 
							"half_alloe", "half_alloe", "half_alloe", 
							"half_alloe", "half_alloe", "full_alloe"};
	const int scales[] = { 16, 8, 4, 4, 1, 1, 1, 1, 1, 1, 1};
	const int max_disps[] = {70, 70, 70,  
							70, 70, 70, 
							100, 150, 250, 
							300, 350, 150};
	for(int i = 10; i < 11; ++i){
		const string file = files[i] + "\\";
		const Mat left = imread(path + file + "imL.png", 0);
		const Mat right = imread(path + file + "imR.png", 0);
		max_disp = max_disps[i];
		Mat disp;
		std::string add_info;
		if(type == SIMPLE){
			disp = simpleProcess(left, right);
			add_info = "SIMPLE";
		} else {
			disp = processWithRange(left, right);
			add_info = "RANGE";
		}
		cv::imwrite(path + "disps\\" + files[i] + "_" + add_info + ".png", disp * scales[i] * 256);
	}
}

void test2(){
	const string path("X:\\Dropbox\\SGM\\dataset\\test\\");
	for(int curr_disp = 100; curr_disp < 350; curr_disp += 50){
		const Mat left = imread(path + "half_alloe\\imL.png", 0);
		const Mat right = imread(path + "half_alloe\\imR.png", 0);
		const Mat readDisp = imread(path + "disps\\half_alloe_RANGE.png", 0);
		Mat estDisp;
		readDisp.convertTo(estDisp, CV_16U, 1);
		Mat minDisp = estDisp.clone();
		Mat maxDisp = estDisp.clone();
		max_disp = curr_disp;
		for(int y = 0; y < estDisp.size().height; ++y){
			for(int x = 0; x < estDisp.size().width; ++x){
				ushort d = *estDisp.ptr<ushort>(y,x);
				*minDisp.ptr<ushort>(y,x) = std::max(0, d - delta);
				*maxDisp.ptr<ushort>(y,x) = std::min(max_disp, d + delta);
			}
		}
		
		Mat	disp = processWithRange(left, right, minDisp, maxDisp);
		std::cout << max_disp << "\n";
		cv::imwrite(path + "disps\\half_alloe_RANGE_2.png", disp * 256);
	}
}

std::ofstream logger("log.txt");

void calError(){
	const string path("X:\\Dropbox\\SGM\\dataset\\test\\");
	const Mat dispReal = imread(path + "cones//disp2.png", 0);
	const Mat disp = imread(path + "disps/cones.png", 0);
	const Mat diff = disp.clone();
	cv::absdiff(dispReal, disp, diff);

	int threshold = 2;
	const Mat binDiff = disp.clone();
	cv::threshold(diff, binDiff, threshold, 255, cv::THRESH_BINARY_INV);
	logger << "Bad pixels " 
		<< sum(binDiff / 255) / (disp.size().width * disp.size().height) 
		<< " penalty1 " << penalty1 << " penalty2 " << penalty2 << "\n";
}

int main(){
	//test2();
	std::cout << "SIMPLE\n";
	//test(SIMPLE);
	std::cout << "RANGE\n";
	test(RANGE);
	std::cout << "HAPPY END";
	getchar();
	//	temp2();
}

//Bad pixels [0.786412, 0, 0, 0] penalty1 19 penalty2 33 - blur + uniq -
//Bad pixels [0.775164, 0, 0, 0] penalty1 19 penalty2 33 - blur - uniq -
//Bad pixels [0.768847, 0, 0, 0] penalty1 19 penalty2 33 - blur - uniq +