#pragma once

#include <opencv2/opencv.hpp>
#include <ctime>

inline void addGausssNoise(const cv::Mat &src,cv::Mat &dst, double ratio)
{
	cv::Mat tmp = cv::Mat(src.rows,src.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::RNG rng(time(NULL));
	rng.fill(tmp, cv::RNG::NORMAL, 0, ratio);
	dst = src + tmp;
}

inline void addUniformNoise(const cv::Mat &src, cv::Mat &dst, double ratio)
{
	cv::Mat tmp = cv::Mat(src.rows, src.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::RNG rng(time(NULL));
	rng.fill(tmp, cv::RNG::UNIFORM, 0, ratio);
	dst = src + tmp;
}

inline void addImpulseNoise(const cv::Mat &src, cv::Mat &dst, double ratio)
{
	cv::Mat tmp;
	int i, j;
	int size;
	size = src.cols*src.rows;
	src.copyTo(tmp);
	int n = int(size * ratio);
	for (int k = 0; k < n; k++)
	{
		i = rand() % src.cols;
		j = rand() % src.rows;
		if (src.channels() == 1)
			tmp.at<uchar>(j, i) = 255;
		else if (src.channels() == 3)
		{
			tmp.at<cv::Vec3b>(j, i)[0] = 255;
			tmp.at<cv::Vec3b>(j, i)[1] = 255;
			tmp.at<cv::Vec3b>(j, i)[2] = 255;
		}
	}
	tmp.copyTo(dst);
}