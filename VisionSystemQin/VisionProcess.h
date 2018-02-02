#pragma once
#include "Resource.h"
#include "afxwin.h"

// CVisionProcess dialog

class CVisionProcess : public CDialog
{
	DECLARE_DYNAMIC(CVisionProcess)

public:
	CVisionProcess(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVisionProcess();

// Dialog Data
	enum { IDD = IDD_VisionProcess };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	float m_FontcolorR;
	float m_FontcolorG;
	float m_FontcolorB;
	afx_msg void OnBnClickedButtonAplicatesetting();
	afx_msg void OnBnClickedButtonDroguepositiondisplay();
	float m_FontX;
	float m_FontY;
	float m_CameraLeft;
	float m_CameraTop;
	float m_CameraDown;
	float m_CameraRight;
	afx_msg void OnBnClickedButtonCamerasetting();
	afx_msg void OnBnClickedButtonCameradisplay();
	afx_msg void OnPaint();
	CMFCButton m_LeftDistance;
	CMFCButton m_UpDistance;
	CMFCButton m_RightDistance;
	CMFCButton m_DownDistance;
	CMFCButton m_ForwardDistance;
	CMFCButton m_BackDistance;
	CMFCButton m_RotateXShun;
	CMFCButton m_RotateYShun;
	CMFCButton m_RotateZShun;
	CMFCButton m_RotateXNi;
	CMFCButton m_RotateYNi;
	CMFCButton m_RotateZNi;
	float m_DistanceSensitivity;
	float m_AngleSensitivity;
	float m_XPosition;
	float m_YPosition;
	float m_ZPosition;
	float m_PitchValue;
	float m_RollValue;
	float m_HeadingValue;
	afx_msg void OnBnClickedRadioLeftback();
	afx_msg void OnBnClickedRadioBack();
	afx_msg void OnBnClickedRadioFarback();
	afx_msg void OnBnClickedRadioUpback();
	afx_msg void OnBnClickedButtonModesetting();
	afx_msg void OnBnClickedButtonUpdistance();
	afx_msg void OnBnClickedButtonDowndistance();
	afx_msg void OnBnClickedButtonLeftdistance();
	afx_msg void OnBnClickedButtonRightdistance();
	afx_msg void OnBnClickedButtonForwarddistance();
	afx_msg void OnBnClickedButtonBackdistance();
	afx_msg void OnBnClickedButtonRotatexshun();
	afx_msg void OnBnClickedButtonRotatexni();
	afx_msg void OnBnClickedButtonRotateyshun();
	afx_msg void OnBnClickedButtonRotateyni();
	afx_msg void OnBnClickedButtonRotatezshun();
	afx_msg void OnBnClickedButtonRotatezni();
	afx_msg void OnBnClickedButtonSystemsetting();
	afx_msg void OnBnClickedRadioSunnyday();
	afx_msg void OnBnClickedRadioCloudyday();
	afx_msg void OnBnClickedRadioFogday();
	afx_msg void OnBnClickedRadioRainday();
	afx_msg void OnBnClickedRadioSnowday();
	afx_msg void OnBnClickedButtonClimatesetting();
	afx_msg void OnCbnSelchangeComboStylechoose();
	afx_msg void OnBnClickedButtonRecieverplanemonisetting();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonClimateopenlight();
	afx_msg void OnBnClickedRadioLightwenliu();
	afx_msg void OnBnClickedRadioMedianwenliu();
	afx_msg void OnBnClickedRadioServewenliu();
};
extern float FontcolorR,FontcolorG,FontcolorB,FontY,FontX,CameraLeft,CameraTop,CameraDown,CameraRight;
extern int ClimateSettingFlag;
extern int m_Slider_Disturbance_Value;
extern int OpenLightFlag;
extern int WenLiuModeChoose;