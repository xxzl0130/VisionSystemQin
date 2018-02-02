// LongLatSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSystemQin.h"
#include "LongLatSetting.h"

int LongLatSettingLongLeftDu = 110, LongLatSettingLongLeftFen = 20, LongLatSettingLongLeftMiao = 30, 
LongLatSettingLatLeftDu = 40, LongLatSettingLatLeftFen = 10, LongLatSettingLatLeftMiao = 20, 
LongLatSettingLongRightDu = 111, LongLatSettingLongRightFen = 20, LongLatSettingLongRightMiao = 40, 
LongLatSettingLatRightDu = 40, LongLatSettingLatRightFen = 50, LongLatSettingLatRightMiao = 20;
int LongLatCoordiateSettingFlag = 0;
double LongLatSettingXLeft = 0,LongLatSettingYLeft = 0, LongLatSettingYRight = 20000, LongLatSettingXRight = 10000;
// CLongLatSetting dialog

IMPLEMENT_DYNAMIC(CLongLatSetting, CDialog)

CLongLatSetting::CLongLatSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CLongLatSetting::IDD, pParent)
	, m_EDIT_LongLatSettingLongLeftDu(0)
	, m_EDIT_LongLatSettingLongLeftFen(0)
	, m_EDIT_LongLatSettingLongLeftMiao(0)
	, m_EDIT_LongLatSettingLatLeftDu(0)
	, m_EDIT_LongLatSettingLatLeftFen(0)
	, m_EDIT_LongLatSettingLatLeftMiao(0)
	, m_EDIT_LongLatSettingLongRightDu(0)
	, m_EDIT_LongLatSettingLongRightFen(0)
	, m_EDIT_LongLatSettingLongRightMiao(0)
	, m_EDIT_LongLatSettingLatRightDu(0)
	, m_EDIT_LongLatSettingLatRightFen(0)
	, m_EDIT_LongLatSettingLatRightMiao(0)
	, m_EDIT_LongLatSettingXLeft(0)
	, m_EDIT_LongLatSettingYLeft(0)
	, m_EDIT_LongLatSettingYRight(0)
	, m_EDIT_LongLatSettingXRight(0)
{

}

CLongLatSetting::~CLongLatSetting()
{
}

void CLongLatSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLongLeftDu, m_EDIT_LongLatSettingLongLeftDu);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLongLeftFen, m_EDIT_LongLatSettingLongLeftFen);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLongLeftMiao, m_EDIT_LongLatSettingLongLeftMiao);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLatLeftDu, m_EDIT_LongLatSettingLatLeftDu);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLatLeftFen, m_EDIT_LongLatSettingLatLeftFen);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLatLeftMiao, m_EDIT_LongLatSettingLatLeftMiao);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLongRightDu, m_EDIT_LongLatSettingLongRightDu);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLongRightFen, m_EDIT_LongLatSettingLongRightFen);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLongRightMiao, m_EDIT_LongLatSettingLongRightMiao);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLatRightDu, m_EDIT_LongLatSettingLatRightDu);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLatRightFen, m_EDIT_LongLatSettingLatRightFen);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingLatRightMiao, m_EDIT_LongLatSettingLatRightMiao);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingXLeft, m_EDIT_LongLatSettingXLeft);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingYLeft, m_EDIT_LongLatSettingYLeft);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingYRight, m_EDIT_LongLatSettingYRight);
	DDX_Text(pDX, IDC_EDIT_LongLatSettingXRight, m_EDIT_LongLatSettingXRight);
}


BEGIN_MESSAGE_MAP(CLongLatSetting, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CLongLatSetting::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_LoadLastHistoryRecord, &CLongLatSetting::OnBnClickedButtonLoadlasthistoryrecord)
END_MESSAGE_MAP()


// CLongLatSetting message handlers

void CLongLatSetting::OnBnClickedButtonOk()
{
	//UpdateData(false);
	UpdateData(TRUE);
	LongLatSettingLongLeftDu = m_EDIT_LongLatSettingLongLeftDu;
	LongLatSettingLongLeftFen = m_EDIT_LongLatSettingLongLeftFen;
	LongLatSettingLongLeftMiao = m_EDIT_LongLatSettingLongLeftMiao; 
	LongLatSettingLatLeftDu = m_EDIT_LongLatSettingLatLeftDu;
	LongLatSettingLatLeftFen = m_EDIT_LongLatSettingLatLeftFen;
	LongLatSettingLatLeftMiao = m_EDIT_LongLatSettingLatLeftMiao;
	LongLatSettingLongRightDu = m_EDIT_LongLatSettingLongRightDu;
	LongLatSettingLongRightFen = m_EDIT_LongLatSettingLongRightFen;
	LongLatSettingLongRightMiao = m_EDIT_LongLatSettingLongRightMiao;
	LongLatSettingLatRightDu = m_EDIT_LongLatSettingLatRightDu;
	LongLatSettingLatRightFen = m_EDIT_LongLatSettingLatRightFen;
	LongLatSettingLatRightMiao = m_EDIT_LongLatSettingLatRightMiao;
	LongLatSettingXLeft = m_EDIT_LongLatSettingXLeft;
	LongLatSettingYLeft =  m_EDIT_LongLatSettingYLeft;
	LongLatSettingYRight =  m_EDIT_LongLatSettingYRight;
	LongLatSettingXRight = m_EDIT_LongLatSettingXRight;
	if(LongLatSettingLongLeftDu >= 180)
		MessageBox("左边界输入经度度数不能超过180，请重新输入！");
	else if(LongLatSettingLongLeftDu < 0)
		MessageBox("左边界输入经度度数不能小于0，请重新输入！");
	else if(LongLatSettingLongLeftFen >= 60)
		MessageBox("左边界输入经度分数不能超过60，请重新输入！");
	else if(LongLatSettingLongLeftFen < 0)
		MessageBox("左边界输入经度分数不能小于0，请重新输入！");
	else if(LongLatSettingLongLeftMiao >= 60)
		MessageBox("左边界输入经度秒数不能超过60，请重新输入！");
	else if(LongLatSettingLongLeftMiao < 0)
		MessageBox("左边界输入经度秒数不能小于0，请重新输入！");
	else if(LongLatSettingLatLeftDu >= 180)
		MessageBox("左边界输入纬度度数不能超过180，请重新输入！");
	else if(LongLatSettingLatLeftDu < 0)
		MessageBox("左边界输入纬度度数不能小于0，请重新输入！");
	else if(LongLatSettingLatLeftFen >= 60)
		MessageBox("左边界输入纬度分数不能超过60，请重新输入！");
	else if(LongLatSettingLatLeftFen < 0)
		MessageBox("左边界输入纬度分数不能小于0，请重新输入！");
	else if(LongLatSettingLatLeftMiao >= 60)
		MessageBox("左边界输入纬度秒数不能超过60，请重新输入！");
	else if(LongLatSettingLatLeftMiao < 0)
		MessageBox("左边界输入纬度秒数不能小于0，请重新输入！");
	else if(LongLatSettingLongRightDu >= 180)
		MessageBox("右边界输入经度度数不能超过180，请重新输入！");
	else if(LongLatSettingLongRightDu < 0)
		MessageBox("右边界输入经度度数不能小于0，请重新输入！");
	else if(LongLatSettingLongRightFen >= 60)
		MessageBox("右边界输入经度分数不能超过60，请重新输入！");
	else if(LongLatSettingLongRightFen < 0)
		MessageBox("右边界输入经度分数不能小于0，请重新输入！");
	else if(LongLatSettingLongRightMiao >= 60)
		MessageBox("右边界输入经度秒数不能超过60，请重新输入！");
	else if(LongLatSettingLongRightMiao < 0)
		MessageBox("右边界输入经度秒数不能小于0，请重新输入！");
	else if(LongLatSettingLatRightDu >= 180)
		MessageBox("右边界输入纬度度数不能超过180，请重新输入！");
	else if(LongLatSettingLatRightDu < 0)
		MessageBox("右边界输入纬度度数不能小于0，请重新输入！");
	else if(LongLatSettingLatRightFen >= 60)
		MessageBox("右边界输入纬度分数不能超过60，请重新输入！");
	else if(LongLatSettingLatRightFen < 0)
		MessageBox("右边界输入纬度分数不能小于0，请重新输入！");
	else if(LongLatSettingLatRightMiao >= 60)
		MessageBox("右边界输入纬度秒数不能超过60，请重新输入！");
	else if(LongLatSettingLatRightMiao < 0)
		MessageBox("右边界输入纬度秒数不能小于0，请重新输入！");
	else
	{
		CStdioFile file("HistoryRecord\\HistoryRecord.txt",CFile::modeCreate|CFile::modeWrite);
		file.SetLength(0);
		CString str,str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16;
		str1.Format("%d",LongLatSettingLongLeftDu);
		str2.Format("%d",LongLatSettingLongLeftFen);
		str3.Format("%d",LongLatSettingLongLeftMiao);
		str4.Format("%d",LongLatSettingLatLeftDu);
		str5.Format("%d",LongLatSettingLatLeftFen);
		str6.Format("%d",LongLatSettingLatLeftMiao);
		str7.Format("%d",LongLatSettingLongRightDu);
		str8.Format("%d",LongLatSettingLongRightFen);
		str9.Format("%d",LongLatSettingLongRightMiao);
		str10.Format("%d",LongLatSettingLatRightDu);
		str11.Format("%d",LongLatSettingLatRightFen);
		str12.Format("%d",LongLatSettingLatRightMiao);
		str13.Format("%f",LongLatSettingXLeft);
		str14.Format("%f",LongLatSettingYLeft);
		str15.Format("%f",LongLatSettingYRight);
		str16.Format("%f",LongLatSettingXRight);
		str = str1 +" "+str2 +" "+str3 +" "+str4 +" "+str5 +" "+str6 +" "+str7 +" "+str8 +
			" "+str9 +" "+str10 +" "+str11 +" "+str12 +" "+str13 +" "+str14 +" "+str15 +" "+str16;
		file.WriteString(str);

		file.Close();
		LongLatCoordiateSettingFlag = 1;
		EndDialog(0); 
	}
	
	// TODO: Add your control notification handler code here
}

BOOL CLongLatSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EDIT_LongLatSettingLongLeftDu = LongLatSettingLongLeftDu;
	m_EDIT_LongLatSettingLongLeftFen = LongLatSettingLongLeftFen;
	m_EDIT_LongLatSettingLongLeftMiao = LongLatSettingLongLeftMiao; 
	m_EDIT_LongLatSettingLatLeftDu = LongLatSettingLatLeftDu;
	m_EDIT_LongLatSettingLatLeftFen = LongLatSettingLatLeftFen;
	m_EDIT_LongLatSettingLatLeftMiao = LongLatSettingLatLeftMiao;
	m_EDIT_LongLatSettingLongRightDu = LongLatSettingLongRightDu;
	m_EDIT_LongLatSettingLongRightFen = LongLatSettingLongRightFen;
	m_EDIT_LongLatSettingLongRightMiao = LongLatSettingLongRightMiao;
	m_EDIT_LongLatSettingLatRightDu = LongLatSettingLatRightDu;
	m_EDIT_LongLatSettingLatRightFen = LongLatSettingLatRightFen;
	m_EDIT_LongLatSettingLatRightMiao = LongLatSettingLatRightMiao;
	m_EDIT_LongLatSettingXLeft = LongLatSettingXLeft;
	m_EDIT_LongLatSettingYLeft =  LongLatSettingYLeft;
	m_EDIT_LongLatSettingYRight =  LongLatSettingYRight;
	m_EDIT_LongLatSettingXRight = LongLatSettingXRight;
	UpdateData(FALSE);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLongLatSetting::OnBnClickedButtonLoadlasthistoryrecord()
{

	FILE *fpx;
	int i = 0, j = 0;
	double HistoryRecord[16];
	fpx = fopen("HistoryRecord\\HistoryRecord.txt", "r");
	if(fpx == NULL)
	{
		MessageBox("未检测到历史记录数据，请手动输入！");
		return;
	}
	while(!feof(fpx))
	{
		if(i != 16)
		{
			fscanf(fpx, "%lf", &HistoryRecord[i]);//注意此处有个空格
			i++;
		}
		
	}
	fclose(fpx);
	fpx = NULL;
	m_EDIT_LongLatSettingLongLeftDu = HistoryRecord[0];
	m_EDIT_LongLatSettingLongLeftFen = HistoryRecord[1];
	m_EDIT_LongLatSettingLongLeftMiao = HistoryRecord[2]; 
	m_EDIT_LongLatSettingLatLeftDu = HistoryRecord[3];
	m_EDIT_LongLatSettingLatLeftFen = HistoryRecord[4];
	m_EDIT_LongLatSettingLatLeftMiao = HistoryRecord[5];
	m_EDIT_LongLatSettingLongRightDu = HistoryRecord[6];
	m_EDIT_LongLatSettingLongRightFen = HistoryRecord[7];
	m_EDIT_LongLatSettingLongRightMiao = HistoryRecord[8];
	m_EDIT_LongLatSettingLatRightDu = HistoryRecord[9];
	m_EDIT_LongLatSettingLatRightFen = HistoryRecord[10];
	m_EDIT_LongLatSettingLatRightMiao = HistoryRecord[11];
	m_EDIT_LongLatSettingXLeft = HistoryRecord[12];
	m_EDIT_LongLatSettingYLeft =  HistoryRecord[13];
	m_EDIT_LongLatSettingYRight =  HistoryRecord[14];
	m_EDIT_LongLatSettingXRight = HistoryRecord[15];
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}
