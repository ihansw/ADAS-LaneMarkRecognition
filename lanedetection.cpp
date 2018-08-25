#include <iostream>
#include "opencv2/core.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "laneDeparture.hpp"

using namespace std;

void roiAdjust(cv::Vec4i *l, int frameHeight, int yTop, double m, int side){

		// adjust the height according to the ROI
		(*l)[1] = (*l)[1] + (yTop);
		(*l)[3] = (*l)[3] + (yTop);

		// side = 1 --> right lane
		if (side == 1) {
				double b = (*l)[3] - m * ((*l)[2]);

				if ((*l)[1] > yTop){
						(*l)[1] = yTop;
						(*l)[0] = ((*l)[1] - b) / m;
				}

				(*l)[3] = frameHeight;
				(*l)[2] = ((*l)[3] - b) / m;
		}

		// side = 2 --> left lane
		else{
				double b = (*l)[1] - m * ((*l)[0]);

				if ((*l)[3] > yTop){
					(*l)[3] = yTop;
					(*l)[2] = ((*l)[3] - b) / m;
				}

				(*l)[1] = frameHeight;
				(*l)[0] = ((*l)[1] - b) / m;
		}

}

cv::Point* detectLanes(cv::Mat& cvFrameRGB) {
		int frameHeight = cvFrameRGB.size().height;
		int frameWidth = cvFrameRGB.size().width;
    int yTop = 4 * cvFrameRGB.size().height / 9;
    int ROIHeight = 3.5 * cvFrameRGB.size().height / 9;

    // Lane detection only on cropped image.
    cv::Rect ROI = cv::Rect(0, yTop, frameWidth, ROIHeight);
    cv::Mat gray;
    cv::cvtColor(cvFrameRGB, gray, CV_RGB2GRAY);
    cv::Mat crop = gray(ROI);

    cv::Mat blurred;
    cv::Mat matCanny,matCannyc;
    cv::Mat canny;

		// Blur the image
    cv::GaussianBlur(crop, blurred, cv::Size(5,5),1.8);
    // Canny
    cv::Canny(blurred,matCanny,50,100 );

    std::vector<cv::Vec4i> plines;
    // HoughLine Transform
    cv::HoughLinesP(matCanny, plines, 1, CV_PI/180, 40, 90, 220 );
    //cv::Mat result(frame.size(), CV_8U, cv::Scalar(255));
    cv::Mat result(crop.size(), CV_8U, cv::Scalar(255));
    crop.copyTo(result);
    //frame.copyTo(result);
    double high_m = 0;
    double low_m = 0;
    int left_i = -1;
    int right_i = -1;

    if (!plines.empty()){
      //plines.resize(plines.cols);
      //cv::Mat plines(1, plines.cols, CV_32SC4, &plines[0]);
      //plines.download(plines_gpu);
    }

		// Go Through all of the lines that were detected and assign right and left lane.
    for (size_t i = 0; i < plines.size(); i++){
      cv::Vec4i l = plines[i];
      // line( result, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA );

	  	// Calculate the slopes
      double delta_y = double(l[3] - l[1]);
      double delta_x = double(l[2] - l[0]);
      double x_mid = double(l[2] + l[0])/2;
      double m = (delta_y) / (delta_x);

			// Check for right lane
      if (m > high_m && (m > 0.5 && m < 20.0) ){
		  		if(x_mid > frameWidth/2){
						  high_m = m;
						  right_i = i;
		  		}
      }

			// Check for left lane
      if (m < low_m && m < -0.5 && m > -20.0 ){
				  if(x_mid < frameWidth/2){
						  low_m = m;
						  left_i = i;
				  }
      }
    }

    cv::Point lanes[4];
    cv::Point* lanesPtr = lanes;

		// if right lane found
    if (right_i != -1){
      cv::Vec4i l = plines[right_i];

			// adjust the height according to the ROI
			roiAdjust(&l, frameHeight, yTop, high_m, 1);
			// adjust the height according to the ROI

      cv::circle(cvFrameRGB, cv::Point(l[0], l[1]), 3, cv::Scalar(0, 255, 0), 5, 8, 0); // Green (Right)
      cv::line(cvFrameRGB, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 3, CV_AA);
      cv::Point startP = cv::Point(l[0], l[1]);
      cv::Point endP = cv::Point(l[2], l[3]);
      lanes[1] = startP;
      lanes[0] = endP;
    }

		// if left lane found
    if (left_i != -1){
      cv::Vec4i l = plines[left_i];

			roiAdjust(&l, frameHeight, yTop, low_m, 2);

      cv::circle(cvFrameRGB, cv::Point(l[2], l[3]), 3, cv::Scalar(0, 255, 255), 5, 8, 0); // Yellow (Left)
      cv::line(cvFrameRGB, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 2, CV_AA);
      cv::Point startP = cv::Point(l[0], l[1]);
			cv::Point endP = cv::Point(l[2], l[3]);
      lanes[2] = startP;
      lanes[3] = endP;
    }

    return lanesPtr;
}
