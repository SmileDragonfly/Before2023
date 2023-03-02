#include "opencv2/opencv.hpp"
#include "spdlog/spdlog.h"
#include "Utils.h"

// Load cam configuaration
cv::Mat extrinMat;
cv::Mat intrinMatLeft;
cv::Mat intrinMatRight;
cv::Mat rotationMatRight2Left;
cv::Mat translateMatRight2Left;
cv::Mat imgLeft;
cv::Mat imgRight;
cv::Mat imgDes;

static void OnMouseCallback(int event, int x, int y, int flags, void* userdata)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		spdlog::info("On left mouse click: ({}, {})", x, y);
		cv::Mat imgShow = imgDes.clone();
		cv::Point pointRight = GetMappingPoint(cv::Point(x, y), intrinMatLeft, intrinMatRight, rotationMatRight2Left, translateMatRight2Left);
		cv::Point pointRightShow(pointRight.x + imgLeft.cols, pointRight.y + imgLeft.rows);
		cv::circle(imgShow, cv::Point(x, y), 2, cv::Scalar(0, 0, 255), cv::FILLED);
		cv::circle(imgShow, pointRightShow, 2, cv::Scalar(0, 255, 0), cv::FILLED);
		cv::imshow("Result", imgShow);
	}
	return;
}

int main()
{
	cv::String sLeft = "D:\\VSProject\\CPP2022\\UWCorrespondenceMatching\\Data\\Left.jpeg";
	cv::String sRight = "D:\\VSProject\\CPP2022\\UWCorrespondenceMatching\\Data\\Right.jpeg";
	cv::String sResultFile = "D:\\VSProject\\CPP2022\\UWCorrespondenceMatching\\Data\\Result.ini";

	LoadCamConfiguration(sResultFile, &extrinMat, &intrinMatLeft, &intrinMatRight, &rotationMatRight2Left, &translateMatRight2Left);

	// Read images
	imgLeft = cv::imread(sLeft);
	imgRight = cv::imread(sRight);
	hconcat(imgLeft, imgRight, imgDes);

	// Set mouse callback on click
	cv::namedWindow("Result");
	cv::setMouseCallback("Result", &OnMouseCallback, 0);

	// Show result
	imshow("Result", imgDes);
	cv::waitKey();
	return 0;
}