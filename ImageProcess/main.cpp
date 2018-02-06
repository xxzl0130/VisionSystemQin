#include <iostream>
#include <string>
#include <windows.h>
#include <algorithm>
#include "opencv2/opencv.hpp"
#include "AddNoise.hpp"
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "msglink.hpp"
#include <ctime>

using namespace std;
using namespace cv;

#define VIDEO_WIDTH			640
#define VIDEO_HEIGHT		480
#define SourceWindowName "Source"

class DispMsg : public MsgData
{
public:
	cv::Mat image;
};

int noiseValue, brightnessValue, contrastValue, noiseMethodValue;
enum {GaussNoise, UniformNoise, ImpulseNoise}noiseMethod;

void initWindows();
void changeNoiseMethod(int state,void *);
void imagePreProcess(MsgLink<DispMsg>* ld);

int main()
{
	cv::VideoCapture cap("test.avi");
	MsgLink<DispMsg> linkd;
	boost::thread th(boost::bind(imagePreProcess, &linkd));
	th.detach();
	int fpsCtrl = min(static_cast<unsigned int>(1000 / cap.get(CV_CAP_PROP_FPS)),125u);
	cout << fpsCtrl << endl;
	while (true)
	{
		for (auto i = 0; i < 10; ++i)
		{
			DispMsg* md = linkd.prepareMsg();
			cap >> md->image;
			// do_some_processing(md->image);
			linkd.send();
			if (linkd.isClosed())
			{
				return 0;
			}
			Sleep(fpsCtrl);
		}
	}
	return 0;
}

void initWindows()
{
	namedWindow(SourceWindowName,WINDOW_AUTOSIZE);
	createTrackbar("噪声比例", SourceWindowName,&noiseValue,100,nullptr);
	createTrackbar("噪声种类", SourceWindowName, &noiseMethodValue,2, changeNoiseMethod,nullptr);
	createTrackbar("亮度调节", SourceWindowName, &brightnessValue, 200, nullptr);
	setTrackbarPos("亮度调节", SourceWindowName, 100);
	createTrackbar("对 比 度", SourceWindowName, &contrastValue, 200, nullptr);
	setTrackbarPos("对 比 度", SourceWindowName, 100);
	waitKey(1);
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
		if (md != NULL)
		{
			md->image.copyTo(srcImg);
			if(brightnessValue || contrastValue)
			{// 亮度&对比度调节
				srcImg.convertTo(srcImg, -1, double(contrastValue) / 100.0, double(brightnessValue - 100));
			}
			if(noiseValue)
			{
				switch (noiseMethod)
				{
				case GaussNoise:
					addGausssNoise(srcImg, srcImg, noiseValue);
				case UniformNoise:
					addUniformNoise(srcImg, srcImg, noiseValue);
				case ImpulseNoise:
					addImpulseNoise(srcImg, srcImg, noiseValue / 100.0);
				}
			}
			imshow(SourceWindowName, srcImg);
			waitKey(1);
		}
		if (waitKey(3) > 0)
		{
			break;
		}
	}
	ld->close();
}