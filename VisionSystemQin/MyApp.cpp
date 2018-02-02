//与vegaprimer相关
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
POSTION3D	Drogue_Pos;        //锥套位置
CPOSTION3D	Receiver_Pos;	     //受油机位置
CHPR		Receiver_HPR;	//受油机姿态
CPOSTION3D	Probe_ref_Receiver_Pos;	//受油插头相对于受油机的位置
CPOSTION3D  Drogue_ref_Tanker_Pos;  //锥套相对于加油机位置
CPOSTION3D  Probe_Pos;//受油插头场景位置
double SetupFax,SetupFay,SetupFaz,SetLeftFax,SetLeftFay,SetLeftFaz;
int VisionMode=1;
bool m_bScreenDisplay,m_bCamaraChannelDisplay;//屏显显示位

float MotionDistance = 0.1,RotateDegree = 1.0;
int VisionPictureCount = 0,m_WheatherMode = 0,SynchronizeVisionAndImageProcessFlag = 0;
vuString filenameCameraChannel;
int count=0;
double DrogueRalativeCameraDistanceX,DrogueRalativeCameraDistanceY,DrogueRalativeCameraDistanceZ;
vuVec3<double> DroguePosition;
vuVec3<double> CameraTranformpos;
double F16PositionX[20000] = {0},F16PositionY[20000] ={0},F16PositionZ[20000] = {0};
int F16PositionCount;
vpObject *pReceiver;	//受油机



CMyApp::CMyApp(void)
{
	m_iDisplayMode = 0;//初始化为无屏显状态
	m_bScreenDisplay = false;
	m_fSpeed = 0;//当前速度
	m_fSetSpeed = 0;//设定速度
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

int CMyApp::configure() //系统配置
{
	// 配置Vega Prime系统
	vpApp::configure();

	CalibrationpMainChannel = vpChannel::find("CalibrationChannel");//主通道	
	assert(CalibrationpMainChannel);


	CalibrationpMainChannel->addSubscriber(vsChannel::EVENT_POST_DRAW, this);


	Calibrationm_data = vuAllocArray<uchar >::malloc(
		MAX_WINDOW_WIDTH*MAX_WINDOW_HEIGHT*3);

	vrFontFactory *fontFactory = new vrFontFactory();
	m_pFont = (vrFont2D*)fontFactory->read("system");//读取系统字体
	fontFactory->unref();

	pTanker = vpObject::find("c130");//加油机
	assert(pTanker);

	pReceiver = vpObject::find("f16");//受油机
	assert(pReceiver);

	pDrogue = vpObject::find("drogue");//受油机
	assert(pDrogue);

	CameraTranform = vpTransform::find("farTransform");
	assert(CameraTranform);

	Ring = vpObject::find("ring");//受油机
	assert(Ring);

	MyLightSetting = vpLight::find("myLight");

	pMainChannel = vpChannel::find("myChannel");//主通道	
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
void CMyApp::onKeyInput(vrWindow::Key key, int mod)//重载键盘消息处理函数
{
	if(ExitCalibrationDiaglog == 0)
	{
		double x,y,z;//存储位置
		double h,p,r;//存储姿态
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
				AfxMessageBox("移动距离已经达到最小值！！");
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
				AfxMessageBox("转动角度已经达到最小值！！");
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
			if(m_bCamaraChannelDisplay)//显示
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

//通道的响应函数，有几个通道响应几次
void CMyApp::notify(vsChannel::Event event, const vsChannel *channel,vrDrawContext *context)//重载pre/post draw消息处理函数
{
	double x,y,z;//存储位置
	double h,p,r;//存储姿态
	CPOSTION3D temppos;
	//通道名字
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
				//读取图像缓冲区数据  
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
		//根据设置决定是否显示屏显
		if(m_bScreenDisplay)
		{
			//-----------------------2009-10-28加入-----------------------------
			//得到受油机位置
			vuVec3<double> pReceiverpos;
			pReceiver->getAbsolutePosition(&pReceiverpos);
			//pReceiver->getTranslate(&x,&y,&z);
			pReceiver->getRotate(&h,&p,&r);
			m_ReceiverPosxText.sprintf("Receiver Xd Pos: %10.5f",pReceiverpos[0]);//按照苏制坐标系
			m_ReceiverPosyText.sprintf("Receiver Yd Pos: %10.5f",pReceiverpos[1]);
			m_ReceiverPoszText.sprintf("Receiver Zd Pos: %10.5f",pReceiverpos[2]);
			m_ReceiverHText.sprintf("Receiver Yaw Angle: %10.5f",h);
			m_ReceiverPText.sprintf("Receiver Pitch Angle: %10.5f",p);
			m_ReceiverRText.sprintf("Receiver Roll Angle: %10.5f",r);

			//计算受油插头空间位置
			Receiver_Pos.x = pReceiverpos[0];
			Receiver_Pos.y = pReceiverpos[1];
			Receiver_Pos.z = pReceiverpos[2];

			Receiver_HPR.h = h;
			Receiver_HPR.p = p;
			Receiver_HPR.r = r;
			
			pTanker->getTranslate(&x,&y,&z);
			pTanker->getRotate(&h,&p,&r);
			m_TankerPosxText.sprintf("Tanker Xd Pos: %10.5f",x);//按照苏制坐标系
			m_TankerPosyText.sprintf("Tanker Yd Pos: %10.5f",y);
			m_TankerPoszText.sprintf("Tanker Zd Pos: %10.5f",z);
			m_TankerHText.sprintf("Tanker Yaw Angle: %10.5f",h);
			m_TankerPText.sprintf("Tanker Pitch Angle: %10.5f",p);
			m_TankerRText.sprintf("Tanker Roll Angle: %10.5f",r);
	
			vuVec3<double> CameraTranformpos;
			CameraTranform->getAbsolutePosition(&CameraTranformpos); 

			//PosTrans(Receiver_HPR,Probe_ref_Receiver_Pos,temppos);//根据受油机的姿态角将受油插头的相对位置转化为场景位置
			Probe_Pos.x = CameraTranformpos[0]; //+ temppos;
			Probe_Pos.y = CameraTranformpos[1]; //+ temppos;
			Probe_Pos.z = CameraTranformpos[2]; //+ temppos;

			m_ProbePosxText.sprintf("Probe Xd Pos: %10.5f",Probe_Pos.x);
			m_ProbePosyText.sprintf("Probe Yd Pos: %10.5f",Probe_Pos.y);
			m_ProbePoszText.sprintf("Probe Zd Pos: %10.5f",Probe_Pos.z);
	

			//得到锥管位置
			vuVec3<double> droguepos;
			pDrogue->getAbsolutePosition(&droguepos); 

			m_DroguePosxText.sprintf("Drogue Xd Pos: %10.5f",droguepos[0]);//苏制坐标系
			m_DroguePosyText.sprintf("Drogue Yd Pos: %10.5f",droguepos[1]);
			m_DroguePoszText.sprintf("Drogue Zd Pos: %10.5f",droguepos[2]);

			//将当前矩阵压入堆栈
			context->pushElements(true);//重置模型视点矩阵

			//禁止深度测试
			vrDepthTest::Element depthTestElement;
			depthTestElement.m_enable = false;
			context->setElement(vrDepthTest::Element::Id,&depthTestElement);

			//开启正投影
			vrTransform::ElementProjection projectionElement;
			projectionElement.makeOrthographic(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);
			context->setElement(vrTransform::ElementProjection::Id,&projectionElement);
			//输出字符串
			vuVec4<float> color(FontcolorR,FontcolorG,FontcolorB,1.0f);

			//根据摄像机窗口是否显示决定文字信息的显示位置
			float delta;
			if(!m_bCamaraChannelDisplay)
			{
				delta = 0.0f;
			}
			else
			{
				delta = -0.3f;
			}
			//显示字符串
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
			//弹出矩阵
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
			//AfxMessageBox("已经截取"+str+"张图片！！");

		}
	}
}

//设置天气
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

//图像处理与视景并行运行
DWORD WINAPI CMyApp::ImageProcess(LPVOID lpParameter)
{
	CMyApp* temp = (CMyApp*)lpParameter;
	
	while(1)
	{
		if(SynchronizeVisionAndImageProcessFlag == 1)
		{  
			int ox = temp->OriginX,oy = temp->OriginY,sx = temp->WidthX,sy = temp->HeightY;
			// 将数据保存在IplImage中   
			// 深度为8，通道数为3,24位图  
			//将图像转化为opencv的格式
			IplImage* DrogueImage = cvCreateImage(cvSize(sx,sy), IPL_DEPTH_8U, 3);  

			memcpy(DrogueImage->imageData, temp->buffer, sx * sy * 3);
			free(temp->buffer);  
			// 由于OpenGL与OpenCv读取buffer的方向在y轴上相反，所以作x轴翻转，第二个参数为 
			// NULL表示作原地翻转. 
			cvFlip(DrogueImage, NULL, 0); 
			CImageProcess ImageProcess;
			if(DrogueImage->imageData)
				ImageProcess.DroguePictureDetect(DrogueImage);//处理图像
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



