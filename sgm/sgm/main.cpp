
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

void test(StereoCorrespondence& alg){
	const char* path = "X:\\Dropbox\\SGM\\dataset\\test";
	const char* files[] = {"tsukuba", "venus", "cones",  
		"teddy", "Aloe", "Cloth4", 
		"half_alloe", "half_alloe", "half_alloe", 
		"half_alloe", "half_alloe", "full_alloe", 
		"test1"};
	const int scales[] = { 16, 8, 4, 
		4, 1, 1, 
		1, 1, 1,
		1, 1, 1,
		4};
	const int max_disps[] = {70, 70, 70,  
		70, 70, 70, 
		100, 150, 250, 
		300, 350, 200,
		50};
	//for(int i = 6; i < 11; ++i){

	//for(int i = 7; i < 8; ++i){
	for(int i = 0; i < 4; ++i){
		std::cout << fmt("%s\\%s\\imL.png", path, files[i]) << "\n";
		const Mat left = imread(fmt("%s\\%s\\imL.png", path, files[i]), 0);
		const Mat right = imread(fmt("%s\\%s\\imR.png", path, files[i]), 0);
		
		max_disp = max_disps[i];
		std::cout << alg.getName() << " with max disp " << max_disp << "\n";

		Mat disp = alg.compute(left, right);
		std::string alg_name = alg.getName();
		cv::imwrite(fmt("%s\\disps\\%s_%s.png", path, files[i] , alg_name.c_str()), disp * scales[i] * 256);
	}
}


void test2(StereoCorrespondence& alg){
	const char* path = "X:\\Dropbox\\SGM\\dataset\\photos";
	const int downscale = 4; //todo

	for(int i = 1; i < 20; ++i){
		std::cout << fmt("%s for %s\\rect_%d_0.png", alg.getName().c_str(), path, i) << "\n";
		const Mat tleft = imread(fmt("%s\\rect_%d_0.png", path, i), 0);
		const Mat tright = imread(fmt("%s\\rect_%d_1.png", path, i), 0);
		Mat left;
		Mat right;

		const Size size(tleft.size().width / downscale, tleft.size().height / downscale);

		cv::resize(tleft, left, size);
		cv::resize(tright, right, size);

		max_disp = 800 / downscale;
		Mat disp = alg.compute(left, right);
		std::string alg_name = alg.getName(); ;
		cv::imwrite(fmt("%s\\rect_%d_0_%s.png", path, i, alg_name.c_str()), disp * 256);
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
	processTimer.changeState(true);
	estimatedTimer.changeState(true);
	//test(Sgbm());
	//test(BM());
	//test(SGM());
	test(RangeSgm(Sgbm()));
	test(RangeSgm(BM()));
	std::cout << "HAPPY END";
	getchar();
}
//todo бага в рэндж подсчете


//Bad pixels [0.786412, 0, 0, 0] penalty1 19 penalty2 33 - blur + uniq -
//Bad pixels [0.775164, 0, 0, 0] penalty1 19 penalty2 33 - blur - uniq -
//Bad pixels [0.768847, 0, 0, 0] penalty1 19 penalty2 33 - blur - uniq +

//6464 ms