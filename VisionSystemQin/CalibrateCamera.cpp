// CalibrateCamera.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSystemQin.h"
#include "CalibrateCamera.h"
#include "CvvImage.h"
#include "MyApp.h"
#include "ImageProcess.h"
#include <Windows.h>
#include "ResultDisplay.h"

//#define Basic_Image_Width 1620
//#define Basic_Image_Height 1220

#define Basic_Image_Width 1024
#define Basic_Image_Height 768



// CCalibrateCamera dialog
float intr[3][3] = {0.0};
float dist[4] = {0.0};
float tranv[3] = {0.0};
float rotv[3] = {0.0};
TCHAR RootDirectory[MAX_PATH] = { 0 };
TCHAR szBuffer[MAX_PATH] = { 0 };
TCHAR szFilename[MAX_PATH][MAX_PATH] = {0};
int PictureCount = 1;
double CalibrationTransformx,CalibrationTransformy,CalibrationTransformz,CalibrationTransformh,
CalibrationTransformp,CalibrationTransformr;
CString csFullPath;


IMPLEMENT_DYNAMIC(CCalibrateCamera, CDialog)

CCalibrateCamera::CCalibrateCamera(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibrateCamera::IDD, pParent)
	, m_XCornorCount(0)
	, m_YCornorCount(0)
	, m_SquareSize(0)
	, m_ChessBoardTimes(0)
	, m_youxiaoshuzi(0)
{

}

CCalibrateCamera::~CCalibrateCamera()
{
}

void CCalibrateCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MethodChoose, m_Combox);
	DDX_Text(pDX, IDC_EDIT_XCornorCount, m_XCornorCount);
	DDX_Text(pDX, IDC_EDIT_YCornorCount, m_YCornorCount);
	DDX_Text(pDX, IDC_EDIT_SquareSize, m_SquareSize);
	DDX_Text(pDX, IDC_EDIT_ChessDetectTimes, m_ChessBoardTimes);
	DDX_Text(pDX, IDC_EDIT_youxiaoshuziweishu, m_youxiaoshuzi);
}


BEGIN_MESSAGE_MAP(CCalibrateCamera, CDialog)
	ON_BN_CLICKED(IDC_RADIO_VisionChessBoard, &CCalibrateCamera::OnBnClickedRadioVisionchessboard)
	ON_BN_CLICKED(IDC_RADIO_LocalChessBoard, &CCalibrateCamera::OnBnClickedRadioLocalchessboard)
	ON_BN_CLICKED(IDC_RADIO_RGB2Gray, &CCalibrateCamera::OnBnClickedRadioRgb2gray)
	ON_BN_CLICKED(IDC_RADIO_RGB2HSV, &CCalibrateCamera::OnBnClickedRadioRgb2hsv)
	ON_BN_CLICKED(IDC_RADIO_RGB2YCrCb, &CCalibrateCamera::OnBnClickedRadioRgb2ycrcb)
	ON_BN_CLICKED(IDC_RADIO_RGB2HLS, &CCalibrateCamera::OnBnClickedRadioRgb2hls)
	ON_BN_CLICKED(IDC_RADIO_RGB2Luv, &CCalibrateCamera::OnBnClickedRadioRgb2luv)
	ON_BN_CLICKED(IDC_RADIO_RGB2XYZ, &CCalibrateCamera::OnBnClickedRadioRgb2xyz)
	ON_BN_CLICKED(IDC_CHECK_FPP, &CCalibrateCamera::OnBnClickedCheckFpp)
	ON_BN_CLICKED(IDC_CHECK_UIG, &CCalibrateCamera::OnBnClickedCheckUig)
	ON_BN_CLICKED(IDC_CHECK_FAR, &CCalibrateCamera::OnBnClickedCheckFar)
	ON_BN_CLICKED(IDC_CHECK_ZTD, &CCalibrateCamera::OnBnClickedCheckZtd)
	ON_BN_CLICKED(IDC_BUTTON_SettingBySystem, &CCalibrateCamera::OnBnClickedButtonSettingbysystem)
	ON_BN_CLICKED(IDC_BUTTON_StartCalibration, &CCalibrateCamera::OnBnClickedButtonStartcalibration)
	ON_EN_CHANGE(IDC_EDIT_youxiaoshuziweishu, &CCalibrateCamera::OnEnChangeEdityouxiaoshuziweishu)
	ON_BN_CLICKED(IDC_BUTTON_LoadLocalChessBoard, &CCalibrateCamera::OnBnClickedButtonLoadlocalchessboard)
	ON_BN_CLICKED(IDC_BUTTON_ExitCalibration, &CCalibrateCamera::OnBnClickedButtonExitcalibration)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_MethodChoose, &CCalibrateCamera::OnCbnSelchangeComboMethodchoose)
END_MESSAGE_MAP()


// CCalibrateCamera message handlers



BOOL CCalibrateCamera::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCalibrateCamera::OnBnClickedButtonSettingbysystem();
	ExitCalibrationFlag = 0;
	LoadPictureFlag = 0;
	LoadVisionFlag = 0;
	current_frame_rgb = NULL;
	FrameProPRocessMethodFlag = 2;

	vpTransform* CalibrationfarTransform = (vpTransform*)vpTransform::find("CalibrationTransform");
	CalibrationfarTransform->getTranslate(&CalibrationTransformx,&CalibrationTransformy,&CalibrationTransformz);
	CalibrationfarTransform->getRotate(&CalibrationTransformh,&CalibrationTransformp,&CalibrationTransformr);

	char pFileName[MAX_PATH]; 
	int nPos = GetCurrentDirectory( MAX_PATH, pFileName); 

	csFullPath = pFileName;  
	csFullPath = csFullPath+"\\Frame\\";

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CCalibrateCamera::OnBnClickedRadioVisionchessboard()
{
	OriginOfChessBoardPicture_Flag = 1;
	GetDlgItem(IDC_BUTTON_LoadLocalChessBoard)->SetWindowText("启动视景");
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedRadioLocalchessboard()
{
	OriginOfChessBoardPicture_Flag = 2;
	GetDlgItem(IDC_BUTTON_LoadLocalChessBoard)->SetWindowText("加载本地图片");
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedRadioRgb2gray()
{
	ColorConvert_Flag = 1;
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedRadioRgb2hsv()
{
	ColorConvert_Flag = 2;
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedRadioRgb2ycrcb()
{
	ColorConvert_Flag = 3;
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedRadioRgb2hls()
{
	ColorConvert_Flag = 4;
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedRadioRgb2luv()
{
	ColorConvert_Flag = 5;
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedRadioRgb2xyz()
{
	ColorConvert_Flag = 6;
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedCheckFpp()
{
	if(((CButton*)GetDlgItem(IDC_CHECK_FPP))->GetCheck()==BST_CHECKED)
		FPP_Flag = CV_CALIB_FIX_PRINCIPAL_POINT;
	else
		FPP_Flag = 0;
	
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedCheckUig()
{
	if(((CButton*)GetDlgItem(IDC_CHECK_UIG))->GetCheck()==BST_CHECKED)
		UIG_Flag = CV_CALIB_USE_INTRINSIC_GUESS;
	else
		UIG_Flag = 0;

	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedCheckFar()
{
	if(((CButton*)GetDlgItem(IDC_CHECK_FAR))->GetCheck()==BST_CHECKED)
		FAR_Flag = CV_CALIB_FIX_ASPECT_RATIO;
	else
		FAR_Flag = 0;
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedCheckZtd()
{
	if(((CButton*)GetDlgItem(IDC_CHECK_FAR))->GetCheck()==BST_CHECKED)
		ZTD_Flag = CV_CALIB_ZERO_TANGENT_DIST;
	else
		ZTD_Flag = 0;
	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedButtonSettingbysystem()
{
	m_youxiaoshuzi = 3;
	UpdateData(FALSE);
	radio=(CButton*)GetDlgItem(IDC_RADIO_VisionChessBoard);
	radio->SetCheck(1);
	radio=(CButton*)GetDlgItem(IDC_RADIO_LocalChessBoard);
	radio->SetCheck(0);
	OriginOfChessBoardPicture_Flag = 1;
	GetDlgItem(IDC_BUTTON_LoadLocalChessBoard)->SetWindowText("启动视景");

	UpdateData(TRUE);
	m_XCornorCount = 10,m_YCornorCount = 10,m_SquareSize = 20,m_ChessBoardTimes = 30;
	UpdateData(FALSE);
		
	((CButton *)GetDlgItem(IDC_CHECK_FPP))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_UIG))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_FAR))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_ZTD))->SetCheck(1);

	FPP_Flag = CV_CALIB_FIX_PRINCIPAL_POINT,UIG_Flag = CV_CALIB_USE_INTRINSIC_GUESS,
		FAR_Flag = CV_CALIB_FIX_ASPECT_RATIO,ZTD_Flag = CV_CALIB_ZERO_TANGENT_DIST;


	radio=(CButton*)GetDlgItem(IDC_RADIO_RGB2Gray);
	radio->SetCheck(1);
	radio=(CButton*)GetDlgItem(IDC_RADIO_RGB2HLS);
	radio->SetCheck(0);
	radio=(CButton*)GetDlgItem(IDC_RADIO_RGB2HSV);
	radio->SetCheck(0);
	radio=(CButton*)GetDlgItem(IDC_RADIO_RGB2Luv);
	radio->SetCheck(0);
	radio=(CButton*)GetDlgItem(IDC_RADIO_RGB2XYZ);
	radio->SetCheck(0);
	radio=(CButton*)GetDlgItem(IDC_RADIO_RGB2YCrCb);
	radio->SetCheck(0);
	ColorConvert_Flag = 1;
	
	GetDlgItem(IDC_BUTTON_ExitCalibration)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_StartCalibration)->EnableWindow(TRUE);

	m_Combox.SetCurSel(2);
	GetDlgItem(IDC_RADIO_RGB2Gray)->EnableWindow(FALSE);	
	GetDlgItem(IDC_RADIO_RGB2HLS)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_RGB2HSV)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_RGB2Luv)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_RGB2XYZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_RGB2YCrCb)->EnableWindow(FALSE);

	// TODO: Add your control notification handler code here
}

void CCalibrateCamera::OnBnClickedButtonStartcalibration()
{

	MaskActiveCottrol();
	//色彩空间转换
	switch(ColorConvert_Flag)
	{
	case 1:
		code = CV_BGR2GRAY;
		break;
	case 2:
		code = CV_RGB2HSV;
		break;
	case 3:
		code = CV_RGB2YCrCb;
		break;
	case 4:
		code = CV_RGB2HLS;
		break;
	case 5:
		code = CV_RGB2Luv;
		break;
	case 6:
		code = CV_RGB2XYZ;
		break;
	default:
		break;
	}

	//帧处理方法选择
	
	if(OriginOfChessBoardPicture_Flag == 1)
	{
		//从视景中获取图像
		if (LoadVisionFlag == 1)  //从本地图片获取图像
		{
			PictureCount = VisionPictureCount;
			UpdateData(TRUE);
			HANDLE CalibrationHandleVision;
			CalibrationHandleVision = CreateThread(NULL,0,CalibrationIng,(LPVOID)this,0,NULL);
			CloseHandle(CalibrationHandleVision);


		}else
		{
			MessageBox("未找到视景截图!!!");
			RecoverActiveControl();
			return;
		}

	}
	else
	{
		if (LoadPictureFlag == 1)  //从本地图片获取图像
		{
			UpdateData(TRUE);
			HANDLE CalibrationHandle;
			CalibrationHandle = CreateThread(NULL,0,CalibrationIng,(LPVOID)this,0,NULL);
			CloseHandle(CalibrationHandle);
		}else
		{
			MessageBox("未加载本地图片!!!");
			RecoverActiveControl();
			return;
		}

	}

}


void CCalibrateCamera::CalibrationResult(float intr[][3], float* dist, float* rotv, float* tranv,int Pointcount)
{
	CString str;
	if(Pointcount >= 0 && Pointcount <= 9)
	{
		char Math2char = Pointcount + 48;
		str.Format("%c",Math2char);
		youxiaoshuzi = _T("%.")+str+_T("f");
	}else
		MessageBox("请输入0-9之间的整数！！！");
	
	str.Format(youxiaoshuzi,intr[0][0]);
	GetDlgItem(IDC_EDIT_Intrinsicfx)->SetWindowText(str);
	str.Format(youxiaoshuzi,intr[1][1]);
	GetDlgItem(IDC_EDIT_Intrinsicfy)->SetWindowText(str);
	str.Format(youxiaoshuzi,intr[0][2]);
	GetDlgItem(IDC_EDIT_Intrinsiccx)->SetWindowText(str);
	str.Format(youxiaoshuzi,intr[1][2]);
	GetDlgItem(IDC_EDIT_Intrinsiccy)->SetWindowText(str);

	str.Format(youxiaoshuzi,dist[0]);
	GetDlgItem(IDC_EDIT_Distortionk1)->SetWindowText(str);
	str.Format(youxiaoshuzi,dist[1]);
	GetDlgItem(IDC_EDIT_Distortionk2)->SetWindowText(str);
	str.Format(youxiaoshuzi,dist[2]);
	GetDlgItem(IDC_EDIT_Distortionp1)->SetWindowText(str);
	str.Format(youxiaoshuzi,dist[3]);
	GetDlgItem(IDC_EDIT_Distortionp2)->SetWindowText(str);

	str.Format(youxiaoshuzi,rotv[0]);
	GetDlgItem(IDC_EDIT_Rotationr1)->SetWindowText(str);
	str.Format(youxiaoshuzi,rotv[1]);
	GetDlgItem(IDC_EDIT_Rotationr2)->SetWindowText(str);
	str.Format(youxiaoshuzi,rotv[2]);
	GetDlgItem(IDC_EDIT_Rotationr3)->SetWindowText(str);

	str.Format(youxiaoshuzi,tranv[0]);
	GetDlgItem(IDC_EDIT_Translationt1)->SetWindowText(str);
	str.Format(youxiaoshuzi,tranv[1]);
	GetDlgItem(IDC_EDIT_Translationt2)->SetWindowText(str);
	str.Format(youxiaoshuzi,tranv[2]);
	GetDlgItem(IDC_EDIT_Translationt3)->SetWindowText(str);
}

void CCalibrateCamera::OnEnChangeEdityouxiaoshuziweishu()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	CalibrationResult(intr,dist,rotv,tranv,m_youxiaoshuzi);
}

void CCalibrateCamera::CamaeraCalibration(CvSize ImageSize, CvSize ChessBoardSize, float SquareSize, int NImages,int ChessBoardTimes)
{
	//设置一 图像尺寸
	int image_width;//ImageSize.width;
	int image_height;//ImageSize.height;//待标定图片的大小
	
	//图片中可标定的角点区域和个数
	int NPoints = ChessBoardSize.width*ChessBoardSize.height;//单张图像中所有的角点个数
	//设置六 设置棋盘格子的边长，单位为mm
	float    SquareWidth = SquareSize; 
	////////////////////需要设置的参数结束////////////////////////////////
	int *corner_count = new int[NImages];
	//int corner_count[NImages] = {0};

	IplImage     *current_frame_gray;//灰度图像，用来提取角点
	IplImage     *chessBoard_Img;//原始载入图像
	IplImage     *Current_frame_canny;
	//CvPoint2D32f corners[NPoints*NImages];//所有的角点坐标
	CvPoint2D32f *corners = new CvPoint2D32f[NPoints*NImages];

	//创建图像
	


	//当前图像序号
	int captured_frames=0;
	for(captured_frames=0;captured_frames<NImages;captured_frames++)
	{

		if(ExitCalibrationFlag == 1)
		{
			ExitCalibrationFlag = 0;
			if(MessageBox("摄像机正在进行标定确定退出吗？","提示",MB_OKCANCEL) == IDOK)
			{
				RecoverActiveControl();
				return;
			}else
				continue;	
		}
		CString filename;
		CString strRootDirectory,strszFilename;
		if(OriginOfChessBoardPicture_Flag == 1)
		{
			CString str;
			str.Format("%02d",captured_frames);
			filename = csFullPath + "Frame/frame"+str+".bmp";

		}else
		{
			if(NImages > 1)
			{
				strRootDirectory.Format("%s",RootDirectory);
				strszFilename.Format("%s",szFilename[captured_frames]);
				filename = strRootDirectory+_T("/")+strszFilename;
			}else
			{
				strRootDirectory.Format("%s",RootDirectory);
				filename = strRootDirectory;	
			}
		}

		chessBoard_Img=cvLoadImage( filename, 1 );
		if (chessBoard_Img==NULL)
		{
			MessageBox("load image failed!");
		}
		
		image_width =chessBoard_Img->width;//ImageSize.width;
		image_height = chessBoard_Img->height;//ImageSize.height;//待标定图片的大小
		//chessBoard_Img =cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);
		current_frame_gray = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 1);
		Current_frame_canny = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 1);
		current_frame_rgb = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);
		
		cvCvtColor(chessBoard_Img, current_frame_gray, CV_BGR2GRAY);
		//cvNamedWindow("1");
		//cvShowImage("1",current_frame_gray);
		//cvWaitKey(0);
		cvCopy(chessBoard_Img,current_frame_rgb,0);
		//查找角点
		int find_corners_result;

		CString str1,str2;
		str1.Format("%02d",captured_frames+1);
		str2.Format("%02d",NImages);
		GetDlgItem(IDC_STATIC_CalibrationTips)->SetWindowText("待标定图片总数为"+str2+"张，目前正在标定第"+str1+"张图片");

		find_corners_result = 
			cvFindChessboardCorners(current_frame_gray,//当前图像
			ChessBoardSize,//角度区域 棋盘图中每行和每列角点的个数
			&corners[captured_frames*NPoints],// 检测到的角点
			&corner_count[captured_frames],//int corner_count[NImages] = {0};输出，角点的个数。如果不是NULL，函数将检测到的角点的个数存储于此变量。
			CV_CALIB_CB_ADAPTIVE_THRESH );//使用自适应阈值
		//通过迭代来发现具有子象素精度的角点位置
		if(ExitCalibrationFlag == 1)
		{
			ExitCalibrationFlag = 0;
			if(MessageBox("摄像机正在进行标定确定退出吗？","提示",MB_OKCANCEL) == IDOK)
			{
				RecoverActiveControl();
				return;
			}else
				continue;	
		}
		cvFindCornerSubPix( current_frame_gray, //当前图像
			&corners[captured_frames*NPoints],//检测到的角点
			NPoints, //角点的个数
			cvSize(5,5),cvSize(-1,-1), //不忽略corner临近的像素进行精确估计，
			cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,ChessBoardTimes,0.01) );
		//迭代次数（iteration）或者最小精度（epsilon）
		if(ExitCalibrationFlag == 1)
		{
			ExitCalibrationFlag = 0;
			if(MessageBox("摄像机正在进行标定确定退出吗？","提示",MB_OKCANCEL) == IDOK)
			{
				RecoverActiveControl();
				return;
			}else
				continue;	
		}
		if(FrameProPRocessMethodFlag == 0)
		{
			cvCanny(current_frame_gray,Current_frame_canny,120,30,3);
			cvDrawChessboardCorners(Current_frame_canny, ChessBoardSize, //绘制检测到的棋盘角点
				&corners[captured_frames*NPoints], 
				NPoints, 
				find_corners_result);
		}else
			cvDrawChessboardCorners(current_frame_rgb, ChessBoardSize, //绘制检测到的棋盘角点
			&corners[captured_frames*NPoints], 
			NPoints, 
			find_corners_result);
	
		if(find_corners_result == 0)
		{
			MessageBox("未检测到角点！！");
			CString str1,str2;
			str1.Format("%02d",captured_frames+1);
			str2.Format("%02d",NImages);
			GetDlgItem(IDC_STATIC_CalibrationTips)->SetWindowText("待标定图片总数为"+str2+"张，标定第"+str1+"张图片发生错误！！");
			RecoverActiveControl();
			return;
		}

		if(ExitCalibrationFlag == 1)
		{
			ExitCalibrationFlag = 0;
			if(MessageBox("摄像机正在进行标定确定退出吗？","提示",MB_OKCANCEL) == IDOK)
			{
				RecoverActiveControl();
				return;
			}else
				continue;	
		}

		//if(code != CV_BGR2GRAY)
		if(FrameProPRocessMethodFlag ==0)
			LoadImage2Control(Current_frame_canny,IDC_STATIC_CalibrationPicture);
		else if(FrameProPRocessMethodFlag == 1)
		{
			if(code != CV_BGR2GRAY)
			{
				cvCvtColor(current_frame_rgb, current_frame_rgb, code);
				LoadImage2Control(current_frame_rgb,IDC_STATIC_CalibrationPicture);	
			}
			else
			{
				IplImage* grayImage = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 1);
				cvCvtColor(current_frame_rgb, grayImage, CV_BGR2GRAY);
				LoadImage2Control(grayImage,IDC_STATIC_CalibrationPicture);
				cvReleaseImage(&grayImage);
			}
		}else
			LoadImage2Control(current_frame_rgb,IDC_STATIC_CalibrationPicture);


		
	}
	CString str2;
	str2.Format("%02d",NImages);
	GetDlgItem(IDC_STATIC_CalibrationTips)->SetWindowText("待标定图片总数为"+str2+"张，标定完毕！！");


	intrinsics         = cvCreateMat(3,3,CV_32FC1);
	distortion_coeff     = cvCreateMat(1,4,CV_32FC1);
	rotation_vectors     = cvCreateMat(NImages,3,CV_32FC1);
	translation_vectors     = cvCreateMat(NImages,3,CV_32FC1);

	point_counts         = cvCreateMat(NImages,1,CV_32SC1);
	object_points     = cvCreateMat(NImages*NPoints,3,CV_32FC1);
	image_points         = cvCreateMat(NImages*NPoints,2,CV_32FC1);
	image_points2         = cvCreateMat(NImages*NPoints,2,CV_32FC1);
	//把2维点转化成三维点（object_points输出量），

	InitCorners3D(object_points, ChessBoardSize, NImages, SquareWidth);
	//把corners 复制到 image_points中
	cvSetData( image_points, corners, sizeof(CvPoint2D32f));
	//把corner_count 复制到 point_counts中 每个图像中多少个角点
	cvSetData( point_counts, corner_count, sizeof(int));

	//计算内参
	cvCalibrateCamera2( object_points,
		image_points,
		point_counts,//指定不同视图里点的数目
		cvSize(image_width,image_height),
		intrinsics,
		distortion_coeff,
		rotation_vectors,
		translation_vectors,
		0);
	//FPP_Flag|UIG_Flag|FAR_Flag|ZTD_Flag
	

	for ( int i = 0; i < 3; i++)
	{
		for ( int j = 0; j < 3; j++)
		{
			intr[i][j] = ((float*)(intrinsics->data.ptr + intrinsics->step*i))[j];
		}
		dist[i] = ((float*)(distortion_coeff->data.ptr))[i];
		tranv[i] = ((float*)(translation_vectors->data.ptr))[i];
		rotv[i] = ((float*)(rotation_vectors->data.ptr))[i];
	}
	dist[3] = ((float*)(distortion_coeff->data.ptr))[3];
	//旋转向量转换为旋转矩阵
	double R_matrix[9];
	CvMat pr_vec;
	CvMat pR_matrix;
	cvInitMatHeader(&pr_vec,1,3,CV_64FC1,rotv,CV_AUTOSTEP);
	cvInitMatHeader(&pR_matrix,3,3,CV_64FC1,R_matrix,CV_AUTOSTEP);
	cvRodrigues2(&pr_vec, &pR_matrix,0);
	//UpdateData(TRUE);
	CalibrationResult(intr,dist,rotv,tranv,m_youxiaoshuzi);
	PassPama2ResultDisplay(intr,dist,rotv,tranv);

	delete[] corner_count;
	delete[] corners;
	RecoverActiveControl();

	cvReleaseMat(&intrinsics);       
	cvReleaseMat(&distortion_coeff); 
	cvReleaseMat(&rotation_vectors);
	cvReleaseMat(&translation_vectors);   
	cvReleaseMat(&point_counts);
	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points);
	cvReleaseImage(&current_frame_gray);
	cvReleaseImage(&chessBoard_Img);
	cvReleaseImage(&Current_frame_canny);
	
}

void CCalibrateCamera::InitCorners3D(CvMat* Corners3D, CvSize ChessBoardSize, int NImages, float SquareSize)
{
	int CurrentImage = 0;
	int CurrentRow = 0;
	int CurrentColumn = 0;
	int NPoints = ChessBoardSize.height*ChessBoardSize.width;
	float * temppoints = new float[NImages*NPoints*3];

	for (CurrentImage = 0 ; CurrentImage < NImages ; CurrentImage++)
	{
		for (CurrentRow = 0; CurrentRow < ChessBoardSize.height; CurrentRow++)
		{
			for (CurrentColumn = 0; CurrentColumn < ChessBoardSize.width; CurrentColumn++)
			{
				temppoints[(CurrentImage*NPoints*3)+(CurrentRow*ChessBoardSize.width + 
					CurrentColumn)*3]=(float)CurrentRow*SquareSize;
				temppoints[(CurrentImage*NPoints*3)+(CurrentRow*ChessBoardSize.width + 
					CurrentColumn)*3+1]=(float)CurrentColumn*SquareSize;
				temppoints[(CurrentImage*NPoints*3)+(CurrentRow*ChessBoardSize.width + 
					CurrentColumn)*3+2]=0.f;
			}
		}
	}
	(*Corners3D) = cvMat(NImages*NPoints,3,CV_32FC1, temppoints);
}
void CCalibrateCamera::OnBnClickedButtonLoadlocalchessboard()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_BUTTON_LoadLocalChessBoard)->GetWindowText(str);
	if(str == "加载本地图片")
	{
		for(int i=0;i<MAX_PATH;i++)
		{
			RootDirectory[0] = '\0';
			szBuffer[0] = '\0';
		}
		
		for(int i =0 ;i < PictureCount; i++)
			for(int j = 0;j<MAX_PATH;j++)
				szFilename[i][j] = '\0';
		PictureCount = 0;

		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFilter = _T("Image文件(*.jpg;*.bmp;*.jpeg;*.tif;*.png)\0*.jpg;*.bmp;*.jpeg;*.tif;*.png\0所有文件(*.*)\0*.*\0");//要选择的文件后缀
		ofn.lpstrInitialDir = csFullPath;//默认的文件路径
		ofn.lpstrFile = szBuffer;//存放文件的缓冲区
		ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
		ofn.nFilterIndex = 0;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR;//标志如果是多选要加上OFN_ALLOWMULTISELECT
		BOOL bSel = GetOpenFileName(&ofn);
		int iii = sizeof(szBuffer);
		for (int i = 0; i < sizeof(szBuffer); i++)
		{
			if (szBuffer[i] == '\\')
			{
				szBuffer[i] = '/';
			}
			RootDirectory[i] = szBuffer[i];
			LoadPictureFlag = 1;
			if(szBuffer[i] == '\0')
			{
				if(szBuffer[i+1] != '\0')
				{
					for(int j = 0; szBuffer[i+1] != '\0';i++,j++ )
						szFilename[PictureCount][j] = szBuffer[i+1];
					PictureCount++;
				}else
					break;
			}
		}
		if(PictureCount > 0)
		{
			CString str1,str2;
			str1.Format("%s",RootDirectory);
			str2.Format("%d",PictureCount);
			MessageBox("已经从"+str1+"文件目录中载入"+str2+"张图片！！！！");
		}else
		{
			PictureCount++;
			CString str1,str2;
			str1.Format("%s",RootDirectory);
			str2.Format("%d",PictureCount);
			MessageBox("已经从"+str1+"文件目录中载入"+str2+"张图片！！！！");
		}
	}

	if(str == "启动视景")
	{
		VisionPictureCount = 0;
		ExitCalibrationDiaglog = 0;
		MaskActiveCottrol();
		GetDlgItem(IDC_BUTTON_ExitCalibration)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LoadLocalChessBoard)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LoadLocalChessBoard)->SetWindowText("停止启动");

		vpTransform* CalibrationfarTransform = (vpTransform*)vpTransform::find("CalibrationTransform");
		CalibrationfarTransform->setTranslate(CalibrationTransformx,CalibrationTransformy,CalibrationTransformz);
		CalibrationfarTransform->setRotate(CalibrationTransformh,CalibrationTransformp,CalibrationTransformr);

		vpWindow *vpWin = (vpWindow*)vpWindow::find("myWindow");
		//CWnd *pwnd = GetDesktopWindow(); //希望找到资源管理器  
		//HWND hwnd = pwnd->GetSafeHwnd(); //得到它的HWND 

		vpWin->setParent(NULL);
		vpWin->setBorderEnable(false);
		vpWin->setOrigin(20,400);
		vpWin->setSize(788,613);
		::SetWindowPos(vpWin->getWindow(),HWND_TOPMOST,20,400,0,0,SWP_NOMOVE |SWP_NOSIZE);	

		vpChannel *CalibrationChannel = (vpChannel*)vpChannel::find("CalibrationChannel");
		CalibrationChannel->setDrawArea(0,1,0,1);

		vpWin->setInputEnable(true);
		//vpWin->setCursorEnable(true);
		::SetFocus(vpWin->getWindow());
		LoadVisionFlag = 1;
		
	}
	if(str == "停止启动")
	{
		ExitCalibrationDiaglog = 1;
		RecoverActiveControl();
		GetDlgItem(IDC_BUTTON_LoadLocalChessBoard)->SetWindowText("启动视景");
		CString str;
		str.Format("%d",VisionPictureCount);
		MessageBox("已经成功截取"+str+"张图片！！！");
	}
}

void CCalibrateCamera::MaskActiveCottrol(void)
{

	GetDlgItem(IDC_EDIT_XCornorCount)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_YCornorCount)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SquareSize)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ChessDetectTimes)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_youxiaoshuziweishu)->EnableWindow(FALSE);

	GetDlgItem(IDC_RADIO_VisionChessBoard)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_LocalChessBoard)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LoadLocalChessBoard)->EnableWindow(FALSE);
	
	((CButton *)GetDlgItem(IDC_CHECK_FPP))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_CHECK_UIG))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_CHECK_FAR))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_CHECK_ZTD))->EnableWindow(FALSE);

	GetDlgItem(IDC_RADIO_RGB2Gray)->EnableWindow(FALSE);	
	GetDlgItem(IDC_RADIO_RGB2HLS)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_RGB2HSV)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_RGB2Luv)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_RGB2XYZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_RGB2YCrCb)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_ExitCalibration)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_StartCalibration)->EnableWindow(FALSE);

	m_Combox.EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_SettingBySystem)->EnableWindow(FALSE);
	RedrawWindow();
		
}

void CCalibrateCamera::RecoverActiveControl(void)
{
	GetDlgItem(IDC_RADIO_VisionChessBoard)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_LocalChessBoard)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LoadLocalChessBoard)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_XCornorCount)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_YCornorCount)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SquareSize)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ChessDetectTimes)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_youxiaoshuziweishu)->EnableWindow(TRUE);

	((CButton *)GetDlgItem(IDC_CHECK_FPP))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK_UIG))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK_FAR))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK_ZTD))->EnableWindow(TRUE);
	if(FrameProPRocessMethodFlag == 1)
	{
		GetDlgItem(IDC_RADIO_RGB2Gray)->EnableWindow(TRUE);	
		GetDlgItem(IDC_RADIO_RGB2HLS)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RGB2HSV)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RGB2Luv)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RGB2XYZ)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RGB2YCrCb)->EnableWindow(TRUE);
	}
	

	GetDlgItem(IDC_BUTTON_ExitCalibration)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_StartCalibration)->EnableWindow(TRUE);
	
	m_Combox.EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SettingBySystem)->EnableWindow(TRUE);
	
}

void CCalibrateCamera::OnBnClickedButtonExitcalibration()
{
	// TODO: Add your control notification handler code here
	ExitCalibrationFlag = 1;
}


void CCalibrateCamera::LoadImage2Control(IplImage* img, UINT ID)
{
	IplImage* Image_Show;
	CDC* pDC = GetDlgItem(ID)->GetDC();
	// 获取 HDC(设备句柄) 来进行绘图操作     
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	// 求出图片控件的宽和高
	int Rect_Width = rect.right - rect.left;
	int Rect_Height = rect.bottom - rect.top;
	// 读取图片的宽和高
	int Image_Width = img->width;
	int Image_Height = img->height;
	float Width_Ratio = (float)(Image_Width)/Rect_Width;
	float Height_Ratio = (float)(Image_Height)/Rect_Height;
	if(Width_Ratio >= Height_Ratio)
	{
		Image_Show = cvCreateImage(cvSize(Image_Width/Width_Ratio, Image_Height/Width_Ratio), img->depth, img->nChannels);
		cvResize(img,Image_Show);
		int tx = 0;
		int ty = (int)(Rect_Height - Image_Height/Width_Ratio) / 2;
		SetRect(rect, tx, ty, tx + Image_Width/Width_Ratio, ty + Image_Height/Width_Ratio);
		// 复制图片
		CvvImage cimg;
		cimg.CopyOf(Image_Show);
		// 将图片绘制到显示控件的指定区域内 
		cimg.DrawToHDC(hDC, &rect);
		ReleaseDC(pDC);
		cvReleaseImage(&Image_Show);

	}
	if (Width_Ratio < Height_Ratio)
	{
		Image_Show = cvCreateImage(cvSize(Image_Width/Height_Ratio, Image_Height/Height_Ratio), img->depth, img->nChannels);
		cvResize(img,Image_Show);
		int tx = (int)(Rect_Width - Image_Width/Height_Ratio) / 2;
		int ty = 0;
		SetRect(rect, tx, ty, tx + Image_Width/Height_Ratio, ty + Image_Height/Height_Ratio);
		// 复制图片
		CvvImage cimg;
		cimg.CopyOf(Image_Show);
		// 将图片绘制到显示控件的指定区域内 
		cimg.DrawToHDC(hDC, &rect);
		ReleaseDC(pDC);
		cvReleaseImage(&Image_Show);
		cimg.Destroy();

	}
		
	// 使图片的显示位置正好在控件的正中
}

void CCalibrateCamera::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(current_frame_rgb)
		LoadImage2Control(current_frame_rgb,IDC_STATIC_CalibrationPicture);
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

DWORD WINAPI CCalibrateCamera::CalibrationIng(LPVOID lpParameter)
{
	CCalibrateCamera* temp = (CCalibrateCamera*)lpParameter;
	temp->CamaeraCalibration(cvSize(Basic_Image_Width,Basic_Image_Height),
	                         cvSize(temp->m_XCornorCount, temp->m_YCornorCount), temp->m_SquareSize, PictureCount,
	                         temp->m_ChessBoardTimes);
	//CloseHandle(CalibrationHandle);
	return 0;
}
void CCalibrateCamera::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{

	}

	CDialog::OnTimer(nIDEvent);
}


void CCalibrateCamera::PassPama2ResultDisplay(float intr[][3],float* dist, float* rotv, float* tranv)
{
	ResultIntrinsicfx = intr[0][0];
	ResultIntrinsicfy = intr[1][1];
	ResultIntrinsiccx = intr[0][2];
	ResultIntrinsiccy = intr[1][2];
	ResultDistortionk1 = dist[0];
	ResultDistortionk2 = dist[1];
	ResultDistortionp1 = dist[2];
	ResultDistortionp2 = dist[3];
	ResultRotationr1 = rotv[0];
	ResultRotationr2 = rotv[1];
	ResultRotationr3 = rotv[2];
	ResultTranslationt1 = tranv[0];
	ResultTranslationt2 = tranv[1];
	ResultTranslationt3 = tranv[2];
}

void CCalibrateCamera::OnCbnSelchangeComboMethodchoose()
{
	switch(m_Combox.GetCurSel())
	{
	case 0:
		FrameProPRocessMethodFlag = 0;
		GetDlgItem(IDC_RADIO_RGB2Gray)->EnableWindow(FALSE);	
		GetDlgItem(IDC_RADIO_RGB2HLS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RGB2HSV)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RGB2Luv)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RGB2XYZ)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RGB2YCrCb)->EnableWindow(FALSE);
		break;
	case 1:
		FrameProPRocessMethodFlag = 1;
		GetDlgItem(IDC_RADIO_RGB2Gray)->EnableWindow(TRUE);	
		GetDlgItem(IDC_RADIO_RGB2HLS)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RGB2HSV)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RGB2Luv)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RGB2XYZ)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RGB2YCrCb)->EnableWindow(TRUE);
		break;
	case 2:
		FrameProPRocessMethodFlag = 2;
		GetDlgItem(IDC_RADIO_RGB2Gray)->EnableWindow(FALSE);	
		GetDlgItem(IDC_RADIO_RGB2HLS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RGB2HSV)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RGB2Luv)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RGB2XYZ)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RGB2YCrCb)->EnableWindow(FALSE);
		break;

	default:
		break;
	}

	// TODO: Add your control notification handler code here
}
