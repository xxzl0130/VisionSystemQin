#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "afxcmn.h"


#define WM_MY_MESSAGE (WM_USER+100) 
// CImageProcess dialog

class CImageProcess : public CDialog
{
	DECLARE_DYNAMIC(CImageProcess)

public:
	CImageProcess(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageProcess();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_ImageProcess };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonImagestartdroguedetect();
	void DroguePictureDetect(IplImage* CurrentVisionImage);
	afx_msg void OnBnClickedButtonDroguedetectdisplay();
	afx_msg void OnBnClickedButtonDroguedetectdynamicdisplay();
	afx_msg void OnBnClickedButtonDroguelocatedisplay();
	afx_msg void OnBnClickedButtonDroguelocatedynamicdisplay();
	afx_msg void OnBnClickedButtonDroguefollowdisplay();
	afx_msg void OnBnClickedButtonDroguefollowdynamicdisplay();
	void MaskControl(void);
	void RecoverControl(void);
	afx_msg void OnBnClickedRadioGuassnoise();
	afx_msg void OnBnClickedRadioUniformnoise();
	afx_msg void OnBnClickedRadioSaltnoise();
	afx_msg LRESULT WriteControl(WPARAM,LPARAM);
	void SaltNoise(cv::Mat& Image, double proportion);
	void GausssNoise(IplImage* image,int bili);
	void UniformNoise(IplImage* image,int bili);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int m_Edit_Dilate;
	int m_Edit_Erode;
	afx_msg void OnBnClickedButtonDilateerodeorder();
	afx_msg void OnCbnSelchangeComboDetectmethod();
	afx_msg void OnCbnSelchangeComboFilteringmethod();
	afx_msg void OnCbnSelchangeComboDroguelocatesolvemethod();
	afx_msg void OnEnChangeEditDilate();
	afx_msg void OnEnChangeEditErode();
	int myOtsu(IplImage* frame);
	afx_msg void OnBnClickedButtonStartfollow();
	afx_msg void OnEnChangeEditInreasearead();
	int m_IncreaseAreaD;
	afx_msg void OnBnClickedButtonLocatecurvedisplay();
	afx_msg void OnBnClickedButtonLockcamerachannel();
	CvMat* SolveAtitude_LHM(CvMat Input3DPoint, CvMat Input2DPoint, CvMat InitRotation);
	void ImagePointSortLeft(double* X, double* Y, double* FinalX, double* FinalY);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

extern int ExitCalibrationDiaglog;
extern bool Droguefollowdynamicdisplay,Droguefollowdisplay,Droguelocatedynamicdisplay,Droguelocatedisplay,
Droguedetectdynamicdisplay,Droguedetectdisplay;
extern double m_Slider_Bright_Value,m_Slider_Contrast_Value,m_Slider_Noise_Value;
extern int Choose_Noise_Flag;
extern int DetectfailureTimes,DetectTotalTimes;
extern CWnd* hWnd;
extern int Edit_Dilate,Edit_Erode,DisplayDroguelocateFlag,FirstFrameOverFlag;
extern double ImageDroguePositionX,ImageDroguePositionY,ImageDroguePositionZ,
		DrogueLocateAccurateX,DrogueLocateAccurateY,DrogueLocateAccurateZ;
extern bool StartFollowFlag,LockCameraChannelFlag;
extern int IncreaseAreaD;
extern double FollowAreaLeft,FollowAreaRight,FollowAreaUp,FollowAreaDown;
extern double DrogueLocateAccurateXArray[40000],DrogueLocateAccurateYArray[40000],
DrogueLocateAccurateZArray[40000],FrameProcessTime[40000],ImageDetectHeading[40000],
ImageDetectPitch[40000];
extern double DrogueLocateXArray[40000],DrogueLocateYArray[40000],
DrogueLocateZArray[40000],DrogueSolveXArray[40000],DrogueSolveYArray[40000],
DrogueSolveZArray[40000];
extern int CurrentFrame;
extern CWnd *globalImageProcessthis;
