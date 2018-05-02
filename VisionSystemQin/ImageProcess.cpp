// ImageProcess.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSystemQin.h"
#include "VisionSystemQinDlg.h"
#include "ImageProcess.h"
#include "CalibrateCamera.h"
#include "ResultDisplay.h"
#include "MyApp.h"
#include "math.h"
#include <cxcore.h>
#include <time.h>
#include "ShowMschart.h"
#include "epnp.h"
#include "../../../../../../../ProgramData/OpenCV2/include/opencv2/legacy/compat.hpp"

#define GrayScale 256  
#define hudu2du 57.29578

using namespace std;
using namespace cv;



int SaveImagenumber = 1;
int ExitCalibrationDiaglog = 0;
bool Droguefollowdynamicdisplay,Droguefollowdisplay,Droguelocatedynamicdisplay,Droguelocatedisplay,
	Droguedetectdynamicdisplay,Droguedetectdisplay;
int Choose_Noise_Flag,ErodeDilateOrderFlag;
int DetectfailureTimes,DetectTotalTimes,DetectSuccessTimes;
double m_Slider_Bright_Value,m_Slider_Contrast_Value,m_Slider_Noise_Value;
int DrogueLocateSolveMethodFlag,FilteringMethodFlag,DetectMethodFlag;
int FilteringMethod;
double ImageCenterX,ImageCenterY;
CWnd* hWnd;
int Edit_Dilate,Edit_Erode;
double ImageDroguePositionX,ImageDroguePositionY,ImageDroguePositionZ;
double DrogueLocateAccurateX,DrogueLocateAccurateY,DrogueLocateAccurateZ;
bool StartFollowFlag,LockCameraChannelFlag;
int DisplayDroguelocateFlag;
int IncreaseAreaD,FirstFrameOverFlag;
double FollowAreaLeft,FollowAreaRight,FollowAreaUp,FollowAreaDown;
double DrogueLocateAccurateXArray[40000] ={0},DrogueLocateAccurateYArray[40000] ={0},
DrogueLocateAccurateZArray[40000] ={0},FrameProcessTime[40000] ={0},ImageDetectHeading[40000]={0},
ImageDetectPitch[40000]={0};
double DrogueLocateXArray[40000] ={0},DrogueLocateYArray[40000] ={0},
DrogueLocateZArray[40000] ={0},DrogueSolveXArray[40000] ={0},DrogueSolveYArray[40000] ={0},
DrogueSolveZArray[40000] ={0};
int CurrentFrame = 0;
double Drogue2DImagepotion[12][3];//,Drogue3Dpotion[12][3] = {{-0.409481,-0.58,-0.000113},
//{-0.355070,-0.58,-0.205000},{-0.205000,-0.58,-0.355070},{0,-0.58,-0.41},{0.205000,-0.58,-0.355070},
//{0.355070,-0.58,-0.205000},{0.410073,-0.58,0.000065},{0.355070,-0.58,0.205000},{0.205000,-0.58,0.355070},
//{0,-0.58,0.41},{-0.205000,-0.58,0.355070},{-0.355070,-0.58,0.205000}};
double Drogue3Dpotion[12][3] = {{-0.409481,0.000113,-0.58},
{-0.355070,0.205000,-0.58},{-0.205000,0.355070,-0.58},{0,0.41,-0.58},{0.205000,0.355070,-0.58},
{0.355070,0.205000,-0.58},{0.410073,-0.000065,-0.58},{0.355070,-0.205000,-0.58},{0.205000,-0.355070,-0.58},
{0,-0.41,-0.58},{-0.205000,-0.355070,-0.58},{-0.355070,-0.205000,-0.58}};
//double Drogue3Dpotion[12][3] = {{0.58,-0.409481,-0.000113},
//{0.58,-0.355070,-0.205000},{0.58,-0.205000,-0.355070},{0.58,0,-0.41},{0.58,0.205000,-0.355070},
//{0.58,0.355070,-0.205000},{0.58,0.410073,0.000065},{0.58,0.355070,0.205000},{0.58,0.205000,0.355070},
//{0.58,0,0.41},{0.58,-0.205000,0.355070},{0.58,-0.355070,0.205000}};
CWnd *globalImageProcessthis;
double ImageCenterPreviousFrameX[30008],ImageCenterPreviousFrameY[30008];

IMPLEMENT_DYNAMIC(CImageProcess, CDialog)

CImageProcess::CImageProcess(CWnd* pParent /*=NULL*/)
	: CDialog(CImageProcess::IDD, pParent)
	, m_Edit_Dilate(1)
	, m_Edit_Erode(1)
	, m_IncreaseAreaD(4)
{

}

CImageProcess::~CImageProcess()
{
}

void CImageProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Dilate, m_Edit_Dilate);  //膨胀次数
	DDX_Text(pDX, IDC_EDIT_Erode, m_Edit_Erode);    //腐蚀次数
	DDX_Text(pDX, IDC_EDIT_InreaseAreaD, m_IncreaseAreaD); //下一帧检测区域
	DDV_MinMaxInt(pDX, m_Edit_Erode, 0, 10);
	DDV_MinMaxInt(pDX, m_Edit_Dilate, 0, 10);
}

//自己生成对，ON_BN_CLICKED鼠标单击按钮时发送的响应
BEGIN_MESSAGE_MAP(CImageProcess, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CImageProcess::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_ImageStartDrogueDetect, &CImageProcess::OnBnClickedButtonImagestartdroguedetect)
	ON_BN_CLICKED(IDC_BUTTON_DrogueDetectDisplay, &CImageProcess::OnBnClickedButtonDroguedetectdisplay)
	ON_BN_CLICKED(IDC_BUTTON_DrogueDetectDynamicDisplay, &CImageProcess::OnBnClickedButtonDroguedetectdynamicdisplay)
	ON_BN_CLICKED(IDC_BUTTON_DrogueLocateDisplay, &CImageProcess::OnBnClickedButtonDroguelocatedisplay)
	ON_BN_CLICKED(IDC_BUTTON_DrogueLocateDynamicDisplay, &CImageProcess::OnBnClickedButtonDroguelocatedynamicdisplay)
	ON_BN_CLICKED(IDC_BUTTON_DrogueFollowDisplay, &CImageProcess::OnBnClickedButtonDroguefollowdisplay)
	ON_BN_CLICKED(IDC_BUTTON_DrogueFollowDynamicDisplay, &CImageProcess::OnBnClickedButtonDroguefollowdynamicdisplay)
	ON_BN_CLICKED(IDC_RADIO_GuassNoise, &CImageProcess::OnBnClickedRadioGuassnoise)
	ON_BN_CLICKED(IDC_RADIO_UniformNoise, &CImageProcess::OnBnClickedRadioUniformnoise)
	ON_BN_CLICKED(IDC_RADIO_SaltNoise, &CImageProcess::OnBnClickedRadioSaltnoise)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DilateErodeOrder, &CImageProcess::OnBnClickedButtonDilateerodeorder)
	ON_CBN_SELCHANGE(IDC_COMBO_DetectMethod, &CImageProcess::OnCbnSelchangeComboDetectmethod)
	ON_CBN_SELCHANGE(IDC_COMBO_FilteringMethod, &CImageProcess::OnCbnSelchangeComboFilteringmethod)
	ON_CBN_SELCHANGE(IDC_COMBO_DrogueLocateSolveMethod, &CImageProcess::OnCbnSelchangeComboDroguelocatesolvemethod)
	ON_EN_CHANGE(IDC_EDIT_Dilate, &CImageProcess::OnEnChangeEditDilate)
	ON_EN_CHANGE(IDC_EDIT_Erode, &CImageProcess::OnEnChangeEditErode)
	ON_MESSAGE(WM_MY_MESSAGE, &CImageProcess::WriteControl)
	ON_BN_CLICKED(IDC_BUTTON_StartFollow, &CImageProcess::OnBnClickedButtonStartfollow)
	ON_EN_CHANGE(IDC_EDIT_InreaseAreaD, &CImageProcess::OnEnChangeEditInreasearead)
	ON_BN_CLICKED(IDC_BUTTON_LocateCurveDisplay, &CImageProcess::OnBnClickedButtonLocatecurvedisplay)
	ON_BN_CLICKED(IDC_BUTTON_LockCameraChannel, &CImageProcess::OnBnClickedButtonLockcamerachannel)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CImageProcess message handlers

BOOL CImageProcess::OnInitDialog()
{
	MaskControl();
	globalImageProcessthis = this;
	Droguefollowdynamicdisplay= true;
	Droguefollowdisplay= true;
	Droguelocatedynamicdisplay= true;
	Droguelocatedisplay= true;
	Droguedetectdynamicdisplay= true;
	Droguedetectdisplay= true;
	Choose_Noise_Flag = 1;
	ErodeDilateOrderFlag = 1;
	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO_GuassNoise);
	radio->SetCheck(1);
	CSliderCtrl   *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BrightAdjust); //亮度调节
	pSlidCtrl->SetRange(-100, 100,TRUE);
	pSlidCtrl->SetPos(0);

	pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ContrastAdjust); //对比度调节
	pSlidCtrl->SetRange(-100, 100,TRUE);
	pSlidCtrl->SetPos(0);
	pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_GuassNoise);  //噪声比调节
	pSlidCtrl->SetRange(0, 600,TRUE);
	m_Edit_Dilate = 1;
	m_Edit_Erode = 1;
	Edit_Dilate = Edit_Erode = 1;
	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT_GuassNoise)->SetWindowText("0");
	GetDlgItem(IDC_EDIT_ContrastAdjust)->SetWindowText("0");
	GetDlgItem(IDC_EDIT_BrightAdjust)->SetWindowText("0");
	m_Slider_Bright_Value = 0,m_Slider_Contrast_Value = 0,m_Slider_Noise_Value = 0;
	m_Slider_Disturbance_Value = 1;
	DetectfailureTimes = 0,DetectTotalTimes = 0;
	DetectSuccessTimes = 0;

	CComboBox *Combo_DetectMethod = (CComboBox*)GetDlgItem(IDC_COMBO_DetectMethod);//检测放法
	Combo_DetectMethod->AddString(_T("图像相减法"));
	Combo_DetectMethod->SetCurSel(0);
	CComboBox *Combo_FilteringMethod = (CComboBox*)GetDlgItem(IDC_COMBO_FilteringMethod); //滤波方法
	Combo_FilteringMethod->AddString(_T("中值滤波"));
	Combo_FilteringMethod->AddString(_T("简单模糊"));
	Combo_FilteringMethod->AddString(_T("高斯模糊"));
	Combo_FilteringMethod->AddString(_T("双向滤波"));
	Combo_FilteringMethod->SetCurSel(3);
	FilteringMethod = CV_MEDIAN;
	CComboBox *Combo_DrogueLocateSolveMethod = (CComboBox*)GetDlgItem(IDC_COMBO_DrogueLocateSolveMethod); //锥套定位求解方法
	Combo_DrogueLocateSolveMethod->AddString(_T("最小椭圆+摄像机模型"));
	Combo_DrogueLocateSolveMethod->AddString(_T("最小椭圆+等比例"));
	Combo_DrogueLocateSolveMethod->AddString(_T("LHM算法"));
	Combo_DrogueLocateSolveMethod->AddString(_T("EPNP算法"));
	Combo_DrogueLocateSolveMethod->SetCurSel(0);

	DrogueLocateSolveMethodFlag = 1,FilteringMethodFlag = 4,DetectMethodFlag = 1;
	ImageDroguePositionX = 0;
	ImageDroguePositionY = 0;
	ImageDroguePositionZ = 0;
	DrogueLocateAccurateX = 0,DrogueLocateAccurateY = 0,DrogueLocateAccurateZ = 0;
	DisplayDroguelocateFlag = 0;
	StartFollowFlag = false;
	LockCameraChannelFlag = false;
	IncreaseAreaD = 4;
	GetDlgItem(IDC_EDIT_InreaseAreaD)->SetWindowText("4");
	FirstFrameOverFlag = 0;

	hWnd=this;
	this->SendMessage(WM_MY_MESSAGE,0,0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
#define SAVE_IMAGE 1
//图像处理函数，核心函数，完成图像的检测、跟踪、定位以及各种干扰的添加。
void CImageProcess::DroguePictureDetect(IplImage* CurrentVisionImage)
{
	clock_t start = clock(); 
	IplImage* FollowImage;
#if SAVE_IMAGE
	char videoname[64];
	static uint32_t cnt = 0;
	int p[3];
	p[0] = CV_IMWRITE_JPEG_QUALITY;
	p[1] = 100;
	p[2] = 0;
	sprintf(videoname, "./saveimage/%d.jpg", cnt++);
	cvSaveImage(videoname, CurrentVisionImage, p);
#endif
	int IntFollowAreaLeft;
	int IntFollowAreaRight;
	int IntFollowAreaUp;
	int IntFollowAreaDown;
	if(StartFollowFlag && FirstFrameOverFlag == 1)
	{
		// 上一次检测推算出的检测区域
		IntFollowAreaLeft = (int)FollowAreaLeft;
		IntFollowAreaRight = (int)FollowAreaRight;
		IntFollowAreaUp = (int)FollowAreaUp;
		IntFollowAreaDown = (int)FollowAreaDown;


		CvSize size= cvSize(IntFollowAreaRight - IntFollowAreaLeft,IntFollowAreaDown - IntFollowAreaUp);//区域大小
		cvSetImageROI(CurrentVisionImage,cvRect(IntFollowAreaLeft,IntFollowAreaUp,size.width, size.height));//设置源图像ROI
		FollowImage = cvCreateImage(size,CurrentVisionImage->depth,CurrentVisionImage->nChannels);//创建目标图像
		cvCopy(CurrentVisionImage,FollowImage); //复制图像
		cvResetImageROI(FollowImage);//源图像用完后，清空ROI
		//cvResetImageROI(CurrentVisionImage);	
		//cvRectangle(CurrentVisionImage,cvPoint(IntFollowAreaLeft,IntFollowAreaUp),
		//	cvPoint(IntFollowAreaRight,IntFollowAreaDown),CV_RGB(0,0,0),2,8);
	}
	else
	{
		FollowImage = cvCreateImage(cvGetSize(CurrentVisionImage),CurrentVisionImage->depth,CurrentVisionImage->nChannels);
		cvCopy(CurrentVisionImage,FollowImage);
	}
 
	DetectTotalTimes++;
	Mat new_image = cvarrToMat(FollowImage);

	// 执行运算 new_image(i,j) = alpha*image(i,j) + beta
	// 亮度 对比度
	if(m_Slider_Contrast_Value != 0 || m_Slider_Bright_Value != 0)
	{
		new_image.convertTo(new_image, -1, 1 + m_Slider_Contrast_Value / 100 + 0.01, m_Slider_Bright_Value);
		/*for (int y = 0; y < new_image.rows; y++)
		{
			for (int x = 0; x < new_image.cols; x++)
			{
				for (int c = 0; c < 3; c++)
				{
					new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>((1 + m_Slider_Contrast_Value / 50)*(new_image.at<Vec3b>(y, x)[c]) + m_Slider_Bright_Value);
				}
			}
		}*/

	}
	
	IplImage M = new_image;
	//噪声
	if(m_Slider_Noise_Value != 0)
	{
		if (Choose_Noise_Flag == 1)
		{
			if (m_Slider_Noise_Value <= 600)
			{
				if (m_Slider_Noise_Value >= 0)
				{
					GausssNoise(&M, m_Slider_Noise_Value/6);
				}
			}
		}
		if (Choose_Noise_Flag == 2)
		{
			if (m_Slider_Noise_Value <= 600)
			{
				if (m_Slider_Noise_Value > 0)
				{
					UniformNoise(&M, m_Slider_Noise_Value/6);
				}
			}

		}
		if (Choose_Noise_Flag == 3)
		{
			if (m_Slider_Noise_Value <= 600)
			{
				if (m_Slider_Noise_Value >= 0)
				{
					SaltNoise(new_image, m_Slider_Noise_Value / 2400);
				}
			}

		}	

	}
	
	IplImage* h_R =cvCreateImage(cvGetSize(FollowImage),IPL_DEPTH_8U,1);//H通道域值化	
	IplImage* h_G =cvCreateImage(cvGetSize(FollowImage),IPL_DEPTH_8U,1);	
	IplImage* h_B =cvCreateImage(cvGetSize(FollowImage),IPL_DEPTH_8U,1);	

	if(DetectMethodFlag == 1)
	{
		cvSplit(FollowImage, h_B, h_G, h_R, 0);
		cvSub(h_R,h_G,h_R);
		cvSub(h_R,h_B,h_R);

	}
	// 二值化
	cvThreshold(h_R,h_R,20,255,CV_THRESH_BINARY);
	if(FilteringMethod == CV_BILATERAL )
		cvSmooth(h_R, h_R, FilteringMethod, 3, 0, 21, 5);//高斯滤波平滑图像
	else
		cvSmooth(h_R, h_R, FilteringMethod, 5, 5, 0, 0);
	// 膨胀、腐蚀
	if(ErodeDilateOrderFlag == 0)
	{
		cvErode(h_R, h_R, NULL, Edit_Erode);
		cvDilate(h_R, h_R, NULL, Edit_Dilate);
	}else
	{
		cvDilate(h_R, h_R, NULL, Edit_Dilate);
		cvErode(h_R, h_R, NULL, Edit_Erode);
	}

	cvReleaseImage(&h_G);
	cvReleaseImage(&h_B);

	CvMemStorage* memstorage = cvCreateMemStorage(0);      
	CvSeq *first_contour = NULL;
	IplImage* img_temp = cvCreateImage(cvGetSize(h_R), 8, 1);
	cvCopyImage(h_R, img_temp);  
	int num = cvFindContours(img_temp,   
		memstorage, // a vector of contours
		&first_contour); // retrieve all pixels of each contours  
	cvZero(img_temp);
	cvDrawContours(img_temp,first_contour,  
		cvScalar(100), // draw all contours  
		cvScalar(100), // in black  
		2); // with a thickness of 2 

	if(num > 12 || num == 0)
	{
		DetectfailureTimes++;
		hWnd->SendMessage(WM_MY_MESSAGE,0,0);
		cvClearMemStorage(memstorage); 
		cvReleaseImage(&h_R);
		cvReleaseImage(&img_temp);
		cvReleaseImage(&FollowImage);
		return;
	}

	if(Droguedetectdisplay)
	{
		if(Droguedetectdynamicdisplay)
		{
			cvShowImage("锥套检测",h_R);
		}
		if(!Detect_Drogue_Image_Flag)
			cvDestroyWindow("锥套检测");
	}
	else
	{
		cvDestroyWindow("锥套检测");
	}

	CvMoments m;
	CvMat *region; 
	double X[100]={0},Y[100]={0},M00,Xmin=10000,Ymin=10000,Xmax=0,Ymax=0;
	int contour_size = 0;
	cv::Mat img = cvarrToMat(img_temp);
	
	for(int i=0; first_contour!= 0; first_contour = first_contour->h_next,i++) 
	{ 
		if(ContourCenterPointSolveMethodFlag == 1)
		{
			first_contour = cvApproxPoly( first_contour, sizeof(CvContour), memstorage, CV_POLY_APPROX_DP, 3, 1 ); 

			region=(CvMat*)first_contour; 
			cvMoments( region,&m,0); 
			M00 = cvGetSpatialMoment(&m,0,0);
			X[i] = cvGetSpatialMoment(&m,1,0)/M00;
			Y[i] = cvGetSpatialMoment(&m,0,1)/M00;	

		}else
		{
			CvPoint *point =new CvPoint[first_contour->total];  
			CvSeqReader reader;  
			CvPoint pt=cvPoint(0,0);  
			cvStartReadSeq(first_contour,&reader);  
			for (int j=0;j<first_contour->total;j++)  
			{  
				CV_READ_SEQ_ELEM(pt,reader);
				point[j]=pt;  
			}  
			for (int j=0;j<first_contour->total;j++)  
			{  
				//int k=(j+1)%first_contour->total;  
				//cvLine(dst,point[j],point[k],cvScalar(0,0,255),1,4,0);  
				X[i] += point[j].x;
				Y[i] += point[j].y;
			}  
			X[i]=double(X[i])/first_contour->total;
			Y[i]=double(Y[i])/first_contour->total;
			delete[] point;

		}
		cv::circle(img,cv::Point2d(X[i],Y[i]),0.5,CV_RGB(255,255,255),-1,8);


		if(StartFollowFlag && FirstFrameOverFlag == 1)
		{
			X[i] = X[i] + IntFollowAreaLeft;
			Y[i] = Y[i] + IntFollowAreaUp;
		}
		
		Xmin = min(Xmin,X[i]);
		Ymin = min(Ymin,Y[i]);
		Xmax = max(Xmax,X[i]);
		Ymax = max(Ymax,Y[i]);

		contour_size++;
	}

	cvClearMemStorage(memstorage); 
	DetectSuccessTimes++;
	FirstFrameOverFlag = 0;
	//最小椭圆+摄像机模型
	if(DrogueLocateSolveMethodFlag == 1)
	{
		if(StartFollowFlag)
		{
			cvRectangle(FollowImage,cvPoint(Xmin-IntFollowAreaLeft,Ymin-IntFollowAreaUp),
				cvPoint(Xmax-IntFollowAreaLeft,Ymax-IntFollowAreaUp),CV_RGB(0,255,0),2,8);
			CvSize size= cvSize(IntFollowAreaRight - IntFollowAreaLeft,IntFollowAreaDown - IntFollowAreaUp);//区域大小
			cvCopy(FollowImage,CurrentVisionImage); //复制图像
			cvResetImageROI(CurrentVisionImage);
			cvRectangle(CurrentVisionImage,cvPoint(IntFollowAreaLeft,IntFollowAreaUp),
				cvPoint(IntFollowAreaRight,IntFollowAreaDown),CV_RGB(0,0,0),2,8);
		}else
		{
			cvRectangle(FollowImage,cvPoint(Xmin,Ymin),cvPoint(Xmax,Ymax),CV_RGB(0,255,0),2,8);

		}

		//cv::rectangle(img,cv::Point2d(Xmin,Ymin),cv::Point2d(Xmax,Ymax),CV_RGB(255,255,255),1,8);

		double LeftMatrix[5][5] = {0},RightMatrix[5] = {0};
		for(int k=0;k<contour_size;k++)
		{
			LeftMatrix[0][0]+=pow(X[k],2)*pow(Y[k],2);
			LeftMatrix[0][1]+=X[k]*pow(Y[k],3);
			LeftMatrix[0][2]+=pow(X[k],2)*Y[k];
			LeftMatrix[0][3]+=X[k]*pow(Y[k],2);
			LeftMatrix[0][4]+=X[k]*Y[k];
			LeftMatrix[1][0]+=X[k]*pow(Y[k],3);
			LeftMatrix[1][1]+=pow(Y[k],4);
			LeftMatrix[1][2]+=X[k]*pow(Y[k],2);
			LeftMatrix[1][3]+=pow(Y[k],3);
			LeftMatrix[1][4]+=pow(Y[k],2);
			LeftMatrix[2][0]+=pow(X[k],2)*Y[k];
			LeftMatrix[2][1]+=X[k]*pow(Y[k],2);
			LeftMatrix[2][2]+=pow(X[k],2);
			LeftMatrix[2][3]+=X[k]*Y[k];
			LeftMatrix[2][4]+=X[k];
			LeftMatrix[3][0]+=X[k]*pow(Y[k],2);
			LeftMatrix[3][1]+=pow(Y[k],3);
			LeftMatrix[3][2]+=X[k]*Y[k];
			LeftMatrix[3][3]+=pow(Y[k],2);
			LeftMatrix[3][4]+=Y[k];
			LeftMatrix[4][0]+=X[k]*Y[k];
			LeftMatrix[4][1]+=pow(Y[k],2);
			LeftMatrix[4][2]+=X[k];
			LeftMatrix[4][3]+=Y[k];
			LeftMatrix[4][4]+=1;

			RightMatrix[0]-=pow(X[k],3)*Y[k];
			RightMatrix[1]-=pow(X[k],2)*pow(Y[k],2);
			RightMatrix[2]-=pow(X[k],3);
			RightMatrix[3]-=pow(X[k],2)*Y[k];
			RightMatrix[4]-=pow(X[k],2);

		} //式4.10
		CvMat Leftmat = cvMat(5,5,CV_64FC1,LeftMatrix);
		CvMat Rightmat = cvMat(5,1,CV_64FC1,RightMatrix);
		CvMat* Dst =  cvCreateMat(5,1,CV_64FC1);
		int result =cvSolve(&Leftmat,&Rightmat,Dst,CV_LU);
		if(result != 1)
		{
			DetectfailureTimes++;
			hWnd->SendMessage(WM_MY_MESSAGE,0,0);
			cvReleaseImage(&h_R);
			cvReleaseImage(&img_temp);
			cvReleaseMat(&Dst);
			cvReleaseImage(&FollowImage);
			return;
		}
		assert(result == 1);
		double CenterX,CenterY;
		long double A = cvmGet(Dst,0,0);
		long double B = cvmGet(Dst,1,0);
		long double C = cvmGet(Dst,2,0);
		long double D = cvmGet(Dst,3,0);
		long double E = cvmGet(Dst,4,0);
		CenterX = double((2*B*C-A*D)/(pow(A,2)-4*B));
		CenterY = double((2*D-A*D)/(pow(A,2)-4*B));
		ImageCenterX = CenterX,ImageCenterY = CenterY;
		if(StartFollowFlag)
		{
			cv::circle(img,cv::Point2d(CenterX - IntFollowAreaLeft,CenterY - IntFollowAreaUp),2,CV_RGB(255,255,255),-1,8);
		}else
			cv::circle(img,cv::Point2d(CenterX,CenterY),2,CV_RGB(255,255,255),-1,8);
		// 椭圆拟合

		double XPixelSize = Xmax-Xmin;
		double YPixelSize =Ymax-Ymin;

		double XSpeed = 0,YSpeed = 0;
		if(DetectSuccessTimes > 30000)
		{
			DetectSuccessTimes = 1;
		}
		ImageCenterPreviousFrameX[DetectSuccessTimes]=CenterX;
		ImageCenterPreviousFrameY[DetectSuccessTimes]=CenterY;
		if(DetectSuccessTimes > 2)
		{
			XSpeed = fabs(ImageCenterPreviousFrameX[DetectSuccessTimes] -  ImageCenterPreviousFrameX[DetectSuccessTimes-1]);
			YSpeed = fabs(ImageCenterPreviousFrameY[DetectSuccessTimes] -  ImageCenterPreviousFrameY[DetectSuccessTimes-1]);
		}

		if(StartFollowFlag && CenterX >= 0 && CenterX <=RealTimeIntrinsiccx*2 && CenterY >= 0 && CenterY <= RealTimeIntrinsiccy*2)
		{
			FollowAreaLeft = Xmin - XPixelSize/IncreaseAreaD - XSpeed;
			FollowAreaRight = Xmax + XPixelSize/IncreaseAreaD + XSpeed;
			FollowAreaUp = Ymin - YPixelSize/IncreaseAreaD - YSpeed; 
			FollowAreaDown = Ymax + YPixelSize/IncreaseAreaD + YSpeed;
			if(FollowAreaLeft <= 0)
				FollowAreaLeft = 0;
			if(FollowAreaRight >= RealTimeIntrinsiccx*2)
				FollowAreaRight =  RealTimeIntrinsiccx*2;
			if(FollowAreaUp <= 0)
				FollowAreaUp = 0;
			if(FollowAreaDown >=  RealTimeIntrinsiccy*2)
				FollowAreaDown = RealTimeIntrinsiccy*2;
			FirstFrameOverFlag = 1;

		}

		if(ResultIntrinsicfx != 0)
		{
			double RealDistance = DrogueRalativeCameraDistanceY;
			double ImageDetectDistance = (double)(RealTimeFocusFx*0.82);
			double ImageDetectDistance1 = ImageDetectDistance/XPixelSize;
			ImageDetectDistance1 = fabs(ImageDetectDistance1) + 0.592;
			double Error = fabs(fabs(RealDistance)-fabs(ImageDetectDistance1));
			double NeedFocus = RealDistance*XPixelSize/0.82;
			double NeedDrougueSize = RealDistance*XPixelSize/RealTimeFocusFx;
			double PhysicSizeMatrix[3][3] = {0},ProbeMatrix[3][4] = {0},PixelMatrix[3] = {0};

			PhysicSizeMatrix[0][0] = 1.0;///0.00026455;//0.0254;
			PhysicSizeMatrix[1][1] = 1.0;//0.00026455;//0.0254;
			PhysicSizeMatrix[0][2] = RealTimeIntrinsiccx; //cx
			PhysicSizeMatrix[1][2] = RealTimeIntrinsiccy; //cy
			PhysicSizeMatrix[2][2] = 1.0;
			ProbeMatrix[0][0] = RealTimeFocusFx;//3780;
			ProbeMatrix[1][1] = RealTimeFocusFy;//3780;
			ProbeMatrix[2][2] = 1; // 内参
			PixelMatrix[0] = ImageCenterX*ImageDetectDistance1;
			PixelMatrix[1] = ImageCenterY*ImageDetectDistance1;
			PixelMatrix[2] = ImageDetectDistance1;
			CvMat PhysicSizeMat = cvMat(3,3,CV_64FC1,PhysicSizeMatrix);
			CvMat ProbeMat = cvMat(3,4,CV_64FC1,ProbeMatrix);
			CvMat PixelMat = cvMat(3,1,CV_64FC1,PixelMatrix);
			CvMat* DstMat =  cvCreateMat(3,4,CV_64FC1);
			CvMat* DstInvertMat =  cvCreateMat(4,3,CV_64FC1);
			CvMat* CameraMat =  cvCreateMat(4,1,CV_64FC1);
			cvGEMM(&PhysicSizeMat,&ProbeMat,1.0,NULL,0,DstMat);
			cvInvert(DstMat,DstInvertMat,CV_SVD);
			cvGEMM(DstInvertMat,&PixelMat,1.0,NULL,0,CameraMat);

			double RealXc = DrogueRalativeCameraDistanceX;
			double RealYc = DrogueRalativeCameraDistanceY;
			double RealZc = DrogueRalativeCameraDistanceZ;

			if(DrogueRalativeCameraDistanceY > 0)
			{
				double Xc = cvmGet(CameraMat,0,0);
				double Yc = cvmGet(CameraMat,2,0);
				double Zc = -cvmGet(CameraMat,1,0);

				ImageDroguePositionX = CameraTranformpos[0] + Xc;
				ImageDroguePositionY = CameraTranformpos[1] + Yc;
				ImageDroguePositionZ = CameraTranformpos[2] + Zc;
				DrogueLocateAccurateX = fabs(DroguePosition[0] - ImageDroguePositionX);
				DrogueLocateAccurateY = fabs(DroguePosition[1] - ImageDroguePositionY);
				DrogueLocateAccurateZ = fabs(DroguePosition[2] - ImageDroguePositionZ);
				DrogueLocateAccurateXArray[CurrentFrame] =DrogueLocateAccurateX;
				DrogueLocateAccurateYArray[CurrentFrame] =DrogueLocateAccurateY;
				DrogueLocateAccurateZArray[CurrentFrame] =DrogueLocateAccurateZ;
				DrogueLocateXArray[CurrentFrame] = DroguePosition[0];
				DrogueLocateYArray[CurrentFrame] = DroguePosition[1];
				DrogueLocateZArray[CurrentFrame] = DroguePosition[2];
				DrogueSolveXArray[CurrentFrame] = ImageDroguePositionX;
				DrogueSolveYArray[CurrentFrame] = ImageDroguePositionY;
				DrogueSolveZArray[CurrentFrame] = ImageDroguePositionZ;
				clock_t end = clock();
				FrameProcessTime[CurrentFrame] =  float((end - start)) / CLOCKS_PER_SEC;
				ResultAtitudeHeading = hudu2du*atan(-cvmGet(CameraMat,0,0)/cvmGet(CameraMat,2,0));
				ResultAtitudePitch = hudu2du*atan(-cvmGet(CameraMat,1,0)/cvmGet(CameraMat,2,0));
				ImageDetectHeading[CurrentFrame] = ResultAtitudeHeading;
				ImageDetectPitch[CurrentFrame] = ResultAtitudePitch;
				CurrentFrame++;
				

			}else
			{
				double Xc = -cvmGet(CameraMat,0,0);
				double Yc = -cvmGet(CameraMat,2,0);
				double Zc = -cvmGet(CameraMat,1,0);

				ImageDroguePositionX = CameraTranformpos[0] + Xc;
				ImageDroguePositionY = CameraTranformpos[1] + Yc;
				ImageDroguePositionZ = CameraTranformpos[2] + Zc;
				DrogueLocateAccurateX = fabs(DroguePosition[0] - ImageDroguePositionX);
				DrogueLocateAccurateY = fabs(DroguePosition[1] - ImageDroguePositionY);
				DrogueLocateAccurateZ = fabs(DroguePosition[2] - ImageDroguePositionZ);
				DrogueLocateAccurateXArray[CurrentFrame] =DrogueLocateAccurateX;
				DrogueLocateAccurateYArray[CurrentFrame] =DrogueLocateAccurateY;
				DrogueLocateAccurateZArray[CurrentFrame] =DrogueLocateAccurateZ;
				DrogueLocateXArray[CurrentFrame] = DroguePosition[0];
				DrogueLocateYArray[CurrentFrame] = DroguePosition[1];
				DrogueLocateZArray[CurrentFrame] = DroguePosition[2];
				DrogueSolveXArray[CurrentFrame] = ImageDroguePositionX;
				DrogueSolveYArray[CurrentFrame] = ImageDroguePositionY;
				DrogueSolveZArray[CurrentFrame] = ImageDroguePositionZ;
				clock_t end = clock();
				FrameProcessTime[CurrentFrame] =  float((end - start)) / CLOCKS_PER_SEC;
				ResultAtitudeHeading = hudu2du*atan(-cvmGet(CameraMat,0,0)/cvmGet(CameraMat,2,0));
				ResultAtitudePitch = hudu2du*atan(-cvmGet(CameraMat,1,0)/cvmGet(CameraMat,2,0));
				ImageDetectHeading[CurrentFrame] = ResultAtitudeHeading;
				ImageDetectPitch[CurrentFrame] = ResultAtitudePitch;
				CurrentFrame++;
			}
			DisplayDroguelocateFlag = 1;
			cvReleaseMat(&DstMat);
			cvReleaseMat(&DstInvertMat);
			cvReleaseMat(&CameraMat);
		}
		cvCircle(FollowImage,cvPoint(CenterX,CenterY),2,CV_RGB(255,255,255),-1,8);
		cvReleaseMat(&Dst);

	}
	//最小椭圆+等比例
	if(DrogueLocateSolveMethodFlag == 2)
	{
		
		if(StartFollowFlag)
		{
			cvRectangle(FollowImage,cvPoint(Xmin-IntFollowAreaLeft,Ymin-IntFollowAreaUp),
				cvPoint(Xmax-IntFollowAreaLeft,Ymax-IntFollowAreaUp),CV_RGB(0,255,0),2,8);
			CvSize size= cvSize(IntFollowAreaRight - IntFollowAreaLeft,IntFollowAreaDown - IntFollowAreaUp);//区域大小
			//cvSetImageROI(CurrentVisionImage,cvRect(IntFollowAreaLeft,IntFollowAreaUp,size.width, size.height));//设置源图像ROI
			cvCopy(FollowImage,CurrentVisionImage); //复制图像
			//cvResetImageROI(FollowImage);//源图像用完后，清空ROI
			cvResetImageROI(CurrentVisionImage);
			cvRectangle(CurrentVisionImage,cvPoint(IntFollowAreaLeft,IntFollowAreaUp),
				cvPoint(IntFollowAreaRight,IntFollowAreaDown),CV_RGB(0,0,0),2,8);
		}else
		{
			cvRectangle(FollowImage,cvPoint(Xmin,Ymin),cvPoint(Xmax,Ymax),CV_RGB(0,255,0),2,8);

		}

		//cv::rectangle(img,cv::Point2d(Xmin,Ymin),cv::Point2d(Xmax,Ymax),CV_RGB(255,255,255),1,8);

		double LeftMatrix[5][5] = {0},RightMatrix[5] = {0};
		for(int k=0;k<contour_size;k++)
		{
			LeftMatrix[0][0]+=pow(X[k],2)*pow(Y[k],2);
			LeftMatrix[0][1]+=X[k]*pow(Y[k],3);
			LeftMatrix[0][2]+=pow(X[k],2)*Y[k];
			LeftMatrix[0][3]+=X[k]*pow(Y[k],2);
			LeftMatrix[0][4]+=X[k]*Y[k];
			LeftMatrix[1][0]+=X[k]*pow(Y[k],3);
			LeftMatrix[1][1]+=pow(Y[k],4);
			LeftMatrix[1][2]+=X[k]*pow(Y[k],2);
			LeftMatrix[1][3]+=pow(Y[k],3);
			LeftMatrix[1][4]+=pow(Y[k],2);
			LeftMatrix[2][0]+=pow(X[k],2)*Y[k];
			LeftMatrix[2][1]+=X[k]*pow(Y[k],2);
			LeftMatrix[2][2]+=pow(X[k],2);
			LeftMatrix[2][3]+=X[k]*Y[k];
			LeftMatrix[2][4]+=X[k];
			LeftMatrix[3][0]+=X[k]*pow(Y[k],2);
			LeftMatrix[3][1]+=pow(Y[k],3);
			LeftMatrix[3][2]+=X[k]*Y[k];
			LeftMatrix[3][3]+=pow(Y[k],2);
			LeftMatrix[3][4]+=Y[k];
			LeftMatrix[4][0]+=X[k]*Y[k];
			LeftMatrix[4][1]+=pow(Y[k],2);
			LeftMatrix[4][2]+=X[k];
			LeftMatrix[4][3]+=Y[k];
			LeftMatrix[4][4]+=1;

			RightMatrix[0]-=pow(X[k],3)*Y[k];
			RightMatrix[1]-=pow(X[k],2)*pow(Y[k],2);
			RightMatrix[2]-=pow(X[k],3);
			RightMatrix[3]-=pow(X[k],2)*Y[k];
			RightMatrix[4]-=pow(X[k],2);

		}
		CvMat Leftmat = cvMat(5,5,CV_64FC1,LeftMatrix);
		CvMat Rightmat = cvMat(5,1,CV_64FC1,RightMatrix);
		CvMat* Dst =  cvCreateMat(5,1,CV_64FC1);
		int result =cvSolve(&Leftmat,&Rightmat,Dst,CV_LU);
		if(result != 1)
		{
			DetectfailureTimes++;
			hWnd->SendMessage(WM_MY_MESSAGE,0,0);
			cvReleaseImage(&h_R);
			cvReleaseImage(&img_temp);
			cvReleaseMat(&Dst);
			cvReleaseImage(&FollowImage);
			return;
		}
		assert(result == 1);
		double CenterX,CenterY;
		long double A = cvmGet(Dst,0,0);
		long double B = cvmGet(Dst,1,0);
		long double C = cvmGet(Dst,2,0);
		long double D = cvmGet(Dst,3,0);
		long double E = cvmGet(Dst,4,0);
		CenterX = double((2*B*C-A*D)/(pow(A,2)-4*B));
		CenterY = double((2*D-A*D)/(pow(A,2)-4*B));
		ImageCenterX = CenterX,ImageCenterY = CenterY;
		if(StartFollowFlag)
		{
			cv::circle(img,cv::Point2d(CenterX - IntFollowAreaLeft,CenterY - IntFollowAreaUp),2,CV_RGB(255,255,255),-1,8);
		}else
			cv::circle(img,cv::Point2d(CenterX,CenterY),2,CV_RGB(255,255,255),-1,8);
		

		double XPixelSize = Xmax-Xmin;
		double YPixelSize =Ymax-Ymin;
		double XSpeed = 0,YSpeed = 0;
		if(DetectSuccessTimes > 30000)
		{
			DetectSuccessTimes = 1;
		}
		ImageCenterPreviousFrameX[DetectSuccessTimes]=CenterX;
		ImageCenterPreviousFrameY[DetectSuccessTimes]=CenterY;
		if(DetectSuccessTimes > 2)
		{
			XSpeed = fabs(ImageCenterPreviousFrameX[DetectSuccessTimes] -  ImageCenterPreviousFrameX[DetectSuccessTimes-1]);
			YSpeed = fabs(ImageCenterPreviousFrameY[DetectSuccessTimes] -  ImageCenterPreviousFrameY[DetectSuccessTimes-1]);
		}

		if(StartFollowFlag && CenterX >= 0 && CenterX <=RealTimeIntrinsiccx*2 && CenterY >= 0 && CenterY <= RealTimeIntrinsiccy*2)
		{
			FollowAreaLeft = Xmin - XPixelSize/IncreaseAreaD - XSpeed;
			FollowAreaRight = Xmax + XPixelSize/IncreaseAreaD + XSpeed;
			FollowAreaUp = Ymin - YPixelSize/IncreaseAreaD - YSpeed; 
			FollowAreaDown = Ymax + YPixelSize/IncreaseAreaD + YSpeed;
			if(FollowAreaLeft <= 0)
				FollowAreaLeft = 0;
			if(FollowAreaRight >= RealTimeIntrinsiccx*2)
				FollowAreaRight =  RealTimeIntrinsiccx*2;
			if(FollowAreaUp <= 0)
				FollowAreaUp = 0;
			if(FollowAreaDown >=  RealTimeIntrinsiccy*2)
				FollowAreaDown = RealTimeIntrinsiccy*2;
			FirstFrameOverFlag = 1;

		}


		
		if(ResultIntrinsicfx != 0)
		{
			double RealDistance = DrogueRalativeCameraDistanceY;
			double ImageDetectDistance = (double)(RealTimeFocusFx*0.82);
			double ImageDetectDistanceZ = ImageDetectDistance/XPixelSize;
			ImageDetectDistanceZ = fabs(ImageDetectDistanceZ) + 0.592;

			double ImageDetectDistanceX = (CenterX-RealTimeIntrinsiccx)*0.82/XPixelSize;
			double ImageDetectDistanceY = (CenterY-RealTimeIntrinsiccy)*0.82/XPixelSize;

			double RealXc = DrogueRalativeCameraDistanceX;
			double RealYc = DrogueRalativeCameraDistanceY;
			double RealZc = DrogueRalativeCameraDistanceZ;

			if(DrogueRalativeCameraDistanceY > 0)
			{
				double Xc = ImageDetectDistanceX;
				double Yc = ImageDetectDistanceZ;
				double Zc = -ImageDetectDistanceY;
				
				ImageDroguePositionX = CameraTranformpos[0] + Xc;
				ImageDroguePositionY = CameraTranformpos[1] + Yc;
				ImageDroguePositionZ = CameraTranformpos[2] + Zc;
				DrogueLocateAccurateX = fabs(DroguePosition[0] - ImageDroguePositionX);
				DrogueLocateAccurateY = fabs(DroguePosition[1] - ImageDroguePositionY);
				DrogueLocateAccurateZ = fabs(DroguePosition[2] - ImageDroguePositionZ);
				DrogueLocateAccurateXArray[CurrentFrame] =DrogueLocateAccurateX;
				DrogueLocateAccurateYArray[CurrentFrame] =DrogueLocateAccurateY;
				DrogueLocateAccurateZArray[CurrentFrame] =DrogueLocateAccurateZ;
				DrogueLocateXArray[CurrentFrame] = DroguePosition[0];
				DrogueLocateYArray[CurrentFrame] = DroguePosition[1];
				DrogueLocateZArray[CurrentFrame] = DroguePosition[2];
				DrogueSolveXArray[CurrentFrame] = ImageDroguePositionX;
				DrogueSolveYArray[CurrentFrame] = ImageDroguePositionY;
				DrogueSolveZArray[CurrentFrame] = ImageDroguePositionZ;
				clock_t end = clock();
				FrameProcessTime[CurrentFrame] =  float((end - start)) / CLOCKS_PER_SEC;
				ResultAtitudeHeading = hudu2du*atan(-Xc/Yc);
				ResultAtitudePitch = hudu2du*atan(Zc/Yc);
				ImageDetectHeading[CurrentFrame] = ResultAtitudeHeading;
				ImageDetectPitch[CurrentFrame] = ResultAtitudePitch;
				CurrentFrame++;
				
			}else
			{
				double Xc = -ImageDetectDistanceX;
				double Yc = -ImageDetectDistanceZ;
				double Zc = -ImageDetectDistanceY;

				ImageDroguePositionX = CameraTranformpos[0] + Xc;
				ImageDroguePositionY = CameraTranformpos[1] + Yc;
				ImageDroguePositionZ = CameraTranformpos[2] + Zc;
				DrogueLocateAccurateX = fabs(DroguePosition[0] - ImageDroguePositionX);
				DrogueLocateAccurateY = fabs(DroguePosition[1] - ImageDroguePositionY);
				DrogueLocateAccurateZ = fabs(DroguePosition[2] - ImageDroguePositionZ);
				DrogueLocateAccurateXArray[CurrentFrame] =DrogueLocateAccurateX;
				DrogueLocateAccurateYArray[CurrentFrame] =DrogueLocateAccurateY;
				DrogueLocateAccurateZArray[CurrentFrame] =DrogueLocateAccurateZ;
				DrogueLocateXArray[CurrentFrame] = DroguePosition[0];
				DrogueLocateYArray[CurrentFrame] = DroguePosition[1];
				DrogueLocateZArray[CurrentFrame] = DroguePosition[2];
				DrogueSolveXArray[CurrentFrame] = ImageDroguePositionX;
				DrogueSolveYArray[CurrentFrame] = ImageDroguePositionY;
				DrogueSolveZArray[CurrentFrame] = ImageDroguePositionZ;
				clock_t end = clock();
				FrameProcessTime[CurrentFrame] =  float((end - start)) / CLOCKS_PER_SEC;
				ResultAtitudeHeading = hudu2du*atan(-Xc/Yc);
				ResultAtitudePitch = hudu2du*atan(-Zc/Yc);
				ImageDetectHeading[CurrentFrame] = ResultAtitudeHeading;
				ImageDetectPitch[CurrentFrame] = ResultAtitudePitch;
				CurrentFrame++;
			}
			DisplayDroguelocateFlag = 1;
		}

		cvCircle(CurrentVisionImage,cvPoint(CenterX,CenterY),2,CV_RGB(255,255,255),-1,8);
		cvReleaseMat(&Dst);

	}
	//LHM
	if(DrogueLocateSolveMethodFlag == 3)
	{

		if(StartFollowFlag)
		{
			cvRectangle(FollowImage,cvPoint(Xmin-IntFollowAreaLeft,Ymin-IntFollowAreaUp),
				cvPoint(Xmax-IntFollowAreaLeft,Ymax-IntFollowAreaUp),CV_RGB(0,255,0),2,8);
			CvSize size= cvSize(IntFollowAreaRight - IntFollowAreaLeft,IntFollowAreaDown - IntFollowAreaUp);//区域大小
			//cvSetImageROI(CurrentVisionImage,cvRect(IntFollowAreaLeft,IntFollowAreaUp,size.width, size.height));//设置源图像ROI
			cvCopy(FollowImage,CurrentVisionImage); //复制图像
			//cvResetImageROI(FollowImage);//源图像用完后，清空ROI
			cvResetImageROI(CurrentVisionImage);
			cvRectangle(CurrentVisionImage,cvPoint(IntFollowAreaLeft,IntFollowAreaUp),
				cvPoint(IntFollowAreaRight,IntFollowAreaDown),CV_RGB(0,0,0),2,8);
		}else
		{
			cvRectangle(FollowImage,cvPoint(Xmin,Ymin),cvPoint(Xmax,Ymax),CV_RGB(0,255,0),2,8);

		}

		//cv::rectangle(img,cv::Point2d(Xmin,Ymin),cv::Point2d(Xmax,Ymax),CV_RGB(255,255,255),1,8);

		double LeftMatrix[5][5] = {0},RightMatrix[5] = {0};
		for(int k=0;k<contour_size;k++)
		{
			LeftMatrix[0][0]+=pow(X[k],2)*pow(Y[k],2);
			LeftMatrix[0][1]+=X[k]*pow(Y[k],3);
			LeftMatrix[0][2]+=pow(X[k],2)*Y[k];
			LeftMatrix[0][3]+=X[k]*pow(Y[k],2);
			LeftMatrix[0][4]+=X[k]*Y[k];
			LeftMatrix[1][0]+=X[k]*pow(Y[k],3);
			LeftMatrix[1][1]+=pow(Y[k],4);
			LeftMatrix[1][2]+=X[k]*pow(Y[k],2);
			LeftMatrix[1][3]+=pow(Y[k],3);
			LeftMatrix[1][4]+=pow(Y[k],2);
			LeftMatrix[2][0]+=pow(X[k],2)*Y[k];
			LeftMatrix[2][1]+=X[k]*pow(Y[k],2);
			LeftMatrix[2][2]+=pow(X[k],2);
			LeftMatrix[2][3]+=X[k]*Y[k];
			LeftMatrix[2][4]+=X[k];
			LeftMatrix[3][0]+=X[k]*pow(Y[k],2);
			LeftMatrix[3][1]+=pow(Y[k],3);
			LeftMatrix[3][2]+=X[k]*Y[k];
			LeftMatrix[3][3]+=pow(Y[k],2);
			LeftMatrix[3][4]+=Y[k];
			LeftMatrix[4][0]+=X[k]*Y[k];
			LeftMatrix[4][1]+=pow(Y[k],2);
			LeftMatrix[4][2]+=X[k];
			LeftMatrix[4][3]+=Y[k];
			LeftMatrix[4][4]+=1;

			RightMatrix[0]-=pow(X[k],3)*Y[k];
			RightMatrix[1]-=pow(X[k],2)*pow(Y[k],2);
			RightMatrix[2]-=pow(X[k],3);
			RightMatrix[3]-=pow(X[k],2)*Y[k];
			RightMatrix[4]-=pow(X[k],2);

		}
		CvMat Leftmat = cvMat(5,5,CV_64FC1,LeftMatrix);
		CvMat Rightmat = cvMat(5,1,CV_64FC1,RightMatrix);
		CvMat* Dst =  cvCreateMat(5,1,CV_64FC1);
		int result =cvSolve(&Leftmat,&Rightmat,Dst,CV_LU);
		if(result != 1)
		{
			DetectfailureTimes++;
			hWnd->SendMessage(WM_MY_MESSAGE,0,0);
			cvReleaseImage(&h_R);
			cvReleaseImage(&img_temp);
			cvReleaseMat(&Dst);
			cvReleaseImage(&FollowImage);
			return;
		}
		assert(result == 1);
		double CenterX,CenterY;
		long double A = cvmGet(Dst,0,0);
		long double B = cvmGet(Dst,1,0);
		long double C = cvmGet(Dst,2,0);
		long double D = cvmGet(Dst,3,0);
		long double E = cvmGet(Dst,4,0);
		CenterX = double((2*B*C-A*D)/(pow(A,2)-4*B));
		CenterY = double((2*D-A*D)/(pow(A,2)-4*B));
		ImageCenterX = CenterX,ImageCenterY = CenterY;
		if(StartFollowFlag)
		{
			cv::circle(img,cv::Point2d(CenterX - IntFollowAreaLeft,CenterY - IntFollowAreaUp),2,CV_RGB(255,255,255),-1,8);
		}else
			cv::circle(img,cv::Point2d(CenterX,CenterY),2,CV_RGB(255,255,255),-1,8);


		double XPixelSize = Xmax-Xmin;
		double YPixelSize =Ymax-Ymin;
		double XSpeed = 0,YSpeed = 0;
		if(DetectSuccessTimes > 30000)
		{
			DetectSuccessTimes = 1;
		}
		ImageCenterPreviousFrameX[DetectSuccessTimes]=CenterX;
		ImageCenterPreviousFrameY[DetectSuccessTimes]=CenterY;
		if(DetectSuccessTimes > 2)
		{
			XSpeed = fabs(ImageCenterPreviousFrameX[DetectSuccessTimes] -  ImageCenterPreviousFrameX[DetectSuccessTimes-1]);
			YSpeed = fabs(ImageCenterPreviousFrameY[DetectSuccessTimes] -  ImageCenterPreviousFrameY[DetectSuccessTimes-1]);
		}

		if(StartFollowFlag && CenterX >= 0 && CenterX <=RealTimeIntrinsiccx*2 && CenterY >= 0 && CenterY <= RealTimeIntrinsiccy*2)
		{
			FollowAreaLeft = Xmin - XPixelSize/IncreaseAreaD - XSpeed;
			FollowAreaRight = Xmax + XPixelSize/IncreaseAreaD + XSpeed;
			FollowAreaUp = Ymin - YPixelSize/IncreaseAreaD - YSpeed; 
			FollowAreaDown = Ymax + YPixelSize/IncreaseAreaD + YSpeed;
			if(FollowAreaLeft <= 0)
				FollowAreaLeft = 0;
			if(FollowAreaRight >= RealTimeIntrinsiccx*2)
				FollowAreaRight =  RealTimeIntrinsiccx*2;
			if(FollowAreaUp <= 0)
				FollowAreaUp = 0;
			if(FollowAreaDown >=  RealTimeIntrinsiccy*2)
				FollowAreaDown = RealTimeIntrinsiccy*2;
			FirstFrameOverFlag = 1;

		}



		if(ResultIntrinsicfx != 0)
		{
			CvMat* Translation = cvCreateMat(3,1,CV_64FC1);
			double FinalXsort[12],FinalYsort[12];
			if(contour_size == 12)
			{
				ImagePointSortLeft(X,Y,FinalXsort,FinalYsort);
				for(int i=0;i<12;i++)
				{
					Drogue2DImagepotion[i][0] = FinalXsort[i];
					Drogue2DImagepotion[i][1] = FinalYsort[i];
					Drogue2DImagepotion[i][2] = 1;
				}
				CvMat Drogue2DImagepotionMat = cvMat(12,3,CV_64FC1,Drogue2DImagepotion);
				CvMat Drogue3DpotionMat = cvMat(12,3,CV_64FC1,Drogue3Dpotion);
				double InitRotate[3][3],theta = 0,fai = 0,gunzhuanjiao = 0;
				InitRotate[0][0] = cos(gunzhuanjiao)*cos(theta);
				InitRotate[1][0] = sin(gunzhuanjiao)*cos(theta);
				InitRotate[2][0] = -sin(theta);
				InitRotate[0][1] = cos(gunzhuanjiao)*sin(theta)*sin(fai)-sin(fai);
				InitRotate[1][1] = sin(gunzhuanjiao)*sin(theta) + cos(fai)*cos(gunzhuanjiao);
				InitRotate[2][1] = cos(fai)*sin(fai);
				InitRotate[0][2] = cos(gunzhuanjiao)*sin(theta) - sin(fai)*sin(gunzhuanjiao);
				InitRotate[1][2] = sin(gunzhuanjiao)*sin(theta)*cos(fai)+cos(gunzhuanjiao)*sin(fai);
				InitRotate[2][2] = cos(theta)*cos(fai);
				CvMat InitRotateMat = cvMat(3,3,CV_64FC1,InitRotate);
				Translation = SolveAtitude_LHM(Drogue3DpotionMat,Drogue2DImagepotionMat,InitRotateMat);
			}else{
				hWnd->SendMessage(WM_MY_MESSAGE,0,0);
				cvReleaseImage(&h_R);
				cvReleaseImage(&img_temp);
				cvReleaseMat(&Dst);
				cvReleaseImage(&FollowImage);
				cvReleaseMat(&Translation);
				return;
			}

			double RealXc = DrogueRalativeCameraDistanceX;
			double RealYc = DrogueRalativeCameraDistanceY;
			double RealZc = DrogueRalativeCameraDistanceZ;

			if(DrogueRalativeCameraDistanceY > 0)
			{
				double Xc = cvmGet(Translation,0,0);
				double Yc = cvmGet(Translation,2,0);
				double Zc = -cvmGet(Translation,1,0);

				ImageDroguePositionX = CameraTranformpos[0] + Xc;
				ImageDroguePositionY = CameraTranformpos[1] + Yc;
				ImageDroguePositionZ = CameraTranformpos[2] + Zc;
				DrogueLocateAccurateX = fabs(DroguePosition[0] - ImageDroguePositionX);
				DrogueLocateAccurateY = fabs(DroguePosition[1] - ImageDroguePositionY);
				DrogueLocateAccurateZ = fabs(DroguePosition[2] - ImageDroguePositionZ);
				DrogueLocateAccurateXArray[CurrentFrame] =DrogueLocateAccurateX;
				DrogueLocateAccurateYArray[CurrentFrame] =DrogueLocateAccurateY;
				DrogueLocateAccurateZArray[CurrentFrame] =DrogueLocateAccurateZ;
				DrogueLocateXArray[CurrentFrame] = DroguePosition[0];
				DrogueLocateYArray[CurrentFrame] = DroguePosition[1];
				DrogueLocateZArray[CurrentFrame] = DroguePosition[2];
				DrogueSolveXArray[CurrentFrame] = ImageDroguePositionX;
				DrogueSolveYArray[CurrentFrame] = ImageDroguePositionY;
				DrogueSolveZArray[CurrentFrame] = ImageDroguePositionZ;
				clock_t end = clock();
				FrameProcessTime[CurrentFrame] =  float((end - start)) / CLOCKS_PER_SEC;
				ResultAtitudeHeading = hudu2du*atan(-Xc/Yc);
				ResultAtitudePitch = hudu2du*atan(Zc/Yc);
				ImageDetectHeading[CurrentFrame] = ResultAtitudeHeading;
				ImageDetectPitch[CurrentFrame] = ResultAtitudePitch;
				CurrentFrame++;

			}else
			{
				double Xc = -cvmGet(Translation,0,0);
				double Yc = -cvmGet(Translation,2,0);
				double Zc = -cvmGet(Translation,1,0);

				ImageDroguePositionX = CameraTranformpos[0] + Xc;
				ImageDroguePositionY = CameraTranformpos[1] + Yc;
				ImageDroguePositionZ = CameraTranformpos[2] + Zc;
				DrogueLocateAccurateX = fabs(DroguePosition[0] - ImageDroguePositionX);
				DrogueLocateAccurateY = fabs(DroguePosition[1] - ImageDroguePositionY);
				DrogueLocateAccurateZ = fabs(DroguePosition[2] - ImageDroguePositionZ);
				DrogueLocateXArray[CurrentFrame] = DroguePosition[0];
				DrogueLocateYArray[CurrentFrame] = DroguePosition[1];
				DrogueLocateZArray[CurrentFrame] = DroguePosition[2];
				DrogueSolveXArray[CurrentFrame] = ImageDroguePositionX;
				DrogueSolveYArray[CurrentFrame] = ImageDroguePositionY;
				DrogueSolveZArray[CurrentFrame] = ImageDroguePositionZ;
				DrogueLocateAccurateXArray[CurrentFrame] =DrogueLocateAccurateX;
				DrogueLocateAccurateYArray[CurrentFrame] =DrogueLocateAccurateY;
				DrogueLocateAccurateZArray[CurrentFrame] =DrogueLocateAccurateZ;
				clock_t end = clock();
				FrameProcessTime[CurrentFrame] =  float((end - start)) / CLOCKS_PER_SEC;
				ResultAtitudeHeading = hudu2du*atan(-Xc/Yc);
				ResultAtitudePitch = hudu2du*atan(-Zc/Yc);
				ImageDetectHeading[CurrentFrame] = ResultAtitudeHeading;
				ImageDetectPitch[CurrentFrame] = ResultAtitudePitch;
				CurrentFrame++;
			}
			DisplayDroguelocateFlag = 1;
			cvReleaseMat(&Translation);
		}

		cvCircle(CurrentVisionImage,cvPoint(CenterX,CenterY),2,CV_RGB(255,255,255),-1,8);
		cvReleaseMat(&Dst);

	}
	//EPNP
	if(DrogueLocateSolveMethodFlag == 4)
	{

		if(StartFollowFlag)
		{
			cvRectangle(FollowImage,cvPoint(Xmin-IntFollowAreaLeft,Ymin-IntFollowAreaUp),
				cvPoint(Xmax-IntFollowAreaLeft,Ymax-IntFollowAreaUp),CV_RGB(0,255,0),2,8);
			CvSize size= cvSize(IntFollowAreaRight - IntFollowAreaLeft,IntFollowAreaDown - IntFollowAreaUp);//区域大小
			//cvSetImageROI(CurrentVisionImage,cvRect(IntFollowAreaLeft,IntFollowAreaUp,size.width, size.height));//设置源图像ROI
			cvCopy(FollowImage,CurrentVisionImage); //复制图像
			//cvResetImageROI(FollowImage);//源图像用完后，清空ROI
			cvResetImageROI(CurrentVisionImage);
			cvRectangle(CurrentVisionImage,cvPoint(IntFollowAreaLeft,IntFollowAreaUp),
				cvPoint(IntFollowAreaRight,IntFollowAreaDown),CV_RGB(0,0,0),2,8);
		}else
		{
			cvRectangle(FollowImage,cvPoint(Xmin,Ymin),cvPoint(Xmax,Ymax),CV_RGB(0,255,0),2,8);

		}

		//cv::rectangle(img,cv::Point2d(Xmin,Ymin),cv::Point2d(Xmax,Ymax),CV_RGB(255,255,255),1,8);

		double LeftMatrix[5][5] = {0},RightMatrix[5] = {0};
		for(int k=0;k<contour_size;k++)
		{
			LeftMatrix[0][0]+=pow(X[k],2)*pow(Y[k],2);
			LeftMatrix[0][1]+=X[k]*pow(Y[k],3);
			LeftMatrix[0][2]+=pow(X[k],2)*Y[k];
			LeftMatrix[0][3]+=X[k]*pow(Y[k],2);
			LeftMatrix[0][4]+=X[k]*Y[k];
			LeftMatrix[1][0]+=X[k]*pow(Y[k],3);
			LeftMatrix[1][1]+=pow(Y[k],4);
			LeftMatrix[1][2]+=X[k]*pow(Y[k],2);
			LeftMatrix[1][3]+=pow(Y[k],3);
			LeftMatrix[1][4]+=pow(Y[k],2);
			LeftMatrix[2][0]+=pow(X[k],2)*Y[k];
			LeftMatrix[2][1]+=X[k]*pow(Y[k],2);
			LeftMatrix[2][2]+=pow(X[k],2);
			LeftMatrix[2][3]+=X[k]*Y[k];
			LeftMatrix[2][4]+=X[k];
			LeftMatrix[3][0]+=X[k]*pow(Y[k],2);
			LeftMatrix[3][1]+=pow(Y[k],3);
			LeftMatrix[3][2]+=X[k]*Y[k];
			LeftMatrix[3][3]+=pow(Y[k],2);
			LeftMatrix[3][4]+=Y[k];
			LeftMatrix[4][0]+=X[k]*Y[k];
			LeftMatrix[4][1]+=pow(Y[k],2);
			LeftMatrix[4][2]+=X[k];
			LeftMatrix[4][3]+=Y[k];
			LeftMatrix[4][4]+=1;

			RightMatrix[0]-=pow(X[k],3)*Y[k];
			RightMatrix[1]-=pow(X[k],2)*pow(Y[k],2);
			RightMatrix[2]-=pow(X[k],3);
			RightMatrix[3]-=pow(X[k],2)*Y[k];
			RightMatrix[4]-=pow(X[k],2);

		}
		CvMat Leftmat = cvMat(5,5,CV_64FC1,LeftMatrix);
		CvMat Rightmat = cvMat(5,1,CV_64FC1,RightMatrix);
		CvMat* Dst =  cvCreateMat(5,1,CV_64FC1);
		int result =cvSolve(&Leftmat,&Rightmat,Dst,CV_LU);
		if(result != 1)
		{
			DetectfailureTimes++;
			hWnd->SendMessage(WM_MY_MESSAGE,0,0);
			cvReleaseImage(&h_R);
			cvReleaseImage(&img_temp);
			cvReleaseMat(&Dst);
			cvReleaseImage(&FollowImage);
			return;
		}
		assert(result == 1);
		double CenterX,CenterY;
		long double A = cvmGet(Dst,0,0);
		long double B = cvmGet(Dst,1,0);
		long double C = cvmGet(Dst,2,0);
		long double D = cvmGet(Dst,3,0);
		long double E = cvmGet(Dst,4,0);
		CenterX = double((2*B*C-A*D)/(pow(A,2)-4*B));
		CenterY = double((2*D-A*D)/(pow(A,2)-4*B));
		ImageCenterX = CenterX,ImageCenterY = CenterY;
		if(StartFollowFlag)
		{
			cv::circle(img,cv::Point2d(CenterX - IntFollowAreaLeft,CenterY - IntFollowAreaUp),2,CV_RGB(255,255,255),-1,8);
		}else
			cv::circle(img,cv::Point2d(CenterX,CenterY),2,CV_RGB(255,255,255),-1,8);


		double XPixelSize = Xmax-Xmin;
		double YPixelSize =Ymax-Ymin;
		double XSpeed = 0,YSpeed = 0;
		if(DetectSuccessTimes > 30000)
		{
			DetectSuccessTimes = 1;
		}
		ImageCenterPreviousFrameX[DetectSuccessTimes]=CenterX;
		ImageCenterPreviousFrameY[DetectSuccessTimes]=CenterY;
		if(DetectSuccessTimes > 2)
		{
			XSpeed = fabs(ImageCenterPreviousFrameX[DetectSuccessTimes] -  ImageCenterPreviousFrameX[DetectSuccessTimes-1]);
			YSpeed = fabs(ImageCenterPreviousFrameY[DetectSuccessTimes] -  ImageCenterPreviousFrameY[DetectSuccessTimes-1]);
		}

		if(StartFollowFlag && CenterX >= 0 && CenterX <=RealTimeIntrinsiccx*2 && CenterY >= 0 && CenterY <= RealTimeIntrinsiccy*2)
		{
			FollowAreaLeft = Xmin - XPixelSize/IncreaseAreaD - XSpeed;
			FollowAreaRight = Xmax + XPixelSize/IncreaseAreaD + XSpeed;
			FollowAreaUp = Ymin - YPixelSize/IncreaseAreaD - YSpeed; 
			FollowAreaDown = Ymax + YPixelSize/IncreaseAreaD + YSpeed;
			if(FollowAreaLeft <= 0)
				FollowAreaLeft = 0;
			if(FollowAreaRight >= RealTimeIntrinsiccx*2)
				FollowAreaRight =  RealTimeIntrinsiccx*2;
			if(FollowAreaUp <= 0)
				FollowAreaUp = 0;
			if(FollowAreaDown >=  RealTimeIntrinsiccy*2)
				FollowAreaDown = RealTimeIntrinsiccy*2;
			FirstFrameOverFlag = 1;

		}

		if(ResultIntrinsicfx != 0)
		{

			double FinalXsort[12],FinalYsort[12],Drogue2DImagepotionarray[12][2];
			double R_est[3][3], t_est[3];
			if(contour_size == 12)
			{
				ImagePointSortLeft(X,Y,FinalXsort,FinalYsort);
				for(int i=0;i<12;i++)
				{
					Drogue2DImagepotion[i][0] = FinalXsort[i];
					Drogue2DImagepotion[i][1] = FinalYsort[i];
					Drogue2DImagepotionarray[i][0] = FinalXsort[i];
					Drogue2DImagepotionarray[i][1] = FinalYsort[i];
					Drogue2DImagepotion[i][2] = 1;
				}	
				PnP.set_internal_parameters(ResultIntrinsiccx, ResultIntrinsiccy, ResultIntrinsicfx, ResultIntrinsicfy);
				PnP.set_maximum_number_of_correspondences(12);
				PnP.reset_correspondences();
				for(int i=0;i<12;i++)
				{
					PnP.add_correspondence(Drogue3Dpotion[i][0],
						Drogue3Dpotion[i][1],Drogue3Dpotion[i][2],Drogue2DImagepotionarray[i][0],
						Drogue2DImagepotionarray[i][1]);

				}
				
				double err2 = PnP.compute_pose(R_est, t_est);				
			}else
			{
				{
					hWnd->SendMessage(WM_MY_MESSAGE,0,0);
					cvReleaseImage(&h_R);
					cvReleaseImage(&img_temp);
					cvReleaseMat(&Dst);
					cvReleaseImage(&FollowImage);
					return;
				}
			}

			double RealXc = DrogueRalativeCameraDistanceX;
			double RealYc = DrogueRalativeCameraDistanceY;
			double RealZc = DrogueRalativeCameraDistanceZ;

			if(DrogueRalativeCameraDistanceY > 0)
			{
				double Xc = t_est[0];
				double Yc = t_est[2]-0.23;
				double Zc = -t_est[1];

				ImageDroguePositionX = CameraTranformpos[0] + Xc;
				ImageDroguePositionY = CameraTranformpos[1] + Yc;
				ImageDroguePositionZ = CameraTranformpos[2] + Zc;
				DrogueLocateAccurateX = fabs(DroguePosition[0] - ImageDroguePositionX);
				DrogueLocateAccurateY = fabs(DroguePosition[1] - ImageDroguePositionY);
				DrogueLocateAccurateZ = fabs(DroguePosition[2] - ImageDroguePositionZ);
				DrogueLocateAccurateXArray[CurrentFrame] =DrogueLocateAccurateX;
				DrogueLocateAccurateYArray[CurrentFrame] =DrogueLocateAccurateY;
				DrogueLocateAccurateZArray[CurrentFrame] =DrogueLocateAccurateZ;
				DrogueLocateXArray[CurrentFrame] = DroguePosition[0];
				DrogueLocateYArray[CurrentFrame] = DroguePosition[1];
				DrogueLocateZArray[CurrentFrame] = DroguePosition[2];
				DrogueSolveXArray[CurrentFrame] = ImageDroguePositionX;
				DrogueSolveYArray[CurrentFrame] = ImageDroguePositionY;
				DrogueSolveZArray[CurrentFrame] = ImageDroguePositionZ;
				clock_t end = clock();
				FrameProcessTime[CurrentFrame] =  float((end - start)) / CLOCKS_PER_SEC;
				ResultAtitudeHeading = hudu2du*atan(-Xc/Yc);
				ResultAtitudePitch = hudu2du*atan(Zc/Yc);
				ImageDetectHeading[CurrentFrame] = ResultAtitudeHeading;
				ImageDetectPitch[CurrentFrame] = ResultAtitudePitch;
				CurrentFrame++;

			}else
			{
				double Xc = -t_est[0];
				double Yc = -t_est[2]+0.23;
				double Zc = -t_est[1];

				ImageDroguePositionX = CameraTranformpos[0] + Xc;
				ImageDroguePositionY = CameraTranformpos[1] + Yc;
				ImageDroguePositionZ = CameraTranformpos[2] + Zc;
				DrogueLocateAccurateX = fabs(DroguePosition[0] - ImageDroguePositionX);
				DrogueLocateAccurateY = fabs(DroguePosition[1] - ImageDroguePositionY);
				DrogueLocateAccurateZ = fabs(DroguePosition[2] - ImageDroguePositionZ);
				DrogueLocateAccurateXArray[CurrentFrame] =DrogueLocateAccurateX;
				DrogueLocateAccurateYArray[CurrentFrame] =DrogueLocateAccurateY;
				DrogueLocateAccurateZArray[CurrentFrame] =DrogueLocateAccurateZ;
				DrogueLocateXArray[CurrentFrame] = DroguePosition[0];
				DrogueLocateYArray[CurrentFrame] = DroguePosition[1];
				DrogueLocateZArray[CurrentFrame] = DroguePosition[2];
				DrogueSolveXArray[CurrentFrame] = ImageDroguePositionX;
				DrogueSolveYArray[CurrentFrame] = ImageDroguePositionY;
				DrogueSolveZArray[CurrentFrame] = ImageDroguePositionZ;
				clock_t end = clock();
				FrameProcessTime[CurrentFrame] =  float((end - start)) / CLOCKS_PER_SEC;
				ResultAtitudeHeading = hudu2du*atan(-Xc/Yc);
				ResultAtitudePitch = hudu2du*atan(-Zc/Yc);
				ImageDetectHeading[CurrentFrame] = ResultAtitudeHeading;
				ImageDetectPitch[CurrentFrame] = ResultAtitudePitch;
				CurrentFrame++;
			}
			DisplayDroguelocateFlag = 1;
		}

		cvCircle(CurrentVisionImage,cvPoint(CenterX,CenterY),2,CV_RGB(255,255,255),-1,8);
		cvReleaseMat(&Dst);

	}

	if(Droguelocatedisplay)
	{
		if(Droguelocatedynamicdisplay)
		{
			cvShowImage("锥套定位",img_temp);
		}
		if(!Detect_Drogue_Image_Flag)
			cvDestroyWindow("锥套定位");
	}
	else
	{
		cvDestroyWindow("锥套定位");
	}

	if(Droguefollowdisplay)
	{
		if(Droguefollowdynamicdisplay)
		{
			if(StartFollowFlag)
			{
				cvShowImage("锥套跟踪",CurrentVisionImage);
			}else
			{
				cvShowImage("锥套跟踪",FollowImage);

			}
			
		}
		if(!Detect_Drogue_Image_Flag)
			cvDestroyWindow("锥套跟踪");

	}
	else
	{
		cvDestroyWindow("锥套跟踪");
	}


// 此段代码将锥套跟踪图片进行存储，方便画图使用
// 	CString strSaveImagenumber;
// 	strSaveImagenumber.Format("%04d.jpg",SaveImagenumber++);
// 	if(StartFollowFlag)
// 	{
// 		cvSaveImage("C:\\Users\\Administrator\\Desktop\\AAAAA\\graduate\\VisionSystemQin\\VisionSystemQin\\SaveImage\\"+strSaveImagenumber,CurrentVisionImage);
// 	}else
// 	{
// 		cvSaveImage("C:\\Users\\Administrator\\Desktop\\AAAAA\\graduate\\VisionSystemQin\\VisionSystemQin\\SaveImage\\"+strSaveImagenumber,FollowImage);
// 	}


	cvWaitKey(1);	
	cvReleaseImage(&h_R);
	cvReleaseImage(&img_temp);
	cvReleaseImage(&FollowImage);

	if(Droguefollowdisplay)
		if(!Detect_Drogue_Image_Flag)
			cvDestroyWindow("锥套跟踪");
	if(Droguedetectdisplay)
		if(!Detect_Drogue_Image_Flag)
			cvDestroyWindow("锥套检测");
	if(Droguelocatedisplay)
		if(!Detect_Drogue_Image_Flag)
			cvDestroyWindow("锥套定位");
	hWnd->SendMessage(WM_MY_MESSAGE,0,0);

	fprintf(stderr, "%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n", DroguePosition[0], DroguePosition[1], DroguePosition[2], ResultAtitudeHeading, ResultAtitudePitch, CameraTranformpos[0],CameraTranformpos[1], CameraTranformpos[2]);
	fflush(stderr);
}


void CImageProcess::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	ExitCalibrationDiaglog = 0;
	MaskControl();
	CCalibrateCamera CalibrateCamera;
	CalibrateCamera.DoModal();
	GetDlgItem(IDC_BUTTON_ImageStartDrogueDetect)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_GuassNoise)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_UniformNoise)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SaltNoise)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLIDER_GuassNoise)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLIDER_ContrastAdjust)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLIDER_BrightAdjust)->EnableWindow(TRUE);

	GetDlgItem(IDC_COMBO_DetectMethod)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_FilteringMethod)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_DrogueLocateSolveMethod)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_Dilate)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_Erode)->EnableWindow(TRUE);
	ExitCalibrationDiaglog = 1;   
	UpdateData(TRUE);
	CString str;
	str.Format("%.3f",ResultIntrinsicfx);
	GetDlgItem(IDC_EDIT_ImageIntrinsicfx)->SetWindowText(str);
	str.Format("%.3f",ResultIntrinsicfy);
	GetDlgItem(IDC_EDIT_ImageIntrinsicfy)->SetWindowText(str);
	RealTimeFocusFx = ResultIntrinsicfx;
	RealTimeIntrinsiccx = ResultIntrinsiccx;
	RealTimeIntrinsiccy = ResultIntrinsiccy;
	RealTimeFocusFy = ResultIntrinsicfy;

}

//开始检测
void CImageProcess::OnBnClickedButtonImagestartdroguedetect()
{
	CString str;
	GetDlgItem(IDC_BUTTON_ImageStartDrogueDetect)->GetWindowText(str);
	if(str == "开始检测")
	{
		Detect_Drogue_Image_Flag = true;
		RecoverControl();
		GetDlgItem(IDC_BUTTON_ImageStartDrogueDetect)->SetWindowText("暂停检测");
		//DetectfailureTimes = 0,DetectTotalTimes = 0;
		CString str_DetectfailureTimes,str_DetectTotalTimes;
		str_DetectfailureTimes.Format("%d",DetectfailureTimes);	
		str_DetectTotalTimes.Format("%d",DetectTotalTimes);
		GetDlgItem(IDC_EDIT_DetectTotalTimes)->SetWindowText(str_DetectTotalTimes);
		GetDlgItem(IDC_EDIT_DetectFailureTimes)->SetWindowText(str_DetectfailureTimes);
		GetDlgItem(IDC_BUTTON_StartFollow)->EnableWindow(TRUE);
		SaveImagenumber = 1;

	}else
	{
		Detect_Drogue_Image_Flag = false;
		MaskControl();
		GetDlgItem(IDC_BUTTON_ImageStartDrogueDetect)->SetWindowText("开始检测");
		GetDlgItem(IDC_BUTTON_ImageStartDrogueDetect)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_GuassNoise)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_UniformNoise)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SaltNoise)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_GuassNoise)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_ContrastAdjust)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_BrightAdjust)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_DetectMethod)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_FilteringMethod)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_DrogueLocateSolveMethod)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_Dilate)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Erode)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_StartFollow)->EnableWindow(FALSE);
		DisplayDroguelocateFlag = 0;
		
	}

	// TODO: Add your control notification handler code here
}

//检测结果显示
void CImageProcess::OnBnClickedButtonDroguedetectdisplay()
{
	CString str;
	GetDlgItem(IDC_BUTTON_DrogueDetectDisplay)->GetWindowText(str);

	if(str == "ON")
	{
		Droguedetectdisplay = true;
		GetDlgItem(IDC_BUTTON_DrogueDetectDisplay)->SetWindowText("OFF");
		GetDlgItem(IDC_BUTTON_DrogueDetectDynamicDisplay)->EnableWindow(TRUE);

	}else
	{
		Droguedetectdisplay = false;
		GetDlgItem(IDC_BUTTON_DrogueDetectDisplay)->SetWindowText("ON");
		GetDlgItem(IDC_BUTTON_DrogueDetectDynamicDisplay)->EnableWindow(FALSE);
	}
	
	// TODO: Add your control notification handler code here
}

void CImageProcess::OnBnClickedButtonDroguedetectdynamicdisplay()
{
	CString str;
	GetDlgItem(IDC_BUTTON_DrogueDetectDynamicDisplay)->GetWindowText(str);
	if(str == "切换至动态显示")
	{
		Droguedetectdynamicdisplay = true;
		GetDlgItem(IDC_BUTTON_DrogueDetectDynamicDisplay)->SetWindowText("切换至静态显示");

	}else
	{
		Droguedetectdynamicdisplay = false;
		GetDlgItem(IDC_BUTTON_DrogueDetectDynamicDisplay)->SetWindowText("切换至动态显示");
	}
	// TODO: Add your control notification handler code here
}

//定位结果显示
void CImageProcess::OnBnClickedButtonDroguelocatedisplay()
{
	CString str;
	GetDlgItem(IDC_BUTTON_DrogueLocateDisplay)->GetWindowText(str);

	if(str == "ON")
	{
		Droguelocatedisplay = true;
		GetDlgItem(IDC_BUTTON_DrogueLocateDisplay)->SetWindowText("OFF");
		GetDlgItem(IDC_BUTTON_DrogueLocateDynamicDisplay)->EnableWindow(TRUE);

	}else
	{
		Droguelocatedisplay = false;
		GetDlgItem(IDC_BUTTON_DrogueLocateDisplay)->SetWindowText("ON");
		GetDlgItem(IDC_BUTTON_DrogueLocateDynamicDisplay)->EnableWindow(FALSE);
	}
	// TODO: Add your control notification handler code here
}

void CImageProcess::OnBnClickedButtonDroguelocatedynamicdisplay()
{
	CString str;
	GetDlgItem(IDC_BUTTON_DrogueLocateDynamicDisplay)->GetWindowText(str);
	if(str == "切换至动态显示")
	{
		Droguelocatedynamicdisplay = true;
		GetDlgItem(IDC_BUTTON_DrogueLocateDynamicDisplay)->SetWindowText("切换至静态显示");

	}else
	{
		Droguelocatedynamicdisplay = false;
		GetDlgItem(IDC_BUTTON_DrogueLocateDynamicDisplay)->SetWindowText("切换至动态显示");
	}
	// TODO: Add your control notification handler code here
}

//跟踪结果显示
void CImageProcess::OnBnClickedButtonDroguefollowdisplay()
{
	CString str;
	GetDlgItem(IDC_BUTTON_DrogueFollowDisplay)->GetWindowText(str);

	if(str == "ON")
	{
		Droguefollowdisplay = true;
		GetDlgItem(IDC_BUTTON_DrogueFollowDisplay)->SetWindowText("OFF");
		GetDlgItem(IDC_BUTTON_DrogueFollowDynamicDisplay)->EnableWindow(TRUE);

	}else
	{
		Droguefollowdisplay = false;
		GetDlgItem(IDC_BUTTON_DrogueFollowDisplay)->SetWindowText("ON");
		GetDlgItem(IDC_BUTTON_DrogueFollowDynamicDisplay)->EnableWindow(FALSE);
	}
	// TODO: Add your control notification handler code here
}

void CImageProcess::OnBnClickedButtonDroguefollowdynamicdisplay()
{
	CString str;
	GetDlgItem(IDC_BUTTON_DrogueFollowDynamicDisplay)->GetWindowText(str);
	if(str == "切换至动态显示")
	{
		Droguefollowdynamicdisplay = true;
		GetDlgItem(IDC_BUTTON_DrogueFollowDynamicDisplay)->SetWindowText("切换至静态显示");

	}else
	{
		Droguefollowdynamicdisplay = false;
		GetDlgItem(IDC_BUTTON_DrogueFollowDynamicDisplay)->SetWindowText("切换至动态显示");
	}
	// TODO: Add your control notification handler code here
}

void CImageProcess::MaskControl(void)
{
	GetDlgItem(IDC_BUTTON_DrogueDetectDisplay)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DrogueDetectDynamicDisplay)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DrogueLocateDisplay)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DrogueLocateDynamicDisplay)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DrogueFollowDisplay)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DrogueFollowDynamicDisplay)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ImageStartDrogueDetect)->EnableWindow(FALSE);

	GetDlgItem(IDC_RADIO_GuassNoise)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_UniformNoise)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_SaltNoise)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_GuassNoise)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_ContrastAdjust)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_BrightAdjust)->EnableWindow(FALSE);

	GetDlgItem(IDC_COMBO_DetectMethod)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_FilteringMethod)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_DrogueLocateSolveMethod)->EnableWindow(FALSE);
	//m_Combo_DetectMethod.EnableWindow(FALSE);
	//m_Combo_FilteringMethod.EnableWindow(FALSE);
	//m_Combo_DrogueLocateSolveMethod.EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_DetectTotalTimes)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DetectFailureTimes)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Dilate)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Erode)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DrogueImageCoordiateX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DrogueImageCoordiateY)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DrogueVisionCoordiateX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DrogueVisionCoordiateY)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DrogueVisionCoordiateZ)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_StartFollow)->EnableWindow(FALSE);
	

}

void CImageProcess::RecoverControl(void)
{
	GetDlgItem(IDC_BUTTON_DrogueDetectDisplay)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DrogueDetectDynamicDisplay)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DrogueLocateDisplay)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DrogueLocateDynamicDisplay)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DrogueFollowDisplay)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DrogueFollowDynamicDisplay)->EnableWindow(TRUE);

	GetDlgItem(IDC_RADIO_GuassNoise)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_UniformNoise)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SaltNoise)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLIDER_GuassNoise)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLIDER_ContrastAdjust)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLIDER_BrightAdjust)->EnableWindow(TRUE);

	GetDlgItem(IDC_COMBO_DetectMethod)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_FilteringMethod)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_DrogueLocateSolveMethod)->EnableWindow(TRUE);
	//m_Combo_DetectMethod.EnableWindow(TRUE);
	//m_Combo_FilteringMethod.EnableWindow(TRUE);
	//m_Combo_DrogueLocateSolveMethod.EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_Dilate)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_Erode)->EnableWindow(TRUE);
}

void CImageProcess::OnBnClickedRadioGuassnoise()
{
	Choose_Noise_Flag =1;
	// TODO: Add your control notification handler code here
}

void CImageProcess::OnBnClickedRadioUniformnoise()
{
	Choose_Noise_Flag =2;
	// TODO: Add your control notification handler code here
}

void CImageProcess::OnBnClickedRadioSaltnoise()
{
	Choose_Noise_Flag =3;
	// TODO: Add your control notification handler code here
}

void CImageProcess::SaltNoise(cv::Mat& Image, double proportion)
{
	int i, j;
	int size;
	size = Image.cols*Image.rows;
	int n = int(size*proportion);
	for (int k = 0; k < n; k++)
	{
		i = rand() % Image.cols;
		j = rand() % Image.rows;
		if (Image.channels() == 1)
			Image.at<uchar>(j, i) = 255;
		else if (Image.channels() == 3)
		{
			Image.at<cv::Vec3b>(j, i)[0] = 255;
			Image.at<cv::Vec3b>(j, i)[1] = 255;
			Image.at<cv::Vec3b>(j, i)[2] = 255;
		}
	}

}

void CImageProcess::GausssNoise(IplImage* image,int bili)
{
	IplImage *img = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	//IplImage *img = cvCreateImage(cvGetSize(image), 8, 1);

	cvZero(img); //img为高斯噪声后图像指针//
	CvRNG rng = cvRNG(-1);  //初始化随机数发生器
	cvRandArr(&rng, img, CV_RAND_NORMAL, cvScalarAll(0), cvScalarAll(bili));
	cvAdd(image, img, image);
	//DrawPicToHDC(img, IDC_STATIC);
	cvReleaseImage(&img);
}

void CImageProcess::UniformNoise(IplImage* image,int bili)
{
	IplImage *img = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	//IplImage *img = cvCreateImage(cvGetSize(image), 8, 1);

	cvZero(img); //img为高斯噪声后图像指针//
	CvRNG rng = cvRNG(-1);  //初始化随机数发生器
	cvRandArr(&rng, img, CV_RAND_UNI, cvScalarAll(-bili), cvScalarAll(bili));
	cvAdd(image, img, image);
	//DrawPicToHDC(img, IDC_STATIC);
	cvReleaseImage(&img);
}

/************************************************************************************************************/
void CImageProcess::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl   *pSlidCtrl_bright = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BrightAdjust);
	m_Slider_Bright_Value = pSlidCtrl_bright->GetPos();//取得当前位置值  

	CSliderCtrl   *pSlidCtrl_Contrast = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ContrastAdjust);
	m_Slider_Contrast_Value = pSlidCtrl_Contrast->GetPos();//取得当前位置值  

	CSliderCtrl   *pSlidCtrl_Noise = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_GuassNoise);
	m_Slider_Noise_Value = pSlidCtrl_Noise->GetPos();//取得当前位置值 

	double mContrastvalue = m_Slider_Contrast_Value / 100;
	CString str_bright, str_noise, str_contrast;

	str_bright.Format(_T("%.0lf"), m_Slider_Bright_Value);
	str_contrast.Format(_T("%.2lf"), mContrastvalue);
	str_noise.Format(_T("%.0lf"), m_Slider_Noise_Value / 6);
	GetDlgItem(IDC_EDIT_GuassNoise)->SetWindowText(str_noise);
	GetDlgItem(IDC_EDIT_ContrastAdjust)->SetWindowText(str_contrast);
	GetDlgItem(IDC_EDIT_BrightAdjust)->SetWindowText(str_bright);


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CImageProcess::OnBnClickedButtonDilateerodeorder()
{
	if(ErodeDilateOrderFlag ==1)
	{
		GetDlgItem(IDC_BUTTON_DilateErodeOrder)->SetWindowText("先膨胀后腐蚀");
		ErodeDilateOrderFlag = 0;

	}else
	{
		GetDlgItem(IDC_BUTTON_DilateErodeOrder)->SetWindowText("先腐蚀后膨胀");
		ErodeDilateOrderFlag = 1;
	}
	// TODO: Add your control notification handler code here
}

void CImageProcess::OnCbnSelchangeComboDetectmethod()
{
	CComboBox *Combo_DetectMethod = (CComboBox*)GetDlgItem(IDC_COMBO_DetectMethod);
	switch (Combo_DetectMethod->GetCurSel())
	{
	case 0:
		DetectMethodFlag = 1;
		break;
	default:
		break;
	}
	// TODO: Add your control notification handler code here
}

void CImageProcess::OnCbnSelchangeComboFilteringmethod()
{

	CComboBox *Combo_FilteringMethod = (CComboBox*)GetDlgItem(IDC_COMBO_FilteringMethod);
	switch (Combo_FilteringMethod->GetCurSel())
	{
	case 0:
		FilteringMethodFlag = 1;
		FilteringMethod = CV_GAUSSIAN;
		break;
	case 1:
		FilteringMethodFlag = 2;
		FilteringMethod = CV_BLUR;
		break;
	case 2:
		FilteringMethodFlag = 3;
		FilteringMethod = CV_MEDIAN;
		break;
	case 3:
		FilteringMethodFlag = 4;
		FilteringMethod = CV_MEDIAN;
		break;
	default:
		break;
	}
	// TODO: Add your control notification handler code here
}

void CImageProcess::OnCbnSelchangeComboDroguelocatesolvemethod()
{
	CComboBox *Combo_DrogueLocateSolveMethod = (CComboBox*)GetDlgItem(IDC_COMBO_DrogueLocateSolveMethod);
	switch (Combo_DrogueLocateSolveMethod->GetCurSel())
	{
	case 0:
		DrogueLocateSolveMethodFlag = 1;
		break;
	case 1:
		DrogueLocateSolveMethodFlag = 2;
		break;
	case 2:
		DrogueLocateSolveMethodFlag = 3;
		break;
	case 3:
		DrogueLocateSolveMethodFlag = 4;
		break;
	case 4:
		DrogueLocateSolveMethodFlag = 5;
		break;

	default:
		
		break;
	}
	// TODO: Add your control notification handler code here
}

void CImageProcess::OnEnChangeEditDilate()
{
	UpdateData(TRUE);
	Edit_Dilate = m_Edit_Dilate;
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CImageProcess::OnEnChangeEditErode()
{
	UpdateData(TRUE);
	Edit_Erode = m_Edit_Erode;
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

LRESULT  CImageProcess::WriteControl(WPARAM wParam, LPARAM lParam)
{
	if(wParam==0 && lParam==0)
	{
		CString str_DetectfailureTimes,str_DetectTotalTimes;
		str_DetectfailureTimes.Format("%d",DetectfailureTimes);	
		str_DetectTotalTimes.Format("%d",DetectTotalTimes);
		GetDlgItem(IDC_EDIT_DetectTotalTimes)->SetWindowText(str_DetectTotalTimes);
		GetDlgItem(IDC_EDIT_DetectFailureTimes)->SetWindowText(str_DetectfailureTimes);
		CString str_CenterX,str_CenterY;
		str_CenterX.Format("%.4lf",ImageCenterX);
		str_CenterY.Format("%.4lf",ImageCenterY);
		GetDlgItem(IDC_EDIT_DrogueImageCoordiateX)->SetWindowText(str_CenterX);
		GetDlgItem(IDC_EDIT_DrogueImageCoordiateY)->SetWindowText(str_CenterY);
		CString str_ImageDroguePositionX,str_ImageDroguePositionY,str_ImageDroguePositionZ;
		str_ImageDroguePositionX.Format("%.4lf",ImageDroguePositionX);
		str_ImageDroguePositionY.Format("%.4lf",ImageDroguePositionY);
		str_ImageDroguePositionZ.Format("%.4lf",ImageDroguePositionZ);
		GetDlgItem(IDC_EDIT_DrogueVisionCoordiateX)->SetWindowText(str_ImageDroguePositionX);
		GetDlgItem(IDC_EDIT_DrogueVisionCoordiateY)->SetWindowText(str_ImageDroguePositionY);
		GetDlgItem(IDC_EDIT_DrogueVisionCoordiateZ)->SetWindowText(str_ImageDroguePositionZ);

		CString str_ImageDrogueLocateX,str_ImageDrogueLocateY,str_ImageDrogueLocateZ,
			str_ResultAtitudeHeading,str_ResultAtitudePitch;
		str_ImageDrogueLocateX.Format("%.5lf",DrogueLocateAccurateX);
		str_ImageDrogueLocateY.Format("%.5lf",DrogueLocateAccurateY);
		str_ImageDrogueLocateZ.Format("%.5lf",DrogueLocateAccurateZ);
		str_ResultAtitudeHeading.Format("%.4lf",ResultAtitudeHeading);
		str_ResultAtitudePitch.Format("%.4lf",ResultAtitudePitch);
		GetDlgItem(IDC_EDIT_DrogueVisionLocateX)->SetWindowText(str_ImageDrogueLocateX);
		GetDlgItem(IDC_EDIT_DrogueVisionLocateY)->SetWindowText(str_ImageDrogueLocateY);
		GetDlgItem(IDC_EDIT_DrogueVisionLocateZ)->SetWindowText(str_ImageDrogueLocateZ);
		GetDlgItem(IDC_EDIT_ImageAtitudeHeading)->SetWindowText(str_ResultAtitudeHeading);
		GetDlgItem(IDC_EDIT_ImageAtitudePitch)->SetWindowText(str_ResultAtitudePitch);
		if(CurrentFrame > 0)
		{
			GetDlgItem(IDC_BUTTON_LocateCurveDisplay)->EnableWindow(TRUE);

		}

		if(StartFollowFlag)
		{
			CString str_FollowAreaLeft,str_FollowAreaRight,str_FollowAreaUp,str_FollowAreaDown;
			str_FollowAreaLeft.Format("%.0lf",FollowAreaLeft);
			str_FollowAreaRight.Format("%.0lf",FollowAreaRight);
			str_FollowAreaUp.Format("%.0lf",FollowAreaUp);
			str_FollowAreaDown.Format("%.0lf",FollowAreaDown);
			GetDlgItem(IDC_EDIT_FollowAreaLeft)->SetWindowText(str_FollowAreaLeft);
			GetDlgItem(IDC_EDIT_FollowAreaRight)->SetWindowText(str_FollowAreaRight);
			GetDlgItem(IDC_EDIT_FollowAreaUp)->SetWindowText(str_FollowAreaUp);
			GetDlgItem(IDC_EDIT_FollowAreaDown)->SetWindowText(str_FollowAreaDown);

		}
		
	}

	return 1;
}

int CImageProcess::myOtsu(IplImage* frame)
{
	int width = frame->width;
	int height = frame->height;
	int pixelCount[GrayScale] = { 0 };
	float pixelPro[GrayScale] = { 0 };
	int i, j, pixelSum = width * height, threshold = 0;
	uchar* data = (uchar*)frame->imageData;

	//统计每个灰度级中像素的个数  
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[(int)data[i * width + j]]++;
		}
	}

	//计算每个灰度级的像素数目占整幅图像的比例  
	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
	}

	//遍历灰度级[0,255],寻找合适的threshold  
	float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0;
	for (i = 0; i < GrayScale; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;
		for (j = 0; j < GrayScale; j++)
		{
			if (j <= i)   //背景部分  
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //前景部分  
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		deltaTmp = (float)(w0 *w1* pow((u0 - u1), 2));
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}
	return threshold;
}

//启用跟踪
void CImageProcess::OnBnClickedButtonStartfollow()    
{
	if(StartFollowFlag)
	{
		GetDlgItem(IDC_BUTTON_StartFollow)->SetWindowText("启用跟踪");
		StartFollowFlag =false;
		GetDlgItem(IDC_EDIT_InreaseAreaD)->EnableWindow(FALSE);

	}else
	{
		GetDlgItem(IDC_BUTTON_StartFollow)->SetWindowText("暂停跟踪");
		StartFollowFlag = true;
		GetDlgItem(IDC_EDIT_InreaseAreaD)->EnableWindow(TRUE);
	}
	// TODO: Add your control notification handler code here
}

void CImageProcess::OnEnChangeEditInreasearead()
{
	UpdateData(TRUE);
	IncreaseAreaD = m_IncreaseAreaD;
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

//曲线显示
void CImageProcess::OnBnClickedButtonLocatecurvedisplay()
{
	// TODO: Add your control notification handler code here
	if(CurrentFrame >0 )
	{
		GetDlgItem(IDC_BUTTON_LocateCurveDisplay)->EnableWindow(false);
		FlightControlCurveDisplay1=new CShowMschart();
		FlightControlCurveDisplay1->Create(IDD_Mschart,GetDesktopWindow());
		FlightControlCurveDisplay1->ShowWindow(SW_SHOW);
		FlightControlCurveDisplay1->SetActiveWindow();
	}else
	{
		MessageBox("未检测到可显示数据，无法进行曲线显示，请检测后进行显示！");
	}
	
	//CShowMschart *ShowMschart = new CShowMschart();
	//ShowMschart->Create(IDD_Mschart,this);
	//ShowMschart->ShowWindow(SW_SHOW);

}

//锁定摄像机通道
void CImageProcess::OnBnClickedButtonLockcamerachannel()
{
	if(LockCameraChannelFlag)
	{
		LockCameraChannelFlag = false;
		GetDlgItem(IDC_BUTTON_LockCameraChannel)->SetWindowText("锁定摄像机通道");
	}else
	{
		LockCameraChannelFlag = true;
		GetDlgItem(IDC_BUTTON_LockCameraChannel)->SetWindowText("解锁摄像机通道");
	}
	
	// TODO: Add your control notification handler code here
}


/***************************************************************(************************************************************/
CvMat* CImageProcess::SolveAtitude_LHM(CvMat Input3DPoint, CvMat Input2DPoint, CvMat InitRotation)
{

	int PointNum = 12;
	double Error = 0.01;
	int count =0;
	CvMat *translation = cvCreateMat(3,1,CV_64FC1);
	double IntriMatrix[3][3] ={0};
	IntriMatrix[0][0] =RealTimeFocusFx;
	IntriMatrix[1][1] =RealTimeFocusFx;
	IntriMatrix[2][2] =1;
	IntriMatrix[0][2] =RealTimeIntrinsiccx;
	IntriMatrix[1][2] =RealTimeIntrinsiccy;
	CvMat IntriMat = cvMat(3,3,CV_64FC1,IntriMatrix);

	while(1)
	{
		double ERt = 0;
		CvMat *temp = cvCreateMat(3, 1, CV_64FC1);
		cvSetZero(temp);
		CvMat *tempVi = cvCreateMat(3,3,CV_64FC1);
		cvSetZero(tempVi);

		for(int i = 0;i<PointNum;i++)
		{
			double viMatrix[3];
			viMatrix[0] = cvmGet(&Input2DPoint,i,0);
			viMatrix[1] = cvmGet(&Input2DPoint,i,1);
			viMatrix[2] = cvmGet(&Input2DPoint,i,2);
			CvMat viMat = cvMat(3,1,CV_64FC1,viMatrix);
			CvMat *IntriMatInvert = cvCreateMat(3,3,CV_64FC1);
			cvInvert(&IntriMat,IntriMatInvert,CV_LU);
			cvGEMM(IntriMatInvert,&viMat,1,NULL,0,&viMat,0);

			CvMat *Dst1 = cvCreateMat(3,3,CV_64FC1);
			CvMat *Dst2 = cvCreateMat(1,1,CV_64FC1);
			cvMulTransposed(&viMat,Dst1,0);
			cvMulTransposed(&viMat,Dst2,-1);
			CvMat *Dst2Inv = cvCreateMat(1,1,CV_64FC1);
			cvInvert(Dst2,Dst2Inv,CV_LU);
			CvMat *ViMat1 = cvCreateMat(3,3,CV_64FC1);
			cvConvertScale(Dst1,ViMat1,cvmGet(Dst2Inv,0,0),0);
			//cvGEMM(Dst1,Dst2Inv,1,NULL,0,ViMat1,0);

			double piMatrix[3];
			piMatrix[0] = cvmGet(&Input3DPoint,i,0);
			piMatrix[1] = cvmGet(&Input3DPoint,i,1);
			piMatrix[2] = cvmGet(&Input3DPoint,i,2);
			CvMat piMat = cvMat(3,1,CV_64FC1,piMatrix);
			CvMat *Dst3 = cvCreateMat(3,1,CV_64FC1);
			CvMat *Dst4 = cvCreateMat(3,1,CV_64FC1);
			CvMat *Dst5 = cvCreateMat(3,3,CV_64FC1);
			CvMat *Tempresult = cvCreateMat(3,1,CV_64FC1);
			CvMat *Indentity = cvCreateMat(3,3,CV_64FC1);
			cvSetIdentity(Indentity);
			cvGEMM(&InitRotation,&piMat,1,NULL,0,Dst3,0);
			cvSub(ViMat1,Indentity,Dst5);
			cvGEMM(Dst5,Dst3,1,NULL,0,Tempresult,0);
			cvAdd(temp,Tempresult,temp);
			cvAdd(tempVi,ViMat1,tempVi);


			cvReleaseMat(&Dst1);
			cvReleaseMat(&Dst2);
			cvReleaseMat(&Dst2Inv);
			cvReleaseMat(&ViMat1);
			cvReleaseMat(&Dst3);
			cvReleaseMat(&Dst4);
			cvReleaseMat(&Dst5);
			cvReleaseMat(&Indentity);
			cvReleaseMat(&Tempresult);
			cvReleaseMat(&IntriMatInvert);

		}
		CvMat *temp1 = cvCreateMat(3,3,CV_64FC1);
		CvMat *temp2 = cvCreateMat(3,3,CV_64FC1);
		CvMat *tempdanwei = cvCreateMat(3,3,CV_64FC1);
		cvSetIdentity(tempdanwei);
		cvConvertScale(tempVi,temp1,1.0/PointNum,0);
		cvSub(tempdanwei,temp1,temp1);
		cvInvert(temp1,temp1,CV_LU);
		cvConvertScale(temp1,temp1,1.0/PointNum,0);
		cvGEMM(temp1,temp,1,NULL,0,translation,0);

		CvMat *tempqMean = cvCreateMat(3,1,CV_64FC1);
		cvSetZero(tempqMean);
		CvMat *temppMean = cvCreateMat(3,1,CV_64FC1);
		cvSetZero(temppMean);

		for(int i = 0;i<PointNum;i++)
		{
			double viMatrix[3];
			viMatrix[0] = cvmGet(&Input2DPoint,i,0);
			viMatrix[1] = cvmGet(&Input2DPoint,i,1);
			viMatrix[2] = cvmGet(&Input2DPoint,i,2);
			CvMat viMat = cvMat(3,1,CV_64FC1,viMatrix);
			CvMat *IntriMatInvert = cvCreateMat(3,3,CV_64FC1);
			cvInvert(&IntriMat,IntriMatInvert,CV_LU);
			cvGEMM(IntriMatInvert,&viMat,1,NULL,0,&viMat,0);


			CvMat *Dst1 = cvCreateMat(3,3,CV_64FC1);
			CvMat *Dst2 = cvCreateMat(1,1,CV_64FC1);
			cvMulTransposed(&viMat,Dst1,0);
			cvMulTransposed(&viMat,Dst2,-1);
			CvMat *Dst2Inv = cvCreateMat(1,1,CV_64FC1);
			cvInvert(Dst2,Dst2Inv,CV_LU);
			CvMat *ViMat1 = cvCreateMat(3,3,CV_64FC1);
			cvConvertScale(Dst1,ViMat1,cvmGet(Dst2Inv,0,0),0);
			//cvGEMM(Dst1,Dst2Inv,1,NULL,0,ViMat1,0);

			double piMatrix[3];
			piMatrix[0] = cvmGet(&Input3DPoint,i,0);
			piMatrix[1] = cvmGet(&Input3DPoint,i,1);
			piMatrix[2] = cvmGet(&Input3DPoint,i,2);
			CvMat piMat = cvMat(3,1,CV_64FC1,piMatrix);

			CvMat *tempERt = cvCreateMat(3,1,CV_64FC1);
			cvGEMM(&InitRotation,&piMat,1,NULL,0,tempERt,0);
			cvAdd(tempERt,translation,tempERt);

			CvMat *Indentity = cvCreateMat(3,3,CV_64FC1);
			cvSetIdentity(Indentity);
			cvSub(Indentity,ViMat1,Indentity);
			cvGEMM(Indentity,tempERt,1,NULL,0,tempERt,0);
			double tempvalue = cvNorm(tempERt,NULL,CV_L2);
			ERt += tempvalue;

			CvMat *qiR = cvCreateMat(3,1,CV_64FC1);
			CvMat *tempMedian = cvCreateMat(3,1,CV_64FC1);
			cvGEMM(&InitRotation,&piMat,1,translation,1,tempMedian,0);
			cvGEMM(ViMat1,tempMedian,1,NULL,0,qiR,0);
			cvAdd(qiR,tempqMean,tempqMean);

			cvAdd(&piMat,temppMean,temppMean);

			cvReleaseMat(&Dst1);
			cvReleaseMat(&Dst2);
			cvReleaseMat(&Dst2Inv);
			cvReleaseMat(&ViMat1);
			cvReleaseMat(&tempERt);
			cvReleaseMat(&Indentity);
			cvReleaseMat(&qiR);
			cvReleaseMat(&tempMedian);
			cvReleaseMat(&IntriMatInvert);
		}

		double xxx = cvmGet(translation,0,0);
		double yyy = cvmGet(translation,1,0);
		double zzz = cvmGet(translation,2,0)+1.18;
		double gunzhuanjiao = hudu2du*atan(cvmGet(&InitRotation,1,0)/cvmGet(&InitRotation,0,0));
		double theta = -hudu2du*atan(cvmGet(&InitRotation,2,1)/cvmGet(&InitRotation,2,2));
		double fai = hudu2du*asin(cvmGet(&InitRotation,2,0));
		double RealXc = DrogueRalativeCameraDistanceX;
		double RealYc = -DrogueRalativeCameraDistanceZ;
		double RealZc = DrogueRalativeCameraDistanceY;

		count++;
		if(count>10)
			Error +=0.1;
		if(ERt < Error)
		{
			cvReleaseMat(&temp);
			cvReleaseMat(&tempVi);
			cvReleaseMat(&temp1);
			cvReleaseMat(&temp2);
			cvReleaseMat(&tempdanwei);
			cvReleaseMat(&tempqMean);
			cvReleaseMat(&temppMean);
			break;
		}


		cvConvertScale(tempqMean,tempqMean,1.0/PointNum,0);
		cvConvertScale(temppMean,temppMean,1.0/PointNum,0);
		
		CvMat *MR = cvCreateMat(3,3,CV_64FC1);
		cvSetZero(MR);
		for(int i = 0;i<PointNum;i++)
		{
			CvMat *DotqiR = cvCreateMat(3,1,CV_64FC1);
			CvMat *Dotpi = cvCreateMat(3,1,CV_64FC1);

			double viMatrix[3];
			viMatrix[0] = cvmGet(&Input2DPoint,i,0);
			viMatrix[1] = cvmGet(&Input2DPoint,i,1);
			viMatrix[2] = cvmGet(&Input2DPoint,i,2);
			CvMat viMat = cvMat(3,1,CV_64FC1,viMatrix);
			CvMat *IntriMatInvert = cvCreateMat(3,3,CV_64FC1);
			cvInvert(&IntriMat,IntriMatInvert,CV_LU);
			cvGEMM(IntriMatInvert,&viMat,1,NULL,0,&viMat,0);

			CvMat *Dst1 = cvCreateMat(3,3,CV_64FC1);
			CvMat *Dst2 = cvCreateMat(1,1,CV_64FC1);
			cvMulTransposed(&viMat,Dst1,0);
			cvMulTransposed(&viMat,Dst2,-1);
			CvMat *Dst2Inv = cvCreateMat(1,1,CV_64FC1);
			cvInvert(Dst2,Dst2Inv,CV_LU);
			CvMat *ViMat1 = cvCreateMat(3,3,CV_64FC1);
			cvConvertScale(Dst1,ViMat1,cvmGet(Dst2Inv,0,0),0);
			//cvGEMM(Dst1,Dst2Inv,1,NULL,0,ViMat1,0);
			double piMatrix[3];
			piMatrix[0] = cvmGet(&Input3DPoint,i,0);
			piMatrix[1] = cvmGet(&Input3DPoint,i,1);
			piMatrix[2] = cvmGet(&Input3DPoint,i,2);
			CvMat piMat = cvMat(3,1,CV_64FC1,piMatrix);

			CvMat *qiR = cvCreateMat(3,1,CV_64FC1);
			CvMat *tempMedian = cvCreateMat(3,1,CV_64FC1);
			cvGEMM(&InitRotation,&piMat,1,translation,1,tempMedian,0);
			cvGEMM(ViMat1,tempMedian,1,NULL,0,qiR,0);

			cvSub(&piMat,temppMean,Dotpi);
			cvSub(qiR,tempqMean,DotqiR);
			CvMat *tempMR = cvCreateMat(3,3,CV_64FC1);

			cvGEMM(DotqiR,Dotpi,1,NULL,0,tempMR,CV_GEMM_B_T);
			cvAdd(MR,tempMR,MR);

			cvReleaseMat(&Dst1);
			cvReleaseMat(&Dst2);
			cvReleaseMat(&Dst2Inv);
			cvReleaseMat(&ViMat1);
			cvReleaseMat(&DotqiR);
			cvReleaseMat(&Dotpi);
			cvReleaseMat(&qiR);
			cvReleaseMat(&tempMedian);
			cvReleaseMat(&tempMR);
			cvReleaseMat(&IntriMatInvert);

		}
		CvMat *W = cvCreateMat(3,3,CV_64FC1);
		CvMat *U = cvCreateMat(3,3,CV_64FC1);
		CvMat *V = cvCreateMat(3,3,CV_64FC1);
		cvSVD(MR,W,U,V);
		cvGEMM(V,U,1,NULL,0,&InitRotation,CV_GEMM_B_T);

		cvReleaseMat(&temp);
		cvReleaseMat(&tempVi);
		cvReleaseMat(&temp1);
		cvReleaseMat(&temp2);
		cvReleaseMat(&tempdanwei);
		cvReleaseMat(&tempqMean);
		cvReleaseMat(&temppMean);
		cvReleaseMat(&MR);
		cvReleaseMat(&U);
		cvReleaseMat(&V);
		cvReleaseMat(&W);

	}
	return translation;
	//cvReleaseMat(&translation);
	
}

void CImageProcess::ImagePointSortLeft(double* X, double* Y, double* FinalX, double* FinalY)
{
	double Xsort[12],Ysort[12];
	for(int i=0;i<12;i++)
	{
		Xsort[i] = X[i];
		Ysort[i] = Y[i];

	}
	int record;
	for(int i = 0;i < 12;i++)
	{
		double temp = 10000;
		for(int j=i;j<12;j++)
		{
			if(temp>Xsort[j])
			{
				temp = Xsort[j];
				record = j;
			}
		}
		double zjbl;
		zjbl = Xsort[i];
		Xsort[i] = Xsort[record];
		Xsort[record] = zjbl;
		zjbl = Ysort[i];
		Ysort[i] = Ysort[record];
		Ysort[record] = zjbl;

	}
	FinalX[0] = Xsort[0];
	FinalY[0] = Ysort[0];
	FinalX[6] = Xsort[11];
	FinalY[6] = Ysort[11];
	if(Ysort[1] > Ysort[2])
	{
		FinalX[1] = Xsort[1];
		FinalY[1] = Ysort[1];
		FinalX[11] = Xsort[2];
		FinalY[11] = Ysort[2];
	}else
	{
		FinalX[1] = Xsort[2];
		FinalY[1] = Ysort[2];
		FinalX[11] = Xsort[1];
		FinalY[11] = Ysort[1];

	}
	if(Ysort[3] > Ysort[4])
	{
		FinalX[2] = Xsort[3];
		FinalY[2] = Ysort[3];
		FinalX[10] = Xsort[4];
		FinalY[10] = Ysort[4];
	}else
	{
		FinalX[2] = Xsort[4];
		FinalY[2] = Ysort[4];
		FinalX[10] = Xsort[3];
		FinalY[10] = Ysort[3];

	}
	if(Ysort[5] > Ysort[6])
	{
		FinalX[3] = Xsort[5];
		FinalY[3] = Ysort[5];
		FinalX[9] = Xsort[6];
		FinalY[9] = Ysort[6];
	}else
	{
		FinalX[3] = Xsort[6];
		FinalY[3] = Ysort[6];
		FinalX[9] = Xsort[5];
		FinalY[9] = Ysort[5];

	}
	if(Ysort[7] > Ysort[8])
	{
		FinalX[4] = Xsort[7];
		FinalY[4] = Ysort[7];
		FinalX[8] = Xsort[8];
		FinalY[8] = Ysort[8];
	}else
	{
		FinalX[4] = Xsort[8];
		FinalY[4] = Ysort[8];
		FinalX[8] = Xsort[7];
		FinalY[8] = Ysort[7];
	}
	if(Ysort[9] > Ysort[10])
	{
		FinalX[5] = Xsort[9];
		FinalY[5] = Ysort[9];
		FinalX[7] = Xsort[10];
		FinalY[7] = Ysort[10];
	}else
	{
		FinalX[5] = Xsort[10];
		FinalY[5] = Ysort[10];
		FinalX[7] = Xsort[9];
		FinalY[7] = Ysort[9];
	}
}

int CImageProcess::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	// TODO:  Add your specialized creation code here

	return 0;
}


