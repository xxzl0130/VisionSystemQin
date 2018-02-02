// VisionProcess.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSystemQin.h"
#include "VisionProcess.h"
#include "MyApp.h"
#include "DrawDrogue.h"
#include "RecieverPlaneSetting.h"



// CVisionProcess dialog
float FontcolorR,FontcolorG,FontcolorB,FontY,FontX,CameraLeft=0,CameraTop=1,CameraDown=0.7,CameraRight=0.3;
int ClimateSettingFlag = 0;
int m_Slider_Disturbance_Value;
int OpenLightFlag = 0;
int WenLiuModeChoose = 1;

IMPLEMENT_DYNAMIC(CVisionProcess, CDialog)

CVisionProcess::CVisionProcess(CWnd* pParent /*=NULL*/)
	: CDialog(CVisionProcess::IDD, pParent)
	, m_FontcolorR(1.0)
	, m_FontcolorG(0.0)
	, m_FontcolorB(0.0)
	, m_FontX(0.0)
	, m_FontY(0.0)
	, m_CameraLeft(0.0)
	, m_CameraTop(1.0)
	, m_CameraRight(0.3)
	, m_CameraDown(0.7)
	, m_DistanceSensitivity(1.0)
	, m_AngleSensitivity(1.0)
	, m_XPosition(0.0)
	, m_YPosition(0.0)
	, m_ZPosition(0.0)
	, m_PitchValue(0.0)
	, m_RollValue(0.0)
	, m_HeadingValue(0.0)
{

}

CVisionProcess::~CVisionProcess()
{
}

void CVisionProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FontcolorR, m_FontcolorR);//DDX_Text将界面中输入的数据传给变量
	DDX_Text(pDX, IDC_EDIT_FontcolorG, m_FontcolorG);
	DDX_Text(pDX, IDC_EDIT_FontcolorB, m_FontcolorB);
	DDX_Text(pDX, IDC_EDIT_FontX, m_FontX);
	DDX_Text(pDX, IDC_EDIT_FontY, m_FontY);
	DDX_Text(pDX, IDC_EDIT_CameraLeft, m_CameraLeft);
	DDX_Text(pDX, IDC_EDIT_CameraTop, m_CameraTop);
	DDX_Text(pDX, IDC_EDIT_CameraDown, m_CameraDown);
	DDX_Text(pDX, IDC_EDIT_CameraRight, m_CameraRight);
	DDX_Text(pDX, IDC_EDIT_DistanceSensitivity, m_DistanceSensitivity);
	DDX_Text(pDX, IDC_EDIT_AngleSensitivity, m_AngleSensitivity);
	DDX_Text(pDX, IDC_EDIT_Xposition, m_XPosition);
	DDX_Text(pDX, IDC_EDIT_YPosition, m_YPosition);
	DDX_Text(pDX, IDC_EDIT_ZPosition, m_ZPosition);
	DDX_Text(pDX, IDC_EDIT_PitchValue, m_PitchValue);
	DDX_Text(pDX, IDC_EDIT_RollValue, m_RollValue);
	DDX_Text(pDX, IDC_EDIT_HeadingValue, m_HeadingValue);
	DDV_MinMaxFloat(pDX, m_DistanceSensitivity, 0.0, 1000.0);//DDV设定限制
	DDV_MinMaxFloat(pDX, m_AngleSensitivity, 0.0, 360.0);
	DDV_MinMaxFloat(pDX, m_FontcolorR, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_FontcolorG, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_FontcolorB, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_FontX, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_FontY, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_CameraLeft, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_CameraTop, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_CameraDown, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_CameraRight, 0.0, 1.0);
	DDX_Control(pDX, IDC_BUTTON_LeftDistance, m_LeftDistance);//DDX_Control将按钮的信息传递给变量
	DDX_Control(pDX, IDC_BUTTON_UpDistance, m_UpDistance);
	DDX_Control(pDX, IDC_BUTTON_RightDistance, m_RightDistance);
	DDX_Control(pDX, IDC_BUTTON_DownDistance, m_DownDistance);
	DDX_Control(pDX, IDC_BUTTON_ForwardDistance, m_ForwardDistance);
	DDX_Control(pDX, IDC_BUTTON_BackDistance, m_BackDistance);
	DDX_Control(pDX, IDC_BUTTON_RotateXShun, m_RotateXShun);
	DDX_Control(pDX, IDC_BUTTON_RotateYShun, m_RotateYShun);
	DDX_Control(pDX, IDC_BUTTON_RotateZShun, m_RotateZShun);
	DDX_Control(pDX, IDC_BUTTON_RotateXNi, m_RotateXNi);
	DDX_Control(pDX, IDC_BUTTON_RotateYNi, m_RotateYNi);
	DDX_Control(pDX, IDC_BUTTON_RotateZNi, m_RotateZNi);
}


BEGIN_MESSAGE_MAP(CVisionProcess, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_Aplicatesetting, &CVisionProcess::OnBnClickedButtonAplicatesetting)
	ON_BN_CLICKED(IDC_BUTTON_DroguePositionDisplay, &CVisionProcess::OnBnClickedButtonDroguepositiondisplay)
	ON_BN_CLICKED(IDC_BUTTON_CameraSetting, &CVisionProcess::OnBnClickedButtonCamerasetting)
	ON_BN_CLICKED(IDC_BUTTON_CameraDisplay, &CVisionProcess::OnBnClickedButtonCameradisplay)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_LeftBack, &CVisionProcess::OnBnClickedRadioLeftback)
	ON_BN_CLICKED(IDC_RADIO_Back, &CVisionProcess::OnBnClickedRadioBack)
	ON_BN_CLICKED(IDC_RADIO_FarBack, &CVisionProcess::OnBnClickedRadioFarback)
	ON_BN_CLICKED(IDC_RADIO_UpBack, &CVisionProcess::OnBnClickedRadioUpback)
	ON_BN_CLICKED(IDC_BUTTON_ModeSetting, &CVisionProcess::OnBnClickedButtonModesetting)
	ON_BN_CLICKED(IDC_BUTTON_UpDistance, &CVisionProcess::OnBnClickedButtonUpdistance)
	ON_BN_CLICKED(IDC_BUTTON_DownDistance, &CVisionProcess::OnBnClickedButtonDowndistance)
	ON_BN_CLICKED(IDC_BUTTON_LeftDistance, &CVisionProcess::OnBnClickedButtonLeftdistance)
	ON_BN_CLICKED(IDC_BUTTON_RightDistance, &CVisionProcess::OnBnClickedButtonRightdistance)
	ON_BN_CLICKED(IDC_BUTTON_ForwardDistance, &CVisionProcess::OnBnClickedButtonForwarddistance)
	ON_BN_CLICKED(IDC_BUTTON_BackDistance, &CVisionProcess::OnBnClickedButtonBackdistance)
	ON_BN_CLICKED(IDC_BUTTON_RotateXShun, &CVisionProcess::OnBnClickedButtonRotatexshun)
	ON_BN_CLICKED(IDC_BUTTON_RotateXNi, &CVisionProcess::OnBnClickedButtonRotatexni)
	ON_BN_CLICKED(IDC_BUTTON_RotateYShun, &CVisionProcess::OnBnClickedButtonRotateyshun)
	ON_BN_CLICKED(IDC_BUTTON_RotateYNi, &CVisionProcess::OnBnClickedButtonRotateyni)
	ON_BN_CLICKED(IDC_BUTTON_RotateZShun, &CVisionProcess::OnBnClickedButtonRotatezshun)
	ON_BN_CLICKED(IDC_BUTTON_RotateZNi, &CVisionProcess::OnBnClickedButtonRotatezni)
	ON_BN_CLICKED(IDC_BUTTON_SystemSetting, &CVisionProcess::OnBnClickedButtonSystemsetting)
	ON_BN_CLICKED(IDC_RADIO_SunnyDay, &CVisionProcess::OnBnClickedRadioSunnyday)
	ON_BN_CLICKED(IDC_RADIO_CloudyDay, &CVisionProcess::OnBnClickedRadioCloudyday)
	ON_BN_CLICKED(IDC_RADIO_FogDay, &CVisionProcess::OnBnClickedRadioFogday)
	ON_BN_CLICKED(IDC_RADIO_RainDay, &CVisionProcess::OnBnClickedRadioRainday)
	ON_BN_CLICKED(IDC_RADIO_SnowDay, &CVisionProcess::OnBnClickedRadioSnowday)
	ON_BN_CLICKED(IDC_BUTTON_ClimateSetting, &CVisionProcess::OnBnClickedButtonClimatesetting)
	ON_BN_CLICKED(IDC_BUTTON_RecieverPlaneMoniSetting, &CVisionProcess::OnBnClickedButtonRecieverplanemonisetting)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_ClimateOpenLight, &CVisionProcess::OnBnClickedButtonClimateopenlight)
	ON_BN_CLICKED(IDC_RADIO_LightWenliu, &CVisionProcess::OnBnClickedRadioLightwenliu)
	ON_BN_CLICKED(IDC_RADIO_MedianWenliu, &CVisionProcess::OnBnClickedRadioMedianwenliu)
	ON_BN_CLICKED(IDC_RADIO_ServeWenliu, &CVisionProcess::OnBnClickedRadioServewenliu)
END_MESSAGE_MAP()


// CVisionProcess message handlers


BOOL CVisionProcess::OnInitDialog()
{
	CDialog::OnInitDialog();
	CSliderCtrl   *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_DisturbanceAdjust);
	pSlidCtrl->SetRange(1, 20);
	pSlidCtrl->SetPos(1);
	m_Slider_Disturbance_Value = 1;
	m_LeftDistance.SetImage(IDB_BITMAP_LeftDistance);
	// 鼠标经过时变成手势 
	m_LeftDistance.SetMouseCursorHand(); 
	// 工具提示 
	m_LeftDistance.SetTooltip(_T("向左移动")); //选择按钮时的文本提示
	// TODO: 在此添加额外的初始化代码
	m_LeftDistance.SizeToContent();  //设置控件大小与图片相同
	m_LeftDistance.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	m_RightDistance.SetImage(IDB_BITMAP_RightDistance);
	// 鼠标经过时变成手势 
	m_RightDistance.SetMouseCursorHand(); 
	// 工具提示 
	m_RightDistance.SetTooltip(_T("向右移动"));
	// TODO: 在此添加额外的初始化代码
	m_RightDistance.SizeToContent();
	m_RightDistance.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	m_UpDistance.SetImage(IDB_BITMAP_UpDistance);
	// 鼠标经过时变成手势 
	m_UpDistance.SetMouseCursorHand(); 
	// 工具提示 
	m_UpDistance.SetTooltip(_T("向上移动"));
	// TODO: 在此添加额外的初始化代码
	m_UpDistance.SizeToContent();
	m_UpDistance.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	m_DownDistance.SetImage(IDB_BITMAP_DownDistance);
	// 鼠标经过时变成手势 
	m_DownDistance.SetMouseCursorHand(); 
	// 工具提示 
	m_DownDistance.SetTooltip(_T("向下移动"));
	// TODO: 在此添加额外的初始化代码
	m_DownDistance.SizeToContent();
	m_DownDistance.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	m_ForwardDistance.SetImage(IDB_BITMAP_ForwardDistance);
	// 鼠标经过时变成手势 
	m_ForwardDistance.SetMouseCursorHand(); 
	// 工具提示 
	m_ForwardDistance.SetTooltip(_T("向前移动"));
	// TODO: 在此添加额外的初始化代码
	m_ForwardDistance.SizeToContent();
	m_ForwardDistance.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	m_BackDistance.SetImage(IDB_BITMAP_BackDistance);
	// 鼠标经过时变成手势 
	m_BackDistance.SetMouseCursorHand(); 
	// 工具提示 
	m_BackDistance.SetTooltip(_T("向后移动"));
	// TODO: 在此添加额外的初始化代码
	m_BackDistance.SizeToContent();
	m_BackDistance.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	m_RotateXShun.SetImage(IDB_BITMAP_RotateXShun);
	// 鼠标经过时变成手势 
	m_RotateXShun.SetMouseCursorHand(); 
	// 工具提示 
	m_RotateXShun.SetTooltip(_T("绕x轴顺时针转"));
	// TODO: 在此添加额外的初始化代码
	m_RotateXShun.SizeToContent();
	m_RotateXShun.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	m_RotateYShun.SetImage(IDB_BITMAP_RotateYShun);
	// 鼠标经过时变成手势 
	m_RotateYShun.SetMouseCursorHand(); 
	// 工具提示 
	m_RotateYShun.SetTooltip(_T("绕y轴顺时针转"));
	// TODO: 在此添加额外的初始化代码
	m_RotateYShun.SizeToContent();
	m_RotateYShun.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	m_RotateZShun.SetImage(IDB_BITMAP_RotateZShun);
	// 鼠标经过时变成手势 
	m_RotateZShun.SetMouseCursorHand(); 
	// 工具提示 
	m_RotateZShun.SetTooltip(_T("绕z轴顺时针转"));
	// TODO: 在此添加额外的初始化代码
	m_RotateZShun.SizeToContent();
	m_RotateZShun.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	m_RotateXNi.SetImage(IDB_BITMAP_RotateXNi);
	// 鼠标经过时变成手势 
	m_RotateXNi.SetMouseCursorHand(); 
	// 工具提示 
	m_RotateXNi.SetTooltip(_T("绕x轴逆时针转"));
	// TODO: 在此添加额外的初始化代码
	m_RotateXNi.SizeToContent();
	m_RotateXNi.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	m_RotateYNi.SetImage(IDB_BITMAP_RotateYNi);
	// 鼠标经过时变成手势 
	m_RotateYNi.SetMouseCursorHand(); 
	// 工具提示 
	m_RotateYNi.SetTooltip(_T("绕y轴逆时针转"));
	// TODO: 在此添加额外的初始化代码
	m_RotateYNi.SizeToContent();
	m_RotateYNi.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT



	m_RotateZNi.SetImage(IDB_BITMAP_RotateZNi);
	// 鼠标经过时变成手势 
	m_RotateZNi.SetMouseCursorHand(); 
	// 工具提示 
	m_RotateZNi.SetTooltip(_T("绕z轴逆时针转"));
	// TODO: 在此添加额外的初始化代码
	m_RotateZNi.SizeToContent();
	m_RotateZNi.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; //BUT

	//m_LeftDistance.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_LeftDistance)));
	//m_RightDistance.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_RightDistance)));
	//m_UpDistance.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_UpDistance)));
	//m_DownDistance.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_DownDistance)));
	//m_ForwardDistance.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_ForwardDistance)));
	//m_BackDistance.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_BackDistance)));
	//m_RotateXShun.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_RotateXShun)));
	//m_RotateYShun.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_RotateYShun)));
	//m_RotateZShun.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_RotateZShun)));
	//m_RotateXNi.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_RotateXNi)));
	//m_RotateYNi.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_RotateYNi)));
	//m_RotateZNi.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_RotateZNi)));
	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO_LeftBack);
	radio->SetCheck(1);
	radio=(CButton*)GetDlgItem(IDC_RADIO_SunnyDay);
	radio->SetCheck(1);

	radio=(CButton*)GetDlgItem(IDC_RADIO_LightWenliu);
	radio->SetCheck(1);

	
	
	//m_LeftDistanse.SizeToContent();
	//m_LeftDistanse.EnableWindow(true);

	return TRUE;

}
void CVisionProcess::OnBnClickedButtonAplicatesetting()
{
	UpdateData(TRUE);
	FontcolorR = m_FontcolorR; 
	FontcolorG = m_FontcolorG;
	FontcolorB = m_FontcolorB;
	FontY = m_FontY;
	FontX = m_FontX;
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonDroguepositiondisplay()
{
	if(m_bScreenDisplay == false)
		GetDlgItem(IDC_BUTTON_DroguePositionDisplay)->SetWindowText("OFF");
	else
		GetDlgItem(IDC_BUTTON_DroguePositionDisplay)->SetWindowText("ON");

	m_bScreenDisplay = (!m_bScreenDisplay);
	
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonCamerasetting()
{
	UpdateData(TRUE);
	CameraLeft = m_CameraLeft;
	CameraTop = m_CameraTop;
	CameraDown = m_CameraDown;
	CameraRight = m_CameraRight;
	if(!m_bCamaraChannelDisplay)
	{
	vpChannel* CameraChannel = (vpChannel*)vpChannel::find("CameraChannel");
	CameraChannel->setDrawArea(CameraLeft,CameraRight,CameraDown,CameraTop);
	}
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonCameradisplay()
{
	
	vpChannel* CameraChannel = (vpChannel*)vpChannel::find("CameraChannel"); 
	if(m_bCamaraChannelDisplay)//显示
	{
		CameraChannel->setDrawArea(CameraLeft,CameraRight,CameraDown,CameraTop);
		GetDlgItem(IDC_BUTTON_CameraDisplay)->SetWindowText("OFF");

	}
	else
	{
		CameraChannel->setDrawArea(0.0,0.0,1.0,1.0);
		GetDlgItem(IDC_BUTTON_CameraDisplay)->SetWindowText("ON");
	}
	m_bCamaraChannelDisplay=(!m_bCamaraChannelDisplay);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void CVisionProcess::OnBnClickedRadioLeftback()
{
	VisionMode = 0;
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedRadioBack()
{
	VisionMode = 1;
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedRadioFarback()
{
	VisionMode = 2;
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedRadioUpback()
{
	VisionMode = 3;
	// TODO: Add your control notification handler code here
}


//应用设置
void CVisionProcess::OnBnClickedButtonModesetting()
{

	if(VisionMode%4 == 0)
	{
		VisionChangeX = -30; VisionChangeY = -70; VisionChangeZ = 5;
		VisionChangeP = -5; VisionChangeH = -25, VisionChangeR = 0;
		m_XPosition = VisionChangeX,m_YPosition = VisionChangeY,m_ZPosition = VisionChangeZ;
		m_PitchValue = VisionChangeP,m_RollValue = VisionChangeR,m_HeadingValue = VisionChangeH;
		UpdateData(FALSE);
	}
	if(VisionMode%4 == 1)
	{
		VisionChangeX = -13; VisionChangeY = -50; VisionChangeZ = -3;
		VisionChangeP = 0; VisionChangeH = 0, VisionChangeR = 0;
		m_XPosition = VisionChangeX,m_YPosition = VisionChangeY,m_ZPosition = VisionChangeZ;
		m_PitchValue = VisionChangeP,m_RollValue = VisionChangeR,m_HeadingValue = VisionChangeH;
		UpdateData(FALSE);
	}
	if(VisionMode%4 == 2)
	{
		VisionChangeX = -13; VisionChangeY = -100; VisionChangeZ = 5;
		VisionChangeP = -10; VisionChangeH = 0, VisionChangeR = 0;
		m_XPosition = VisionChangeX,m_YPosition = VisionChangeY,m_ZPosition = VisionChangeZ;
		m_PitchValue = VisionChangeP,m_RollValue = VisionChangeR,m_HeadingValue = VisionChangeH;
		UpdateData(FALSE);
	}
	if(VisionMode%4 == 3)
	{
		VisionChangeX = 0; VisionChangeY = -150; VisionChangeZ = 30;
		VisionChangeP = -18; VisionChangeH = 0, VisionChangeR = 0;
		m_XPosition = VisionChangeX,m_YPosition = VisionChangeY,m_ZPosition = VisionChangeZ;
		m_PitchValue = VisionChangeP,m_RollValue = VisionChangeR,m_HeadingValue = VisionChangeH;
		UpdateData(FALSE);
	}
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonUpdistance()
{
	UpdateData(TRUE);
	VisionChangeZ = VisionChangeZ + m_DistanceSensitivity;
	m_ZPosition = VisionChangeZ;
	UpdateData(FALSE);

	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonDowndistance()
{
	UpdateData(TRUE);
	VisionChangeZ = VisionChangeZ - m_DistanceSensitivity;
	m_ZPosition = VisionChangeZ;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonLeftdistance()
{
	UpdateData(TRUE);
	VisionChangeX = VisionChangeX - m_DistanceSensitivity;
	m_XPosition = VisionChangeX;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonRightdistance()
{
	UpdateData(TRUE);
	VisionChangeX = VisionChangeX + m_DistanceSensitivity;
	m_XPosition = VisionChangeX;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonForwarddistance()
{
	UpdateData(TRUE);
	VisionChangeY = VisionChangeY + m_DistanceSensitivity;
	m_YPosition = VisionChangeY;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonBackdistance()
{
	UpdateData(TRUE);
	VisionChangeY = VisionChangeY - m_DistanceSensitivity;
	m_YPosition = VisionChangeY;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonRotatexshun()
{
	UpdateData(TRUE);
	VisionChangeP = VisionChangeP + m_AngleSensitivity;
	m_PitchValue = VisionChangeP;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonRotatexni()
{
	UpdateData(TRUE);
	VisionChangeP = VisionChangeP - m_AngleSensitivity;
	m_PitchValue = VisionChangeP;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonRotateyshun()
{
	UpdateData(TRUE);
	VisionChangeR = VisionChangeR + m_AngleSensitivity;
	m_RollValue = VisionChangeR;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonRotateyni()
{
	UpdateData(TRUE);
	VisionChangeR = VisionChangeR - m_AngleSensitivity;
	m_RollValue = VisionChangeR;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonRotatezshun()
{
	UpdateData(TRUE);
	VisionChangeH = VisionChangeH + m_AngleSensitivity;
	m_HeadingValue = VisionChangeH;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonRotatezni()
{
	UpdateData(TRUE);
	VisionChangeH = VisionChangeH - m_AngleSensitivity;
	m_HeadingValue = VisionChangeH;
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonSystemsetting()
{
	VisionChangeX = -50; VisionChangeY = -50; VisionChangeZ = -5;
	VisionChangeP = 0; VisionChangeH = -45, VisionChangeR = 0;
	m_XPosition = VisionChangeX,m_YPosition = VisionChangeY,m_ZPosition = VisionChangeZ;
	m_PitchValue = VisionChangeP,m_RollValue = VisionChangeR,m_HeadingValue = VisionChangeH;
	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO_LeftBack);
	radio->SetCheck(1);
	radio=(CButton*)GetDlgItem(IDC_RADIO_Back);
	radio->SetCheck(0);
	radio=(CButton*)GetDlgItem(IDC_RADIO_FarBack);
	radio->SetCheck(0);
	radio=(CButton*)GetDlgItem(IDC_RADIO_UpBack);
	radio->SetCheck(0);
	m_AngleSensitivity = 1;
	m_DistanceSensitivity = 1;
	UpdateData(FALSE);

		
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedRadioSunnyday()
{
	m_WheatherMode = 1;
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedRadioCloudyday()
{
	m_WheatherMode = 3;
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedRadioFogday()
{
	m_WheatherMode = 2;
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedRadioRainday()
{
	m_WheatherMode = 0;
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedRadioSnowday()
{
	m_WheatherMode = 4;
	// TODO: Add your control notification handler code here
}

void CVisionProcess::OnBnClickedButtonClimatesetting()
{
	ClimateSettingFlag = 1;
	// TODO: Add your control notification handler code here
}


//受油机模拟设置
void CVisionProcess::OnBnClickedButtonRecieverplanemonisetting()
{
	// TODO: Add your control notification handler code here
	CRecieverPlaneSetting RecieverPlaneSetting;
	RecieverPlaneSetting.DoModal();
}

void CVisionProcess::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl   *pSlidCtrl_Disturbance = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_DisturbanceAdjust);
	m_Slider_Disturbance_Value = pSlidCtrl_Disturbance->GetPos();//取得当前位置值 

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CVisionProcess::OnBnClickedButtonClimateopenlight()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_BUTTON_ClimateOpenLight)->GetWindowText(str);
	if(str == "开启灯光")
	{
		OpenLightFlag = 1;
		GetDlgItem(IDC_BUTTON_ClimateOpenLight)->SetWindowText("关闭灯光");
	}else{
		OpenLightFlag = 2;
		GetDlgItem(IDC_BUTTON_ClimateOpenLight)->SetWindowText("开启灯光");
	}
		
}

void CVisionProcess::OnBnClickedRadioLightwenliu()
{
	// TODO: Add your control notification handler code here
	WenLiuModeChoose = 1;
}

void CVisionProcess::OnBnClickedRadioMedianwenliu()
{
	// TODO: Add your control notification handler code here
	WenLiuModeChoose = 2;
}

void CVisionProcess::OnBnClickedRadioServewenliu()
{
	// TODO: Add your control notification handler code here
	WenLiuModeChoose = 3;
}
