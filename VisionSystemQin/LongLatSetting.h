#pragma once


// CLongLatSetting dialog

class CLongLatSetting : public CDialog
{
	DECLARE_DYNAMIC(CLongLatSetting)

public:
	CLongLatSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLongLatSetting();

// Dialog Data
	enum { IDD = IDD_LongLatSetting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_EDIT_LongLatSettingLongLeftDu;
	int m_EDIT_LongLatSettingLongLeftFen;
	int m_EDIT_LongLatSettingLongLeftMiao;
	int m_EDIT_LongLatSettingLatLeftDu;
	int m_EDIT_LongLatSettingLatLeftFen;
	int m_EDIT_LongLatSettingLatLeftMiao;
	int m_EDIT_LongLatSettingLongRightDu;
	int m_EDIT_LongLatSettingLongRightFen;
	int m_EDIT_LongLatSettingLongRightMiao;
	int m_EDIT_LongLatSettingLatRightDu;
	int m_EDIT_LongLatSettingLatRightFen;
	int m_EDIT_LongLatSettingLatRightMiao;
	afx_msg void OnBnClickedButtonOk();
	double m_EDIT_LongLatSettingXLeft;
	double m_EDIT_LongLatSettingYLeft;
	double m_EDIT_LongLatSettingYRight;
	double m_EDIT_LongLatSettingXRight;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLoadlasthistoryrecord();
};
extern int LongLatSettingLongLeftDu, LongLatSettingLongLeftFen, LongLatSettingLongLeftMiao, 
LongLatSettingLatLeftDu, LongLatSettingLatLeftFen, LongLatSettingLatLeftMiao, 
LongLatSettingLongRightDu, LongLatSettingLongRightFen, LongLatSettingLongRightMiao, 
LongLatSettingLatRightDu, LongLatSettingLatRightFen, LongLatSettingLatRightMiao;
extern int LongLatCoordiateSettingFlag;
extern double LongLatSettingXLeft,LongLatSettingYLeft, LongLatSettingYRight, LongLatSettingXRight;
