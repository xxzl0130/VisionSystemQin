#pragma once


// CRecieverPlaneSetting dialog

class CRecieverPlaneSetting : public CDialog
{
	DECLARE_DYNAMIC(CRecieverPlaneSetting)

public:
	CRecieverPlaneSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRecieverPlaneSetting();

// Dialog Data
	enum { IDD = IDD_RecieverPlaneSetting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonRecoversystemsetting();
	double m_FinalRecieveX;
	double m_FinalRecieveY;
	double m_FinalRecieveZ;
	double m_StartRecieveX;
	double m_StartRecieveY;
	double m_StartRecieveZ;
	double m_RelativeSpeed;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
extern double FinalRecieverX,FinalRecieverY,FinalRecieverZ,
StartRecieverX,StartRecieverY,StartRecieverZ,RelativeSpeed;
