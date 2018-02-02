#pragma once


// CSplasScreen dialog

class CSplasScreen : public CDialog
{
	DECLARE_DYNAMIC(CSplasScreen)

public:
	CSplasScreen(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSplasScreen();

// Dialog Data
	enum { IDD = IDD_SPLASHSCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
