// ResultResport.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSystemQin.h"
#include "ResultResport.h"
#include "ImageProcess.h"

int LeaveDotNumber = 4;


// CResultResport dialog

IMPLEMENT_DYNAMIC(CResultResport, CDialog)

CResultResport::CResultResport(CWnd* pParent /*=NULL*/)
	: CDialog(CResultResport::IDD, pParent)
	, m_LeaveDotNumber(0)
{

}

CResultResport::~CResultResport()
{
}

void CResultResport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ResultReport, m_ListCtrlResultReport);
	DDX_Text(pDX,IDC_EDIT_LeaveDotNumber,m_LeaveDotNumber);
}


BEGIN_MESSAGE_MAP(CResultResport, CDialog)
	ON_EN_CHANGE(IDC_EDIT_LeaveDotNumber, &CResultResport::OnEnChangeEditLeavedotnumber)
	ON_BN_CLICKED(IDC_BUTTON_SaveResult, &CResultResport::OnBnClickedButtonSaveresult)
END_MESSAGE_MAP()


BOOL CResultResport::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ListCtrlResultReport.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);
	m_ListCtrlResultReport.InsertColumn( 0, "帧数", LVCFMT_CENTER, 60 );
	m_ListCtrlResultReport.InsertColumn( 1, "X方向定位精度(m)", LVCFMT_CENTER, 120);
	m_ListCtrlResultReport.InsertColumn( 2, "Y方向定位精度(m)", LVCFMT_CENTER, 120);
	m_ListCtrlResultReport.InsertColumn( 3, "Z方向定位精度(m)", LVCFMT_CENTER, 120);
	m_ListCtrlResultReport.InsertColumn( 4, "俯仰角(°)", LVCFMT_CENTER, 80);
	m_ListCtrlResultReport.InsertColumn( 5, "偏航角(°)", LVCFMT_CENTER, 80);
	m_ListCtrlResultReport.InsertColumn( 6, "处理时间(s)", LVCFMT_CENTER, 80);
	CString strCurrentFrame;
	strCurrentFrame.Format("%d",CurrentFrame);
	GetDlgItem(IDC_EDIT_CurrentFrame)->SetWindowText(strCurrentFrame);

	CString strLeaveDotNumber;
	strCurrentFrame.Format("%d",LeaveDotNumber);
	strLeaveDotNumber = "%."+ strCurrentFrame +"lf";
	GetDlgItem(IDC_EDIT_LeaveDotNumber)->SetWindowText(strCurrentFrame);



	for(int i = 0;i<CurrentFrame;i++)
	{
		CString str;
		str.Format("%d",i+1);
		m_ListCtrlResultReport.InsertItem(i, str);//插入行
		str.Format(strLeaveDotNumber,DrogueLocateAccurateXArray[i]);
		m_ListCtrlResultReport.SetItemText(i, 1, str);//设置数据
		str.Format(strLeaveDotNumber,DrogueLocateAccurateYArray[i]);
		m_ListCtrlResultReport.SetItemText(i, 2, str);//设置数据
		str.Format(strLeaveDotNumber,DrogueLocateAccurateZArray[i]);
		m_ListCtrlResultReport.SetItemText(i, 3, str);//设置数据
		str.Format(strLeaveDotNumber,ImageDetectPitch[i]);
		m_ListCtrlResultReport.SetItemText(i, 4, str);//设置数据
		str.Format(strLeaveDotNumber,ImageDetectHeading[i]);
		m_ListCtrlResultReport.SetItemText(i, 5, str);//设置数据
		str.Format(strLeaveDotNumber,FrameProcessTime[i]);
		m_ListCtrlResultReport.SetItemText(i, 6, str);//设置数据

	}

	return TRUE;
}
// CResultResport message handlers

void CResultResport::OnEnChangeEditLeavedotnumber()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);

	LeaveDotNumber = m_LeaveDotNumber;

	CString strLeaveDotNumber,strCurrentFrame;
	strCurrentFrame.Format("%d",LeaveDotNumber);
	strLeaveDotNumber = "%."+ strCurrentFrame +"lf";

	strCurrentFrame.Format("%d",CurrentFrame);
	GetDlgItem(IDC_EDIT_CurrentFrame)->SetWindowText(strCurrentFrame);
	m_ListCtrlResultReport.DeleteAllItems();

	for(int i = 0;i<CurrentFrame;i++)
	{
		CString str;
		str.Format("%d",i+1);
		m_ListCtrlResultReport.InsertItem(i, str);//插入行
		str.Format(strLeaveDotNumber,DrogueLocateAccurateXArray[i]);
		m_ListCtrlResultReport.SetItemText(i, 1, str);//设置数据
		str.Format(strLeaveDotNumber,DrogueLocateAccurateYArray[i]);
		m_ListCtrlResultReport.SetItemText(i, 2, str);//设置数据
		str.Format(strLeaveDotNumber,DrogueLocateAccurateZArray[i]);
		m_ListCtrlResultReport.SetItemText(i, 3, str);//设置数据
		str.Format(strLeaveDotNumber,ImageDetectPitch[i]);
		m_ListCtrlResultReport.SetItemText(i, 4, str);//设置数据
		str.Format(strLeaveDotNumber,ImageDetectHeading[i]);
		m_ListCtrlResultReport.SetItemText(i, 5, str);//设置数据
		str.Format(strLeaveDotNumber,FrameProcessTime[i]);
		m_ListCtrlResultReport.SetItemText(i, 6, str);//设置数据

	}

}

void CResultResport::OnBnClickedButtonSaveresult()
{
	// TODO: Add your control notification handler code here
	CFileDialog fg(false,_T("*.txt"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Text File(*.txt)|*.txt|All File(*.*)|*.*||"),NULL);
	if (fg.DoModal()==IDOK)
	{
		CString filename=fg.GetPathName();
		CStdioFile file(filename,CFile::modeCreate|CFile::modeWrite);
// 		CString str1 = "X方向定位精度(m)",str2 = "Z方向定位精度(m)",str3 = "偏航角(°)",str4 = "锥套基准位置(m)",str4 = "锥套定位位置(m)";
// 		CString str =  "第一列到最后一列依次为帧数," +str1 + ",Y方向定位精度(m)," +str2 + ",俯仰角(°)," + str3 +",处理时间(s)\n";
// 		file.WriteString(str);
		for (int i =0;i<CurrentFrame;i++)   
		{

			CString strLeaveDotNumber,strCurrentFrame;
			strCurrentFrame.Format("%d",LeaveDotNumber);
			strLeaveDotNumber = "%."+ strCurrentFrame +"lf";
			CString str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str;
			str1.Format("%d",i+1);
			str2.Format(strLeaveDotNumber,DrogueLocateAccurateXArray[i]);
			str3.Format(strLeaveDotNumber,DrogueLocateAccurateYArray[i]);
			str4.Format(strLeaveDotNumber,DrogueLocateAccurateZArray[i]);
			str5.Format(strLeaveDotNumber,ImageDetectPitch[i]);
			str6.Format(strLeaveDotNumber,ImageDetectHeading[i]);
			str7.Format(strLeaveDotNumber,FrameProcessTime[i]);
			str8.Format(strLeaveDotNumber,DrogueLocateXArray[i]);
			str9.Format(strLeaveDotNumber,DrogueLocateYArray[i]);
			str10.Format(strLeaveDotNumber,DrogueLocateZArray[i]);
			str11.Format(strLeaveDotNumber,DrogueSolveXArray[i]);
			str12.Format(strLeaveDotNumber,DrogueSolveYArray[i]);
			str13.Format(strLeaveDotNumber,DrogueSolveZArray[i]);
			str = str1 +"\t   "+str2 +"\t   "+str3 +"\t   "+str4 +"\t   "+str5 +"\t   "+str6 +"\t   "+str7 
				+"\t   "+str8 +"\t   "+str9 +"\t   "+str10 +"\t   "+str11 +"\t   "+str12 +"\t   "+"\t   "+str13+"\n";
			file.WriteString(str);
		}
		file.Close();
	}
}
