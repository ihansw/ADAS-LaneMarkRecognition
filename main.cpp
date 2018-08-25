#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/calib3d.hpp"
#include <chrono>
#include "lanedetection.hpp"

using namespace std;

int main(int argc, char* argv[]){

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	cv::VideoCapture cap(argv[1]);

	if (!cap.isOpened()){
			cerr << "Unable to connect to camera or No video input" << endl;
			return 1;
	}

	int width_frame = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int height_frame = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	cap.set(CV_CAP_PROP_AUTOFOCUS, 0);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);

	// ROI
	int roiTop = 4 * height_frame / 9;
	int roiBottom = 3.5 * height_frame / 9;

	// Record video
	cv::VideoWriter writer; //Video writer
	string vid_filename = "OutputVideo.avi"; //filename string
	int fcc = CV_FOURCC('D', 'I', 'V', '3'); //fourcc intege
	int fps = 5; //frames per sec integer
	cv::Size frameSize(width_frame, height_frame); //frame size
	writer = cv::VideoWriter(vid_filename, fcc, fps, frameSize);
	if (!writer.isOpened()){
			cout << "ERROR OPENING FILE FOR WRITE" << endl;
			getchar();
			return -1;
	}

	cv::VideoWriter writer2; //Video writer
	string vid_filename2 = "RawVideo.avi";   //filename string
	writer2 = cv::VideoWriter(vid_filename2, fcc, fps, frameSize);
	if (!writer2.isOpened()){
			cout << "ERROR OPENING FILE FOR WRITE" << endl;
			getchar();
			return -1;
	}

	cv::Mat img;
	cv::Mat img_raw;
	// FPS calculation
	auto FPS_StartTime = std::chrono::system_clock::now();
	int numFrame = 0;

	// Initialize parameters for Lane Departure Alert
	bool prevLane = true;

	cv::Point prev_lanes[4];

	cv::Point LT = cv::Point(0,0);
	cv::Point LB = cv::Point(0,0);
	cv::Point RT = cv::Point(0,0);
	cv::Point RB = cv::Point(0,0);
	prev_lanes[0] = LT;
	prev_lanes[1] = LB;
	prev_lanes[2] = RT;
	prev_lanes[3] = RB;

	cv::Point* arr_prev = prev_lanes;

	int count = 0;

	for(;;){
			count ++;
			// record the duration
			end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = end-start;

			// FPS calculation
			if(numFrame != 15)	numFrame++;

			else{
					std::chrono::duration<double, std::milli> FPS_Duration = (std::chrono::system_clock::now() - FPS_StartTime);
					double FPS_Duration_double = FPS_Duration.count()/1000.0;
					double FPS = 15/FPS_Duration_double;
					//cout << "After 30 frames, Time elapsed: " << Duration_double << endl;
					std::cout << "FPS: " << FPS << std::endl;
					numFrame = 0;
					FPS_StartTime = std::chrono::system_clock::now();
			}

			// apply the detector to an opencv mat
			cap >> img;
			img.copyTo(img_raw);

			//lane departure warning
			cv::Point* arr = detectLanes(img);

			// cout << "arr[0]: " << arr[0]  << endl;
			// cout << "arr[1]: " << arr[1]  << endl;
			// cout << "arr[2]: " << arr[2]  << endl;
			// cout << "arr[3]: " << arr[3]  << endl;
			// // cout << arr_0.x << endl;
			// // printf("arr[0].x:  %d\n", arr_0.x);
			// // printf("arr_prev[0].x:  %d\n", arr_prev[0].x);
			// bool ans = inLane(arr, arr_prev, &img, prevLane);
			//
			// //~ printf("Previous X: %d\n", LSXX);
			// //~ printf("Left difference X: %d\n", LDX);
			//
			// arr_prev[0] = arr[0];
			// arr_prev[1] = arr[1];
			// arr_prev[2] = arr[2];
			// arr_prev[3] = arr[3];

			//~ printf("Previous X: %d\n", LSXX);
			//~ printf("Left difference X: %d\n", LDX);

			// prevLane = ans;

			// cv::Rect ROI = cv::Rect(0, roiTop, width_frame, roiBottom);
			// cv::Mat crop = img(ROI);

			cv::imshow("rectangles in output", img);
			// cv::imshow("roi", crop);
			//~ if(count > 300){
			writer.write(img);
			writer2.write(img_raw);
			//~ }
			switch (cv::waitKey(1)){
					case 27:
					//'esc' has been pressed(ASCII value for 'esc' is 27)
					// exit the program
					writer.release();
					return 0;
			}

	}

	return 0;
}
