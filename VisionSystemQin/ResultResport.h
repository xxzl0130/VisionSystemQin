#pragma once
#include "afxcmn.h"


// CResultResport dialog

class CResultResport : public CDialog
{
	DECLARE_DYNAMIC(CResultResport)

public:
	CResultResport(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResultResport();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_ResultReport };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrlResultReport;
	int m_LeaveDotNumber;
	afx_msg void OnEnChangeEditLeavedotnumber();
	afx_msg void OnBnClickedButtonSaveresult();
};
