// VisionSystemQinDlg.cpp : ʵ���ļ�
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

//���ͻ����ͻ����Ӿ�y�ᶨֵƽ�ɵı���
float yy = -2001;
//ʹ׶���˶�ƽ���˶�������������ı�־
int IncreaseOrDeincreaseFlag = 1;
//��ǰ׶���˶��������ݵ�����������
int Current_Row=0,Current_Col=0;
//��������£�24��׶���ڷ���ʱ��100s�ڵ�x��y��z���꣬
//x��ǰΪ����y����Ϊ����z����Ϊ������Ҫת�����Ӿ�����
double HoseDrogue_X[ROWS][24]={0},HoseDrogue_Y[ROWS][24]={0},HoseDrogue_Z[ROWS][24]={0};
//�ж������£�24��׶���ڷ���ʱ��100s�ڵ�x��y��z���꣬
double HoseDrogue_X_Median[ROWS][24]={0},HoseDrogue_Y_Median[ROWS][24]={0},HoseDrogue_Z_Median[ROWS][24]={0};
//�ض������£�24��׶���ڷ���ʱ��100s�ڵ�x��y��z���꣬
double HoseDrogue_X_Large[ROWS][24]={0},HoseDrogue_Y_Large[ROWS][24]={0},HoseDrogue_Z_Large[ROWS][24]={0};
//�Ӿ����ر�־��Ϊ����ʾ���Ӿ������С��⼸������
int Vision_Loading_Flag = 0;
//ͨ��������궨��ȡ��x��y�ό���ͼ�����ĵ�����
double RealTimrFocusFx, RealTimeIntrinsiccx,RealTimeIntrinsiccy,RealTimrFocusFy;
//������ⷽ����־
int ContourCenterPointSolveMethodFlag = 0;
//ϵͳ����ʱ���ӦΪСʱ�����ӣ���
int SystemSimulinkTimeH =0,SystemSimulinkTimeM =0,SystemSimulinkTimeS =0;
//f16�ɻ���ά�Ӿ�����
vuVec3<double> F16Position;
//�����갴�¡������굯���Ҽ���갴�¡��Ҽ���굯������ƶ������յ��Ӧ����Ļ�����
//�������ٿ��Ӿ�
CPoint LeftButtonDownPoint,LeftButtonUpPoint,RightButtonUpPoint,RightButtonDownPoint,
MouseMovePoint,LatestMouseMovePoint;
int RightOrLeftDownFlag = 0,FirstDownPointFlag = 0;
HCURSOR hCur;
//�Ӿ��Ƿ������ϱ�־
int StartLoadVisionOver=0;
//�Ӿ�ͼƬ�ؼ���������ֵ
CRect LoadVisionPictureControlRect;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
CRect VisionDisplayRect;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CVisionSystemQinDlg �Ի���




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


// CVisionSystemQinDlg ��Ϣ�������

BOOL CVisionSystemQinDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
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

    //���������ӶԻ���
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Tab.InsertItem(0," �Ӿ����� ");
	m_Tab.InsertItem(1," ͼ���� ");
	m_Tab.InsertItem(2," �����ʾ ");
	
	VisionProcess.Create(IDD_VisionProcess,GetDlgItem(IDC_TAB_VisionImageResult)); 
	ImageProcess.Create(IDD_ImageProcess,GetDlgItem(IDC_TAB_VisionImageResult)); 
	ResultDisplay.Create(IDD_ResultDisplay,GetDlgItem(IDC_TAB_VisionImageResult)); 

	CRect rs; 
	m_Tab.GetClientRect(&rs); 
	//�����ӶԻ����ڸ������е�λ�� 
	rs.top += 22; 
	rs.bottom -= 1; 
	rs.left += 1; 
	rs.right -= 1; 
 
	//�����ӶԻ���ߴ粢�ƶ���ָ��λ�� 
	VisionProcess.MoveWindow(&rs); 
	ImageProcess.MoveWindow(&rs);
	ResultDisplay.MoveWindow(&rs);
 
	//�ֱ��������غ���ʾ 
	VisionProcess.ShowWindow(true); 
	ImageProcess.ShowWindow(false); 
	ResultDisplay.ShowWindow(false); 

	//����Ĭ�ϵ�ѡ� 
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

	font_set.CreatePointFont(25 * 15, "�����п�");
	GetDlgItem(IDC_STATIC_Loading)->SetWindowText("�Ӿ�������");
	GetDlgItem(IDC_STATIC_Loading)->SetFont(&font_set);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVisionSystemQinDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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
		//IDB_BITMAP�����Լ���ͼ��Ӧ��ID   
		BITMAP   bitmap;   
		bmpBackground.GetBitmap(&bitmap);   
		CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
			bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);  */
		CDialog::OnPaint();
	}
	VisionDisplayRect = GetCameraWndRect();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
			GetDlgItem(IDC_STATIC_Status)->SetWindowText("�Ӿ�������ϣ����������У���");
			GetDlgItem(IDC_STATIC_Loading)->ShowWindow(FALSE);
			SetTimer(3,1000,NULL);
		}
		if(Vision_Loading_Flag%4 == 0)
		{
			GetDlgItem(IDC_STATIC_Status)->SetWindowText("�Ӿ����ڼ�����");
			GetDlgItem(IDC_STATIC_Loading)->SetWindowText("�Ӿ�������");
			GetDlgItem(IDC_STATIC_Loading)->SetFont(&font_set);
		}
		if(Vision_Loading_Flag%4 == 1)
		{
			GetDlgItem(IDC_STATIC_Status)->SetWindowText("�Ӿ����ڼ�����.");
			GetDlgItem(IDC_STATIC_Loading)->SetWindowText("�Ӿ�������.");
			GetDlgItem(IDC_STATIC_Loading)->SetFont(&font_set);
		}
		if(Vision_Loading_Flag%4 == 2)
		{
			GetDlgItem(IDC_STATIC_Status)->SetWindowText("�Ӿ����ڼ�����..");
			GetDlgItem(IDC_STATIC_Loading)->SetWindowText("�Ӿ�������..");
			GetDlgItem(IDC_STATIC_Loading)->SetFont(&font_set);
		}
		if(Vision_Loading_Flag%4 == 3)
		{
			GetDlgItem(IDC_STATIC_Status)->SetWindowText("�Ӿ����ڼ�����...");
			GetDlgItem(IDC_STATIC_Loading)->SetWindowText("�Ӿ�������...");
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


//��ȡ������������µ��������ݣ�����׶�׻�׼�ռ�����
void CVisionSystemQinDlg::ReadDogueXYZ(void)
{

	FILE *fpx,*fpy,*fpz;
	int i = 0, j = 0;
	fpx = fopen("HoseDrogue_Coordinator\\x.txt", "r");
	if(fpx == NULL)
	{
		printf("cannot open file!\n");
		MessageBox("��ȡ׶��x����ʧ�ܣ�");
	}
	while(!feof(fpx))
	{
		if(j != 23)
			fscanf(fpx, "%lf", &HoseDrogue_X[i][j++]);//ע��˴��и��ո�
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
		MessageBox("��ȡ׶��y����ʧ�ܣ�");;
	}
	while(!feof(fpy))
	{
		if(j != 23)
			fscanf(fpy, "%lf", &HoseDrogue_Y[i][j++]);//ע��˴��и��ո�
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
		MessageBox("��ȡ׶��z����ʧ�ܣ�");
	}
	while(!feof(fpz))
	{
		if(j != 23)
			fscanf(fpz, "%lf", &HoseDrogue_Z[i][j++]);//ע��˴��и��ո�
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
		MessageBox("��ȡ׶��x����ʧ�ܣ�");
	}
	while(!feof(fpx))
	{
		if(j != 23)
			fscanf(fpx, "%lf", &HoseDrogue_X_Median[i][j++]);//ע��˴��и��ո�
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
		MessageBox("��ȡ׶��y����ʧ�ܣ�");;
	}
	while(!feof(fpy))
	{
		if(j != 23)
			fscanf(fpy, "%lf", &HoseDrogue_Y_Median[i][j++]);//ע��˴��и��ո�
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
		MessageBox("��ȡ׶��z����ʧ�ܣ�");
	}
	while(!feof(fpz))
	{
		if(j != 23)
			fscanf(fpz, "%lf", &HoseDrogue_Z_Median[i][j++]);//ע��˴��и��ո�
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
		MessageBox("��ȡ׶��x����ʧ�ܣ�");
	}
	while(!feof(fpx))
	{
		if(j != 23)
			fscanf(fpx, "%lf", &HoseDrogue_X_Large[i][j++]);//ע��˴��и��ո�
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
		MessageBox("��ȡ׶��y����ʧ�ܣ�");;
	}
	while(!feof(fpy))
	{
		if(j != 23)
			fscanf(fpy, "%lf", &HoseDrogue_Y_Large[i][j++]);//ע��˴��и��ո�
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
		MessageBox("��ȡ׶��z����ʧ�ܣ�");
	}
	while(!feof(fpz))
	{
		if(j != 23)
			fscanf(fpz, "%lf", &HoseDrogue_Z_Large[i][j++]);//ע��˴��и��ո�
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

//��ά׶��ʵʱ�˶��������������Զ��������������л������Ե��ڶ�Ӧ�Ĳ���
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

//��һ�����
		Drawdrogue->DrawDrogue(point1,point2,point3,point4,point5,point6,point7,point8,point9,point10,point11,point12,
			point13,point14,point15,point16,point17,point18,point19,point20,point21,point22,point23,
			point24,vuVec4f(1.0f,1.0f,1.0f,1.0f),vuVec3f(10000,yy+1000,1500),vuVec3f(0,0,0),0.15);//12785,yy,1500


		vpObject* f16 = (vpObject*)vpObject::find("f16");
		yy = yy + 2;

		//���ͻ����ͻ�������˶�
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


//������Tab�ؼ���Ӧ�������л�Tabʱ����ʾ��ͬ���Ӿ�����
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
				if(!m_bCamaraChannelDisplay)//��ʾ
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
				if(!m_bCamaraChannelDisplay)//��ʾ
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
			if(!m_bCamaraChannelDisplay)//ֻ�����Ӿ������д������ͨ������
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
				if(!m_bCamaraChannelDisplay)//��ʾ
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
				if(!m_bCamaraChannelDisplay)//��ʾ
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


//�Ӿ���ʾ�����̺߳���
DWORD WINAPI CVisionSystemQinDlg::VisionDisplay(LPVOID lpParameter)
{
	CVisionSystemQinDlg* temp = (CVisionSystemQinDlg*)lpParameter;
	const int TIMER_ID1 = 1;
	int timeout = 750;// Timeout of 33 means send a WM_TIMER message at 30Hz.
	int returnValue = temp->SetTimer(TIMER_ID1, timeout, NULL);  //��ʱ��1�������Ӿ�
	temp->GetDlgItem(IDC_TAB_VisionImageResult)->EnableWindow(false);
	temp->ReadDogueXYZ();	   
	assert(returnValue == TIMER_ID1);
	

	//temp->GetDlgItem(IDC_STATIC_Status)->SetWindowText("�Ӿ������У������ĵȺ򣡣���");

	int result;     
	result = vp::initialize(__argc, __argv);
	assert(result == vsgu::SUCCESS);

	pVpApp = new CMyApp();//����vpApp����,ע��������vp::initialize֮��
	Drawdrogue = new CDrawDrogue();

	// ����acf�ļ�
	const char* acfFile;
	//testtest.acf
	if (__argc <= 1)
		acfFile = "f16.acf";
	else
		acfFile = __argv[1];

	// ����acf�ļ�
	result = pVpApp->define(acfFile);
	assert(result == vsgu::SUCCESS);

	// ����Vega Prime
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

	// ȥ��Vega Prime���ڵı߽�
	vpWin->setBorderEnable(false);

	// ����Vega Prime���ڳ���View����
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

	// ȷ��acf��������һ������
	assert(vpWindow::size() != 0);
	while(1)
	{
		// ����beginFrame/endFrameǿ�ƴ�Vega Prime����
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
			// ���ͼƬ�ؼ��Ŀ�͸�
			int Rect_Width = rect.right - rect.left;
			int Rect_Height = rect.bottom - rect.top;

			HWND hwnd1;
			hwnd1 = temp->GetSafeHwnd();
			temp->GetDlgItem(IDC_STATIC_VisionDisplay,&hwnd1);
			vpWin1->setParent(hwnd1);

			// ȥ��Vega Prime���ڵı߽�
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

//��ȡ�Ӿ���ʾ�ؼ���������
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

//��ϵͳʱ����ʾΪ��ɫ
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

//�������Ӿ�λ����̬��Ӧ����
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


//�Ƿ�����ӭ���溯��
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
			MessageBox("�޷���welcome�ı��ļ�������");
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
			MessageBox("�޷���welcome�ı��ļ�������");
		}
		

	}
}
