#pragma once
#include "MAPX.H"

// CShowMap dialog

class CShowMap : public CDialog
{
	DECLARE_DYNAMIC(CShowMap)

public:
	CShowMap(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShowMap();

// Dialog Data
	enum { IDD = IDD_ShowMapX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOnlarge();
	CMapX *m_ctrMap;
	CMapX m_ctrMapX;
	afx_msg void OnBnClickedButtonOnsmall();
	afx_msg void OnBnClickedButtonOnmove();
	afx_msg void OnBnClickedButtonOnmiddle();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	afx_msg void OnMenuonlarge();
	afx_msg void OnMenuonsmall();
	afx_msg void OnMenuonmove();
	afx_msg void OnMenuonmiddle();
	afx_msg void OnMenuonprojection();
	afx_msg void OnMenuShowwholemap();
	afx_msg void OnMenumousecontrol();
	afx_msg void OnBnClickedButtontest();
	double DUFenMiao2DotDu(int Du, int Fen, int Miao);
	vuVec3<int> DotDu2DuFenMiao(double DotDu);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStarttrackinglocation();
	afx_msg void OnBnClickedButtonShowflightline();
	afx_msg void OnBnClickedRadioLocklocation();
	afx_msg void OnBnClickedRadioShowfree();
};
