#include "opencv2/core.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "laneDeparture.hpp"

static int count = 0;



//////////////////////////////////////////////////////////////////////////////////////////////////////


bool inLane(cv::Point* lanes, cv::Point* prev_lanes, cv::Mat* framePtr, bool prev) {

	cv::Point rightStart = lanes[0]; // RB
	cv::Point rightEnd = lanes[1]; // RT
	cv::Point leftStart = lanes[2]; // LB
	cv::Point leftEnd = lanes[3]; // LT

	cv::Point p_rightStart = prev_lanes[0];
	cv::Point p_leftStart = prev_lanes[2];
	cv::Point p_rightEnd = prev_lanes[1];
	cv::Point p_leftEnd = prev_lanes[3];

	cv::Mat frame = *framePtr;


	//printf("rightStart: %d, %d\n", rightStart.x, rightStart.y);
	//printf("leftStart: %d\n", leftStart.x);

	int LSX = leftStart.x;
	int RSX = rightStart.x;
	//int LSY = leftStart.y;
	//int RSY = rightStart.y;./

	int LSXX = p_leftStart.x;
	int RSXX = p_rightStart.x;
	//int LSYY = p_leftStart.y;
	//int RSYY = p_rightStart.y;

	int LDX = (LSX - LSXX);
	int RDX = (RSX - RSXX);
	//int LDY = LSY - LSYY;
	//int RDY = RSY - RSYY;

	// printf("Current X: %d\n", LSX);
	// printf("Previous X: %d\n", LSXX);
	// printf("Left difference X: %d\n", LDX);
	std::string LDX_text = std::to_string(LDX);
	cv::putText(frame, LDX_text, cv::Point(500,500), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0,0,255), 2);
	//printf("Right difference X: %d\n", RDX);

	int i = 0;

	if (LSX > LSXX) {
	ConditionFeedback2(frame);
	i++;
	if (i>4) {
		ConditionFeedback5(frame);
		return false;
	}
	return true;
}
	else if (LSX < LSXX) {
		ConditionFeedback1(frame);
		if (i>0) {
			i = 0;
			return false;
		}
		i--;
		if (i<(-4)){
			ConditionFeedback6(frame);
			return false;
		}
		return true;
	}

	else {
		ConditionFeedback4(frame);
		return true;
	}










	//~ // printf("rightStart: %d, %d\n", rightStart.x, rightStart.y);
	//~ // printf("rightEnd: %d, %d\n", rightEnd.x, rightEnd.y);
	//~ // printf("leftStart: %d, %d\n", leftStart.x, leftStart.y);
	//~ // printf("leftEnd: %d, %d\n", leftEnd.x, leftEnd.y);
	//~ int leftMid = (leftStart.x + leftEnd.x) / 2;
	//~ int rightMid = (rightStart.x + rightEnd.x) / 2;
	//~ float threshold = 0.27;
	//~ float cameraOffset = 0.00; // positive value means that camera is on the right side of th vehicle, negative means left side
	//~ printf("------------------------------\n");


//~ /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//~ if (leftMid != 0 && rightMid != 0) {

		//~ // printf("mids: %d, %d\n", leftMid, rightMid);

	//~ // printf("rightStart: %d, %d\n", rightStart.x, rightStart.y);
	//~ // printf("rightEnd: %d, %d\n", rightEnd.x, rightEnd.y);
	//~ // printf("leftStart: %d, %d\n", leftStart.x, leftStart.y);
	//~ // printf("leftEnd: %d, %d\n", leftEnd.x, leftEnd.y);
	//~ // int avg = (leftEnd.x + rightEnd.x) / 2;
		//~ // int mid = frame.cols / 2;
		//~ // int offset = abs(avg - mid);

		//~ // float leftSlope = fabs(((float)leftEnd.y - leftStart.y) / (leftEnd.x - leftStart.x));
		//~ // float rightSlope = fabs(((float)rightEnd.y - rightStart.y) / (rightEnd.x - rightStart.x));
		//~ // // printf("slopes: %f, %f\n", leftSlope, rightSlope);

		//~ // float percent = (float) offset / frame.cols;
		//~ // // printf("%f\n", percent);
		//~ // float slopeDiff = fabs(leftSlope - rightSlope)*10;
		//~ // printf("slopeDiff: %f\n", slopeDiff);

		//~ float leftDiff = fabs(((float)leftStart.x - (frame.cols/2)) / frame.cols);
		//~ float rightDiff = fabs(((float)rightStart.x - (frame.cols/2)) / frame.cols);
		//~ printf("left diff: %f\n", leftDiff - cameraOffset);
		//~ printf("right diff: %f\n", rightDiff + cameraOffset);

		//~ // if (slopeDiff > 4) {
		//~ if (leftDiff - cameraOffset < threshold || rightDiff + cameraOffset < threshold) {
			//~ drawAlert(frame);
			//~ ConditionFeedback1(frame);
			//~ return false;
		//~ }
		//~ return true;

//~ /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//~ /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//~ } else if (leftMid != 0) {
		//~ float diff = fabs(((float)leftStart.x - (frame.cols/2)) / frame.cols);
		//~ printf("left diff: %f\n", diff - cameraOffset);
		//~ if (diff - cameraOffset < threshold) {
			//~ drawAlert(frame);
			//~ ConditionFeedback2(frame);
			//~ return false;
		//~ }
		//~ return true;
//~ /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//~ /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//~ } else if (rightMid != 0) {
		//~ float diff = fabs(((float)rightStart.x - (frame.cols/2)) / frame.cols);
		//~ printf("right diff: %f\n", diff + cameraOffset);
		//~ if (diff + cameraOffset < threshold) {
			//~ drawAlert(frame);
			//~ ConditionFeedback3(frame);
			//~ return false;
		//~ }
		//~ return true;
//~ /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//~ /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//~ } else {
		//~ printf("Unable to establish lanes\n");
		//~ if (!prev) {
			//~ cv::Mat frame = *framePtr;
			//~ drawAlert(frame);
			//~ ConditionFeedback4(frame);
		//~ }
		//~ return prev;
	//~ }
//~ /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}
