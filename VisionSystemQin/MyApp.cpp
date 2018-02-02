//��vegaprimer���
#include "stdafx.h"
#include "SimStructDefine.h"
#include "SimVisionSystem.h"
#include "MyApp.h"
#include "DrawDrogue.h"
#include "ImageProcess.h"
#include "VisionProcess.h"
#include <Windows.h>
#include "ResultDisplay.h"
#include "VisionSystemQinDlg.h"


#define MAX_WINDOW_WIDTH 1600
#define MAX_WINDOW_HEIGHT 1200

bool Detect_Drogue_Image_Flag = false;
CMyApp* pVpApp;
POSTION3D	Drogue_Pos;        //׶��λ��
CPOSTION3D	Receiver_Pos;	     //���ͻ�λ��
CHPR		Receiver_HPR;	//���ͻ���̬
CPOSTION3D	Probe_ref_Receiver_Pos;	//���Ͳ�ͷ��������ͻ���λ��
CPOSTION3D  Drogue_ref_Tanker_Pos;  //׶������ڼ��ͻ�λ��
CPOSTION3D  Probe_Pos;//���Ͳ�ͷ����λ��
double SetupFax,SetupFay,SetupFaz,SetLeftFax,SetLeftFay,SetLeftFaz;
int VisionMode=1;
bool m_bScreenDisplay,m_bCamaraChannelDisplay;//������ʾλ

float MotionDistance = 0.1,RotateDegree = 1.0;
int VisionPictureCount = 0,m_WheatherMode = 0,SynchronizeVisionAndImageProcessFlag = 0;
vuString filenameCameraChannel;
int count=0;
double DrogueRalativeCameraDistanceX,DrogueRalativeCameraDistanceY,DrogueRalativeCameraDistanceZ;
vuVec3<double> DroguePosition;
vuVec3<double> CameraTranformpos;
double F16PositionX[20000] = {0},F16PositionY[20000] ={0},F16PositionZ[20000] = {0};
int F16PositionCount;
vpObject *pReceiver;	//���ͻ�



CMyApp::CMyApp(void)
{
	m_iDisplayMode = 0;//��ʼ��Ϊ������״̬
	m_bScreenDisplay = false;
	m_fSpeed = 0;//��ǰ�ٶ�
	m_fSetSpeed = 0;//�趨�ٶ�
	m_fHeight = 0;
	m_fSetHeight = 0;
	m_bCamaraChannelDisplay = false;

	Calibrationm_bCapture = false;
	Calibrationm_factory = new vuImageFactory;
	
}
CMyApp::~CMyApp(void)
{
	if(Calibrationm_data != NULL)
		vuAllocArray<uchar >::free(Calibrationm_data);
}

int CMyApp::configure() //ϵͳ����
{
	// ����Vega Primeϵͳ
	vpApp::configure();

	CalibrationpMainChannel = vpChannel::find("CalibrationChannel");//��ͨ��	
	assert(CalibrationpMainChannel);


	CalibrationpMainChannel->addSubscriber(vsChannel::EVENT_POST_DRAW, this);


	Calibrationm_data = vuAllocArray<uchar >::malloc(
		MAX_WINDOW_WIDTH*MAX_WINDOW_HEIGHT*3);

	vrFontFactory *fontFactory = new vrFontFactory();
	m_pFont = (vrFont2D*)fontFactory->read("system");//��ȡϵͳ����
	fontFactory->unref();

	pTanker = vpObject::find("c130");//���ͻ�
	assert(pTanker);

	pReceiver = vpObject::find("f16");//���ͻ�
	assert(pReceiver);

	pDrogue = vpObject::find("drogue");//���ͻ�
	assert(pDrogue);

	CameraTranform = vpTransform::find("farTransform");
	assert(CameraTranform);

	Ring = vpObject::find("ring");//���ͻ�
	assert(Ring);

	MyLightSetting = vpLight::find("myLight");

	pMainChannel = vpChannel::find("myChannel");//��ͨ��	
	assert(pMainChannel);

	CameraChannel = vpChannel::find("CameraChannel");
	assert(CameraChannel);
	CameraChannel->addSubscriber(vsChannel::EVENT_POST_DRAW, this);


	
	CutPictureChannel = vpChannel::find("CutDroguePictureChannel");
	assert(CutPictureChannel);
	CutPictureChannel->addSubscriber(vsChannel::EVENT_POST_DRAW, this);
	

	pMainChannel->addSubscriber(vsChannel::EVENT_POST_DRAW, this);

	HANDLE ImageProcessHandle;
	ImageProcessHandle = CreateThread(NULL,0,ImageProcess,(LPVOID)this,0,NULL);
	CloseHandle(ImageProcessHandle);

	return vsgu::SUCCESS;
}
int CMyApp::endFrame()
{
	return vpApp::endFrame();
}
void CMyApp::onKeyInput(vrWindow::Key key, int mod)//���ؼ�����Ϣ������
{
	if(ExitCalibrationDiaglog == 0)
	{
		double x,y,z;//�洢λ��
		double h,p,r;//�洢��̬
		vpTransform* CalibrationfarTransform = (vpTransform*)vpTransform::find("CalibrationTransform");
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

	}else if(ExitCalibrationDiaglog == 2)
	{
		switch(key)
		{
		case vrWindow::KEY_v:
			{
				if(VisionMode%4 == 0)
				{
					VisionChangeX = -30; VisionChangeY = -70; VisionChangeZ = 5;
					VisionChangeP = -5; VisionChangeH = -25, VisionChangeR = 0;
					VisionMode++;
					break;
				}
				if(VisionMode%4 == 1)
				{
					VisionChangeX = -13; VisionChangeY = -50; VisionChangeZ = -3;
					VisionChangeP = 0; VisionChangeH = 0, VisionChangeR = 0;
					VisionMode++;
					break;
				}
				if(VisionMode%4 == 2)
				{
					VisionChangeX = -13; VisionChangeY = -100; VisionChangeZ = 5;
					VisionChangeP = -10; VisionChangeH = 0, VisionChangeR = 0;
					VisionMode++;
					break;
				}
				if(VisionMode%4 == 3)
				{
					VisionChangeX = 0; VisionChangeY = -150; VisionChangeZ = 30;
					VisionChangeP = -18; VisionChangeH = 0, VisionChangeR = 0;
					VisionMode++;
					break;
				}
				break;			
			}
		case vrWindow::KEY_d:
			m_bScreenDisplay = (!m_bScreenDisplay);
			break;
		case vrWindow::KEY_c:
			if(m_bCamaraChannelDisplay)//��ʾ
			{
				CameraChannel->setDrawArea(CameraLeft,CameraRight,CameraDown,CameraTop);

			}
			else
			{
				CameraChannel->setDrawArea(0.0,0.0,1.0,1.0);
			}
			m_bCamaraChannelDisplay=(!m_bCamaraChannelDisplay);
			break;
		case vrWindow::KEY_w:	
			m_WheatherMode = m_WheatherMode%5;
			m_WheatherMode++;
			postWeather(m_WheatherMode);
			break;
		default: ;


		}

	}
	
	//vpApp::onKeyInput(key, mod);
}

//ͨ������Ӧ�������м���ͨ����Ӧ����
void CMyApp::notify(vsChannel::Event event, const vsChannel *channel,vrDrawContext *context)//����pre/post draw��Ϣ������
{
	double x,y,z;//�洢λ��
	double h,p,r;//�洢��̬
	CPOSTION3D temppos;
	//ͨ������
	if(ClimateSettingFlag == 1)
	{
		ClimateSettingFlag = 0;
		int ID =m_WheatherMode +1;
		postWeather(ID);
	}

	if(OpenLightFlag == 1)
	{
		OpenLightFlag = 0;
		MyLightSetting->setEnable(true);
	}

	if(OpenLightFlag == 2)
	{
		OpenLightFlag = 0;
		MyLightSetting->setEnable(false);
	}



	CString strName = channel->getName();
	if(event == vsChannel::EVENT_POST_DRAW && strName == "CutDroguePictureChannel")
	{

		/*HBITMAP bmp;
		CImageProcess ImageProcess;
		bmp = CopyScreen2BitMap(VisionDisplayRect);
		IplImage * CameraScreen = cvCreateImage(cvSize(VisionDisplayRect.Width(),VisionDisplayRect.Height()),IPL_DEPTH_8U,3);
		HBitmap2Ipl(bmp,CameraScreen);
		if(Detect_Drogue_Image_Flag)
		{
			cvShowImage("Screen",CameraScreen);
			cvWaitKey(0);
			//ImageProcess.DroguePictureDetect(CameraScreen);
		}
			

		DeleteObject(bmp);
		cvReleaseImage(&CameraScreen);*/

		int ox,oy,sx,sy;


		if(Detect_Drogue_Image_Flag)
		{
			if(SynchronizeVisionAndImageProcessFlag == 0)
			{
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
				buffer = (GLubyte*)malloc(sx * sy *   
					sizeof(GLubyte) * 3);  
				//��ȡͼ�񻺳�������  
				glReadPixels(ox, oy, sx, sy,   
					GL_BGR, GL_UNSIGNED_BYTE, buffer);

				OriginX = ox,OriginY = oy,WidthX = sx,HeightY = sy;
				Sleep(1);
		
				pDrogue->getAbsolutePosition(&DroguePosition); 
				CameraTranform->getAbsolutePosition(&CameraTranformpos); 			
				DrogueRalativeCameraDistanceY = -(CameraTranformpos[1] - DroguePosition[1]);
				DrogueRalativeCameraDistanceX = -(CameraTranformpos[0] - DroguePosition[0]);
				DrogueRalativeCameraDistanceZ = -(CameraTranformpos[2] - DroguePosition[2]);
				SynchronizeVisionAndImageProcessFlag = 1;

			}

		}else
		{
			SynchronizeVisionAndImageProcessFlag = 0;
		
		}
	}
	if(event == vsChannel::EVENT_POST_DRAW && strName == "myChannel")
	{
		//�������þ����Ƿ���ʾ����
		if(m_bScreenDisplay)
		{
			//-----------------------2009-10-28����-----------------------------
			//�õ����ͻ�λ��
			vuVec3<double> pReceiverpos;
			pReceiver->getAbsolutePosition(&pReceiverpos);
			//pReceiver->getTranslate(&x,&y,&z);
			pReceiver->getRotate(&h,&p,&r);
			m_ReceiverPosxText.sprintf("Receiver Xd Pos: %10.5f",pReceiverpos[0]);//������������ϵ
			m_ReceiverPosyText.sprintf("Receiver Yd Pos: %10.5f",pReceiverpos[1]);
			m_ReceiverPoszText.sprintf("Receiver Zd Pos: %10.5f",pReceiverpos[2]);
			m_ReceiverHText.sprintf("Receiver Yaw Angle: %10.5f",h);
			m_ReceiverPText.sprintf("Receiver Pitch Angle: %10.5f",p);
			m_ReceiverRText.sprintf("Receiver Roll Angle: %10.5f",r);

			//�������Ͳ�ͷ�ռ�λ��
			Receiver_Pos.x = pReceiverpos[0];
			Receiver_Pos.y = pReceiverpos[1];
			Receiver_Pos.z = pReceiverpos[2];

			Receiver_HPR.h = h;
			Receiver_HPR.p = p;
			Receiver_HPR.r = r;
			
			pTanker->getTranslate(&x,&y,&z);
			pTanker->getRotate(&h,&p,&r);
			m_TankerPosxText.sprintf("Tanker Xd Pos: %10.5f",x);//������������ϵ
			m_TankerPosyText.sprintf("Tanker Yd Pos: %10.5f",y);
			m_TankerPoszText.sprintf("Tanker Zd Pos: %10.5f",z);
			m_TankerHText.sprintf("Tanker Yaw Angle: %10.5f",h);
			m_TankerPText.sprintf("Tanker Pitch Angle: %10.5f",p);
			m_TankerRText.sprintf("Tanker Roll Angle: %10.5f",r);
	
			vuVec3<double> CameraTranformpos;
			CameraTranform->getAbsolutePosition(&CameraTranformpos); 

			//PosTrans(Receiver_HPR,Probe_ref_Receiver_Pos,temppos);//�������ͻ�����̬�ǽ����Ͳ�ͷ�����λ��ת��Ϊ����λ��
			Probe_Pos.x = CameraTranformpos[0]; //+ temppos;
			Probe_Pos.y = CameraTranformpos[1]; //+ temppos;
			Probe_Pos.z = CameraTranformpos[2]; //+ temppos;

			m_ProbePosxText.sprintf("Probe Xd Pos: %10.5f",Probe_Pos.x);
			m_ProbePosyText.sprintf("Probe Yd Pos: %10.5f",Probe_Pos.y);
			m_ProbePoszText.sprintf("Probe Zd Pos: %10.5f",Probe_Pos.z);
	

			//�õ�׶��λ��
			vuVec3<double> droguepos;
			pDrogue->getAbsolutePosition(&droguepos); 

			m_DroguePosxText.sprintf("Drogue Xd Pos: %10.5f",droguepos[0]);//��������ϵ
			m_DroguePosyText.sprintf("Drogue Yd Pos: %10.5f",droguepos[1]);
			m_DroguePoszText.sprintf("Drogue Zd Pos: %10.5f",droguepos[2]);

			//����ǰ����ѹ���ջ
			context->pushElements(true);//����ģ���ӵ����

			//��ֹ��Ȳ���
			vrDepthTest::Element depthTestElement;
			depthTestElement.m_enable = false;
			context->setElement(vrDepthTest::Element::Id,&depthTestElement);

			//������ͶӰ
			vrTransform::ElementProjection projectionElement;
			projectionElement.makeOrthographic(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);
			context->setElement(vrTransform::ElementProjection::Id,&projectionElement);
			//����ַ���
			vuVec4<float> color(FontcolorR,FontcolorG,FontcolorB,1.0f);

			//��������������Ƿ���ʾ����������Ϣ����ʾλ��
			float delta;
			if(!m_bCamaraChannelDisplay)
			{
				delta = 0.0f;
			}
			else
			{
				delta = -0.3f;
			}
			//��ʾ�ַ���
			m_pFont->displayStringAt(context,"All Data In Soviet Coords:",color,FontX,0.48f+FontY);

			m_pFont->displayStringAt(context,m_TankerPosxText.c_str(),color,FontX,0.46f+FontY);
			m_pFont->displayStringAt(context,m_TankerPosyText.c_str(),color,FontX,0.44f+FontY);
			m_pFont->displayStringAt(context,m_TankerPoszText.c_str(),color,FontX,0.42f+FontY);

			m_pFont->displayStringAt(context,m_TankerHText.c_str(),color,FontX,0.38f+FontY);
			m_pFont->displayStringAt(context,m_TankerPText.c_str(),color,FontX,0.36f+FontY);
			m_pFont->displayStringAt(context,m_TankerRText.c_str(),color,FontX,0.34f+FontY);

			m_pFont->displayStringAt(context,m_ReceiverPosxText.c_str(),color,FontX,0.30f+FontY);
			m_pFont->displayStringAt(context,m_ReceiverPosyText.c_str(),color,FontX,0.28f+FontY);
			m_pFont->displayStringAt(context,m_ReceiverPoszText.c_str(),color,FontX,0.26f+FontY);

			m_pFont->displayStringAt(context,m_ReceiverHText.c_str(),color,FontX,0.22f+FontY);
			m_pFont->displayStringAt(context,m_ReceiverPText.c_str(),color,FontX,0.20f+FontY);
			m_pFont->displayStringAt(context,m_ReceiverRText.c_str(),color,FontX,0.18f+FontY);

			m_pFont->displayStringAt(context,m_DroguePosxText.c_str(),color,FontX,0.14f+FontY);
			m_pFont->displayStringAt(context,m_DroguePosyText.c_str(),color,FontX,0.12f+FontY);
			m_pFont->displayStringAt(context,m_DroguePoszText.c_str(),color,FontX,0.10f+FontY);

			m_pFont->displayStringAt(context,m_ProbePosxText.c_str(),color,FontX,0.06f+FontY);
			m_pFont->displayStringAt(context,m_ProbePosyText.c_str(),color,FontX,0.04f+FontY);
			m_pFont->displayStringAt(context,m_ProbePoszText.c_str(),color,FontX,0.02f+FontY);
			//��������
			context->popElements();		
		}
	}
	if(event == vsChannel::EVENT_POST_DRAW && strName == "CalibrationChannel")
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
			filename.sprintf("Frame/frame%02d.bmp", VisionPictureCount);//%08d

			// write the image
			// the .rgb extension on the filename tells the image factory which
			// image loader to use to write out the file.
			Calibrationm_factory->write( filename.c_str(), &image );//filename.c_str()
			VisionPictureCount++;
			Calibrationm_bCapture = false;

			//CString str;
			//str.Format("%d",VisionPictureCount);
			//AfxMessageBox("�Ѿ���ȡ"+str+"��ͼƬ����");

		}
	}
}

//��������
void  CMyApp::postWeather(int ID)
{
	//float fval, s, c;
	static float angle = 0.0f;
	char str[256], *cp = getenv("PRESAGIS_VEGA_PRIME_4");

	vpEnv* m_env=(vpEnv*)vpEnv::find("myEnv");
	vpEnvSun* m_sun =(vpEnvSun*)vpEnvSun::find("myEnvSun");
	vpEnvSkyDome* m_skyDome = (vpEnvSkyDome*)vpEnvSkyDome::find("myEnvSkyDome");
	vpEnvCloudLayer* m_cloudLayer = (vpEnvCloudLayer*)vpEnvCloudLayer::find("myEnvCloudLayer");
	vpEnvWind* m_wind = (vpEnvWind*)vpEnvWind::find("myEnvWind");
	vpEnvSnow* m_snow = (vpEnvSnow*)vpEnvSnow::find("myEnvSnow");
	vpEnvRain* m_rain = (vpEnvRain*)vpEnvRain::find("myEnvRain");

	switch (ID) {

	case 1: // default settings
		m_env->setVisibilityColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_env->setVisibilityRangeOpaque(60000.0f);
		m_sun->setHorizonColor(1.0f, 0.545f, 0.239f, 1.0f);
		m_skyDome->setGroundColor(0.2117f, 0.286f, 0.15f, 1.0f);
		if (!vpEnvGlobals::instance()->getSkyLightEnable())
		{
			m_cloudLayer->setCoverageScale(0.26f);
			m_cloudLayer->setTextureTiling(2.0f, 2.0f);
			m_cloudLayer->setColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			sprintf(str, "%s/config/vegaprime/vpenv/cloud_scattered.inta", cp);
			m_cloudLayer->setTextureFile(str);
			m_cloudLayer->setTextureTiling(3.0f, 3.0f);
			m_cloudLayer->setColor(0.96f, 0.98f, 0.98f, 1.0f);
		}
		m_cloudLayer->setElevation(3000.0f, 5000.0f);
		m_cloudLayer->setTransitionRange(500.0f, 500.0f);
		m_wind->setSpeed(100.0f);
		m_snow->setEnable(false);
		m_rain->setEnable(false);
		break;

	case 2: // clear day, sparse clouds
		m_env->setVisibilityColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_env->setVisibilityRangeOpaque(100000.0f);
		m_sun->setHorizonColor(1.0f, 0.6f, 0.29f, 1.0f);//(1.0f, 0.6f, 0.29f, 1.0f)
		m_skyDome->setGroundColor(0.545098f, 0.756863f, 1.0f, 1.0f); //(0.33f, 0.29f, 0.15f, 1.0f)
		if (!vpEnvGlobals::instance()->getSkyLightEnable())
		{
			m_cloudLayer->setCoverageScale(0.1f);
		}
		else
		{
			sprintf(str, "%s/config/vegaprime/vpenv/cloud_few.inta", cp);
			m_cloudLayer->setTextureFile(str);
			m_cloudLayer->setTextureTiling(5.0f, 5.0f);
			m_cloudLayer->setColor(0.96f, 0.98f, 0.98f, 1.0f);
		}
		m_cloudLayer->setElevation(3000.0f, 5000.0f);
		m_cloudLayer->setTransitionRange(500.0f, 500.0f);
		m_wind->setSpeed(15.0f);
		m_snow->setEnable(false);
		m_rain->setEnable(false);
		break;
	case 3: // overcast with fog
		m_env->setVisibilityColor(0.75f, 0.75f, 0.75f, 1.0f);  
		m_env->setVisibilityRangeOpaque(8000.0f);
		m_sun->setHorizonColor(1.0f, 0.6f, 0.29f, 1.0f);
		m_skyDome->setGroundColor(0.745098f, 0.745098f, 0.745098f, 0.745098f);
		if (!vpEnvGlobals::instance()->getSkyLightEnable())
		{
			m_cloudLayer->setCoverageScale(0.5f);
		}
		else
		{
			sprintf(str, "%s/config/vegaprime/vpenv/cloud_overcast.inta", cp);
			m_cloudLayer->setTextureFile(str);
			m_cloudLayer->setTextureTiling(3.0f, 3.0f);
			m_cloudLayer->setColor(0.69f, 0.75f, 0.77f, 1.0f);
		}
		m_cloudLayer->setElevation(500.0f, 6000.0f);
		m_cloudLayer->setTransitionRange(500.0f, 500.0f);
		m_wind->setSpeed(0.0f);
		m_snow->setEnable(false);
		m_rain->setEnable(false);
		break;

	case 4: // stormy, high wind,rain, 
		m_env->setVisibilityColor(0.40f, 0.43f, 0.45f, 1.0f);
		m_env->setVisibilityRangeOpaque(50000.0f);
		m_sun->setHorizonColor(0.65f, 0.45f, 0.25f, 1.0f);
		m_skyDome->setGroundColor(0.33f, 0.29f, 0.15f, 1.0f);
		if (!vpEnvGlobals::instance()->getSkyLightEnable())
		{
			m_cloudLayer->setCoverageScale(0.4f);
		}
		else
		{
			sprintf(str, "%s/config/vegaprime/vpenv/cloud_storm.inta", cp);
			m_cloudLayer->setTextureFile(str);
			m_cloudLayer->setTextureTiling(3.0f, 3.0f);
			m_cloudLayer->setColor(0.52f, 0.56f, 0.61f, 1.0f);
		}
		m_cloudLayer->setElevation(1000.0f, 4000.0f);
		m_cloudLayer->setTransitionRange(500.0f, 500.0f);
		m_wind->setSpeed(5.0f);
		m_snow->setEnable(false);
		//m_rain->setNumParticles(50000);
		//m_rain->setEmitterBoxSize(5);
		m_rain->setEnable(true);
		
		break;
	case 5:
		m_env->setVisibilityColor(0.40f, 0.43f, 0.45f, 1.0f);
		m_env->setVisibilityRangeOpaque(50000.0f);
		m_sun->setHorizonColor(0.65f, 0.45f, 0.25f, 1.0f);
		m_skyDome->setGroundColor(0.33f, 0.29f, 0.15f, 1.0f);
		if (!vpEnvGlobals::instance()->getSkyLightEnable())
		{
			m_cloudLayer->setCoverageScale(0.4f);
		}
		else
		{
			sprintf(str, "%s/config/vegaprime/vpenv/cloud_storm.inta", cp);
			m_cloudLayer->setTextureFile(str);
			m_cloudLayer->setTextureTiling(3.0f, 3.0f);
			m_cloudLayer->setColor(0.52f, 0.56f, 0.61f, 1.0f);
		}
		m_cloudLayer->setElevation(1000.0f, 4000.0f);
		m_cloudLayer->setTransitionRange(500.0f, 500.0f);
		m_wind->setSpeed(5.0f);
		//m_snow->setNumParticles(50000);
		//m_snow->setEmitterBoxSize(5);
		m_snow->setEnable(true);
		m_rain->setEnable(false);
		
		break;


	default:
		break;

	}
}

//ͼ�������Ӿ���������
DWORD WINAPI CMyApp::ImageProcess(LPVOID lpParameter)
{
	CMyApp* temp = (CMyApp*)lpParameter;
	
	while(1)
	{
		if(SynchronizeVisionAndImageProcessFlag == 1)
		{  
			int ox = temp->OriginX,oy = temp->OriginY,sx = temp->WidthX,sy = temp->HeightY;
			// �����ݱ�����IplImage��   
			// ���Ϊ8��ͨ����Ϊ3,24λͼ  
			//��ͼ��ת��Ϊopencv�ĸ�ʽ
			IplImage* DrogueImage = cvCreateImage(cvSize(sx,sy), IPL_DEPTH_8U, 3);  

			memcpy(DrogueImage->imageData, temp->buffer, sx * sy * 3);
			free(temp->buffer);  
			// ����OpenGL��OpenCv��ȡbuffer�ķ�����y�����෴��������x�ᷭת���ڶ�������Ϊ 
			// NULL��ʾ��ԭ�ط�ת. 
			cvFlip(DrogueImage, NULL, 0); 
			CImageProcess ImageProcess;
			if(DrogueImage->imageData)
				ImageProcess.DroguePictureDetect(DrogueImage);//����ͼ��
			cvReleaseImage(&DrogueImage);
			SynchronizeVisionAndImageProcessFlag = 0;
			if(!Detect_Drogue_Image_Flag)
			{
				DisplayDroguelocateFlag = 0;

			}
			// create an image 

		}

	}

	return 0;
}

HBITMAP CMyApp::CopyScreen2BitMap(CRect& Rect)
{
	HDC      hScrDC, hMemDC;      
	HBITMAP  hOldBitmap,hBitmap;  
	int      xScrn, yScrn;    

	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL); 
	hMemDC = CreateCompatibleDC(hScrDC); 

	xScrn = GetDeviceCaps(hScrDC, HORZRES); 
	yScrn = GetDeviceCaps(hScrDC, VERTRES); 

	if (Rect.left < 0)  Rect.left = 0; 
	if (Rect.top < 0)   Rect.top  = 0; 
	if (Rect.right  > xScrn)  Rect.right = xScrn; 
	if (Rect.bottom > yScrn) Rect.bottom = yScrn; 

	hBitmap = CreateCompatibleBitmap(hScrDC, Rect.Width(),Rect.Height()); 
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); 
	BitBlt(hMemDC,0,0,Rect.Width(),Rect.Height(),hScrDC,Rect.left,Rect.top,SRCCOPY); 
	hBitmap =(HBITMAP)SelectObject(hMemDC,hOldBitmap); 
	ASSERT(hBitmap);

	DeleteDC(hScrDC); 
	DeleteDC(hMemDC); 
	DeleteObject(hOldBitmap);

	return hBitmap; 
}

void CMyApp::HBitmap2Ipl(HBITMAP hBmp, IplImage* dst)
{
	BITMAP bmp;    
	GetObject(hBmp,sizeof(BITMAP),&bmp);    

	// get channels which equal 1 2 3 or 4    
	// bmBitsPixel :   
	// Specifies the number of bits    
	// required to indicate the color of a pixel.    
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8 ;   

	// get depth color bitmap or grayscale   
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;    


	// create header image   
	IplImage* img = cvCreateImage(cvSize(bmp.bmWidth,bmp.bmHeight),depth,nChannels);    

	// allocat memory for the pBuffer   
	BYTE *pBuffer = new BYTE[bmp.bmHeight*bmp.bmWidth*nChannels];    

	// copies the bitmap bits of a specified device-dependent bitmap into a buffer   
	GetBitmapBits(hBmp,bmp.bmHeight*bmp.bmWidth*nChannels,pBuffer);    

	// copy data to the imagedata   
	memcpy(img->imageData,pBuffer,bmp.bmHeight*bmp.bmWidth*nChannels);   
	delete pBuffer;    

	// create the image   
	//IplImage *dst = cvCreateImage(cvGetSize(img),img->depth,3);   
	// convert color   
	cvCvtColor(img,dst,CV_BGRA2BGR);   
	cvReleaseImage(&img);   
}



