#pragma once
#include "mschart.h"
#include "afxwin.h"

// CShowMschart dialog

class CShowMschart : public CDialog
{
	DECLARE_DYNAMIC(CShowMschart)

public:
	CShowMschart(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShowMschart();

// Dialog Data
	enum { IDD = IDD_Mschart };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	CMSChart m_Chart;
	CStatic m_Picture;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_MschartChooseCurve;
	CComboBox m_MschartChooseDisplayMode;
	CString m_MschartTitleSetting;
	int m_MschartBackgroundColorR;
	int m_MschartBackgroundColorG;
	int m_MschartBackgroundColorB;
	int m_MschartOtherColorR;
	int m_MschartOtherColorG;
	int m_MschartOtherColorB;
	int m_MschartCurveColorR;
	int m_MschartCurveColorG;
	int m_MschartCurveColorB;
	int m_MschartCurveWidth;
	CButton m_MschartCurveValueDisplay;
	CButton m_MschartCurveMarkDisplay;
	CComboBox m_MschartCurveMarkPattern;
	int m_MschartCurveMarkColorR;
	int m_MschartCurveMarkColorG;
	int m_MschartCurveMarkColorB;
	int m_MschartXAxisMaxValue;
	int m_MschartXAxisMarkMeanDistance;
	int m_MschartXAxisMeanDistance;
	double m_MschartYAxisMaxValue;
	int m_MschartYAxisMeanDistance;
	CButton m_MschartSecondYAxisDisplay;
	int m_MschartYAxisDisplaykeshujiange;
	CComboBox m_MschartDynamicRefreshSpeed;
	CButton m_MschartSettingBySystem;
	afx_msg void OnBnClickedButtonMschartapplysetting();
	afx_msg void OnCbnSelchangeComboMschartchoosecurve();
	afx_msg void OnCbnSelchangeComboMschartchoosedisplaymode();
	afx_msg void OnCbnSelchangeComboMschartdynamicrefreshspeed();
	afx_msg void OnCbnSelchangeComboMschartcurvemarkpattern();
	afx_msg void OnBnClickedButtonMschartcurvevaluedisplay();
	afx_msg void OnBnClickedButtonMschartcurvemarkdisplay();
	afx_msg void OnBnClickedButtonMschartsecondyaxisdisplay();
	afx_msg void OnBnClickedButtonMschartsettingbysystem();
	static DWORD WINAPI CurveDisplay(LPVOID lpParameter);

	double m_MschartYAxisMinValue;
	afx_msg void OnBnClickedButtonMschartexit();
};

extern int MschartBackgroundColorR,MschartBackgroundColorG,MschartBackgroundColorB,MschartOtherColorR,MschartOtherColorG,
MschartOtherColorB,MschartCurveColorR,MschartCurveColorG,MschartCurveColorB,MschartCurveWidth,MschartCurveMarkColorR,
MschartCurveMarkColorG,MschartCurveMarkColorB,MschartXAxisMaxValue,MschartXAxisMarkMeanDistance,
MschartXAxisMeanDistance,MschartYAxisMeanDistance,MschartYAxisDisplaykeshujiange;
extern double MschartYAxisMaxValue,MschartYAxisMinValue;
extern CShowMschart *FlightControlCurveDisplay1;

