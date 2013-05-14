
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
	const string files[] = {"tsukuba", "venus", "cones", "teddy", "Aloe", "Cloth4", "half_alloe", "full_alloe"};
	const int scales[] = { 16, 8, 4, 4, 1, 1, 1, 1};
	const int max_disps[] = {70, 70, 70, 70, 70, 70, 350, 50};
	for(int i = 6; i < 7; ++i){
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