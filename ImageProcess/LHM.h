#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "opencv2/opencv.hpp"

extern cv::Mat probeMat;

cv::Mat SolveAtitude_LHM(const cv::Mat& input2DPoints, const cv::Mat& input3DPoints, cv::Mat initRotation);
double point2Angle(const cv::Point2f& point, const cv::Point2f& center);
void sortPointsAnticlockXn(std::vector<cv::Point2f>& points, const cv::Point2f& center);
void ImagePointSortLeft(std::vector<cv::Point2f>& points);
cv::Mat LHM_Locate(const std::vector<cv::Point2f>& centers, double centerX, double centerY, const cv::Rect& roi);

CvMat* SolveAtitude_LHM2(CvMat Input3DPoint, CvMat Input2DPoint, CvMat InitRotation);
