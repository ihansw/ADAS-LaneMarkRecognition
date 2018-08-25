#include "opencv2/core.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"



bool inLane(cv::Point* lanes, cv::Point* prev_lanes, cv::Mat* framePtr, bool prev);
void drawAlert(cv::Mat frame);
