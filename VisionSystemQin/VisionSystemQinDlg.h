// VisionSystemQinDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "VisionProcess.h"
#include "ResultDisplay.h"
#include "ImageProcess.h"
#include "Resource.h"

// CVisionSystemQinDlg �Ի���
class CVisionSystemQinDlg : public CDialog
{
// ����
public:
	CVisionSystemQinDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CVisionProcess VisionProcess;
	CResultDisplay ResultDisplay;
	CImageProcess ImageProcess;

// �Ի�������
	enum { IDD = IDD_VISIONSYSTEMQIN_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	static vpObject* RealTime_Drogue; 
	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��

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