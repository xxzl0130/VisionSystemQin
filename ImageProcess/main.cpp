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
#define _USE_MATH_DEFINES
#include <math.h>
#include "LHM.hpp"

using namespace std;
using namespace cv;

#define SourceWindowName "Source"

#define CAM_INI_FILE_NAME	"./camera.ini"

#define rad2deg(x) ((x) / M_PI * 180)

class DispMsg : public MsgData
{
public:
	cv::Mat image;
	double realX, realY, realZ, realHeading, realPitch, offsetX, offsetY, offsetZ;
};

struct
{
	double fx, fy, x0, y0, cx, cy, s, k1, k2, p1, p2, r1, r2, r3, t1, t2, t3;
}camPara;



int noiseValue, brightnessValue, contrastValue, noiseMethodValue;
enum {GaussNoise, UniformNoise, ImpulseNoise}noiseMethod;
Mat physicSizeMatrix, probeMat, pixelMat;

struct
{
	enum Filter { GaussBlur, UniformBlur, MedianBlur, BilaFilter, noFilter } filter;
	enum DE { dilateAndErode, erodeAndDilate } de;
	enum Locate3 { Pinhole, LHM, EPNP }loacteMethod3D;
	enum Locate2 { Center, Ellipse} locateMethod2D;
	bool roi;
} imageProcessMethod;

void initWindows();
void initCameraParameters();
void changeNoiseMethod(int state,void *);
void imagePreProcess(MsgLink<DispMsg>* ld);
Mat imageProcess(Mat& img);
vector<vector<vector<Point>>> twoMeans(const vector<vector<Point>>& items);
// 质心法定位
vector<Point2f> centerLocate(const vector<vector<Point>>& contours);
// 椭圆法定位
vector<Point2f> ellipseLocate(const vector<vector<Point>>& contours);
void printMethods();
Mat pinholeLocate(const vector<Point2f>& centers, double centerX, double centerY,double xPixelSize,double yPixelSize,const Rect& roi);

int main()
{
	initCameraParameters();
	cv::VideoCapture cap("test.avi");
	MsgLink<DispMsg> linkd;
	boost::thread th(boost::bind(imagePreProcess, &linkd));
	th.detach();
	int fpsCtrl = min(static_cast<unsigned int>(1000 / cap.get(CV_CAP_PROP_FPS)),125u);
	std::cout << fpsCtrl << endl;
	FILE* locateData;
	fopen_s(&locateData,"locate.csv", "r");
	while (true)
	{
		DispMsg* md = linkd.prepareMsg();
		cap >> md->image;
		fscanf_s(locateData, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &md->realX, &md->realY, &md->realZ, &md->realHeading,
		         &md->realPitch, &md->offsetX, &md->offsetY, &md->offsetZ);
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
	double physicSize[3][3] = { {1.0, 0.0, camPara.cx},
					            {0.0, 1.0, camPara.cy},
	                            {0.0, 0.0, 1.0 } };
	double prob[3][3] = { { camPara.fx, camPara.s , camPara.x0},
						  { 0.0,        camPara.fy, camPara.y0},
						  { 0.0,           0.0,         1.0 } };
	Mat(3, 3, CV_64FC1, physicSize).copyTo(physicSizeMatrix);
	Mat(3, 3, CV_64FC1, prob).copyTo(probeMat);
	Mat(3, 1, CV_64FC1).copyTo(pixelMat);
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
	FILE* resData;
	fopen_s(&resData, "res.csv", "w");
	fprintf_s(resData,
	          "realX,measureX,errorX,realY,measureY,errorY,realZ,measureZ,errorZ,realHeading,measureHeading,errorHeading,realPitch,measurePitch,errorPitch,timeCost\n");
	imageProcessMethod.de = imageProcessMethod.dilateAndErode;
	imageProcessMethod.filter = imageProcessMethod.GaussBlur;
	imageProcessMethod.loacteMethod3D = imageProcessMethod.Pinhole;
	imageProcessMethod.locateMethod2D = imageProcessMethod.Ellipse;
	imageProcessMethod.roi = true;
	printMethods();
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
			double s = getTickCount() / getTickFrequency();
			Mat res = imageProcess(srcImg);
			double t = getTickCount() / getTickFrequency();
			//std::cout << (t - s) << endl;

			fprintf_s(resData, "%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n",
				md->realX, res.at<double>(0) + md->offsetX, md->realX - res.at<double>(0) - md->offsetX,
				md->realY, res.at<double>(1) + md->offsetY, md->realY - res.at<double>(1) - md->offsetY,
				md->realZ, res.at<double>(2) + md->offsetZ, md->realZ - res.at<double>(2) - md->offsetZ,
				md->realHeading, res.at<double>(3), md->realHeading - res.at<double>(3),
				md->realPitch, res.at<double>(4), md->realPitch - res.at<double>(4),t - s);
			std::fflush(resData);
		}
		auto key = waitKey(1);
		switch(key)
		{
		case 0:
			continue;
			break;
		case 'g':
			imageProcessMethod.filter = imageProcessMethod.GaussBlur;
			break;
		case 'b':
			imageProcessMethod.filter = imageProcessMethod.BilaFilter;
			break;
		case 'u':
			imageProcessMethod.filter = imageProcessMethod.UniformBlur;
			break;
		case 'm':
			imageProcessMethod.filter = imageProcessMethod.MedianBlur;
			break;
		case 'n':
			imageProcessMethod.filter = imageProcessMethod.noFilter;
			break;
		case 'r':
			imageProcessMethod.roi = !imageProcessMethod.roi;
			break;
		case 'p':
			imageProcessMethod.loacteMethod3D = imageProcessMethod.Pinhole;
			break;
		case 'l':
			imageProcessMethod.loacteMethod3D = imageProcessMethod.LHM;
			break;
		case 'e':
			imageProcessMethod.loacteMethod3D = imageProcessMethod.EPNP;
			break;
		case 'd':
			if(imageProcessMethod.de == imageProcessMethod.dilateAndErode)
			{
				imageProcessMethod.de = imageProcessMethod.erodeAndDilate;
			}
			else
			{
				imageProcessMethod.de = imageProcessMethod.dilateAndErode;
			}
			break;
		case 'c':
			if (imageProcessMethod.locateMethod2D == imageProcessMethod.Center)
			{
				imageProcessMethod.locateMethod2D = imageProcessMethod.Ellipse;
			}
			else
			{
				imageProcessMethod.locateMethod2D = imageProcessMethod.Center;
			}
			break;
		case 27:
			goto endProcess;
		default:
			break;
		}
		if(key > 0)
		{
			printMethods();
		}
	}
endProcess:
	ld->close();
}

Mat imageProcess(Mat& img)
{
	static bool firstIn = true;
	static double lastCenterX = 0, lastCenterY = 0;
	const static double offsetSpeedFactor = 15,offsetPixelFactor = 7;
	static Rect roi(0,0,0,0);
	static uint32_t frameCount = 0;

	vector<Mat> channels;
	Mat dst;
	Mat tmpImg;

	++frameCount;

	// 设置roi
	if(!firstIn)
	{
		img(roi).copyTo(dst);
	}
	else
	{
		img.copyTo(dst);
	}
	if(imageProcessMethod.roi)
		rectangle(img, roi, Scalar(0, 255, 0), 2, LINE_AA);
	//imwrite("./image/src" + to_string(frameCount) + ".jpg", img, vector<int>({ CV_IMWRITE_JPEG_QUALITY ,100,0 }));
	imshow("roi", img);

	// 颜色分离差分
	split(dst, channels);
	channels.at(2).copyTo(dst);
	// R-G-B
	dst -= channels.at(0) + channels.at(1);
	//imshow("R-G-B", dst);

	// 滤波
	switch(imageProcessMethod.filter)
	{
	case imageProcessMethod.GaussBlur:
		GaussianBlur(dst, dst, Size(5, 5), 1);
		break;
	case imageProcessMethod.MedianBlur:
		medianBlur(dst, dst, 5);
		break;
	case imageProcessMethod.UniformBlur:
		blur(dst, dst, Size(5, 5));
		break;
	case imageProcessMethod.BilaFilter:
		bilateralFilter(dst, tmpImg, 5, 10, 20);
		tmpImg.copyTo(dst);
		break;
	case imageProcessMethod.noFilter:
		break;
	default:
		break;
	}
	//imshow("smooth", dst);

	// 二值化
	threshold(dst, dst, 0, 255, THRESH_OTSU);
	//imshow("binary", dst);

	// 膨胀 腐蚀
	const auto element = getStructuringElement(MORPH_ELLIPSE,Size(5,5));
	switch(imageProcessMethod.de)
	{
	case imageProcessMethod.dilateAndErode:
		dilate(dst, dst, element);
		erode(dst, dst, element);
		break;
	case imageProcessMethod.erodeAndDilate:
		erode(dst, dst, element);
		dilate(dst, dst, element);
		break;
	default:
		dilate(dst, dst, element);
		erode(dst, dst, element);
	}
	//imshow("Dilation", dst);

	// 寻找轮廓，数量大于12时k聚类处理
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//cout << contourArea(it) << endl;
	if(contours.size() > 12)
	{
		contours = twoMeans(contours)[1];
		//cout << contours.size() << endl;
	}
	
	//dst.convertTo(tmpImg, CV_8UC3);
	cvtColor(dst, tmpImg, CV_GRAY2BGR);
	drawContours(tmpImg, contours, -1, Scalar(0, 0, 255), 2, LINE_8);

	// 二维定位
	vector<Point2f> centers;
	double centerX, centerY;
	RotatedRect ell;
	Moments mo;
	switch(imageProcessMethod.locateMethod2D)
	{
	case imageProcessMethod.Center:
		centers = centerLocate(contours);
		mo = moments(centers);
		centerX = mo.m10 / mo.m00;
		centerY = mo.m01 / mo.m00;
		break;
	case imageProcessMethod.Ellipse:
		centers = ellipseLocate(contours);
		ell = fitEllipse(centers);
		centerX = ell.center.x; 
		centerY = ell.center.y;
		break;
	default:
		centers = ellipseLocate(contours);
		ell = fitEllipse(centers);
		centerX = ell.center.x;
		centerY = ell.center.y;
		break;
	}

	circle(tmpImg, Point(centerX, centerY), 2, Scalar(0, 0, 255), 2, LINE_8);
	imshow("contours", tmpImg);
	//imwrite("./image/dst" + to_string(frameCount) + ".jpg", tmpImg, vector<int>({ CV_IMWRITE_JPEG_QUALITY ,100,0 }));

	float locateMinX = 1e8, locateMaxX = -1e8, locateMinY = 1e8, locateMaxY = -1e8;
	for (auto& it : centers)
	{
		//circle(tmpImg, it, 1, Scalar(0, 0, 255), 2);
		locateMinX = min(locateMinX, it.x);
		locateMaxX = max(locateMaxX, it.x);
		locateMinY = min(locateMinY, it.y);
		locateMaxY = max(locateMaxY, it.y);
		cout << it.x - centerX << " " << it.y - centerY << " " << point2Angle(it,Point2f(centerX,centerY)) << endl;
	}
	double xPixelSize = locateMaxX - locateMinX;
	double yPixelSize = locateMaxY - locateMinY;

	// 向三维求解
	// 针孔模型
	auto res = pinholeLocate(centers, centerX, centerY,xPixelSize,yPixelSize, roi);
	
	
	if (imageProcessMethod.roi)
	{
		if (firstIn)
		{
			roi.x = max(locateMinX - xPixelSize / offsetPixelFactor * 2, 0.0);
			roi.y = max(locateMinY - yPixelSize / offsetPixelFactor * 2, 0.0);
			roi.width = xPixelSize + xPixelSize / offsetPixelFactor * 4;
			roi.height = yPixelSize + xPixelSize / offsetPixelFactor * 4;
			firstIn = false;
		}
		else
		{
			// 差分求速度
			double vX = centerX - lastCenterX;
			double vY = centerY - lastCenterY;
			double offsetX1 = xPixelSize / offsetPixelFactor, offsetX2 = vX * offsetSpeedFactor;
			double offsetY1 = yPixelSize / offsetPixelFactor, offsetY2 = vY * offsetSpeedFactor;
			roi.x = max(locateMinX - offsetX1, 0.0);
			roi.y = max(locateMinY - offsetY1, 0.0);
			roi.width = xPixelSize + offsetX1 * 2;
			roi.height = yPixelSize + offsetY1 * 2;
			// 向运动方向偏移更多
			if (vX < 0)
			{
				roi.x += offsetX2;
				roi.width -= offsetX2;
			}
			else
			{
				roi.width += offsetX2;
			}
			if (vY < 0)
			{
				roi.y += offsetY2;
				roi.height -= offsetY2;
			}
			else
			{
				roi.height += offsetY2;
			}
			if (roi.x + roi.width > img.cols)
			{
				roi.width = img.cols - roi.x;
			}
			if (roi.y + roi.height > img.rows)
			{
				roi.height = img.rows - roi.x;
			}
		}
	}
	else
	{
		roi.x = roi.y = 0;
		roi.width = img.cols;
		roi.height = img.rows;
	}
	lastCenterX = centerX;
	lastCenterY = centerY;

	return res;
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

void printMethods()
{
	system("cls");
	cout << "Image filter: ";
	switch (imageProcessMethod.filter)
	{
	case imageProcessMethod.BilaFilter:
		cout << ">Bila Filter(b)< Gauss Blur(g)  Media Blur(m)  Uniform Blur(u)  no filter(n)" << endl;
		break;
	case imageProcessMethod.GaussBlur:
		cout << " Bila Filter(b) >Gauss Blur(g)< Media Blur(m)  Uniform Blur(u)  no filter(n)" << endl;
		break;
	case imageProcessMethod.MedianBlur:
		cout << " Bila Filter(b)  Gauss Blur(g) >Media Blur(m)< Uniform Blur(u)  no filter(n)" << endl;
		break;
	case imageProcessMethod.UniformBlur:
		cout << " Bila Filter(b)  Gauss Blur(g)  Media Blur(m) >Uniform Blur(u)< no filter(n)" << endl;
		break;
	case imageProcessMethod.noFilter:
		cout << " Bila Filter (b)  Gauss Blur(g)  Media Blur(m)  Uniform Blur(u) >no filter(n)<" << endl;
		break;
	}

	cout << "2D locate method: ";
	switch (imageProcessMethod.locateMethod2D)
	{
	case imageProcessMethod.Center:
		cout << ">center(c)< ellipse(c)" << endl;
		break;
	case imageProcessMethod.Ellipse:
		cout << " center(c) >ellipse(c)<" << endl;
		break;
	}

	cout << "3D locate method: ";
	switch (imageProcessMethod.loacteMethod3D)
	{
	case imageProcessMethod.EPNP:
		cout << ">EPNP(e)< LHM(l)  PinHole(p)" << endl;
		break;
	case imageProcessMethod.LHM:
		cout << " EPNP(e) >LHM(l)< PinHole(p)" << endl;
		break;
	case imageProcessMethod.Pinhole:
		cout << " EPNP(e)  LHM(l) >PinHole(p)<" << endl;
		break;
	}

	cout << "Use ROI (r):" << (imageProcessMethod.roi ? "True" : "False") << endl;

	cout << "Morphology (d): " <<
		(imageProcessMethod.de == imageProcessMethod.dilateAndErode ? "Dilate and Erode" : "Erode and Dilate") << endl;
}

Mat pinholeLocate(const vector<Point2f>& centers, double centerX, double centerY, double xPixelSize, double yPixelSize, const Rect& roi)
{
	// 追踪状态
	centerX += roi.x;
	centerY += roi.y;
	
	double imageDetectDistanceX = camPara.fx * 0.82 / xPixelSize + 0.592;
	double imageDetectDistanceY = camPara.fy * 0.82 / yPixelSize + 0.592;
	pixelMat.at<double>(0) = centerX * imageDetectDistanceX;
	pixelMat.at<double>(1) = centerY * imageDetectDistanceX;
	pixelMat.at<double>(2) = imageDetectDistanceX;

	Mat tmpMat = physicSizeMatrix * probeMat, tmpMatInv;
	invert(tmpMat, tmpMatInv);
	Mat cameraMat = tmpMatInv * pixelMat;

	double solveX = cameraMat.at<double>(0), solveY = cameraMat.at<double>(2), solveZ = -cameraMat.at<double>(1);
	double solveHeading = atan(-solveX / solveY), solvePitch = atan(solveZ / solveY);

	Mat res(5, 1, CV_64FC1);
	res.at<double>(0) = solveX;
	res.at<double>(1) = solveY;
	res.at<double>(2) = solveZ;
	res.at<double>(3) = rad2deg(solveHeading);
	res.at<double>(4) = rad2deg(solvePitch);
	return Mat();
}
