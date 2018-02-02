#pragma once
#include "Resource.h"

// CResultDisplay dialog

class CResultDisplay : public CDialog
{
	DECLARE_DYNAMIC(CResultDisplay)

public:
	CResultDisplay(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResultDisplay();
	vpObject *pReceiver;	//受油机
	vpObject *pTanker;		//加油机
	vpObject *pDrogue;	//受油机
	vpObject *Ring;
	vpTransform *CameraTranform;

// Dialog Data
	enum { IDD = IDD_ResultDisplay };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_Resultyouxiaoshuziweishu;
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonChangedisplatcoordiate();
	afx_msg void OnBnClickedButtonResultdynamicdisplay();
	afx_msg void OnBnClickedButtonResultreport();
	afx_msg void OnBnClickedButtonMapdisplay();
	afx_msg void OnBnClickedButtonLonglatsetting();
};
extern float ResultIntrinsicfx,ResultIntrinsicfy,ResultIntrinsiccx,ResultIntrinsiccy,ResultDistortionk1,
	ResultDistortionk2,ResultDistortionp1,ResultDistortionp2,ResultRotationr1,ResultRotationr2,ResultRotationr3,
	ResultTranslationt1,ResultTranslationt2,ResultTranslationt3,ResultTankerX,ResultTankerY,ResultTankerZ,
	ResultTankerPitch,ResultTankerRoll,ResultTankerHeading,ResultRecieverX,ResultRecieverY,ResultRecieverZ,
	ResultRecieverPitch,ResultRecieverRoll,ResultRecieverHeading,ResultProbeX,ResultProbeY,ResultProbeZ,
	ResultDrogueX,ResultDrogueY,ResultDrogueZ,ResultImageProcessDrogueX,ResultImageProcessDrogueY,ResultImageProcessDrogueZ,
	ResultPositioningAccuracyX,ResultPositioningAccuracyY,ResultPositioningAccuracyZ,ResultAtitudeHeading,ResultAtitudePitch;
extern int VisionStartOverFlag;


