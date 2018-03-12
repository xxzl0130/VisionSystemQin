#include <iostream>
#include <string>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <random>
#include "opencv2/opencv.hpp"
#include "AddNoise.hpp"
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "msglink.hpp"
#include <ctime>
#include <Eigen/Dense>

using namespace std;
using namespace cv;

#define VIDEO_WIDTH			640
#define VIDEO_HEIGHT		480
#define SourceWindowName "Source"

#define CAM_INI_FILE_NAME	"./camera.ini"

class DispMsg : public MsgData
{
public:
	cv::Mat image;
};

struct
{
	double fx, fy, x0, y0, cx, cy, s, k1, k2, p1, p2, r1, r2, r3, t1, t2, t3;
}camPara;

int noiseValue, brightnessValue, contrastValue, noiseMethodValue;
enum {GaussNoise, UniformNoise, ImpulseNoise}noiseMethod;

void initWindows();
void initCameraParameters();
void changeNoiseMethod(int state,void *);
void imagePreProcess(MsgLink<DispMsg>* ld);
void imageProcess(Mat& img);
vector<vector<vector<Point>>> twoMeans(const vector<vector<Point>>& items);
// 质心法定位
vector<Point2f> centerLocate(const vector<vector<Point>>& contours);
// 椭圆法定位
vector<Point2f> ellipseLocate(const vector<vector<Point>>& contours);

int main()
{
	initCameraParameters();
	cv::VideoCapture cap("test.avi");
	MsgLink<DispMsg> linkd;
	boost::thread th(boost::bind(imagePreProcess, &linkd));
	th.detach();
	int fpsCtrl = min(static_cast<unsigned int>(1000 / cap.get(CV_CAP_PROP_FPS)),125u);
	cout << fpsCtrl << endl;
	while (true)
	{
		DispMsg* md = linkd.prepareMsg();
		cap >> md->image;
		// do_some_processing(md->image);
		linkd.send();
		if (linkd.isClosed())
		{
			break;
		}
		Sleep(fpsCtrl);
	}
	return 0;
}

void initWindows()
{
	namedWindow(SourceWindowName,WINDOW_AUTOSIZE);
	createTrackbar("噪声比例", SourceWindowName, &noiseValue, 100, nullptr);
	createTrackbar("噪声种类", SourceWindowName, &noiseMethodValue,2, changeNoiseMethod,nullptr);
	createTrackbar("亮度调节", SourceWindowName, &brightnessValue, 200, nullptr);
	setTrackbarPos("亮度调节", SourceWindowName, 100);
	createTrackbar("对 比 度", SourceWindowName, &contrastValue, 200, nullptr);
	setTrackbarPos("对 比 度", SourceWindowName, 100);
	waitKey(1);
}

void initCameraParameters()
{
	char paraStr[32];
	GetPrivateProfileStringA("Cam", "fx", "1.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.fx);
	GetPrivateProfileStringA("Cam", "fy", "1.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.fy);
	GetPrivateProfileStringA("Cam", "x0", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.x0);
	GetPrivateProfileStringA("Cam", "y0", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.y0);
	GetPrivateProfileStringA("Cam", "cx", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.cx);
	GetPrivateProfileStringA("Cam", "cy", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.cy);
	GetPrivateProfileStringA("Cam", "s", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.s);
	GetPrivateProfileStringA("Cam", "k1", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.k1);
	GetPrivateProfileStringA("Cam", "k2", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.k2);
	GetPrivateProfileStringA("Cam", "p1", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.p1);
	GetPrivateProfileStringA("Cam", "p2", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.p2);
	GetPrivateProfileStringA("Cam", "r1", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.r1);
	GetPrivateProfileStringA("Cam", "r2", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.r2);
	GetPrivateProfileStringA("Cam", "r3", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.r3);
	GetPrivateProfileStringA("Cam", "t1", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.t1);
	GetPrivateProfileStringA("Cam", "t2", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.t2);
	GetPrivateProfileStringA("Cam", "t3", "0.0", paraStr, 32, CAM_INI_FILE_NAME);
	sscanf_s(paraStr, "%lf", &camPara.t3);
}

void changeNoiseMethod(int state, void *)
{
	switch (state)
	{
	case 0:
		noiseMethod = GaussNoise;
		break;
	case 1:
		noiseMethod = UniformNoise;
		break;
	case 2:
		noiseMethod = ImpulseNoise;
		break;
	default:
		noiseMethod = GaussNoise;
		break;
	}
}

void imagePreProcess(MsgLink<DispMsg>* ld)
{
	initWindows();
	DispMsg* md;
	Mat srcImg;
	while (true)
	{
		md = ld->receive();
		if (md != nullptr)
		{
			md->image.copyTo(srcImg);
			if(brightnessValue != 100 || contrastValue != 100)
			{// 亮度&对比度调节
				srcImg.convertTo(srcImg, -1, double(contrastValue) / 100.0, double(brightnessValue - 100));
			}
			if(noiseValue)
			{
				switch (noiseMethod)
				{
				case GaussNoise:
					addGausssNoise(srcImg, srcImg, noiseValue);
					break;
				case UniformNoise:
					addUniformNoise(srcImg, srcImg, noiseValue);
					break;
				case ImpulseNoise:
					addImpulseNoise(srcImg, srcImg, noiseValue / 100.0);
					break;
				}
			}
			imshow(SourceWindowName, srcImg);
			double s = double(clock()) / CLOCKS_PER_SEC;
			imageProcess(srcImg);
			double t = double(clock()) / CLOCKS_PER_SEC;
			cout << (t - s) << endl;
		}
		if (waitKey(1) > 0)
		{
			break;
		}
	}
	ld->close();
}

void imageProcess(Mat& img)
{
	vector<Mat> channels;
	Mat dst;

	split(img, channels);
	channels.at(2).copyTo(dst);
	// R-G-B
	dst -= channels.at(0) + channels.at(1);
	//imshow("R-G-B", dst);
	GaussianBlur(dst, dst, Size(5, 5), 1);
	//imshow("smooth", dst);
	threshold(dst, dst, 0, 255, THRESH_OTSU);
	//imshow("binary", dst);

	// 膨胀 腐蚀
	const auto element = getStructuringElement(MORPH_ELLIPSE,Size(5,5));
	dilate(dst, dst, element);
	erode(dst, dst, element);
	imshow("Dilation", dst);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//cout << contourArea(it) << endl;
	if(contours.size() > 12)
	{
		contours = twoMeans(contours)[1];
		//cout << contours.size() << endl;
	}
	Mat tmp;
	dst.convertTo(tmp, CV_8UC3);
	cvtColor(tmp, tmp, CV_GRAY2BGR);
	drawContours(tmp, contours, -1, Scalar(0, 0, 255), 2, LINE_AA);
	
	auto centers = ellipseLocate(contours);
	// 大圆的拟合
	auto ell = fitEllipse(centers);
	for(auto& it:centers)
	{
		circle(tmp, it, 1, Scalar(0, 0, 255), 2);
	}
	ellipse(tmp, ell, Scalar(0, 0, 255), 2, LINE_AA);
	imshow("contours", tmp);
}

vector<vector<vector<Point>>> twoMeans(const vector<vector<Point>>& items)
{
	const static uint32_t maxCnt = 1000;
	uint32_t cnt = 0;
	const static double eps = 1e-5;
	const static int K = 2;
	// 两个质心
	double center[K];
	auto n = items.size();
	vector<bool> in(n, false);
	uniform_int_distribution<int> rand(0, n - 1);
	default_random_engine engine;
	vector<pair<vector<Point>, double>> data;

	// 预处理面积 and 初始分类
	double minArea = (numeric_limits<double>::max)();
	int minIndex = 0;
	double maxArea = (numeric_limits<double>::min)();
	int maxIndex = 0;
	for(auto i = 0;i < items.size();++i)
	{
		double area = contourArea(items[i]);
		data.emplace_back(make_pair(items[i], area));
		if(area < minArea && !in[i])
		{
			minArea = area;
			in[minIndex] = false;
			in[minIndex = i] = true;
			center[0] = area;
		}
		if (area > maxArea && !in[i])
		{
			maxArea = area;
			in[maxIndex] = false;
			in[maxIndex = i] = true;
			center[1] = area;
		}
	}
	//cout << "min:" << center[0] << ",max:" << center[1] << endl;
	bool centerChanged = true;
	while(true)
	{
		centerChanged = false;
		vector<vector<vector<Point>>> tempRes(K);
		vector<vector<double>> tempResArea(K);
		for(auto& it:data)
		{
			double minDist = (numeric_limits<double>::max)();
			uint32_t index = 0;
			for(int i = 0;i < K;++i)
			{
				auto t = abs(it.second - center[i]);
				if(t < minDist)
				{
					minDist = t;
					index = i;
				}
			}
			tempRes[index].push_back(it.first);
			tempResArea[index].push_back(it.second);
		}

		for(int i = 0;i < K;++i)
		{
			double t = 0;
			for (double it : tempResArea[i])
			{
				t += it;
			}
			t /= tempResArea[i].size();
			if(abs(center[i] - t) > eps)
			{// 质心移动距离超过阈值视为改变
				centerChanged = true;
			}
			center[i] = t;
		}
		++cnt;
		if ((!centerChanged) || cnt > maxCnt)
		{
			/*cout << "cnt:" << cnt << endl;
			for (int i = 0; i < K; ++i)
			{
				for(auto it: tempResArea[i])
				{
					cout << it << endl;
				}
				cout << "----------" << endl;
				
			}
			//system("pause");
			*/
			return tempRes;
		}
	}
}

vector<Point2f> centerLocate(const vector<vector<Point>>& contours)
{
	vector<Point2f> locate;
	Moments mo;
	for (auto& it:contours)
	{
		mo = moments(it, true);
		locate.emplace_back(mo.m10 / mo.m00, mo.m01 / mo.m00);
	}
	return locate;
}

vector<Point2f> ellipseLocate(const vector<vector<Point>>& contours)
{
	vector<Point2f> locate;
	RotatedRect re;
	for (auto& it : contours)
	{
		re = fitEllipse(it);
		locate.push_back(re.center);
	}
	return locate;
}
