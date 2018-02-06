#include <iostream>
#include <windows.h>
#include <algorithm>
#include "opencv2/opencv.hpp"
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "msglink.hpp"
#include <ctime>

using namespace std;
using namespace cv;


class DispMsg : public MsgData
{
public:
	cv::Mat image;
};

void dispThread(MsgLink<DispMsg>* ld)
{
	cv::namedWindow("disp", CV_WINDOW_AUTOSIZE);
	while (true)
	{
		DispMsg* md = ld->receive();
		if (md != NULL)
		{
			imshow("disp", md->image);
		}
		if (waitKey(3) > 0)
		{
			break;
		}
	}
	ld->close();
}

int main()
{
	cv::VideoCapture cap(0);
	MsgLink<DispMsg> linkd;
	boost::thread th(boost::bind(dispThread, &linkd));
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
