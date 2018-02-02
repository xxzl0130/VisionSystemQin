// ShowMschart.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSystemQin.h"
#include "ShowMschart.h"

#include "mschart.h" //�����ص�ͷ�ļ�
#include "VcAxis.h"
#include "VcAxisTitle.h"
#include "VcTextLayout.h"
#include "VcDataGrid.h"
#include "VcPlot.h"
#include "VcValueScale.h"
#include "VcSeriesCollection.h"
#include "VcSeries.h"
#include "VcPen.h"
#include "VcCategoryScale.h"
#include "VcColor.h"
#include "VcDataGrid.h"
#include "VcBackdrop.h"
#include "VcFill.h"
#include "VcBrush.h"
#include "VcDataPoints.h"
#include "VcDataPoint.h"
#include "VcDataPointLabel.h"
#include "VcAxisTitle.h"
#include "VcAxisGrid.h"
#include "VcAxisScale.h"
#include "VcFont.h"
#include "VcLabels.h"
#include "VcLabel.h"
#include "VcLegend.h"
#include "VcMarker.h"
#include "VcSeriesMarker.h"
#include "VcTitle.h"

#include "ImageProcess.h"

int MschartChooseCurveFlag,MschartChooseDisplayModeFlag,MschartCurveMarkPatternFlag,MschartDynamicRefreshSpeedFlag;
bool MschartCurveValueDisplayFlag,MschartCurveMarkDisplayFlag,MschartSecondYAxisDisplayFlag;
int OverFlag = 0,LoadCurveFlag;

int MschartBackgroundColorR = 0,MschartBackgroundColorG = 0,MschartBackgroundColorB = 0,
MschartOtherColorR = 0,MschartOtherColorG = 255,MschartOtherColorB = 0,MschartCurveColorR = 255,
MschartCurveColorG = 0,MschartCurveColorB = 0,MschartCurveWidth = 30,MschartCurveMarkColorR = 255,
MschartCurveMarkColorG = 0,MschartCurveMarkColorB = 0,MschartXAxisMaxValue = 128,
MschartXAxisMarkMeanDistance = 16,MschartXAxisMeanDistance = 16,MschartYAxisMeanDistance = 10,
MschartYAxisDisplaykeshujiange = 1;
double MschartYAxisMaxValue = 0.1,MschartYAxisMinValue = -0.1;
CShowMschart *FlightControlCurveDisplay1;

// CShowMschart dialog

IMPLEMENT_DYNAMIC(CShowMschart, CDialog)

CShowMschart::CShowMschart(CWnd* pParent /*=NULL*/)
	: CDialog(CShowMschart::IDD, pParent)
	, m_MschartTitleSetting(_T("��λ��������ͼ"))
	, m_MschartBackgroundColorR(0)
	, m_MschartBackgroundColorG(0)
	, m_MschartBackgroundColorB(0)
	, m_MschartOtherColorR(0)
	, m_MschartOtherColorG(255)
	, m_MschartOtherColorB(0)
	, m_MschartCurveColorR(255)
	, m_MschartCurveColorG(0)
	, m_MschartCurveColorB(0)
	, m_MschartCurveWidth(30)
	, m_MschartCurveMarkColorR(255)
	, m_MschartCurveMarkColorG(0)
	, m_MschartCurveMarkColorB(0)
	, m_MschartXAxisMaxValue(128)
	, m_MschartXAxisMarkMeanDistance(16)
	, m_MschartXAxisMeanDistance(16)
	, m_MschartYAxisMaxValue(0.1)
	, m_MschartYAxisMeanDistance(-0.1)
	, m_MschartYAxisDisplaykeshujiange(1)
	, m_MschartYAxisMinValue(0)
{

}

CShowMschart::~CShowMschart()
{
}

void CShowMschart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCHART1, m_Chart);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_COMBO_MschartChooseCurve, m_MschartChooseCurve);
	DDX_Control(pDX, IDC_COMBO_MschartChooseDisplayMode, m_MschartChooseDisplayMode);
	DDX_Control(pDX, IDC_BUTTON_MschartCurveValueDisplay, m_MschartCurveValueDisplay);
	DDX_Control(pDX, IDC_BUTTON_MschartCurveMarkDisplay, m_MschartCurveMarkDisplay);
	DDX_Control(pDX, IDC_COMBO_MschartCurveMarkPattern, m_MschartCurveMarkPattern);
	DDX_Control(pDX, IDC_BUTTON_MschartSecondYAxisDisplay, m_MschartSecondYAxisDisplay);
	DDX_Control(pDX, IDC_COMBO_MschartDynamicRefreshSpeed, m_MschartDynamicRefreshSpeed);
	DDX_Control(pDX, IDC_BUTTON_MschartSettingBySystem, m_MschartSettingBySystem);
	DDX_Text(pDX,IDC_EDIT_MschartTitleSetting,m_MschartTitleSetting);
	DDX_Text(pDX,IDC_EDIT_MschartBackgroundColorR,m_MschartBackgroundColorR);
	DDX_Text(pDX,IDC_EDIT_MschartBackgroundColorG,m_MschartBackgroundColorG);
	DDX_Text(pDX,IDC_EDIT_MschartBackgroundColorB,m_MschartBackgroundColorB);
	DDX_Text(pDX,IDC_EDIT_MschartOtherColorR,m_MschartOtherColorR);
	DDX_Text(pDX,IDC_EDIT_MschartOtherColorG,m_MschartOtherColorG);
	DDX_Text(pDX,IDC_EDIT_MschartOtherColorB,m_MschartOtherColorB);
	DDX_Text(pDX,IDC_EDIT_MschartCurveColorR,m_MschartCurveColorR);
	DDX_Text(pDX,IDC_EDIT_MschartCurveColorG,m_MschartCurveColorG);
	DDX_Text(pDX,IDC_EDIT_MschartCurveColorB,m_MschartCurveColorB);
	DDX_Text(pDX,IDC_EDIT_MschartCurveWidth,m_MschartCurveWidth);
	DDX_Text(pDX,IDC_EDIT_MschartCurveMarkColorR,m_MschartCurveMarkColorR);
	DDX_Text(pDX,IDC_EDIT_MschartCurveMarkColorG,m_MschartCurveMarkColorG);
	DDX_Text(pDX,IDC_EDIT_MschartCurveMarkColorB,m_MschartCurveMarkColorB);
	DDX_Text(pDX,IDC_EDIT_MschartXAxisMaxValue,m_MschartXAxisMaxValue);
	DDX_Text(pDX,IDC_EDIT_MschartXAxisMarkMeanDistance,m_MschartXAxisMarkMeanDistance);
	DDX_Text(pDX,IDC_EDIT_MschartXAxisMeanDistance,m_MschartXAxisMeanDistance);
	DDX_Text(pDX,IDC_EDIT_MschartYAxisMaxValue,m_MschartYAxisMaxValue);
	DDX_Text(pDX,IDC_EDIT_MschartYAxisMinValue,m_MschartYAxisMinValue);
	DDX_Text(pDX,IDC_EDIT_MschartYAxisMeanDistance,m_MschartYAxisMeanDistance);
	DDX_Text(pDX,IDC_EDIT_MschartYAxisDisplaykeshujiange,m_MschartYAxisDisplaykeshujiange);

}


BEGIN_MESSAGE_MAP(CShowMschart, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MschartApplySetting, &CShowMschart::OnBnClickedButtonMschartapplysetting)
	ON_CBN_SELCHANGE(IDC_COMBO_MschartChooseCurve, &CShowMschart::OnCbnSelchangeComboMschartchoosecurve)
	ON_CBN_SELCHANGE(IDC_COMBO_MschartChooseDisplayMode, &CShowMschart::OnCbnSelchangeComboMschartchoosedisplaymode)
	ON_CBN_SELCHANGE(IDC_COMBO_MschartDynamicRefreshSpeed, &CShowMschart::OnCbnSelchangeComboMschartdynamicrefreshspeed)
	ON_CBN_SELCHANGE(IDC_COMBO_MschartCurveMarkPattern, &CShowMschart::OnCbnSelchangeComboMschartcurvemarkpattern)
	ON_BN_CLICKED(IDC_BUTTON_MschartCurveValueDisplay, &CShowMschart::OnBnClickedButtonMschartcurvevaluedisplay)
	ON_BN_CLICKED(IDC_BUTTON_MschartCurveMarkDisplay, &CShowMschart::OnBnClickedButtonMschartcurvemarkdisplay)
	ON_BN_CLICKED(IDC_BUTTON_MschartSecondYAxisDisplay, &CShowMschart::OnBnClickedButtonMschartsecondyaxisdisplay)
	ON_BN_CLICKED(IDC_BUTTON_MschartSettingBySystem, &CShowMschart::OnBnClickedButtonMschartsettingbysystem)
	ON_BN_CLICKED(IDC_BUTTON_MschartExit, &CShowMschart::OnBnClickedButtonMschartexit)
END_MESSAGE_MAP()

BOOL CShowMschart::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_MschartChooseCurve.AddString(_T("X�����������"));
	m_MschartChooseCurve.AddString(_T("Y�����������"));
	m_MschartChooseCurve.AddString(_T("Z�����������"));
	m_MschartChooseCurve.AddString(_T("֡����ʱ������"));
	m_MschartChooseCurve.AddString(_T("��������̬����"));
	m_MschartChooseCurve.AddString(_T("ƫ������̬����"));
	m_MschartChooseCurve.AddString(_T("X�����׼����"));
	m_MschartChooseCurve.AddString(_T("Y�����׼����"));
	m_MschartChooseCurve.AddString(_T("Z�����׼����"));
	m_MschartChooseCurve.AddString(_T("X�����������"));
	m_MschartChooseCurve.AddString(_T("Y�����������"));
	m_MschartChooseCurve.AddString(_T("Z�����������"));


	m_MschartChooseDisplayMode.AddString(_T("��̬��ʾ"));
	m_MschartChooseDisplayMode.AddString(_T("��̬�ۻ���ʾ"));
	m_MschartChooseDisplayMode.AddString(_T("��̬������ʾ"));

	m_MschartDynamicRefreshSpeed.AddString(_T("10msˢ��һ��"));
	m_MschartDynamicRefreshSpeed.AddString(_T("20msˢ��һ��"));
	m_MschartDynamicRefreshSpeed.AddString(_T("50msˢ��һ��"));
	m_MschartDynamicRefreshSpeed.AddString(_T("100msˢ��һ��"));
	m_MschartDynamicRefreshSpeed.AddString(_T("200msˢ��һ��"));
	m_MschartDynamicRefreshSpeed.AddString(_T("500msˢ��һ��"));
	m_MschartDynamicRefreshSpeed.AddString(_T("1000msˢ��һ��"));

	m_MschartCurveMarkPattern.AddString(_T("���"));
	m_MschartCurveMarkPattern.AddString(_T("ʮ��"));
	m_MschartCurveMarkPattern.AddString(_T("��"));
	m_MschartCurveMarkPattern.AddString(_T("��"));
	m_MschartCurveMarkPattern.AddString(_T("ԲȦ"));
	m_MschartCurveMarkPattern.AddString(_T("����"));
	m_MschartCurveMarkPattern.AddString(_T("����"));
	m_MschartCurveMarkPattern.AddString(_T("����"));
	m_MschartCurveMarkPattern.AddString(_T("������"));
	m_MschartCurveMarkPattern.AddString(_T("ʵ�ĵ�"));
	m_MschartCurveMarkPattern.AddString(_T("ʵ�ķ���"));
	m_MschartCurveMarkPattern.AddString(_T("ʵ������"));
	m_MschartCurveMarkPattern.AddString(_T("ʵ������"));
	m_MschartCurveMarkPattern.AddString(_T("ʵ�ĵ�����"));
	m_MschartCurveMarkPattern.AddString(_T("����ĵ�"));



	OnBnClickedButtonMschartsettingbysystem();

	m_MschartBackgroundColorR = MschartBackgroundColorR;
	m_MschartBackgroundColorG = MschartBackgroundColorG;
	m_MschartBackgroundColorB = MschartBackgroundColorB;
	m_MschartOtherColorR = MschartOtherColorR;
	m_MschartOtherColorG = MschartOtherColorG;
	m_MschartOtherColorB = MschartOtherColorB;
	m_MschartCurveColorR = MschartCurveColorR;
	m_MschartCurveColorG = MschartCurveColorG;
	m_MschartCurveColorB = MschartCurveColorB;
	m_MschartCurveWidth = MschartCurveWidth;
	m_MschartCurveMarkColorR = MschartCurveMarkColorR;
	m_MschartCurveMarkColorG = MschartCurveMarkColorG;
	m_MschartCurveMarkColorB = MschartCurveMarkColorB;
	m_MschartXAxisMaxValue = MschartXAxisMaxValue;
	m_MschartXAxisMarkMeanDistance = MschartXAxisMarkMeanDistance;
	m_MschartXAxisMeanDistance = MschartXAxisMeanDistance;
	m_MschartYAxisMaxValue = MschartYAxisMaxValue;
	m_MschartYAxisMeanDistance = MschartYAxisMeanDistance;
	m_MschartYAxisDisplaykeshujiange = MschartYAxisDisplaykeshujiange;
	m_MschartYAxisMinValue = MschartYAxisMinValue;
	UpdateData(FALSE);
	OnBnClickedButtonMschartapplysetting();
	LoadCurveFlag = 0;
	
	//SetTimer(2,100,NULL);
	SetTimer(1,1000,NULL);

	//HANDLE CurveDisplay1;
	//CurveDisplay1 = CreateThread(NULL,0,CurveDisplay,(LPVOID)this,0,NULL);
	//CloseHandle(CurveDisplay1);



	return TRUE;
}
// CShowMschart message handlers

int CShowMschart::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CShowMschart::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if( m_Chart.GetSafeHwnd())
		m_Chart.MoveWindow( 0, 0, cx, cy ); 


	// TODO: Add your message handler code here
}

void CShowMschart::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		LoadCurveFlag = 1;
		//UpdateData(TRUE);
		if(CurrentFrame <= m_MschartXAxisMaxValue)
		{
			if(CurrentFrame > 0)
			{
				m_Picture.BringWindowToTop(); 
				m_Picture.ShowWindow(SW_SHOW);
				HANDLE hDib;
				CDC *pDC = GetDlgItem(IDC_PICTURE)->GetDC();
				m_Chart.SetRow(CurrentFrame);
				char buf[32];
				for (int i=0;i<sizeof(buf);i++)
				{
					buf[i]=0;
				}
				sprintf(buf, "%d",CurrentFrame);

				// X������ֵ
				m_Chart.SetRowLabel((LPCTSTR)CString(buf));

				// ��������(���)
				if(MschartChooseCurveFlag == 1)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateXArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 2)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateYArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 3)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateZArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 4)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,FrameProcessTime[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 5)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,ImageDetectPitch[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 6)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,ImageDetectHeading[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 7)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateXArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 8)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateYArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 9)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateZArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 10)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveXArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 11)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveYArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 12)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveZArray[CurrentFrame-1],0);



				m_Chart.EditCopy(); 
				if(OpenClipboard())
				{
					hDib = GetClipboardData(CF_DIB);  
					CloseClipboard();
				}
				BITMAPINFO* pbi = (BITMAPINFO*)GlobalLock(hDib);
				if (pbi != NULL)
				{
					BYTE* data = (BYTE*)(pbi->bmiColors);
					if (pbi->bmiHeader.biBitCount <= 8)
					{
						int nColor = (pbi->bmiHeader.biClrUsed==0) ?
							1<<(pbi->bmiHeader.biBitCount) : pbi->bmiHeader.biClrUsed; // nColor��ɫ���е���ɫ��
						data += sizeof(RGBQUAD) *nColor;
					}
					StretchDIBits(pDC->GetSafeHdc(),0,0, pbi->bmiHeader.biWidth,
						pbi->bmiHeader.biHeight,0, 0, pbi->bmiHeader.biWidth,
						pbi->bmiHeader.biHeight, data, pbi, DIB_RGB_COLORS, SRCCOPY);// ��ʾ����Ϊ�˱�������ķ���ǰ���Ⱥ͸߶ȶ�����1
					GlobalUnlock(hDib);
				}
				m_Chart.Refresh();


			}
			
		}else
		{
			m_Picture.BringWindowToTop(); 
			m_Picture.ShowWindow(SW_SHOW);
			HANDLE hDib;
			CDC* pDC = GetDlgItem(IDC_PICTURE)->GetDC();
			VARIANT var;
			if(MschartChooseDisplayModeFlag == 1)
			{

				int m_rowPos=m_Chart.GetRowCount()+1;

				// ������һ��
				m_Chart.GetDataGrid().InsertRows(m_rowPos,1);
				//m_Chart.SetRow(m_rowPos);
				int temp = CurrentFrame;

				for(UINT i=temp - m_Chart.GetRowCount()+1,j = 1;i<=temp;i++,j++)
				{
					m_Chart.SetRow(j);
					CString str;
					str.Format(_T("%d"),i);
					m_Chart.SetRowLabel((LPCTSTR)str);	
					if(MschartChooseCurveFlag == 1)
						m_Chart.GetDataGrid().SetData(j,1,DrogueLocateAccurateXArray[i-1],0);
					if(MschartChooseCurveFlag == 2)
						m_Chart.GetDataGrid().SetData(j,1,DrogueLocateAccurateYArray[i-1],0);
					if(MschartChooseCurveFlag == 3)
						m_Chart.GetDataGrid().SetData(j,1,DrogueLocateAccurateZArray[i-1],0);
					if(MschartChooseCurveFlag == 4)
						m_Chart.GetDataGrid().SetData(j,1,FrameProcessTime[i-1],0);
					if(MschartChooseCurveFlag == 5)
						m_Chart.GetDataGrid().SetData(j,1,ImageDetectPitch[i-1],0);
					if(MschartChooseCurveFlag == 6)
						m_Chart.GetDataGrid().SetData(j,1,ImageDetectHeading[i-1],0);
					if(MschartChooseCurveFlag == 7)
						m_Chart.GetDataGrid().SetData(j,1,DrogueLocateXArray[i-1],0);
					if(MschartChooseCurveFlag == 8)
						m_Chart.GetDataGrid().SetData(j,1,DrogueLocateYArray[i-1],0);
					if(MschartChooseCurveFlag == 9)
						m_Chart.GetDataGrid().SetData(j,1,DrogueLocateZArray[i-1],0);
					if(MschartChooseCurveFlag == 10)
						m_Chart.GetDataGrid().SetData(j,1,DrogueSolveXArray[i-1],0);
					if(MschartChooseCurveFlag == 11)
						m_Chart.GetDataGrid().SetData(j,1,DrogueSolveYArray[i-1],0);
					if(MschartChooseCurveFlag == 12)
						m_Chart.GetDataGrid().SetData(j,1,DrogueSolveZArray[i-1],0);
				}
				if (m_Chart.GetRowCount() > m_MschartXAxisMaxValue)
				{
					m_Chart.GetDataGrid().DeleteRows(m_rowPos-m_MschartXAxisMaxValue,1);
				}


			}
			if(MschartChooseDisplayModeFlag == 2)
			{
				if(m_Chart.GetRowCount() <= CurrentFrame)
					OverFlag =1;
				if(OverFlag == 1)
				{
					m_Chart.SetRowCount(m_Chart.GetRowCount()*2);
					OverFlag = 0;

				}
				for(UINT i=1;i<=m_Chart.GetRowCount();i++)
				{
					m_Chart.SetRow(i);
					CString str;
					str.Format(_T("%d"),i);
					m_Chart.SetRowLabel((LPCTSTR)str);	
				}


				m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�

				m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
				m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

				if(MschartChooseCurveFlag == 1)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateXArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 2)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateYArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 3)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateZArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 4)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,FrameProcessTime[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 5)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,ImageDetectPitch[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 6)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,ImageDetectHeading[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 7)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateXArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 8)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateYArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 9)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateZArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 10)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveXArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 11)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveYArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 12)
					m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveZArray[CurrentFrame-1],0);


			}
			if(MschartChooseDisplayModeFlag == 3)
			{

				MschartChooseDisplayModeFlag = 0;
				m_Chart.SetRowCount(CurrentFrame);
				for(UINT i=1;i<=m_Chart.GetRowCount();i++)
				{
					m_Chart.SetRow(i);
					CString str;
					str.Format(_T("%d"),i);
					m_Chart.SetRowLabel((LPCTSTR)str);	
					if(MschartChooseCurveFlag == 1)
						m_Chart.GetDataGrid().SetData(i,1,DrogueLocateAccurateXArray[i-1],0);
					if(MschartChooseCurveFlag == 2)
						m_Chart.GetDataGrid().SetData(i,1,DrogueLocateAccurateYArray[i-1],0);
					if(MschartChooseCurveFlag == 3)
						m_Chart.GetDataGrid().SetData(i,1,DrogueLocateAccurateZArray[i-1],0);
					if(MschartChooseCurveFlag == 4)
						m_Chart.GetDataGrid().SetData(i,1,FrameProcessTime[i-1],0);
					if(MschartChooseCurveFlag == 5)
						m_Chart.GetDataGrid().SetData(i,1,ImageDetectPitch[i-1],0);
					if(MschartChooseCurveFlag == 6)
						m_Chart.GetDataGrid().SetData(i,1,ImageDetectHeading[i-1],0);
					if(MschartChooseCurveFlag == 7)
						m_Chart.GetDataGrid().SetData(i,1,DrogueLocateXArray[i-1],0);
					if(MschartChooseCurveFlag == 8)
						m_Chart.GetDataGrid().SetData(i,1,DrogueLocateYArray[i-1],0);
					if(MschartChooseCurveFlag == 9)
						m_Chart.GetDataGrid().SetData(i,1,DrogueLocateZArray[i-1],0);
					if(MschartChooseCurveFlag == 10)
						m_Chart.GetDataGrid().SetData(i,1,DrogueSolveXArray[i-1],0);
					if(MschartChooseCurveFlag == 11)
						m_Chart.GetDataGrid().SetData(i,1,DrogueSolveYArray[i-1],0);
					if(MschartChooseCurveFlag == 12)
						m_Chart.GetDataGrid().SetData(i,1,DrogueSolveZArray[i-1],0);


				}
				m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�

				m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
				m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

			}

			m_Chart.EditCopy(); 
			if(OpenClipboard())
			{
				hDib = GetClipboardData(CF_DIB);  
				CloseClipboard();
			}
			BITMAPINFO* pbi = (BITMAPINFO*)GlobalLock(hDib);
			if (pbi != NULL)
			{
				BYTE* data = (BYTE*)(pbi->bmiColors);
				if (pbi->bmiHeader.biBitCount <= 8)
				{
					int nColor = (pbi->bmiHeader.biClrUsed==0) ?
						1<<(pbi->bmiHeader.biBitCount) : pbi->bmiHeader.biClrUsed; // nColor��ɫ���е���ɫ��
					data += sizeof(RGBQUAD) *nColor;
				}
				StretchDIBits(pDC->GetSafeHdc(),0,0, pbi->bmiHeader.biWidth,
					pbi->bmiHeader.biHeight,0, 0, pbi->bmiHeader.biWidth,
					pbi->bmiHeader.biHeight, data, pbi, DIB_RGB_COLORS, SRCCOPY);// ��ʾ����Ϊ�˱�������ķ���ǰ���Ⱥ͸߶ȶ�����1
				GlobalUnlock(hDib);
			}
			m_Chart.Refresh();

		}

		
	}
	if(nIDEvent == 2)
	{
		KillTimer(2);
		m_Picture.BringWindowToTop(); 
		m_Picture.ShowWindow(SW_SHOW);
		HANDLE hDib;
		CDC *pDC = GetDlgItem(IDC_PICTURE)->GetDC();

		m_Chart.EditCopy(); 
		if(OpenClipboard())
		{
			hDib = GetClipboardData(CF_DIB);  
			CloseClipboard();
		}
		BITMAPINFO* pbi = (BITMAPINFO*)GlobalLock(hDib);
		if (pbi != NULL)
		{
			BYTE* data = (BYTE*)(pbi->bmiColors);
			if (pbi->bmiHeader.biBitCount <= 8)
			{
				int nColor = (pbi->bmiHeader.biClrUsed==0) ?
					1<<(pbi->bmiHeader.biBitCount) : pbi->bmiHeader.biClrUsed; // nColor��ɫ���е���ɫ��
				data += sizeof(RGBQUAD) *nColor;
			}
			StretchDIBits(pDC->GetSafeHdc(),1,1, pbi->bmiHeader.biWidth-1,
				pbi->bmiHeader.biHeight-1,0, 0, pbi->bmiHeader.biWidth,
				pbi->bmiHeader.biHeight, data, pbi, DIB_RGB_COLORS, SRCCOPY);// ��ʾ����Ϊ�˱�������ķ���ǰ���Ⱥ͸߶ȶ�����1
			GlobalUnlock(hDib);
		}
		m_Chart.Refresh();

	}
	if(MschartDynamicRefreshSpeedFlag == 1)
	{
		KillTimer(1);
		SetTimer(1,1000,NULL);
	}
	if(MschartDynamicRefreshSpeedFlag == 2)
	{
		KillTimer(1);
		SetTimer(1,100,NULL);

	}
	if(MschartDynamicRefreshSpeedFlag == 3)
	{
		KillTimer(1);
		SetTimer(1,10,NULL);
	}
	if(MschartDynamicRefreshSpeedFlag == 4)
	{
		KillTimer(1);
		SetTimer(1,200,NULL);
	}
	if(MschartDynamicRefreshSpeedFlag == 5)
	{
		KillTimer(1);
		SetTimer(1,20,NULL);
	}
	if(MschartDynamicRefreshSpeedFlag == 6)
	{
		KillTimer(1);
		SetTimer(1,500,NULL);
	}
	if(MschartDynamicRefreshSpeedFlag == 7)
	{
		KillTimer(1);
		SetTimer(1,50,NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

void CShowMschart::OnBnClickedButtonMschartapplysetting()
{
	// TODO: Add your control notification handler code here
	m_Picture.BringWindowToTop(); 
	m_Picture.ShowWindow(SW_SHOW);
	HANDLE hDib;
	CDC *pDC = GetDlgItem(IDC_PICTURE)->GetDC();
	UpdateData(TRUE);

	MschartBackgroundColorR = m_MschartBackgroundColorR;
	MschartBackgroundColorG = m_MschartBackgroundColorG;
	MschartBackgroundColorB = m_MschartBackgroundColorB;
	MschartOtherColorR = m_MschartOtherColorR;
	MschartOtherColorG = m_MschartOtherColorG;
	MschartOtherColorB = m_MschartOtherColorB;
	MschartCurveColorR = m_MschartCurveColorR;
	MschartCurveColorG = m_MschartCurveColorG;
	MschartCurveColorB = m_MschartCurveColorB;
	MschartCurveWidth = m_MschartCurveWidth;
	MschartCurveMarkColorR = m_MschartCurveMarkColorR;
	MschartCurveMarkColorG = m_MschartCurveMarkColorG;
	MschartCurveMarkColorB = m_MschartCurveMarkColorB;
	MschartXAxisMaxValue = m_MschartXAxisMaxValue;
	MschartXAxisMarkMeanDistance = m_MschartXAxisMarkMeanDistance;
	MschartXAxisMeanDistance = m_MschartXAxisMeanDistance;
	MschartYAxisMaxValue = m_MschartYAxisMaxValue;
	MschartYAxisMeanDistance = m_MschartYAxisMeanDistance;
	MschartYAxisDisplaykeshujiange = m_MschartYAxisDisplaykeshujiange;
	MschartYAxisMinValue = m_MschartYAxisMinValue;
	m_Chart.SetTitleText(m_MschartTitleSetting);

	// ���ñ�����ɫ
	m_Chart.GetTitle().GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);

	// �ı������С
	m_Chart.GetTitle().GetVtFont().SetSize(14);

	// �ı䱳��ɫ
	m_Chart.GetBackdrop().GetFill().SetStyle(1);
	m_Chart.GetBackdrop().GetFill().GetBrush().GetFillColor().Set(m_MschartBackgroundColorR,m_MschartBackgroundColorG,m_MschartBackgroundColorB);

	// ��ʾͼ��
	/*m_Chart.SetShowLegend(FALSE);
	m_Chart.SetColumn(1);
	m_Chart.SetColumnLabel((LPCTSTR)_T("point num"));

	// ����ͼ����ɫ
	m_Chart.GetLegend().GetVtFont().GetVtColor().Set(0,255,0);*/

	// ��ʼ������row
	/*if(MschartChooseDisplayModeFlag == 1)
	{
		if(CurrentFrame <= m_MschartXAxisMaxValue)
		{
			m_Chart.SetRowCount(CurrentFrame);
		}else
		{
			m_Chart.SetRowCount(m_MschartXAxisMaxValue);
		}
		
	}
	if(MschartChooseDisplayModeFlag == 2)
	{
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			m_Chart.SetRowCount(CurrentFrame);
		}else
		{
			m_Chart.SetRowCount(m_MschartXAxisMaxValue);
		}

	}
	if(MschartChooseDisplayModeFlag == 3)
	{
		m_Chart.SetRowCount(CurrentFrame);

	}

	if(m_MschartXAxisMaxValue >= CurrentFrame)
	{
		m_Chart.SetRowCount(CurrentFrame);
	}else
	{
		m_Chart.SetRowCount(m_MschartXAxisMaxValue);
	}


	for(UINT i=1;i<=m_Chart.GetRowCount();i++)
	{
		m_Chart.SetRow(i);
		CString str;
		str.Format(_T("%d"),i);
		m_Chart.SetRowLabel((LPCTSTR)str);
		if(MschartChooseCurveFlag == 1)
			m_Chart.GetDataGrid().SetData(i,1,DrogueLocateAccurateXArray[i-1],0);
		if(MschartChooseCurveFlag == 2)
			m_Chart.GetDataGrid().SetData(i,1,DrogueLocateAccurateYArray[i-1],0);
		if(MschartChooseCurveFlag == 3)
			m_Chart.GetDataGrid().SetData(i,1,DrogueLocateAccurateZArray[i-1],0);
		if(MschartChooseCurveFlag == 4)
			m_Chart.GetDataGrid().SetData(i,1,FrameProcessTime[i-1],0);
		if(MschartChooseCurveFlag == 5)
			m_Chart.GetDataGrid().SetData(i,1,ImageDetectPitch[i-1],0);
		if(MschartChooseCurveFlag == 6)
			m_Chart.GetDataGrid().SetData(i,1,ImageDetectHeading[i-1],0);

	}*/


	// ��ʾ���� ��ϵķ�ʽ 0Ϊ3D 1Ϊ2D
	m_Chart.SetChartType(1|2);
	m_Chart.SetSeriesType(11);

	// ջģʽ
	m_Chart.SetStacking(FALSE);

	VARIANT var;	

	// X��Y������
	m_Chart.GetPlot().GetAxis(0,var).GetAxisTitle().SetText(_T("���֡��"));            // X������
	m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("X�ᶨλ����"));    // Y������
	//m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetProperty()

	// X������
	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
	m_Chart.GetPlot().GetAxis(0,var).GetValueScale().SetMaximum(m_MschartXAxisMaxValue); // X�����̶�
	m_Chart.GetPlot().GetAxis(0,var).GetValueScale().SetMinimum(0); // X����С�̶�

	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().
		SetDivisionsPerLabel(m_MschartXAxisMarkMeanDistance);// ÿ�Ŀ̶�һ����ע
	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().
		SetDivisionsPerTick(m_MschartXAxisMeanDistance); // ÿ�̶�һ���̶���

	// X��ÿ�̶����ߣ�0Ϊ�����á�1Ϊ����
	m_Chart.GetPlot().GetAxis(0,var).GetAxisGrid().GetMajorPen().SetStyle(1);

	// ���߸���
	m_Chart.SetColumnCount(1); 

	// ������ɫ
	m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetPen().GetVtColor().
		Set(m_MschartCurveColorR,m_MschartCurveColorG,m_MschartCurveColorB);

	// ���߿��(�Ե���ͼ��Ч)
	m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetPen().SetWidth(m_MschartCurveWidth);


	// ���ݵ�������ʾ����ֵ��ģʽ(������״ͼ�͵���ͼ��Ч)
	// 0: ����ʾ    1: ��ʾ����״ͼ��
	// 2: ��ʾ����״ͼ���Ϸ�    3: ��ʾ����״ͼ���м�    4: ��ʾ����״ͼ���·�
	if(MschartCurveValueDisplayFlag)
	{
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).
			GetDataPointLabel().SetLocationType(0);

	}else
	{
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).
			GetDataPointLabel().SetLocationType(1);
		// �������������ݵ���ɫ
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().
			GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	}

	if(MschartCurveMarkDisplayFlag)
	{
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetSeriesMarker().SetAuto(FALSE); // MUST!!
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1)
			.GetMarker().SetVisible(FALSE);

	}else
	{
		// ��������ʮ�ֱ��
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetSeriesMarker().SetAuto(FALSE); // MUST!!
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1)
			.GetMarker().SetVisible(TRUE);

		// 0��ܣ�1ʮ�֣�2�ǲ棬3���ǣ�4��ԲȦ��5�Ƿ��飬6����
		// 7���ǣ�8�����ǣ�9ʵ�ĵ㣬10ʵ�ķ��飬11ʵ�����Σ�12ʵ�����ǣ�13ʵ�ĵ����ǣ�14����ĵ�
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1)
			.GetMarker().SetStyle(MschartCurveMarkPatternFlag); 

		// ����������ʮ����ɫ
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1)
			.GetMarker().GetFillColor().Set(m_MschartCurveMarkColorR,m_MschartCurveMarkColorG,m_MschartCurveMarkColorB);
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1)
			.GetMarker().GetPen().GetVtColor().Set(m_MschartCurveMarkColorR,m_MschartCurveMarkColorG,m_MschartCurveMarkColorB);
	}

	// �趨��������ɫ
	m_Chart.GetPlot().GetAxis(0,var).GetPen().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(1,var).GetPen().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(2,var).GetPen().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);

	// ������������̶�ֵ��ɫ
	m_Chart.GetPlot().GetAxis(1,var).GetLabels().GetItem(1).GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(0,var).GetLabels().GetItem(1).GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(2,var).GetLabels().GetItem(1).GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);

	// �趨��������
	m_Chart.GetPlot().GetAxis(0,var).GetPen().SetWidth(30);
	m_Chart.GetPlot().GetAxis(1,var).GetPen().SetWidth(30);

	//�趨X��Y�������ɫ
	m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(0,var).GetAxisTitle().GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);

	// ����Y�����������ɫ
	m_Chart.GetPlot().GetAxis(1,var).GetAxisGrid().GetMajorPen().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(0,var).GetAxisGrid().GetMajorPen().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	// ����Y��
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(FALSE);    // ���Զ���עY��̶�
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(m_MschartYAxisMaxValue);    // Y�����̶�
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMinimum(m_MschartYAxisMinValue);        // Y����С�̶�
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMajorDivision(m_MschartYAxisMeanDistance);    // Y��̶�5�ȷ�
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMinorDivision(m_MschartYAxisDisplaykeshujiange);    // ÿ�̶�һ���̶���

	// ����Y�����ƴ�С������
	m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().GetVtFont().SetStyle(0);
	m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().GetVtFont().SetSize(12);

	// ���صڶ�Y��
	if(MschartSecondYAxisDisplayFlag)
	{
		m_Chart.GetPlot().GetAxis(2,var).GetAxisScale().SetHide(FALSE);
	}else
	{
		m_Chart.GetPlot().GetAxis(2,var).GetAxisScale().SetHide(TRUE);

	}

	m_Chart.EditCopy(); 
	if(OpenClipboard())
	{
		hDib = GetClipboardData(CF_DIB);  
		CloseClipboard();
	}
	BITMAPINFO* pbi = (BITMAPINFO*)GlobalLock(hDib);
	if (pbi != NULL)
	{
		BYTE* data = (BYTE*)(pbi->bmiColors);
		if (pbi->bmiHeader.biBitCount <= 8)
		{
			int nColor = (pbi->bmiHeader.biClrUsed==0) ?
				1<<(pbi->bmiHeader.biBitCount) : pbi->bmiHeader.biClrUsed; // nColor��ɫ���е���ɫ��
			data += sizeof(RGBQUAD) *nColor;
		}
		StretchDIBits(pDC->GetSafeHdc(),0,0, pbi->bmiHeader.biWidth,
			pbi->bmiHeader.biHeight,0, 0, pbi->bmiHeader.biWidth,
			pbi->bmiHeader.biHeight, data, pbi, DIB_RGB_COLORS, SRCCOPY);// ��ʾ����Ϊ�˱�������ķ���ǰ���Ⱥ͸߶ȶ�����1
		GlobalUnlock(hDib);
	}
	m_Chart.Refresh();
}

void CShowMschart::OnCbnSelchangeComboMschartchoosecurve()
{
	//UpdateData(TRUE);
	switch(m_MschartChooseCurve.GetCurSel())
	{
	case 0:
		MschartChooseCurveFlag = 1;	
		m_Chart.SetTitleText("X�ᶨλ��������ͼ");
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateAccurateXArray[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��λ����(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateAccurateXArray[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��λ����(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���


		}
		
		break;
	case 1:
		MschartChooseCurveFlag = 2;
		m_Chart.SetTitleText("Y�ᶨλ��������ͼ");
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateAccurateYArray[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��λ����(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateAccurateYArray[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��λ����(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
		break;
	case 2:
		m_Chart.SetTitleText("Z�ᶨλ��������ͼ");
		MschartChooseCurveFlag = 3;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateAccurateZArray[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��λ����(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateAccurateZArray[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��λ����(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
		break;
	case 3:
		m_Chart.SetTitleText("ÿ֡ͼƬ����ʱ������");
		MschartChooseCurveFlag = 4;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					FrameProcessTime[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("ʱ��(s)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					FrameProcessTime[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("ʱ��(s)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
	    break;
	case 4:
		m_Chart.SetTitleText("����������ͼ");
		MschartChooseCurveFlag = 5;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					ImageDetectPitch[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("������(��)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					ImageDetectPitch[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("������(��)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
		break;
	case 5:
		m_Chart.SetTitleText("ƫ��������ͼ");
		MschartChooseCurveFlag = 6;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					ImageDetectHeading[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("ƫ����(��)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					ImageDetectHeading[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("ƫ����(��)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
		break;
	case 6:
		m_Chart.SetTitleText("X�����׼����ͼ");
		MschartChooseCurveFlag = 7;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateXArray[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateXArray[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
		break;
	case 7:
		m_Chart.SetTitleText("Y�����׼����ͼ");
		MschartChooseCurveFlag = 8;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateYArray[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateYArray[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
		break;
	case 8:
		m_Chart.SetTitleText("Z�����׼����ͼ");
		MschartChooseCurveFlag = 9;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateZArray[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateZArray[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
		break;
	case 9:
		m_Chart.SetTitleText("X�����������ͼ");
		MschartChooseCurveFlag = 10;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueSolveXArray[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueSolveXArray[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
		break;
	case 10:
		m_Chart.SetTitleText("Y�����������ͼ");
		MschartChooseCurveFlag = 11;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueSolveYArray[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueSolveYArray[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
		break;
	case 11:
		m_Chart.SetTitleText("Z�����������ͼ");
		MschartChooseCurveFlag = 12;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			int temp = m_Chart.GetRowCount();
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueSolveZArray[i-1],0);
			}
			m_Chart.SetRowCount(temp);
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���
		}else
		{
			for(UINT i=1;i<=CurrentFrame;i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);
				m_Chart.GetDataGrid().SetData(i,1,
					DrogueSolveZArray[i-1],0);
			}
			VARIANT var;
			m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��(m)")); 
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
			m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

		}
		break;

	default:
		break;

	}

	// TODO: Add your control notification handler code here
}

void CShowMschart::OnCbnSelchangeComboMschartchoosedisplaymode()
{
	//UpdateData(TRUE);
	switch(m_MschartChooseDisplayMode.GetCurSel())
	{
	case 0:
		MschartChooseDisplayModeFlag = 1;
		VARIANT var;
		m_Chart.SetRowCount(m_MschartXAxisMaxValue);
		/*for(UINT i=m_Chart.GetRowCount(),j=1;i>=1;i--,j++)
		{
			m_Chart.SetRow(j);
			CString str;
			str.Format(_T("%d"),CurrentFrame - m_Chart.GetRowCount());
			m_Chart.SetRowLabel((LPCTSTR)str);	
		}*/
		m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�

		m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
		m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���


		break;
	case 1:
		MschartChooseDisplayModeFlag = 2;
		if(CurrentFrame >= m_MschartXAxisMaxValue)
		{
			m_Chart.SetRowCount(CurrentFrame);
			for(UINT i=1;i<=m_Chart.GetRowCount();i++)
			{
				m_Chart.SetRow(i);
				CString str;
				str.Format(_T("%d"),i);
				m_Chart.SetRowLabel((LPCTSTR)str);	
				if(MschartChooseCurveFlag == 1)
					m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateAccurateXArray[i-1],0);
				if(MschartChooseCurveFlag == 2)
					m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateAccurateYArray[i-1],0);
				if(MschartChooseCurveFlag == 3)
					m_Chart.GetDataGrid().SetData(i,1,
					DrogueLocateAccurateZArray[i-1],0);
				if(MschartChooseCurveFlag == 4)
					m_Chart.GetDataGrid().SetData(i,1,FrameProcessTime[i-1],0);
				if(MschartChooseCurveFlag == 5)
					m_Chart.GetDataGrid().SetData(i,1,ImageDetectPitch[i-1],0);
				if(MschartChooseCurveFlag == 6)
					m_Chart.GetDataGrid().SetData(i,1,ImageDetectHeading[i-1],0);
				if(MschartChooseCurveFlag == 7)
					m_Chart.GetDataGrid().SetData(i,1,DrogueLocateXArray[i-1],0);
				if(MschartChooseCurveFlag == 8)
					m_Chart.GetDataGrid().SetData(i,1,DrogueLocateYArray[i-1],0);
				if(MschartChooseCurveFlag == 9)
					m_Chart.GetDataGrid().SetData(i,1,DrogueLocateZArray[i-1],0);
				if(MschartChooseCurveFlag == 10)
					m_Chart.GetDataGrid().SetData(i,1,DrogueSolveXArray[i-1],0);
				if(MschartChooseCurveFlag == 11)
					m_Chart.GetDataGrid().SetData(i,1,DrogueSolveYArray[i-1],0);
				if(MschartChooseCurveFlag == 12)
					m_Chart.GetDataGrid().SetData(i,1,DrogueSolveZArray[i-1],0);

			}

		}
		break;
	case 2:
		MschartChooseDisplayModeFlag = 3;
		break;
	default:
		break;

	}

	// TODO: Add your control notification handler code here
}

void CShowMschart::OnCbnSelchangeComboMschartdynamicrefreshspeed()
{
	switch(m_MschartDynamicRefreshSpeed.GetCurSel())
	{
	case 0:
		MschartDynamicRefreshSpeedFlag = 1;
		break;
	case 1:
		MschartDynamicRefreshSpeedFlag = 2;
		break;
	case 2:
		MschartDynamicRefreshSpeedFlag = 3;
		break;
	case 3:
		MschartDynamicRefreshSpeedFlag = 4;
		break;
	case 4:
		MschartDynamicRefreshSpeedFlag = 5;
		break;
	case 5:
		MschartDynamicRefreshSpeedFlag = 6;
		break;
	case 6:
		MschartDynamicRefreshSpeedFlag = 7;
		break;
	default:
		break;

	}
	// TODO: Add your control notification handler code here
}

void CShowMschart::OnCbnSelchangeComboMschartcurvemarkpattern()
{
	// 0��ܣ�1ʮ�֣�2�ǲ棬3���ǣ�4��ԲȦ��5�Ƿ��飬6����
	// 7���ǣ�8�����ǣ�9ʵ�ĵ㣬10ʵ�ķ��飬11ʵ�����Σ�12ʵ�����ǣ�13ʵ�ĵ����ǣ�14����ĵ�
	switch(m_MschartCurveMarkPattern.GetCurSel())
	{
	case 0:
		MschartCurveMarkPatternFlag = 2;
		break;
	case 1:
		MschartCurveMarkPatternFlag = 8;
		break;
	case 2:
		MschartCurveMarkPatternFlag = 14;
		break;
	case 3:
		MschartCurveMarkPatternFlag = 5;
		break;
	case 4:
		MschartCurveMarkPatternFlag = 0;
		break;
	case 5:
		MschartCurveMarkPatternFlag = 6;
		break;
	case 6:
		MschartCurveMarkPatternFlag = 7;
		break;
	case 7:
		MschartCurveMarkPatternFlag = 1;
		break;
	case 8:
		MschartCurveMarkPatternFlag = 13;
		break;
	case 9:
		MschartCurveMarkPatternFlag = 9;
		break;
	case 10:
		MschartCurveMarkPatternFlag = 10;
		break;
	case 11:
		MschartCurveMarkPatternFlag = 11;
		break;
	case 12:
		MschartCurveMarkPatternFlag = 12;
		break;
	case 13:
		MschartCurveMarkPatternFlag = 4;
		break;
	case 14:
		MschartCurveMarkPatternFlag = 15;
		break;
	default:
		break;
	}
	// TODO: Add your control notification handler code here
}

void CShowMschart::OnBnClickedButtonMschartcurvevaluedisplay()
{
	if(MschartCurveValueDisplayFlag)
	{
		GetDlgItem(IDC_BUTTON_MschartCurveValueDisplay)->SetWindowText("NO");
		MschartCurveValueDisplayFlag = FALSE;

	}else
	{
		GetDlgItem(IDC_BUTTON_MschartCurveValueDisplay)->SetWindowText("YES");
		MschartCurveValueDisplayFlag = TRUE;

	}	

	// TODO: Add your control notification handler code here
}

void CShowMschart::OnBnClickedButtonMschartcurvemarkdisplay()
{
	if(MschartCurveMarkDisplayFlag)
	{
		GetDlgItem(IDC_BUTTON_MschartCurveMarkDisplay)->SetWindowText("NO");
		MschartCurveMarkDisplayFlag = FALSE;
		GetDlgItem(IDC_COMBO_MschartCurveMarkPattern)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MschartCurveMarkColorR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MschartCurveMarkColorG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MschartCurveMarkColorB)->EnableWindow(TRUE);

	}else
	{
		GetDlgItem(IDC_BUTTON_MschartCurveMarkDisplay)->SetWindowText("YES");
		MschartCurveMarkDisplayFlag = TRUE;
		GetDlgItem(IDC_COMBO_MschartCurveMarkPattern)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MschartCurveMarkColorR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MschartCurveMarkColorG)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MschartCurveMarkColorB)->EnableWindow(FALSE);

	}
	// TODO: Add your control notification handler code here
}

void CShowMschart::OnBnClickedButtonMschartsecondyaxisdisplay()
{
	if(MschartSecondYAxisDisplayFlag)
	{
		GetDlgItem(IDC_BUTTON_MschartSecondYAxisDisplay)->SetWindowText("NO");
		MschartSecondYAxisDisplayFlag = FALSE;

	}else
	{
		GetDlgItem(IDC_BUTTON_MschartSecondYAxisDisplay)->SetWindowText("YES");
		MschartSecondYAxisDisplayFlag = TRUE;

	}
	// TODO: Add your control notification handler code here
}

void CShowMschart::OnBnClickedButtonMschartsettingbysystem()
{

	m_MschartChooseCurve.SetCurSel(0);
	MschartChooseCurveFlag = 1;

	m_MschartChooseDisplayMode.SetCurSel(0);
	MschartChooseDisplayModeFlag = 1;

	m_MschartDynamicRefreshSpeed.SetCurSel(1);
	MschartDynamicRefreshSpeedFlag = 2;

	m_MschartCurveMarkPattern.SetCurSel(7);
	MschartCurveMarkPatternFlag = 8;
	MschartCurveValueDisplayFlag =TRUE;
	MschartCurveMarkDisplayFlag =TRUE;
	MschartSecondYAxisDisplayFlag = TRUE;

	m_MschartTitleSetting = _T("��λ��������ͼ");
	m_MschartBackgroundColorR = 0;
	m_MschartBackgroundColorG = 0;
	m_MschartBackgroundColorB = 0;
	m_MschartOtherColorR = 0;
	m_MschartOtherColorG = 255;
	m_MschartOtherColorB = 0;
	m_MschartCurveColorR = 0;
	m_MschartCurveColorG = 255;
	m_MschartCurveColorB = 0;
	m_MschartCurveWidth = 30;
	m_MschartCurveMarkColorR = 255;
	m_MschartCurveMarkColorG = 0;
	m_MschartCurveMarkColorB = 0;
	m_MschartXAxisMaxValue = 128;
	m_MschartXAxisMarkMeanDistance = 16;
	m_MschartXAxisMeanDistance = 16;
	m_MschartYAxisMaxValue = 0.1;
	m_MschartYAxisMinValue = 0;
	m_MschartYAxisMeanDistance = 10;
	m_MschartYAxisDisplaykeshujiange = 1;


	// ���ñ���
	m_Chart.SetTitleText(m_MschartTitleSetting);

	// ���ñ�����ɫ
	m_Chart.GetTitle().GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);

	// �ı������С
	m_Chart.GetTitle().GetVtFont().SetSize(14);

	// �ı䱳��ɫ
	m_Chart.GetBackdrop().GetFill().SetStyle(1);
	m_Chart.GetBackdrop().GetFill().GetBrush().GetFillColor().Set(m_MschartBackgroundColorR,m_MschartBackgroundColorG,m_MschartBackgroundColorB);

	// ��ʾͼ��
	/*m_Chart.SetShowLegend(FALSE);
	m_Chart.SetColumn(1);
	m_Chart.SetColumnLabel((LPCTSTR)_T("point num"));

	// ����ͼ����ɫ
	m_Chart.GetLegend().GetVtFont().GetVtColor().Set(0,255,0);*/

	// ��ʼ������row
	m_Chart.SetRowCount(m_MschartXAxisMaxValue);

	for(UINT i=1;i<=m_Chart.GetRowCount();i++)
	{
		m_Chart.SetRow(i);
		CString str;
		str.Format(_T("%d"),i);
		m_Chart.SetRowLabel((LPCTSTR)str);	
	}

	// ��ʾ���� ��ϵķ�ʽ 0Ϊ3D 1Ϊ2D
	m_Chart.SetChartType(1|2);
	m_Chart.SetSeriesType(11);

	// ջģʽ
	m_Chart.SetStacking(FALSE);

	VARIANT var;	

	// X��Y������
	m_Chart.GetPlot().GetAxis(0,var).GetAxisTitle().SetText(_T("���֡��"));            // X������
	m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("��λ����(m)"));    // Y������
	//m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetProperty()

	// X������
	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�
	m_Chart.GetPlot().GetAxis(0,var).GetValueScale().SetMaximum(m_MschartXAxisMaxValue); // X�����̶�
	m_Chart.GetPlot().GetAxis(0,var).GetValueScale().SetMinimum(0); // X����С�̶�

	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().
		SetDivisionsPerLabel(m_MschartXAxisMarkMeanDistance);// ÿ�Ŀ̶�һ����ע
	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().
		SetDivisionsPerTick(m_MschartXAxisMeanDistance); // ÿ�̶�һ���̶���

	// X��ÿ�̶����ߣ�0Ϊ�����á�1Ϊ����
	m_Chart.GetPlot().GetAxis(0,var).GetAxisGrid().GetMajorPen().SetStyle(1);

	// ���߸���
	m_Chart.SetColumnCount(1); 

	// ������ɫ
	m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetPen().GetVtColor().
		Set(m_MschartCurveColorR,m_MschartCurveColorG,m_MschartCurveColorB);

	// ���߿��(�Ե���ͼ��Ч)
	m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetPen().SetWidth(m_MschartCurveWidth);


	// ���ݵ�������ʾ����ֵ��ģʽ(������״ͼ�͵���ͼ��Ч)
	// 0: ����ʾ    1: ��ʾ����״ͼ��
	// 2: ��ʾ����״ͼ���Ϸ�    3: ��ʾ����״ͼ���м�    4: ��ʾ����״ͼ���·�
	if(MschartCurveValueDisplayFlag)
	{
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).
			GetDataPointLabel().SetLocationType(0);

	}else
	{
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).
			GetDataPointLabel().SetLocationType(1);
		// �������������ݵ���ɫ
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().
			GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	}
		
	if(MschartCurveMarkDisplayFlag)
	{
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetSeriesMarker().SetAuto(FALSE); // MUST!!
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1)
			.GetMarker().SetVisible(FALSE);

	}else
	{
		// ��������ʮ�ֱ��
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetSeriesMarker().SetAuto(FALSE); // MUST!!
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1)
			.GetMarker().SetVisible(TRUE);

		// 0��ܣ�1ʮ�֣�2�ǲ棬3���ǣ�4��ԲȦ��5�Ƿ��飬6����
		// 7���ǣ�8�����ǣ�9ʵ�ĵ㣬10ʵ�ķ��飬11ʵ�����Σ�12ʵ�����ǣ�13ʵ�ĵ����ǣ�14����ĵ�
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1)
			.GetMarker().SetStyle(MschartCurveMarkPatternFlag); 

		// ����������ʮ����ɫ
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1)
			.GetMarker().GetFillColor().Set(m_MschartCurveMarkColorR,m_MschartCurveMarkColorG,m_MschartCurveMarkColorB);
		m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1)
			.GetMarker().GetPen().GetVtColor().Set(m_MschartCurveMarkColorR,m_MschartCurveMarkColorG,m_MschartCurveMarkColorB);
	}
	
	// �趨��������ɫ
	m_Chart.GetPlot().GetAxis(0,var).GetPen().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(1,var).GetPen().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(2,var).GetPen().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);

	// ������������̶�ֵ��ɫ
	m_Chart.GetPlot().GetAxis(1,var).GetLabels().GetItem(1).GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(0,var).GetLabels().GetItem(1).GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(2,var).GetLabels().GetItem(1).GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);

	// �趨��������
	m_Chart.GetPlot().GetAxis(0,var).GetPen().SetWidth(30);
	m_Chart.GetPlot().GetAxis(1,var).GetPen().SetWidth(30);

	//�趨X��Y�������ɫ
	m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(0,var).GetAxisTitle().GetVtFont().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);

	// ����Y�����������ɫ
	m_Chart.GetPlot().GetAxis(1,var).GetAxisGrid().GetMajorPen().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);
	m_Chart.GetPlot().GetAxis(0,var).GetAxisGrid().GetMajorPen().GetVtColor().Set(m_MschartOtherColorR,m_MschartOtherColorG,m_MschartOtherColorB);

	// ����Y��
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(FALSE);    // ���Զ���עY��̶�
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(m_MschartYAxisMaxValue);    // Y�����̶�
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMinimum(m_MschartYAxisMinValue);        // Y����С�̶�
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMajorDivision(m_MschartYAxisMeanDistance);    // Y��̶�5�ȷ�
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMinorDivision(m_MschartYAxisDisplaykeshujiange);    // ÿ�̶�һ���̶���

	// ����Y�����ƴ�С������
	m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().GetVtFont().SetStyle(0);
	m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().GetVtFont().SetSize(12);

	// ���صڶ�Y��
	if(MschartSecondYAxisDisplayFlag)
	{
		m_Chart.GetPlot().GetAxis(2,var).GetAxisScale().SetHide(FALSE);
	}else
	{
		m_Chart.GetPlot().GetAxis(2,var).GetAxisScale().SetHide(TRUE);

	}
	
	m_Chart.Refresh();

	// ˢ��
	UpdateData(FALSE);

	// TODO: Add your control notification handler code here
}



DWORD WINAPI CShowMschart::CurveDisplay(LPVOID lpParameter)
{
	CShowMschart* temp = (CShowMschart*)lpParameter;
	while(1)
	{
		if(LoadCurveFlag == 1)
		{
			LoadCurveFlag = 0;
			if(CurrentFrame <= 128)
			{
				temp->m_Picture.BringWindowToTop(); 
				temp->m_Picture.ShowWindow(SW_SHOW);
				HANDLE hDib;
				CDC *pDC = temp->GetDlgItem(IDC_PICTURE)->GetDC();
				temp->m_Chart.SetRow(CurrentFrame);
				char buf[32];
				for (int i=0;i<sizeof(buf);i++)
				{
					buf[i]=0;
				}
				sprintf(buf, "%d",CurrentFrame);

				// X������ֵ
				temp->m_Chart.SetRowLabel((LPCTSTR)CString(buf));

				// ��������(���)
				if(MschartChooseCurveFlag == 1)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateXArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 2)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateYArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 3)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateZArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 4)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,FrameProcessTime[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 5)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,ImageDetectPitch[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 6)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,ImageDetectHeading[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 7)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateXArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 8)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateYArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 9)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateZArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 10)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveXArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 11)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveYArray[CurrentFrame-1],0);
				if(MschartChooseCurveFlag == 12)
					temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveZArray[CurrentFrame-1],0);

				temp->m_Chart.EditCopy(); 
				if(temp->OpenClipboard())
				{
					hDib = ::GetClipboardData(CF_DIB);  
					::CloseClipboard();
				}
				BITMAPINFO* pbi = (BITMAPINFO*)GlobalLock(hDib);
				if (pbi != NULL)
				{
					BYTE* data = (BYTE*)(pbi->bmiColors);
					if (pbi->bmiHeader.biBitCount <= 8)
					{
						int nColor = (pbi->bmiHeader.biClrUsed==0) ?
							1<<(pbi->bmiHeader.biBitCount) : pbi->bmiHeader.biClrUsed; // nColor��ɫ���е���ɫ��
						data += sizeof(RGBQUAD) *nColor;
					}
					StretchDIBits(pDC->GetSafeHdc(),0,0, pbi->bmiHeader.biWidth,
						pbi->bmiHeader.biHeight,0, 0, pbi->bmiHeader.biWidth,
						pbi->bmiHeader.biHeight, data, pbi, DIB_RGB_COLORS, SRCCOPY);// ��ʾ����Ϊ�˱�������ķ���ǰ���Ⱥ͸߶ȶ�����1
					GlobalUnlock(hDib);
				}
				temp->m_Chart.Refresh();

			}else
			{
				temp->m_Picture.BringWindowToTop(); 
				temp->m_Picture.ShowWindow(SW_SHOW);
				HANDLE hDib;
				CDC* pDC = temp->GetDlgItem(IDC_PICTURE)->GetDC();
				VARIANT var;
				if(MschartChooseDisplayModeFlag == 1)
				{

					int m_rowPos=temp->m_Chart.GetRowCount()+1;

					// ������һ��
					temp->m_Chart.GetDataGrid().InsertRows(m_rowPos,1);
					//m_Chart.SetRow(m_rowPos);
					int tempCurrentFrame = CurrentFrame;

					for(UINT i=tempCurrentFrame - temp->m_Chart.GetRowCount()+1,j = 1;i<=tempCurrentFrame;i++,j++)
					{
						temp->m_Chart.SetRow(j);
						CString str;
						str.Format(_T("%d"),i);
						temp->m_Chart.SetRowLabel((LPCTSTR)str);	
						if(MschartChooseCurveFlag == 1)
							temp->m_Chart.GetDataGrid().SetData(j,1,DrogueLocateAccurateXArray[i-1],0);
						if(MschartChooseCurveFlag == 2)
							temp->m_Chart.GetDataGrid().SetData(j,1,DrogueLocateAccurateYArray[i-1],0);
						if(MschartChooseCurveFlag == 3)
							temp->m_Chart.GetDataGrid().SetData(j,1,DrogueLocateAccurateZArray[i-1],0);
						if(MschartChooseCurveFlag == 4)
							temp->m_Chart.GetDataGrid().SetData(j,1,FrameProcessTime[i-1],0);
						if(MschartChooseCurveFlag == 5)
							temp->m_Chart.GetDataGrid().SetData(j,1,ImageDetectPitch[i-1],0);
						if(MschartChooseCurveFlag == 6)
							temp->m_Chart.GetDataGrid().SetData(j,1,ImageDetectHeading[i-1],0);
						if(MschartChooseCurveFlag == 7)
							temp->m_Chart.GetDataGrid().SetData(j,1,DrogueLocateXArray[i-1],0);
						if(MschartChooseCurveFlag == 8)
							temp->m_Chart.GetDataGrid().SetData(j,1,DrogueLocateYArray[i-1],0);
						if(MschartChooseCurveFlag == 9)
							temp->m_Chart.GetDataGrid().SetData(j,1,DrogueLocateZArray[i-1],0);
						if(MschartChooseCurveFlag == 10)
							temp->m_Chart.GetDataGrid().SetData(j,1,DrogueSolveXArray[i-1],0);
						if(MschartChooseCurveFlag == 11)
							temp->m_Chart.GetDataGrid().SetData(j,1,DrogueSolveYArray[i-1],0);
						if(MschartChooseCurveFlag == 12)
							temp->m_Chart.GetDataGrid().SetData(j,1,DrogueSolveZArray[i-1],0);


					}
					if (temp->m_Chart.GetRowCount() > 128)
					{
						temp->m_Chart.GetDataGrid().DeleteRows(m_rowPos-128,1);
					}


				}
				if(MschartChooseDisplayModeFlag == 2)
				{
					if(temp->m_Chart.GetRowCount() <= CurrentFrame)
						OverFlag =1;
					if(OverFlag == 1)
					{
						temp->m_Chart.SetRowCount(temp->m_Chart.GetRowCount()*2);
						OverFlag = 0;

					}
					for(UINT i=1;i <= temp->m_Chart.GetRowCount();i++)
					{
						temp->m_Chart.SetRow(i);
						CString str;
						str.Format(_T("%d"),i);
						temp->m_Chart.SetRowLabel((LPCTSTR)str);	
					}


					temp->m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�

					temp->m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(temp->m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
					temp->m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(temp->m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

					if(MschartChooseCurveFlag == 1)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateXArray[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 2)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateYArray[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 3)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateAccurateZArray[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 4)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,FrameProcessTime[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 5)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,ImageDetectPitch[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 6)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,ImageDetectHeading[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 7)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateXArray[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 8)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateYArray[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 9)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueLocateZArray[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 10)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveXArray[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 11)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveYArray[CurrentFrame-1],0);
					if(MschartChooseCurveFlag == 12)
						temp->m_Chart.GetDataGrid().SetData(CurrentFrame,1,DrogueSolveZArray[CurrentFrame-1],0);


				}
				if(MschartChooseDisplayModeFlag == 3)
				{

					MschartChooseDisplayModeFlag = 0;
					temp->m_Chart.SetRowCount(CurrentFrame);
					for(UINT i=1;i <= temp->m_Chart.GetRowCount();i++)
					{
						temp->m_Chart.SetRow(i);
						CString str;
						str.Format(_T("%d"),i);
						temp->m_Chart.SetRowLabel((LPCTSTR)str);	
						if(MschartChooseCurveFlag == 1)
							temp->m_Chart.GetDataGrid().SetData(i,1,DrogueLocateAccurateXArray[i-1],0);
						if(MschartChooseCurveFlag == 2)
							temp->m_Chart.GetDataGrid().SetData(i,1,DrogueLocateAccurateYArray[i-1],0);
						if(MschartChooseCurveFlag == 3)
							temp->m_Chart.GetDataGrid().SetData(i,1,DrogueLocateAccurateZArray[i-1],0);
						if(MschartChooseCurveFlag == 4)
							temp->m_Chart.GetDataGrid().SetData(i,1,FrameProcessTime[i-1],0);
						if(MschartChooseCurveFlag == 5)
							temp->m_Chart.GetDataGrid().SetData(i,1,ImageDetectPitch[i-1],0);
						if(MschartChooseCurveFlag == 6)
							temp->m_Chart.GetDataGrid().SetData(i,1,ImageDetectHeading[i-1],0);
						if(MschartChooseCurveFlag == 7)
							temp->m_Chart.GetDataGrid().SetData(i,1,DrogueLocateXArray[i-1],0);
						if(MschartChooseCurveFlag == 8)
							temp->m_Chart.GetDataGrid().SetData(i,1,DrogueLocateYArray[i-1],0);
						if(MschartChooseCurveFlag == 9)
							temp->m_Chart.GetDataGrid().SetData(i,1,DrogueLocateZArray[i-1],0);
						if(MschartChooseCurveFlag == 10)
							temp->m_Chart.GetDataGrid().SetData(i,1,DrogueSolveXArray[i-1],0);
						if(MschartChooseCurveFlag == 11)
							temp->m_Chart.GetDataGrid().SetData(i,1,DrogueSolveYArray[i-1],0);
						if(MschartChooseCurveFlag == 12)
							temp->m_Chart.GetDataGrid().SetData(i,1,DrogueSolveZArray[i-1],0);



					}
					temp->m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);            // ���Զ���עX��̶�

					temp->m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(temp->m_Chart.GetRowCount()/8);// ÿ�Ŀ̶�һ����ע
					temp->m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(temp->m_Chart.GetRowCount()/8); // ÿ�̶�һ���̶���

				}

				temp->m_Chart.EditCopy(); 
				if(temp->OpenClipboard())
				{
					hDib = ::GetClipboardData(CF_DIB);  
					::CloseClipboard();
				}
				BITMAPINFO* pbi = (BITMAPINFO*)GlobalLock(hDib);
				if (pbi != NULL)
				{
					BYTE* data = (BYTE*)(pbi->bmiColors);
					if (pbi->bmiHeader.biBitCount <= 8)
					{
						int nColor = (pbi->bmiHeader.biClrUsed==0) ?
							1<<(pbi->bmiHeader.biBitCount) : pbi->bmiHeader.biClrUsed; // nColor��ɫ���е���ɫ��
						data += sizeof(RGBQUAD) *nColor;
					}
					StretchDIBits(pDC->GetSafeHdc(),0,0, pbi->bmiHeader.biWidth,
						pbi->bmiHeader.biHeight,0, 0, pbi->bmiHeader.biWidth,
						pbi->bmiHeader.biHeight, data, pbi, DIB_RGB_COLORS, SRCCOPY);// ��ʾ����Ϊ�˱�������ķ���ǰ���Ⱥ͸߶ȶ�����1
					GlobalUnlock(hDib);
				}
				temp->m_Chart.Refresh();

			}
		}

	}
	return 0;
}
void CShowMschart::OnBnClickedButtonMschartexit()
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
	delete FlightControlCurveDisplay1;
	FlightControlCurveDisplay1=NULL;
	globalImageProcessthis->GetDlgItem(IDC_BUTTON_LocateCurveDisplay)->EnableWindow(false);

}
