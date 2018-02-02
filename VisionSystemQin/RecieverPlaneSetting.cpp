// RecieverPlaneSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSystemQin.h"
#include "RecieverPlaneSetting.h"


// CRecieverPlaneSetting dialog
double FinalRecieverX = -13.729466,FinalRecieverY = -33,FinalRecieverZ = -11,
StartRecieverX = -13.729466,StartRecieverY = -33,StartRecieverZ = -11,RelativeSpeed = 1.0;
CString RecieveYstr;

IMPLEMENT_DYNAMIC(CRecieverPlaneSetting, CDialog)

CRecieverPlaneSetting::CRecieverPlaneSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CRecieverPlaneSetting::IDD, pParent)
	,m_FinalRecieveX(FinalRecieverX)
	,m_FinalRecieveY(FinalRecieverY)
	,m_FinalRecieveZ(FinalRecieverZ)
	,m_StartRecieveX(StartRecieverX)
	,m_StartRecieveY(StartRecieverY)
	,m_StartRecieveZ(StartRecieverZ)
	,m_RelativeSpeed(RelativeSpeed)
{

}

CRecieverPlaneSetting::~CRecieverPlaneSetting()
{
}

void CRecieverPlaneSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_FinalRelativeX,m_FinalRecieveX);
	DDX_Text(pDX,IDC_EDIT_FinalRelativeY,m_FinalRecieveY);
	DDX_Text(pDX,IDC_EDIT_FinalRelativeZ,m_FinalRecieveZ);
	DDX_Text(pDX,IDC_EDIT_StartRelativeX,m_StartRecieveX);
	DDX_Text(pDX,IDC_EDIT_StartRelativeY,m_StartRecieveY);
	DDX_Text(pDX,IDC_EDIT_StartRelativeZ,m_StartRecieveZ);
	DDX_Text(pDX,IDC_EDIT_RelativeSpeed,m_RelativeSpeed);
}


BEGIN_MESSAGE_MAP(CRecieverPlaneSetting, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CRecieverPlaneSetting::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CRecieverPlaneSetting::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_RecoverSystemSetting, &CRecieverPlaneSetting::OnBnClickedButtonRecoversystemsetting)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRecieverPlaneSetting message handlers

BOOL CRecieverPlaneSetting::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_FinalRecieveX = FinalRecieverX;
	m_FinalRecieveY = FinalRecieverY;
	m_FinalRecieveZ = FinalRecieverZ;
	m_StartRecieveX = StartRecieverX;
	m_StartRecieveY = StartRecieverY;
	m_StartRecieveZ = StartRecieverZ;
	UpdateData(FALSE);
	SetTimer(1,500,NULL);


	return true;
}
void CRecieverPlaneSetting::OnBnClickedButtonOk()
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	FinalRecieverX = m_FinalRecieveX;
	FinalRecieverY = m_FinalRecieveY;
	FinalRecieverZ = m_FinalRecieveZ;
	RelativeSpeed = m_RelativeSpeed;
	EndDialog(0);
}

void CRecieverPlaneSetting::OnBnClickedButtonCancel()
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CRecieverPlaneSetting::OnBnClickedButtonRecoversystemsetting()
{
	// TODO: Add your control notification handler code here
	FinalRecieverX = -13.729466;
	FinalRecieverY = -33;
	FinalRecieverZ = -11;
	RelativeSpeed = 1.0;
	m_FinalRecieveX = FinalRecieverX;
	m_FinalRecieveY = FinalRecieverY;
	m_FinalRecieveZ = FinalRecieverZ;
	m_RelativeSpeed = RelativeSpeed;
	UpdateData(FALSE);
}

void CRecieverPlaneSetting::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		RecieveYstr.Format("%6.4f",StartRecieverY);
		GetDlgItem(IDC_EDIT_StartRelativeY)->SetWindowText(RecieveYstr);
	}

	CDialog::OnTimer(nIDEvent);
}
