// VisionSystemQinDlg.cpp : 实现文件
//
#include "stdafx.h"
#include <fstream>
#include<string>
#include<iostream>
#include "stdio.h"

#include "SimStructDefine.h"
#include "SimVisionSystem.h"
#include "VisionSystemQin.h"
#include "VisionSystemQinDlg.h"
#include "DrawDrogue.h"
#include "MyApp.h"
#include "CalibrateCamera.h"
#include "VisionProcess.h"
#include "ResultDisplay.h"
#include "ImageProcess.h"
#include "ResultDisplay.h"
#include "SplasScreen.h"
#include "RecieverPlaneSetting.h"
using namespace std;  
using namespace cv;

#define ROWS 10001
#define RED_Color RGB(255,0,0)
#define GREEN_Color RGB(0,255,0)
#define BLUE_Color RGB(0,0,255)

//受油机加油机沿视景y轴定值平飞的变量
float yy = -2001;
//使锥套运动平滑运动，不出现跳变的标志
int IncreaseOrDeincreaseFlag = 1;
//当前锥套运动加载数据的行数和列数
int Current_Row=0,Current_Col=0;
//轻度紊流下，24段锥套在仿真时间100s内的x，y，z坐标，
//x向前为正，y向右为正，z向下为正。需要转换成视景坐标
double HoseDrogue_X[ROWS][24]={0},HoseDrogue_Y[ROWS][24]={0},HoseDrogue_Z[ROWS][24]={0};
//中度紊流下，24段锥套在仿真时间100s内的x，y，z坐标，
double HoseDrogue_X_Median[ROWS][24]={0},HoseDrogue_Y_Median[ROWS][24]={0},HoseDrogue_Z_Median[ROWS][24]={0};
//重度紊流下，24段锥套在仿真时间100s内的x，y，z坐标，
double HoseDrogue_X_Large[ROWS][24]={0},HoseDrogue_Y_Large[ROWS][24]={0},HoseDrogue_Z_Large[ROWS][24]={0};
//视景加载标志，为了显示“视景加载中”这几个字用
int Vision_Loading_Flag = 0;
//通过摄像机标定获取的x，y轴焦距和图像中心点坐标
double RealTimrFocusFx, RealTimeIntrinsiccx,RealTimeIntrinsiccy,RealTimrFocusFy;
//轮廓求解方法标志
int ContourCenterPointSolveMethodFlag = 0;
//系统仿真时间对应为小时，分钟，秒
int SystemSimulinkTimeH =0,SystemSimulinkTimeM =0,SystemSimulinkTimeS =0;
//f16飞机三维视景坐标
vuVec3<double> F16Position;
//左键鼠标按下、左键鼠标弹起，右键鼠标按下、右键鼠标弹起、鼠标移动起点和终点对应的屏幕坐标点
//用于鼠标操控视景
CPoint LeftButtonDownPoint,LeftButtonUpPoint,RightButtonUpPoint,RightButtonDownPoint,
MouseMovePoint,LatestMouseMovePoint;
int RightOrLeftDownFlag = 0,FirstDownPointFlag = 0;
HCURSOR hCur;
//视景是否加载完毕标志
int StartLoadVisionOver=0;
//视景图片控件区域坐标值
CRect LoadVisionPictureControlRect;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CRect VisionDisplayRect;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVisionSystemQinDlg 对话框




CVisionSystemQinDlg::CVisionSystemQinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVisionSystemQinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_PLANE);//IDR_MAINFRAME);
}

void CVisionSystemQinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_VisionImageResult, m_Tab);
}

BEGIN_MESSAGE_MAP(CVisionSystemQinDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VisionImageResult, &CVisionSystemQinDlg::OnTcnSelchangeTabVisionimageresult)
	ON_WM_MOVING()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_WelcomeInterfaceOnOff, &CVisionSystemQinDlg::OnBnClickedButtonWelcomeinterfaceonoff)
END_MESSAGE_MAP()


// CVisionSystemQinDlg 消息处理程序

BOOL CVisionSystemQinDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	char* pszFileName="welcome.txt"; 
	CStdioFile myFile;  
	CFileException fileException;  
	CString strONOFF;  
	if(myFile.Open(pszFileName,CFile::typeText|CFile::modeReadWrite),&fileException) 
	{  
		myFile.SeekToBegin(); 	
		myFile.ReadString(strONOFF);
		myFile.Close();
	}
	if(strONOFF == "ON")
	{
		CSplasScreen SplashScreen;
		SplashScreen.DoModal();
		GetDlgItem(IDC_BUTTON_WelcomeInterfaceOnOff)->SetWindowText("OFF");
	}
	if(strONOFF == "OFF")
	{
		GetDlgItem(IDC_BUTTON_WelcomeInterfaceOnOff)->SetWindowText("ON");
	}
	
	GetDlgItem(IDC_STATIC_VisionDisplay)->GetClientRect(&LoadVisionPictureControlRect);

    //设置三个子对话框
	// TODO: 在此添加额外的初始化代码
	m_Tab.InsertItem(0," 视景处理 ");
	m_Tab.InsertItem(1," 图像处理 ");
	m_Tab.InsertItem(2," 结果显示 ");
	
	VisionProcess.Create(IDD_VisionProcess,GetDlgItem(IDC_TAB_VisionImageResult)); 
	ImageProcess.Create(IDD_ImageProcess,GetDlgItem(IDC_TAB_VisionImageResult)); 
	ResultDisplay.Create(IDD_ResultDisplay,GetDlgItem(IDC_TAB_VisionImageResult)); 

	CRect rs; 
	m_Tab.GetClientRect(&rs); 
	//调整子对话框在父窗口中的位置 
	rs.top += 22; 
	rs.bottom -= 1; 
	rs.left += 1; 
	rs.right -= 1; 
 
	//设置子对话框尺寸并移动到指定位置 
	VisionProcess.MoveWindow(&rs); 
	ImageProcess.MoveWindow(&rs);
	ResultDisplay.MoveWindow(&rs);
 
	//分别设置隐藏和显示 
	VisionProcess.ShowWindow(true); 
	ImageProcess.ShowWindow(false); 
	ResultDisplay.ShowWindow(false); 

	//设置默认的选项卡 
	m_Tab.SetCurSel(0);

	HANDLE VisionDisplay1;
	VisionDisplay1 = CreateThread(NULL,0,VisionDisplay,(LPVOID)this,0,NULL);
	CloseHandle(VisionDisplay1);
	Vision_Loading_Flag = 0;
	VisionDisplayRect = GetCameraWndRect();
	SetTimer(2,1000,NULL);
	CString str;
	str.Format("%d",SystemSimulinkTimeH);
	GetDlgItem(IDC_EDIT_SystemSimulinkTimeH)->SetWindowText(str);
	str.Format("%d",SystemSimulinkTimeM);
	GetDlgItem(IDC_EDIT_SystemSimulinkTimeM)->SetWindowText(str);
	str.Format("%d",SystemSimulinkTimeS);
	GetDlgItem(IDC_EDIT_SystemSimulinkTimeS)->SetWindowText(str);

	font_set.CreatePointFont(25 * 15, "华文行楷");
	GetDlgItem(IDC_STATIC_Loading)->SetWindowText("视景加载中");
	GetDlgItem(IDC_STATIC_Loading)->SetFont(&font_set);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVisionSystemQinDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVisionSystemQinDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{ 
		/*CPaintDC   dc(this);   
		CRect   rect; 
		GetClientRect(&rect); 
		//GetDlgItem(IDC_STATIC_VisionDisplay)->GetWindowRect(&rect);
		//ScreenToClient(rect);  
		//GetDlgItem(IDC_STATIC_VisionDisplay)->SetBkMode(TRANSPARENT);
		CDC   dcMem;   
		dcMem.CreateCompatibleDC(&dc);   
		CBitmap   bmpBackground;   
		bmpBackground.LoadBitmap(IDB_BITMAP_Background);   
		//IDB_BITMAP是你自己的图对应的ID   
		BITMAP   bitmap;   
		bmpBackground.GetBitmap(&bitmap);   
		CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
			bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);  */
		CDialog::OnPaint();
	}
	VisionDisplayRect = GetCameraWndRect();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVisionSystemQinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CVisionSystemQinDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CVisionSystemQinDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	if(nIDEvent == 1)
	{
		if(Vision_Loading_Flag == -1)
		{
			KillTimer(1);
			GetDlgItem(IDC_STATIC_Status)->SetWindowText("视景加载完毕，正在运行中！！");
			GetDlgItem(IDC_STATIC_Loading)->ShowWindow(FALSE);
			SetTimer(3,1000,NULL);
		}
		if(Vision_Loading_Flag%4 == 0)
		{
			GetDlgItem(IDC_STATIC_Status)->SetWindowText("视景正在加载中");
			GetDlgItem(IDC_STATIC_Loading)->SetWindowText("视景加载中");
			GetDlgItem(IDC_STATIC_Loading)->SetFont(&font_set);
		}
		if(Vision_Loading_Flag%4 == 1)
		{
			GetDlgItem(IDC_STATIC_Status)->SetWindowText("视景正在加载中.");
			GetDlgItem(IDC_STATIC_Loading)->SetWindowText("视景加载中.");
			GetDlgItem(IDC_STATIC_Loading)->SetFont(&font_set);
		}
		if(Vision_Loading_Flag%4 == 2)
		{
			GetDlgItem(IDC_STATIC_Status)->SetWindowText("视景正在加载中..");
			GetDlgItem(IDC_STATIC_Loading)->SetWindowText("视景加载中..");
			GetDlgItem(IDC_STATIC_Loading)->SetFont(&font_set);
		}
		if(Vision_Loading_Flag%4 == 3)
		{
			GetDlgItem(IDC_STATIC_Status)->SetWindowText("视景正在加载中...");
			GetDlgItem(IDC_STATIC_Loading)->SetWindowText("视景加载中...");
			GetDlgItem(IDC_STATIC_Loading)->SetFont(&font_set);
		}
		Vision_Loading_Flag++;
			
	}
	if(nIDEvent == 2)
	{
		CTime t = CTime::GetCurrentTime(); 
		CString strTime = t.Format(_T( "%Y-%m-%d %H:%M:%S"));
		GetDlgItem(IDC_STATIC_SystemTime)->SetWindowText(strTime);
		SystemSimulinkTimeS++;
		if(SystemSimulinkTimeS == 60)
		{
			SystemSimulinkTimeS = 0;
			SystemSimulinkTimeM++;
			if(SystemSimulinkTimeM == 60)
			{
				SystemSimulinkTimeH = 1;
				SystemSimulinkTimeM = 0;

			}
		}
		CString str;
		str.Format("%d",SystemSimulinkTimeH);
		GetDlgItem(IDC_EDIT_SystemSimulinkTimeH)->SetWindowText(str);
		str.Format("%d",SystemSimulinkTimeM);
		GetDlgItem(IDC_EDIT_SystemSimulinkTimeM)->SetWindowText(str);
		str.Format("%d",SystemSimulinkTimeS);
		GetDlgItem(IDC_EDIT_SystemSimulinkTimeS)->SetWindowText(str);

	}
	if(nIDEvent == 3)
	{
		pReceiver->getAbsolutePosition(&F16Position);
		F16PositionX[F16PositionCount] = F16Position[0];
		F16PositionY[F16PositionCount] = F16Position[1];
		F16PositionZ[F16PositionCount] = F16Position[2];
		DrogueRalativeCameraDistanceY = -(CameraTranformpos[1] - DroguePosition[1]);
		DrogueRalativeCameraDistanceX = -(CameraTranformpos[0] - DroguePosition[0]);
		DrogueRalativeCameraDistanceZ = -(CameraTranformpos[2] - DroguePosition[2]);
		F16PositionCount++;
	}


	/////////////////////////////////////////////////////////////////
	CDialog::OnTimer(nIDEvent);

}


//读取三种紊流情况下的三轴数据，用作锥套基准空间坐标
void CVisionSystemQinDlg::ReadDogueXYZ(void)
{

	FILE *fpx,*fpy,*fpz;
	int i = 0, j = 0;
	fpx = fopen("HoseDrogue_Coordinator\\x.txt", "r");
	if(fpx == NULL)
	{
		printf("cannot open file!\n");
		MessageBox("读取锥套x坐标失败！");
	}
	while(!feof(fpx))
	{
		if(j != 23)
			fscanf(fpx, "%lf", &HoseDrogue_X[i][j++]);//注意此处有个空格
		else
		{
			fscanf(fpx, "%lf", &HoseDrogue_X[i][j]);
			j = 0;
			++i;
		}
	}
	fclose(fpx);
	fpx = NULL;

	i = 0, j = 0;
	fpy = fopen("HoseDrogue_Coordinator\\y.txt", "r");
	if(fpy == NULL)
	{
		printf("cannot open file!\n");
		MessageBox("读取锥套y坐标失败！");;
	}
	while(!feof(fpy))
	{
		if(j != 23)
			fscanf(fpy, "%lf", &HoseDrogue_Y[i][j++]);//注意此处有个空格
		else
		{
			fscanf(fpy, "%lf", &HoseDrogue_Y[i][j]);
			j = 0;
			++i;
		}
	}
	fclose(fpy);
	fpy = NULL;

	i = 0, j = 0;
	fpz = fopen("HoseDrogue_Coordinator\\z.txt", "r");
	if(fpz == NULL)
	{
		printf("cannot open file!\n");
		MessageBox("读取锥套z坐标失败！");
	}
	while(!feof(fpz))
	{
		if(j != 23)
			fscanf(fpz, "%lf", &HoseDrogue_Z[i][j++]);//注意此处有个空格
		else
		{
			fscanf(fpz, "%lf", &HoseDrogue_Z[i][j]);
			j = 0;
			++i;
		}
	}
	fclose(fpz);
	fpz = NULL;




	i = 0, j = 0;
	fpx = fopen("HoseDrogue_Coordinator\\x_Median.txt", "r");
	if(fpx == NULL)
	{
		printf("cannot open file!\n");
		MessageBox("读取锥套x坐标失败！");
	}
	while(!feof(fpx))
	{
		if(j != 23)
			fscanf(fpx, "%lf", &HoseDrogue_X_Median[i][j++]);//注意此处有个空格
		else
		{
			fscanf(fpx, "%lf", &HoseDrogue_X_Median[i][j]);
			j = 0;
			++i;
		}
	}
	fclose(fpx);
	fpx = NULL;

	i = 0, j = 0;
	fpy = fopen("HoseDrogue_Coordinator\\y_Median.txt", "r");
	if(fpy == NULL)
	{
		printf("cannot open file!\n");
		MessageBox("读取锥套y坐标失败！");;
	}
	while(!feof(fpy))
	{
		if(j != 23)
			fscanf(fpy, "%lf", &HoseDrogue_Y_Median[i][j++]);//注意此处有个空格
		else
		{
			fscanf(fpy, "%lf", &HoseDrogue_Y_Median[i][j]);
			j = 0;
			++i;
		}
	}
	fclose(fpy);
	fpy = NULL;

	i = 0, j = 0;
	fpz = fopen("HoseDrogue_Coordinator\\z_Median.txt", "r");
	if(fpz == NULL)
	{
		printf("cannot open file!\n");
		MessageBox("读取锥套z坐标失败！");
	}
	while(!feof(fpz))
	{
		if(j != 23)
			fscanf(fpz, "%lf", &HoseDrogue_Z_Median[i][j++]);//注意此处有个空格
		else
		{
			fscanf(fpz, "%lf", &HoseDrogue_Z_Median[i][j]);
			j = 0;
			++i;
		}
	}
	fclose(fpz);
	fpz = NULL;






	i = 0, j = 0;
	fpx = fopen("HoseDrogue_Coordinator\\x_Large.txt", "r");
	if(fpx == NULL)
	{
		printf("cannot open file!\n");
		MessageBox("读取锥套x坐标失败！");
	}
	while(!feof(fpx))
	{
		if(j != 23)
			fscanf(fpx, "%lf", &HoseDrogue_X_Large[i][j++]);//注意此处有个空格
		else
		{
			fscanf(fpx, "%lf", &HoseDrogue_X_Large[i][j]);
			j = 0;
			++i;
		}
	}
	fclose(fpx);
	fpx = NULL;

	i = 0, j = 0;
	fpy = fopen("HoseDrogue_Coordinator\\y_Large.txt", "r");
	if(fpy == NULL)
	{
		printf("cannot open file!\n");
		MessageBox("读取锥套y坐标失败！");;
	}
	while(!feof(fpy))
	{
		if(j != 23)
			fscanf(fpy, "%lf", &HoseDrogue_Y_Large[i][j++]);//注意此处有个空格
		else
		{
			fscanf(fpy, "%lf", &HoseDrogue_Y_Large[i][j]);
			j = 0;
			++i;
		}
	}
	fclose(fpy);
	fpy = NULL;

	i = 0, j = 0;
	fpz = fopen("HoseDrogue_Coordinator\\z_Large.txt", "r");
	if(fpz == NULL)
	{
		printf("cannot open file!\n");
		MessageBox("读取锥套z坐标失败！");
	}
	while(!feof(fpz))
	{
		if(j != 23)
			fscanf(fpz, "%lf", &HoseDrogue_Z_Large[i][j++]);//注意此处有个空格
		else
		{
			fscanf(fpz, "%lf", &HoseDrogue_Z_Large[i][j]);
			j = 0;
			++i;
		}
	}
	fclose(fpz);
	fpz = NULL;

}

//三维锥套实时运动产生函数，可以对三种紊流进行切换，可以调节对应的步长
void CVisionSystemQinDlg::DrogueMotionPointValue(void)
{
	if(WenLiuModeChoose == 1)
	{
		vuVec3f point1,point2,point3,point4,point5,point6,point7,point8,point9,point10,point11,point12,
			point13,point14,point15,point16,point17,point18,point19,point20,point21,point22,point23,point24;
		point1[0]=-HoseDrogue_Y[Current_Row][Current_Col],point1[1]=HoseDrogue_X[Current_Row][Current_Col],point1[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point2[0]=-HoseDrogue_Y[Current_Row][Current_Col],point2[1]=HoseDrogue_X[Current_Row][Current_Col],point2[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point3[0]=-HoseDrogue_Y[Current_Row][Current_Col],point3[1]=HoseDrogue_X[Current_Row][Current_Col],point3[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point4[0]=-HoseDrogue_Y[Current_Row][Current_Col],point4[1]=HoseDrogue_X[Current_Row][Current_Col],point4[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point5[0]=-HoseDrogue_Y[Current_Row][Current_Col],point5[1]=HoseDrogue_X[Current_Row][Current_Col],point5[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point6[0]=-HoseDrogue_Y[Current_Row][Current_Col],point6[1]=HoseDrogue_X[Current_Row][Current_Col],point6[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point7[0]=-HoseDrogue_Y[Current_Row][Current_Col],point7[1]=HoseDrogue_X[Current_Row][Current_Col],point7[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point8[0]=-HoseDrogue_Y[Current_Row][Current_Col],point8[1]=HoseDrogue_X[Current_Row][Current_Col],point8[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point9[0]=-HoseDrogue_Y[Current_Row][Current_Col],point9[1]=HoseDrogue_X[Current_Row][Current_Col],point9[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point10[0]=-HoseDrogue_Y[Current_Row][Current_Col],point10[1]=HoseDrogue_X[Current_Row][Current_Col],point10[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point11[0]=-HoseDrogue_Y[Current_Row][Current_Col],point11[1]=HoseDrogue_X[Current_Row][Current_Col],point11[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point12[0]=-HoseDrogue_Y[Current_Row][Current_Col],point12[1]=HoseDrogue_X[Current_Row][Current_Col],point12[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point13[0]=-HoseDrogue_Y[Current_Row][Current_Col],point13[1]=HoseDrogue_X[Current_Row][Current_Col],point13[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point14[0]=-HoseDrogue_Y[Current_Row][Current_Col],point14[1]=HoseDrogue_X[Current_Row][Current_Col],point14[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point15[0]=-HoseDrogue_Y[Current_Row][Current_Col],point15[1]=HoseDrogue_X[Current_Row][Current_Col],point15[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point16[0]=-HoseDrogue_Y[Current_Row][Current_Col],point16[1]=HoseDrogue_X[Current_Row][Current_Col],point16[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point17[0]=-HoseDrogue_Y[Current_Row][Current_Col],point17[1]=HoseDrogue_X[Current_Row][Current_Col],point17[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point18[0]=-HoseDrogue_Y[Current_Row][Current_Col],point18[1]=HoseDrogue_X[Current_Row][Current_Col],point18[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point19[0]=-HoseDrogue_Y[Current_Row][Current_Col],point19[1]=HoseDrogue_X[Current_Row][Current_Col],point19[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point20[0]=-HoseDrogue_Y[Current_Row][Current_Col],point20[1]=HoseDrogue_X[Current_Row][Current_Col],point20[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point21[0]=-HoseDrogue_Y[Current_Row][Current_Col],point21[1]=HoseDrogue_X[Current_Row][Current_Col],point21[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point22[0]=-HoseDrogue_Y[Current_Row][Current_Col],point22[1]=HoseDrogue_X[Current_Row][Current_Col],point22[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point23[0]=-HoseDrogue_Y[Current_Row][Current_Col],point23[1]=HoseDrogue_X[Current_Row][Current_Col],point23[2]=-HoseDrogue_Z[Current_Row][Current_Col++];
		point24[0]=-HoseDrogue_Y[Current_Row][Current_Col],point24[1]=HoseDrogue_X[Current_Row][Current_Col],point24[2]=-HoseDrogue_Z[Current_Row][Current_Col++];

		if(IncreaseOrDeincreaseFlag == 1)
		{
			Current_Row += m_Slider_Disturbance_Value*IncreaseOrDeincreaseFlag;
			Current_Col=0;
		}else
		{
			Current_Row += m_Slider_Disturbance_Value*IncreaseOrDeincreaseFlag;
			Current_Col=0;
		}
		
		if(Current_Row>10000)
		{
			IncreaseOrDeincreaseFlag = -1;
			Current_Row = 10000;
		}
		if(Current_Row < 2500  )
		{
			IncreaseOrDeincreaseFlag = 1;
			Current_Row = 2500;
		}

//画一段软管
		Drawdrogue->DrawDrogue(point1,point2,point3,point4,point5,point6,point7,point8,point9,point10,point11,point12,
			point13,point14,point15,point16,point17,point18,point19,point20,point21,point22,point23,
			point24,vuVec4f(1.0f,1.0f,1.0f,1.0f),vuVec3f(10000,yy+1000,1500),vuVec3f(0,0,0),0.15);//12785,yy,1500


		vpObject* f16 = (vpObject*)vpObject::find("f16");
		yy = yy + 2;

		//加油机受油机的相对运动
		if(FinalRecieverY > StartRecieverY)
		{
			StartRecieverY += 0.03*RelativeSpeed;
			f16->setTranslate(-13.729466,StartRecieverY,-11);
			if(StartRecieverY >= FinalRecieverY)
			{
				StartRecieverY = FinalRecieverY;
				f16->setTranslate(-13.729466,StartRecieverY,-11);
			}

		}
		if(FinalRecieverY < StartRecieverY)
		{
			StartRecieverY -= 0.03*RelativeSpeed;
			f16->setTranslate(-13.729466,StartRecieverY,-11);
			if(StartRecieverY <= FinalRecieverY)
			{
				StartRecieverY = FinalRecieverY;
				f16->setTranslate(-13.729466,StartRecieverY,-11);
			}

		}

	}


	if(WenLiuModeChoose == 2)
	{
		vuVec3f point1,point2,point3,point4,point5,point6,point7,point8,point9,point10,point11,point12,
	    point13,point14,point15,point16,point17,point18,point19,point20,point21,point22,point23,point24;
		point1[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point1[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point1[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point2[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point2[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point2[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point3[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point3[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point3[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point4[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point4[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point4[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point5[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point5[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point5[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point6[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point6[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point6[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point7[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point7[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point7[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point8[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point8[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point8[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point9[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point9[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point9[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point10[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point10[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point10[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point11[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point11[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point11[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point12[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point12[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point12[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point13[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point13[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point13[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point14[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point14[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point14[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point15[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point15[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point15[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point16[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point16[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point16[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point17[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point17[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point17[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point18[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point18[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point18[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point19[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point19[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point19[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point20[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point20[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point20[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point21[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point21[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point21[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point22[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point22[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point22[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point23[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point23[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point23[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];
		point24[0]=-HoseDrogue_Y_Median[Current_Row][Current_Col],point24[1]=HoseDrogue_X_Median[Current_Row][Current_Col],point24[2]=-HoseDrogue_Z_Median[Current_Row][Current_Col++];

		if(IncreaseOrDeincreaseFlag == 1)
		{
			Current_Row += m_Slider_Disturbance_Value*IncreaseOrDeincreaseFlag;
			Current_Col=0;
		}else
		{
			Current_Row += m_Slider_Disturbance_Value*IncreaseOrDeincreaseFlag;
			Current_Col=0;
		}

		if(Current_Row>10000)
		{
			IncreaseOrDeincreaseFlag = -1;
			Current_Row = 10000;
		}
		if(Current_Row < 0)
		{
			IncreaseOrDeincreaseFlag = 1;
			Current_Row = 0;
		}

		Drawdrogue->DrawDrogue(point1,point2,point3,point4,point5,point6,point7,point8,point9,point10,point11,point12,
			point13,point14,point15,point16,point17,point18,point19,point20,point21,point22,point23,
			point24,vuVec4f(1.0f,1.0f,1.0f,1.0f),vuVec3f(10000,yy+1000,1500),vuVec3f(0,0,0),0.15);//12785,yy,1500


		vpObject* f16 = (vpObject*)vpObject::find("f16");
		yy = yy + 2;

		if(FinalRecieverY > StartRecieverY)
		{
			StartRecieverY += 0.03*RelativeSpeed;
			f16->setTranslate(-13.729466,StartRecieverY,-11);
			if(StartRecieverY >= FinalRecieverY)
			{
				StartRecieverY = FinalRecieverY;
				f16->setTranslate(-13.729466,StartRecieverY,-11);
			}

		}
		if(FinalRecieverY < StartRecieverY)
		{
			StartRecieverY -= 0.03*RelativeSpeed;
			f16->setTranslate(-13.729466,StartRecieverY,-11);
			if(StartRecieverY <= FinalRecieverY)
			{
				StartRecieverY = FinalRecieverY;
				f16->setTranslate(-13.729466,StartRecieverY,-11);
			}

		}

	}


	if(WenLiuModeChoose == 3)
	{
		vuVec3f point1,point2,point3,point4,point5,point6,point7,point8,point9,point10,point11,point12,
			point13,point14,point15,point16,point17,point18,point19,point20,point21,point22,point23,point24;
		point1[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point1[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point1[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point2[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point2[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point2[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point3[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point3[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point3[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point4[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point4[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point4[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point5[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point5[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point5[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point6[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point6[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point6[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point7[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point7[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point7[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point8[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point8[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point8[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point9[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point9[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point9[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point10[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point10[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point10[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point11[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point11[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point11[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point12[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point12[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point12[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point13[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point13[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point13[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point14[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point14[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point14[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point15[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point15[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point15[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point16[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point16[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point16[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point17[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point17[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point17[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point18[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point18[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point18[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point19[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point19[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point19[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point20[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point20[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point20[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point21[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point21[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point21[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point22[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point22[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point22[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point23[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point23[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point23[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];
		point24[0]=-HoseDrogue_Y_Large[Current_Row][Current_Col],point24[1]=HoseDrogue_X_Large[Current_Row][Current_Col],point24[2]=-HoseDrogue_Z_Large[Current_Row][Current_Col++];

		if(IncreaseOrDeincreaseFlag == 1)
		{
			Current_Row += m_Slider_Disturbance_Value*IncreaseOrDeincreaseFlag;
			Current_Col=0;
		}else
		{
			Current_Row += m_Slider_Disturbance_Value*IncreaseOrDeincreaseFlag;
			Current_Col=0;
		}

		if(Current_Row>10000)
		{
			IncreaseOrDeincreaseFlag = -1;
			Current_Row = 10000;
		}
		if(Current_Row < 0)
		{
			IncreaseOrDeincreaseFlag = 1;
			Current_Row = 0;
		}

		Drawdrogue->DrawDrogue(point1,point2,point3,point4,point5,point6,point7,point8,point9,point10,point11,point12,
			point13,point14,point15,point16,point17,point18,point19,point20,point21,point22,point23,
			point24,vuVec4f(1.0f,1.0f,1.0f,1.0f),vuVec3f(10000,yy+1000,1500),vuVec3f(0,0,0),0.15);//12785,yy,1500


		vpObject* f16 = (vpObject*)vpObject::find("f16");
		yy = yy + 2;

		if(FinalRecieverY > StartRecieverY)
		{
			StartRecieverY += 0.03*RelativeSpeed;
			f16->setTranslate(-13.729466,StartRecieverY,-11);
			if(StartRecieverY >= FinalRecieverY)
			{
				StartRecieverY = FinalRecieverY;
				f16->setTranslate(-13.729466,StartRecieverY,-11);
			}

		}
		if(FinalRecieverY < StartRecieverY)
		{
			StartRecieverY -= 0.03*RelativeSpeed;
			f16->setTranslate(-13.729466,StartRecieverY,-11);
			if(StartRecieverY <= FinalRecieverY)
			{
				StartRecieverY = FinalRecieverY;
				f16->setTranslate(-13.729466,StartRecieverY,-11);
			}

		}

	}

}


//主界面Tab控件响应函数，切换Tab时，显示不同的视景区域
void CVisionSystemQinDlg::OnTcnSelchangeTabVisionimageresult(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int CurSel = m_Tab.GetCurSel(); 
	switch(CurSel) 
	{ 
	case 0:
		VisionProcess.ShowWindow(true); 
		ImageProcess.ShowWindow(false); 
		ResultDisplay.ShowWindow(false); 
		if(VisionStartOverFlag == 1)
		{
			if(LockCameraChannelFlag)
			{
				vpChannel* CameraChannel = (vpChannel*)vpChannel::find("CameraChannel");
				assert(CameraChannel);
				if(!m_bCamaraChannelDisplay)//显示
				{
					CameraChannel->setDrawArea(0.0,1.0,0.0,1.0);

				}
				else
				{
					CameraChannel->setDrawArea(0.0,0.0,1.0,1.0);
				}

				RealTimrFocusFx = ResultIntrinsicfx;
				RealTimrFocusFy = ResultIntrinsicfy;
				RealTimeIntrinsiccx = ResultIntrinsiccx;
				RealTimeIntrinsiccy = ResultIntrinsiccy;
				ContourCenterPointSolveMethodFlag = 0; 

			}else
			{
				vpChannel* CameraChannel = (vpChannel*)vpChannel::find("CameraChannel");
				assert(CameraChannel);
				if(!m_bCamaraChannelDisplay)//显示
				{
					CameraChannel->setDrawArea(CameraLeft,CameraRight,CameraDown,CameraTop);

				}
				else
				{
					CameraChannel->setDrawArea(0.0,0.0,1.0,1.0);
				}
			}
			
		}
		
		break;
	case 1:
		VisionProcess.ShowWindow(false); 
		ImageProcess.ShowWindow(true); 
		ResultDisplay.ShowWindow(false); 
		if(VisionStartOverFlag == 1)
		{
			vpChannel* CameraChannel = (vpChannel*)vpChannel::find("CameraChannel");
			assert(CameraChannel);
			if(!m_bCamaraChannelDisplay)//只有在视景处理中打开摄像机通道才行
			{
				CameraChannel->setDrawArea(0.0,1.0,0.0,1.0);

			}
			else
			{
				CameraChannel->setDrawArea(0.0,0.0,1.0,1.0);
			}
			RealTimrFocusFx = ResultIntrinsicfx;
			RealTimrFocusFy = ResultIntrinsicfy;
			RealTimeIntrinsiccx = ResultIntrinsiccx;
			RealTimeIntrinsiccy = ResultIntrinsiccy;
			ContourCenterPointSolveMethodFlag = 0; 
		}
		
		break;
	case 2:
		VisionProcess.ShowWindow(false); 
		ImageProcess.ShowWindow(false); 
		ResultDisplay.ShowWindow(true); 
		if(VisionStartOverFlag == 1)
		{	
			if(LockCameraChannelFlag)
			{
				vpChannel* CameraChannel = (vpChannel*)vpChannel::find("CameraChannel");
				assert(CameraChannel);
				if(!m_bCamaraChannelDisplay)//显示
				{
					CameraChannel->setDrawArea(0.0,1.0,0.0,1.0);

				}
				else
				{
					CameraChannel->setDrawArea(0.0,0.0,1.0,1.0);
				}

				RealTimrFocusFx = ResultIntrinsicfx;
				RealTimrFocusFy = ResultIntrinsicfy;
				RealTimeIntrinsiccx = ResultIntrinsiccx;
				RealTimeIntrinsiccy = ResultIntrinsiccy;
				ContourCenterPointSolveMethodFlag = 0; 

			}else
			{
				vpChannel* CameraChannel = (vpChannel*)vpChannel::find("CameraChannel");
				assert(CameraChannel);
				if(!m_bCamaraChannelDisplay)//显示
				{
					CameraChannel->setDrawArea(CameraLeft,CameraRight,CameraDown,CameraTop);

				}
				else
				{
					CameraChannel->setDrawArea(0.0,0.0,1.0,1.0);
				}
			}

		}
		
		break;

	}     
	*pResult = 0;
}


//视景显示处理线程函数
DWORD WINAPI CVisionSystemQinDlg::VisionDisplay(LPVOID lpParameter)
{
	CVisionSystemQinDlg* temp = (CVisionSystemQinDlg*)lpParameter;
	const int TIMER_ID1 = 1;
	int timeout = 750;// Timeout of 33 means send a WM_TIMER message at 30Hz.
	int returnValue = temp->SetTimer(TIMER_ID1, timeout, NULL);  //定时器1，启动视景
	temp->GetDlgItem(IDC_TAB_VisionImageResult)->EnableWindow(false);
	temp->ReadDogueXYZ();	   
	assert(returnValue == TIMER_ID1);
	

	//temp->GetDlgItem(IDC_STATIC_Status)->SetWindowText("视景加载中，请耐心等候！！！");

	int result;     
	result = vp::initialize(__argc, __argv);
	assert(result == vsgu::SUCCESS);

	pVpApp = new CMyApp();//生成vpApp对象,注意必须放在vp::initialize之后
	Drawdrogue = new CDrawDrogue();

	// 设置acf文件
	const char* acfFile;
	//testtest.acf
	if (__argc <= 1)
		acfFile = "f16.acf";
	else
		acfFile = __argv[1];

	// 加载acf文件
	result = pVpApp->define(acfFile);
	assert(result == vsgu::SUCCESS);

	// 配置Vega Prime
	result = pVpApp->configure();
	assert(result == vsgu::SUCCESS);

	vpSearchPath* pSearchPath_mySearchPath = vpSearchPath::instance();
	char pFileName[MAX_PATH]; 
	int nPos = GetCurrentDirectory( MAX_PATH, pFileName); 
	CString csFullPath(pFileName);  
	pSearchPath_mySearchPath->append( csFullPath+"/VisionModel/c130grey" );
	pSearchPath_mySearchPath->append( csFullPath+"/VisionModel/tree/f16" );
	pSearchPath_mySearchPath->append( csFullPath+"/VisionModel" );

	Drawdrogue->initialize();

	vpWindow *vpWin = (vpWindow*)vpWindow::find("myWindow");
	vpChannel *CalibrationChannel = (vpChannel*)vpChannel::find("CalibrationChannel");
	CalibrationChannel->setDrawArea(0.96,1.0,0.96,1.0);


	HWND hwnd;
	hwnd = temp->GetSafeHwnd();
	temp->GetDlgItem(IDC_STATIC_VisionDisplay,&hwnd);
	vpWin->setParent(hwnd);

	// 去掉Vega Prime窗口的边界
	vpWin->setBorderEnable(false);

	// 设置Vega Prime窗口充满View窗口
	vpWin->setFullScreenEnable(true);
	//vpWin->setInputEnable(true);
	

	vpWin->open();
	::SetFocus(vpWin->getWindow());
	ExitCalibrationDiaglog = 2;
	Vision_Loading_Flag = -1;
	temp->GetDlgItem(IDC_TAB_VisionImageResult)->EnableWindow(true);
	VisionStartOverFlag = 1;

	vpWin->setUserMessageHandler(temp->WinMsgHandler);
	StartLoadVisionOver = 1;

	// 确保acf中至少由一个窗口
	assert(vpWindow::size() != 0);
	while(1)
	{
		// 调用beginFrame/endFrame强制打开Vega Prime窗口
		//NOTE: OnActivateView requires the Vega Prime window open before it is
		//		called.  If it is not open, the Vega Prime window will never
		//		get focus and thus will not handle keyboard input.
		if(ExitCalibrationDiaglog == 1)
		{
			ExitCalibrationDiaglog = 2;
			vpWindow *vpWin1 = (vpWindow*)vpWindow::find("myWindow");
			vpChannel *CalibrationChannel = (vpChannel*)vpChannel::find("CalibrationChannel");
			CalibrationChannel->setDrawArea(0.96,1.0,0.96,1.0);
			CRect rect;
			temp->GetDlgItem(IDC_STATIC_VisionDisplay)->GetClientRect(&rect);
			// 求出图片控件的宽和高
			int Rect_Width = rect.right - rect.left;
			int Rect_Height = rect.bottom - rect.top;

			HWND hwnd1;
			hwnd1 = temp->GetSafeHwnd();
			temp->GetDlgItem(IDC_STATIC_VisionDisplay,&hwnd1);
			vpWin1->setParent(hwnd1);

			// 去掉Vega Prime窗口的边界
			vpWin1->setBorderEnable(false);
			//vpWin1->setFullScreenEnable(true);
			vpWin1->setOrigin(0,0);
			vpWin1->setSize(Rect_Width,Rect_Height);

			vpWin1->open();
			::SetFocus(vpWin1->getWindow());
		}
		

		int frameNum = pVpApp->beginFrame();
		assert(frameNum != 0);

		temp->DrogueMotionPointValue();

	}
	return 0;
	
}

//获取视景显示控件区域坐标
CRect CVisionSystemQinDlg::GetCameraWndRect()
{
	CRect rect;
	GetDlgItem(IDC_STATIC_VisionDisplay)->GetWindowRect(&rect);
	//GetDlgItem(IDC_STATIC_VisionDisplay)->GetClientRect(&rect);
	return rect;
}
void CVisionSystemQinDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	VisionDisplayRect = GetCameraWndRect();
	CDialog::OnMoving(fwSide, pRect);

	// TODO: Add your message handler code here
}

//将系统时间显示为红色
HBRUSH CVisionSystemQinDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_SystemTime)
	{
		pDC->SetTextColor(RED_Color);
		return hbr;
	}
	return hbr;
}

//鼠标调节视景位置姿态响应函数
bool CVisionSystemQinDlg::WinMsgHandler(vrWindow* vrWin,vrWindow::Message id, int param1,int param2,int param3)
{
	CPoint point;
	CRect rect;
	point.x = param1;
	point.y = param2;
	if(LoadVisionPictureControlRect.PtInRect(point))
	{
		if(StartLoadVisionOver == 1)
		{
			switch (id)
			{
			case vrWindow::MESSAGE_MOUSE_LEFT_DOWN:
				RightOrLeftDownFlag = 1;
				::GetCursorPos(&LeftButtonDownPoint);
				hCur = LoadCursor( NULL , IDC_CROSS ) ;
				::SetCursor(hCur);
				break;
			case vrWindow::MESSAGE_MOUSE_LEFT_UP:
				::GetCursorPos(&LeftButtonUpPoint);
				hCur = LoadCursor( NULL , IDC_ARROW ) ;
				::SetCursor(hCur);
				RightOrLeftDownFlag = 0;
				FirstDownPointFlag = 0;
				break;
			case vrWindow::MESSAGE_MOUSE_RIGHT_DOWN:
				RightOrLeftDownFlag = 2;
				::GetCursorPos(&RightButtonDownPoint);
				hCur = LoadCursor( NULL , IDC_HAND ) ;
				::SetCursor(hCur);
				break;
			case vrWindow::MESSAGE_MOUSE_RIGHT_UP:
				::GetCursorPos(&RightButtonUpPoint);
				hCur = LoadCursor( NULL , IDC_ARROW ) ;
				::SetCursor(hCur);
				RightOrLeftDownFlag = 0;
				FirstDownPointFlag = 0;
				break;
			case vrWindow::MESSAGE_MOUSE_WHEEL:
				int aa;
				aa = 1;		
				break;
			case vrWindow::MESSAGE_MOUSE_MOVE:
				if(RightOrLeftDownFlag == 1)
				{	
					FirstDownPointFlag++;
					if(FirstDownPointFlag > 1)
					{
						LatestMouseMovePoint.x = MouseMovePoint.x;
						LatestMouseMovePoint.y = MouseMovePoint.y;

					}
					::GetCursorPos(&MouseMovePoint);
					hCur = LoadCursor( NULL , IDC_CROSS ) ;
					::SetCursor(hCur);
					if(FirstDownPointFlag > 1)
					{
						VisionChangeX = VisionChangeX - (MouseMovePoint.x - LatestMouseMovePoint.x)*10.0/LoadVisionPictureControlRect.Width();
						VisionChangeZ = VisionChangeZ + (MouseMovePoint.y - LatestMouseMovePoint.y)*10.0/LoadVisionPictureControlRect.Height();

					}


				}
				if(RightOrLeftDownFlag == 2)
				{
					FirstDownPointFlag++;
					if(FirstDownPointFlag > 1)
					{
						LatestMouseMovePoint.x = MouseMovePoint.x;
						LatestMouseMovePoint.y = MouseMovePoint.y;

					}
					::GetCursorPos(&MouseMovePoint);
					hCur = LoadCursor( NULL , IDC_HAND ) ;
					::SetCursor(hCur);
					if(FirstDownPointFlag > 1)
					{
						VisionChangeH = VisionChangeH - (MouseMovePoint.x - LatestMouseMovePoint.x)*10.0/LoadVisionPictureControlRect.Width();
						VisionChangeP = VisionChangeP - (MouseMovePoint.y - LatestMouseMovePoint.y)*10.0/LoadVisionPictureControlRect.Height();

					}
				}
				break;
			}

		}	

	}

	vrWin->defaultMessageHandler(vrWin,id,param1,param2,param3);
	return 1;
}


//是否开启欢迎界面函数
void CVisionSystemQinDlg::OnBnClickedButtonWelcomeinterfaceonoff()
{
	CString str;
	GetDlgItem(IDC_BUTTON_WelcomeInterfaceOnOff)->GetWindowText(str);
	if(str == "ON")
	{
		GetDlgItem(IDC_BUTTON_WelcomeInterfaceOnOff)->SetWindowText("OFF");
		CStdioFile mFile; 
		CFileException mExcept; 
		if(mFile.Open( "welcome.txt", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary , &mExcept ))
		{
			CString string="ON"; 
			mFile.WriteString(string); 
			mFile.Close(); 
		}else
		{
			MessageBox("无法打开welcome文本文件！！！");
		}

	}else{
		GetDlgItem(IDC_BUTTON_WelcomeInterfaceOnOff)->SetWindowText("ON");
		CStdioFile mFile; 
		CFileException mExcept; 
		if(mFile.Open( "welcome.txt", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary , &mExcept))
		{
			CString string="OFF"; 
			mFile.WriteString(string); 
			mFile.Close(); 
		}else
		{
			MessageBox("无法打开welcome文本文件！！！");
		}
		

	}
}
