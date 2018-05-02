#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "opencv2/opencv.hpp"

cv::Mat SolveAtitude_LHM(const cv::Mat& input2DPoints, const cv::Mat& input3DPoints, cv::Mat initRotation);
double point2Angle(const cv::Point2f& point, const cv::Point2f& center);
void sortPointsAnticlockXn(std::vector<cv::Point2f>& points, const cv::Point2f& center);
extern cv::Mat probeMat;

inline double point2Angle(const cv::Point2f& point,const cv::Point2f& center)
{
	return atan2(point.x - center.x, point.y - center.y);
}

void sortPointsAnticlockXn(std::vector<cv::Point2f>& points, const cv::Point2f& center)
{
	for (auto i = 0; i < points.size();++i)
	{
		double maxAngle = -M_PI * 2;
		int maxIndex = i;
		for(auto j = i;j < points.size();++j)
		{
			auto tmp = point2Angle(points[j], center);
			if(tmp > maxAngle)
			{
				maxAngle = tmp;
				maxIndex = j;
			}
		}
		std::swap(points[i], points[maxIndex]);
	}
}

cv::Mat LHM_Locate(const std::vector<cv::Point2f>& centers, double centerX, double centerY, double xPixelSize, double yPixelSize, const cv::Rect& roi)
{
	std::vector<cv::Point2f> points(centers);
	sortPointsAnticlockXn(points, cv::Point2f(centerX, centerY));
	cv::Mat drogue2DImagePotionMat(points.size(), 3, CV_64FC1);
	cv::Mat drogue3DPotionMat(points.size(), 3, CV_64FC1);
	for(int i = 0;i < points.size();++i)
	{
		drogue2DImagePotionMat.at<double>(i, 0) = points[i].x;
		drogue2DImagePotionMat.at<double>(i, 1) = points[i].y;
		drogue2DImagePotionMat.at<double>(i, 2) = 1;
	}
	double InitRotate[3][3], pitch = 0, yaw = 0, roll = 0;
	InitRotate[0][0] = cos(roll)*cos(pitch);
	InitRotate[1][0] = sin(roll)*cos(pitch);
	InitRotate[2][0] = -sin(pitch);
	InitRotate[0][1] = cos(roll)*sin(pitch)*sin(yaw) - sin(yaw);
	InitRotate[1][1] = sin(roll)*sin(pitch) + cos(yaw)*cos(roll);
	InitRotate[2][1] = cos(yaw)*sin(yaw);
	InitRotate[0][2] = cos(roll)*sin(pitch) - sin(yaw)*sin(roll);
	InitRotate[1][2] = sin(roll)*sin(pitch)*cos(yaw) + cos(roll)*sin(yaw);
	InitRotate[2][2] = cos(pitch)*cos(yaw);
	cv::Mat initRotateMat(3, 3, CV_64FC1, InitRotate);
	cv::Mat translation = SolveAtitude_LHM(drogue2DImagePotionMat, drogue3DPotionMat, initRotateMat);
	return cv::Mat({ translation.at<double>(0,0),translation.at<double>(2,0),translation.at<double>(1,0) });
}

cv::Mat SolveAtitude_LHM(const cv::Mat& input2DPoints, const cv::Mat& input3DPoints, cv::Mat initRotation)
{
	int pointNum = input2DPoints.rows;
	double Error = 0.01;
	cv::Mat translation(3, 1, CV_64FC1);
	cv::Mat probeMatInv;
	cv::invert(probeMat, probeMatInv);
	int count = 0;

	while(true)
	{
		double ERt = 0;
		cv::Mat temp(3, 1, CV_64FC1, cv::Scalar::all(0));
		cv::Mat tempVi(3, 3, CV_64FC1, cv::Scalar::all(0));
		cv::Mat identity(3, 3, CV_64FC1);
		cv::setIdentity(identity);

		for(int i = 0;i < pointNum;++i)
		{
			cv::Mat viMat(input2DPoints.row(i).t());

			viMat = probeMatInv * viMat;
			cv::Mat dst1 = viMat * viMat.t();
			cv::Mat dst2 = viMat.t() * viMat;
			cv::Mat viMat1(3, 3, CV_64FC1);
			dst1.convertTo(viMat1, CV_64FC1, 1/dst2.at<double>(0, 0));

			cv::Mat piMat(input3DPoints.row(i).t());
			cv::Mat dst3 = initRotation * piMat;
			cv::Mat dst5 = viMat - identity;
			temp += dst5 * dst3;
			tempVi += viMat1;
		}

		cv::Mat temp2(3, 3, CV_64FC1);
		tempVi.convertTo(temp2, -1, 1.0 / pointNum);
		temp2 = identity - temp2;
		cv::invert(temp2, temp2);
		temp2.convertTo(temp2, -1, 1.0 / pointNum);
		translation = temp2 * temp2;

		cv::Mat tempQMean(3, 1, CV_64FC1, cv::Scalar::all(0));
		cv::Mat tempPMean(3, 1, CV_64FC1, cv::Scalar::all(0));

		for(int i = 0;i < pointNum;++i)
		{
			cv::Mat viMat(input2DPoints.row(i).t());
			viMat = probeMatInv * viMat;
			cv::Mat dst1 = viMat * viMat.t();
			cv::Mat dst2 = viMat.t() * viMat;
			cv::Mat viMat1(3, 3, CV_64FC1);
			dst1.convertTo(viMat1, CV_64FC1, 1 / dst2.at<double>(0, 0));

			cv::Mat piMat(input3DPoints.row(i).t());
			cv::Mat tempERt = initRotation * piMat + translation;
			tempERt = (identity - viMat1) * tempERt;
			ERt += cv::norm(tempERt);

			cv::Mat tempMedia = initRotation * piMat + translation;
			cv::Mat qiR = viMat1 * tempMedia;
			tempQMean += qiR;
			tempPMean += piMat;
		}

		if(++count > 10)
		{
			Error += 0.1;
		}
		if(ERt < Error)
		{
			break;
		}

		tempQMean.convertTo(tempQMean, -1, 1.0 / pointNum);
		tempPMean.convertTo(tempPMean, -1, 1.0 / pointNum);

		cv::Mat MR(3, 3, CV_64FC1, cv::Scalar::all(0));
		for(int i = 0;i < pointNum;++i)
		{
			cv::Mat dQiR(3, 1, CV_64FC1);
			cv::Mat dPi(3, 1, CV_64FC1);
			cv::Mat viMat(input2DPoints.row(i).t());

			viMat = probeMatInv * viMat;
			cv::Mat dst1 = viMat * viMat.t();
			cv::Mat dst2 = viMat.t() * viMat;
			cv::Mat viMat1(3, 3, CV_64FC1);
			dst1.convertTo(viMat1, CV_64FC1, 1 / dst2.at<double>(0, 0));

			cv::Mat piMat(input3DPoints.row(i).t());
			cv::Mat tempMedia = initRotation * piMat + translation;
			cv::Mat qiR = viMat1 * tempMedia;

			dPi = piMat - tempPMean;
			dQiR = qiR - tempQMean;
			cv::Mat tempMR = dQiR * dPi.t();
			MR += tempMR;
		}

		cv::Mat W(3, 3, CV_64FC1), U(3, 3, CV_64FC1), V(3, 3, CV_64FC1);
		cv::SVD::compute(MR, W, U, V);
		initRotation = V * U.t();
	}

	return translation;
}