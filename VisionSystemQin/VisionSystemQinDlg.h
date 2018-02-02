// VisionSystemQinDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "VisionProcess.h"
#include "ResultDisplay.h"
#include "ImageProcess.h"
#include "Resource.h"

// CVisionSystemQinDlg 对话框
class CVisionSystemQinDlg : public CDialog
{
// 构造
public:
	CVisionSystemQinDlg(CWnd* pParent = NULL);	// 标准构造函数
	CVisionProcess VisionProcess;
	CResultDisplay ResultDisplay;
	CImageProcess ImageProcess;

// 对话框数据
	enum { IDD = IDD_VISIONSYSTEMQIN_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	static vpObject* RealTime_Drogue; 
	

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ReadDogueXYZ(void);
	void DrogueMotionPointValue(void);
	CTabCtrl m_Tab;
	CFont font_set;
	afx_msg void OnTcnSelchangeTabVisionimageresult(NMHDR *pNMHDR, LRESULT *pResult);
	static DWORD WINAPI VisionDisplay(LPVOID lpParameter);
	CRect GetCameraWndRect();
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	static bool WinMsgHandler(vrWindow* vrWin,vrWindow::Message id, int param1,int param2,int param3);

	afx_msg void OnBnClickedButtonWelcomeinterfaceonoff();
};
extern CRect VisionDisplayRect;
extern float yy;
extern double RealTimrFocusFx, RealTimeIntrinsiccx,RealTimeIntrinsiccy,RealTimrFocusFy;
extern int ContourCenterPointSolveMethodFlag;