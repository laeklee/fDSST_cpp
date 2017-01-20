#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

#include "fdssttracker.hpp"


#include <windows.h>
//#include <dirent.h>



using namespace std;
using namespace cv;

std::vector <cv::Mat> imgVec;

int main(int argc, char* argv[]){

	if (argc > 5) return -1;

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = true;
	bool LAB = false;
	// Create KCFTracker object
	FDSSTTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	// DSSTTracker tracker;

	int count = 1;
	cv::Mat processImg;
	char name[7];
	std::string imgName;
	std::string imgPath = "./person19/";

	//get init target box params from information file
	std::ifstream initInfoFile;
	std::string fileName = imgPath + "initInfo.txt";
	initInfoFile.open(fileName);
	std::string firstLine;
	std::getline(initInfoFile, firstLine);
	float initX, initY, initWidth, initHegiht;
	char ch;
	std::istringstream ss(firstLine);
	ss >> initX, ss >> ch;
	ss >> initY, ss >> ch;
	ss >> initWidth, ss >> ch;
	ss >> initHegiht, ss >> ch;

	cv::Rect initRect = cv::Rect(initX, initY, initWidth, initHegiht);

	double duration = 0;
	for (;;)
	{
		auto t_start = clock();
		sprintf_s(name, "%06d", count);
		std::string imgFinalPath = imgPath + std::string(name) + ".jpg";
		processImg = cv::imread(imgFinalPath, IMREAD_GRAYSCALE);

		//processImg = cv::imread(imgFinalPath, CV_LOAD_IMAGE_COLOR);

		if (processImg.empty())
		{
			break;
		}
		cv::Rect showRect;
		if (count == 1)
		{
			tracker.init(initRect, processImg);
			showRect = initRect;
		}
		else{
			showRect = tracker.update(processImg);
			// printf( "rect (w h): %d %d \n" , showRect.width, showRect.height);
		}
		auto t_end = clock();
		duration += (double)(t_end - t_start) / CLOCKS_PER_SEC;
		// cout << "all duration: " << duration << "\n";

		cv::rectangle(processImg, showRect, cv::Scalar(0, 255, 0));
		cv::imshow("windows", processImg);
		cv::waitKey(1);
		count++;


	}
	std::cout << "FPS: " <<count / duration << "\n";

	system("pause");
	return 0;

}
