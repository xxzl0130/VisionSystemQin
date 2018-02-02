#include "stdafx.h"
#include "CalibrationMyApp.h"

#define MAX_WINDOW_WIDTH 1600
#define MAX_WINDOW_HEIGHT 1200

bool CalibrationCycle_Mean_time_Flag = false;
CCalibrationMyApp* CalibrationVpApp;
float MotionDistance = 0.5,RotateDegree = 1.0;
int VisionPictureCount = 0;

CCalibrationMyApp::CCalibrationMyApp(void)
{ 
	Calibrationm_bCapture = false;
	Calibrationm_factory = new vuImageFactory;

}
CCalibrationMyApp::~CCalibrationMyApp(void)
{
	if(Calibrationm_data != NULL)
		vuAllocArray<uchar >::free(Calibrationm_data);
}

int CCalibrationMyApp::configure() //ϵͳ����
{
	// ����Vega Primeϵͳ
	vpApp::configure();

	CalibrationpMainChannel = vpChannel::find("myChannel");//��ͨ��	
	assert(CalibrationpMainChannel);


	CalibrationpMainChannel->addSubscriber(vsChannel::EVENT_POST_DRAW, this);

	//farChannel->addSubscriber(vsChannel::EVENT_POST_DRAW, this);

	Calibrationm_data = vuAllocArray<uchar >::malloc(
		MAX_WINDOW_WIDTH*MAX_WINDOW_HEIGHT*3);

	return vsgu::SUCCESS;
}
int CCalibrationMyApp::endFrame()
{
	return vpApp::endFrame();
}
void CCalibrationMyApp::onKeyInput(vrWindow::Key key, int mod)//���ؼ�����Ϣ������
{

	double x,y,z;//�洢λ��
	double h,p,r;//�洢��̬
	vpTransform* CalibrationfarTransform = (vpTransform*)vpTransform::find("farTransform");
	CalibrationfarTransform->getTranslate(&x,&y,&z);
	CalibrationfarTransform->getRotate(&h,&p,&r);
	switch(key)
	{
	case vrWindow::KEY_l:
		CalibrationfarTransform->setTranslate(x-MotionDistance,y,z);
		break;
	case vrWindow::KEY_r:
		CalibrationfarTransform->setTranslate(x+MotionDistance,y,z);
		break;
	case vrWindow::KEY_f:
		CalibrationfarTransform->setTranslate(x,y+MotionDistance,z);
		break;
	case vrWindow::KEY_b:
		CalibrationfarTransform->setTranslate(x,y-MotionDistance,z);
		break;
	case vrWindow::KEY_u:
		CalibrationfarTransform->setTranslate(x,y,z+MotionDistance);
		break;
	case vrWindow::KEY_d:
		CalibrationfarTransform->setTranslate(x,y,z-MotionDistance);
		break;
	case vrWindow::KEY_h:
		CalibrationfarTransform->setRotateH(h+RotateDegree);
		break;
	case vrWindow::KEY_g:
		CalibrationfarTransform->setRotateH(h-RotateDegree);
		break;
	case vrWindow::KEY_p:
		CalibrationfarTransform->setRotateP(p+RotateDegree);
		break;
	case vrWindow::KEY_o:
		CalibrationfarTransform->setRotateP(p-RotateDegree);
		break;
	case vrWindow::KEY_w:
		CalibrationfarTransform->setRotateR(r+RotateDegree);
		break;
	case vrWindow::KEY_e:
		CalibrationfarTransform->setRotateR(r-RotateDegree);
		break;
	case vrWindow::KEY_LEFT:
		MotionDistance = MotionDistance - 0.1;
		if(MotionDistance == 0)
		{
			MotionDistance = 0.1;
			AfxMessageBox("�ƶ������Ѿ��ﵽ��Сֵ����");
		}
		break;
	case vrWindow::KEY_RIGHT:
		MotionDistance++;
		break;
	case vrWindow::KEY_UP:
		RotateDegree++;
		break;
	case vrWindow::KEY_DOWN:
		RotateDegree = RotateDegree - 0.1;
		if(RotateDegree == 0)
		{
			RotateDegree = 0.1;
			AfxMessageBox("ת���Ƕ��Ѿ��ﵽ��Сֵ����");
		}
		break;

	case vrWindow::KEY_SPACE:
		Calibrationm_bCapture = TRUE;
		break;
	default: ;


	}
	//vpApp::onKeyInput(key, mod);
}
void CCalibrationMyApp::notify(vsChannel::Event event, const vsChannel *channel,vrDrawContext *context)//����pre/post draw��Ϣ������
{

	CString strName = channel->getName();
	if(event == vsChannel::EVENT_POST_DRAW && strName == "myChannel")
	{
		if(Calibrationm_bCapture)
		{
			// get the viewport
			int ox, oy, sx, sy;
			channel->getVrChannel()->getViewport(&ox, &oy, &sx, &sy);

			// error checking
			if(sx > MAX_WINDOW_WIDTH || sy > MAX_WINDOW_HEIGHT)
			{
				vuNotify::print(vuNotify::LEVEL_WARN, NULL,
					"Window is larger than %d %d, image will be cropped",
					MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT);
				sx = MAX_WINDOW_WIDTH;
				sy = MAX_WINDOW_HEIGHT;
			}

			// capture the frame
			glReadPixels(ox, oy, sx, sy, GL_RGB, GL_UNSIGNED_BYTE, Calibrationm_data);

			// create an image 
			vuImageUserBuffer  image(Calibrationm_data);

			// load the data into the vuImage
			image.setDimensions( sx, sy );
			image.setPixelType(vuImageBase::PixelType(vuImageBase::TYPE_UNSIGNED_BYTE,8,8,8,0));


			// the image filename will be frame########.rgb
			vuString filename;
			filename.sprintf("Frame/frame%02d.jpg", VisionPictureCount);//%08d

			// write the image
			// the .rgb extension on the filename tells the image factory which
			// image loader to use to write out the file.
			Calibrationm_factory->write( filename.c_str(), &image );//filename.c_str()
			VisionPictureCount++;
			Calibrationm_bCapture = false;

		}

	}
}

