#pragma once

#include <opencv2/opencv.hpp>
#include <ctime>
#include <random>

inline void addGausssNoise(const cv::Mat &src,cv::Mat &dst, double ratio)
{
	cv::Mat tmp = cv::Mat(src.rows,src.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::RNG rng(time(NULL) ^ clock());
	rng.fill(tmp, cv::RNG::NORMAL, 0, ratio);
	dst = src + tmp;
}

inline void addUniformNoise(const cv::Mat &src, cv::Mat &dst, double ratio)
{
	cv::Mat tmp = cv::Mat(src.rows, src.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::RNG rng(time(NULL) ^ clock());
	rng.fill(tmp, cv::RNG::UNIFORM, -ratio / 2, ratio / 2);
	dst = src + tmp;
}

inline void addImpulseNoise(const cv::Mat &src, cv::Mat &dst, double ratio)
{
	cv::Mat tmp;
	int i, j, k, p;
	int size;
	size = src.cols*src.rows;
	src.copyTo(tmp);
	int n = int(size * ratio);
	std::uniform_int_distribution<int> rand(0, n);
	std::default_random_engine engine(time(NULL) ^ clock());
	for (k = 0; k < n; k++)
	{
		i = int(double(rand(engine)) / rand.max() * src.cols - 0.5);
		j = int(double(rand(engine)) / rand.max() * src.rows - 0.5);
		p = rand(engine) & 1 ? 0 : 255;
		if (src.channels() == 1)
			tmp.at<uchar>(j, i) = p;
		else if (src.channels() == 3)
		{
			tmp.at<cv::Vec3b>(j, i)[0] = p;
			tmp.at<cv::Vec3b>(j, i)[1] = p;
			tmp.at<cv::Vec3b>(j, i)[2] = p;
		}
	}
	tmp.copyTo(dst);
}