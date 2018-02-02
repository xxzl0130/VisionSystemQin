// ResultDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSystemQin.h"
#include "ResultDisplay.h"
#include "DrawDrogue.h"
#include "MyApp.h"
#include "ImageProcess.h"
#include "ResultResport.h"
#include "ShowMap.h"
#include "LongLatSetting.h"


// CResultDisplay dialog
float ResultIntrinsicfx = 0,ResultIntrinsicfy = 0,ResultIntrinsiccx = 0,ResultIntrinsiccy = 0,ResultDistortionk1 = 0,
ResultDistortionk2 = 0,ResultDistortionp1 = 0,ResultDistortionp2 = 0,ResultRotationr1 = 0,ResultRotationr2 = 0,ResultRotationr3 = 0,
ResultTranslationt1 = 0,ResultTranslationt2 = 0,ResultTranslationt3 = 0,ResultTankerX = 0,ResultTankerY = 0,ResultTankerZ = 0,
ResultTankerPitch = 0,ResultTankerRoll = 0,ResultTankerHeading = 0,ResultRecieverX = 0,ResultRecieverY = 0,ResultRecieverZ = 0,
ResultRecieverPitch = 0,ResultRecieverRoll = 0,ResultRecieverHeading = 0,ResultProbeX = 0,ResultProbeY = 0,ResultProbeZ = 0,
ResultDrogueX = 0,ResultDrogueY = 0,ResultDrogueZ = 0,ResultImageProcessDrogueX = 0,ResultImageProcessDrogueY = 0,ResultImageProcessDrogueZ = 0,
ResultPositioningAccuracyX = 0,ResultPositioningAccuracyY = 0,ResultPositioningAccuracyZ = 0,ResultAtitudeHeading = 0,ResultAtitudePitch = 0;
int VisionStartOverFlag = 0,ResultDisplayStatusFlag = 0,ResultCoordiateChooseFlag = 0;

IMPLEMENT_DYNAMIC(CResultDisplay, CDialog)

CResultDisplay::CResultDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CResultDisplay::IDD, pParent)
	, m_Resultyouxiaoshuziweishu(3)
{

}

CResultDisplay::~CResultDisplay()
{
}

BOOL CResultDisplay::OnInitDialog()
{
	CDialog::OnInitDialog();

	int TIMER_ID = 1;
	int timeout = 100;// Timeout of 33 means send a WM_TIMER message at 30Hz.
	int returnValue = SetTimer(TIMER_ID, timeout, NULL);    
	assert(returnValue == TIMER_ID);
	return TRUE;


}

void CResultDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Resultyouxiaoshuziweishu, m_Resultyouxiaoshuziweishu);


}


BEGIN_MESSAGE_MAP(CResultDisplay, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ChangeDisplatCoordiate, &CResultDisplay::OnBnClickedButtonChangedisplatcoordiate)
	ON_BN_CLICKED(IDC_BUTTON_ResultDynamicDisplay, &CResultDisplay::OnBnClickedButtonResultdynamicdisplay)
	ON_BN_CLICKED(IDC_BUTTON_ResultReport, &CResultDisplay::OnBnClickedButtonResultreport)
	ON_BN_CLICKED(IDC_BUTTON_MapDisplay, &CResultDisplay::OnBnClickedButtonMapdisplay)
	ON_BN_CLICKED(IDC_BUTTON_LongLatSetting, &CResultDisplay::OnBnClickedButtonLonglatsetting)
END_MESSAGE_MAP()


// CResultDisplay message handlers

void CResultDisplay::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void CResultDisplay::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		if(ResultCoordiateChooseFlag == 0)
		{
			if(VisionStartOverFlag == 1)
			{
				double x,y,z;//存储位置
				double h,p,r;//存储姿态

				pTanker = vpObject::find("c130");//加油机
				assert(pTanker);

				pReceiver = vpObject::find("f16");//受油机
				assert(pReceiver);

				pDrogue = vpObject::find("drogue");//受油机
				assert(pDrogue);

				Ring = vpObject::find("ring");//受油机
				assert(Ring);

				CameraTranform = vpTransform::find("farTransform");
				assert(CameraTranform);

				pReceiver->getTranslate(&x,&y,&z);
				pReceiver->getRotate(&h,&p,&r);

				ResultRecieverX = x;
				ResultRecieverY = y;
				ResultRecieverZ = z;
				ResultRecieverPitch = p;
				ResultRecieverRoll = r;
				ResultRecieverHeading = h;

				vuVec3<double> CameraTranformpos;
				CameraTranform->getAbsolutePosition(&CameraTranformpos); 

				ResultProbeX = CameraTranformpos[0];
				ResultProbeY = CameraTranformpos[1];
				ResultProbeZ = CameraTranformpos[2];


				pTanker->getTranslate(&x,&y,&z);
				pTanker->getRotate(&h,&p,&r);

				ResultTankerX = x;
				ResultTankerY = y;
				ResultTankerZ = z;
				ResultTankerPitch = p;
				ResultTankerRoll = r;
				ResultTankerHeading = h;
				//得到锥管位置
				vuVec3<double> droguepos;
				pDrogue->getAbsolutePosition(&droguepos); 
				if(DisplayDroguelocateFlag == 1)
				{
					ResultDrogueX = DroguePosition[0];
					ResultDrogueY = DroguePosition[1];
					ResultDrogueZ = DroguePosition[2];

					ResultImageProcessDrogueX = ImageDroguePositionX;
					ResultImageProcessDrogueY = ImageDroguePositionY;
					ResultImageProcessDrogueZ = ImageDroguePositionZ;

					ResultPositioningAccuracyX = DrogueLocateAccurateX;
					ResultPositioningAccuracyY = DrogueLocateAccurateY;
					ResultPositioningAccuracyZ = DrogueLocateAccurateZ;

				}
				else
				{
					ResultDrogueX = DroguePositionX;
					ResultDrogueY = DroguePositionY;
					ResultDrogueZ = DroguePositionZ;

				}
	
			}

			UpdateData(TRUE);
			CString str,youxiaoshuzi;
			if(m_Resultyouxiaoshuziweishu >= 0 && m_Resultyouxiaoshuziweishu <= 9)
			{
				char Math2char = m_Resultyouxiaoshuziweishu + 48;
				str.Format("%c",Math2char);
				youxiaoshuzi = _T("%.")+str+_T("f");
			}else
				MessageBox("请输入0-9之间的整数！！！");

			str.Format(youxiaoshuzi,ResultIntrinsicfx);
			GetDlgItem(IDC_EDIT_ResultIntrinsicfx)->SetWindowText(str);  //内参系数
			str.Format(youxiaoshuzi,ResultIntrinsicfy);
			GetDlgItem(IDC_EDIT_ResultIntrinsicfy)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultIntrinsiccx);
			GetDlgItem(IDC_EDIT_ResultIntrinsiccx)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultIntrinsiccy);
			GetDlgItem(IDC_EDIT_ResultIntrinsiccy)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultDistortionk1);
			GetDlgItem(IDC_EDIT_ResultDistortionk1)->SetWindowText(str); //形变参数
			str.Format(youxiaoshuzi,ResultDistortionk2);
			GetDlgItem(IDC_EDIT_ResultDistortionk2)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultDistortionp1);
			GetDlgItem(IDC_EDIT_ResultDistortionp1)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultDistortionp2);
			GetDlgItem(IDC_EDIT_ResultDistortionp2)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultRotationr1);
			GetDlgItem(IDC_EDIT_ResultRotationr1)->SetWindowText(str);   //旋转向量
			str.Format(youxiaoshuzi,ResultRotationr2);
			GetDlgItem(IDC_EDIT_ResultRotationr2)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultRotationr3);
			GetDlgItem(IDC_EDIT_ResultRotationr3)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultTranslationt1);
			GetDlgItem(IDC_EDIT_ResultTranslationt1)->SetWindowText(str);//平移向量
			str.Format(youxiaoshuzi,ResultTranslationt2);
			GetDlgItem(IDC_EDIT_ResultTranslationt2)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultTranslationt3);
			GetDlgItem(IDC_EDIT_ResultTranslationt3)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultTankerX);
			GetDlgItem(IDC_EDIT_ResultTankerX)->SetWindowText(str);      //加油机位置
			str.Format(youxiaoshuzi,ResultTankerY);
			GetDlgItem(IDC_EDIT_ResultTankerY)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultTankerZ);
			GetDlgItem(IDC_EDIT_ResultTankerZ)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultTankerPitch);
			GetDlgItem(IDC_EDIT_ResultTankerPitch)->SetWindowText(str);  //加油机姿态
			str.Format(youxiaoshuzi,ResultTankerRoll);
			GetDlgItem(IDC_EDIT_ResultTankerRoll)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultTankerHeading);
			GetDlgItem(IDC_EDIT_ResultTankerHeading)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultRecieverX);
			GetDlgItem(IDC_EDIT_ResultRecieverX)->SetWindowText(str);    //受油机位置
			str.Format(youxiaoshuzi,ResultRecieverY);
			GetDlgItem(IDC_EDIT_ResultRecieverY)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultRecieverZ);
			GetDlgItem(IDC_EDIT_ResultRecieverZ)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultRecieverPitch);
			GetDlgItem(IDC_EDIT_ResultRecieverPitch)->SetWindowText(str); //受油机姿态
			str.Format(youxiaoshuzi,ResultRecieverRoll);
			GetDlgItem(IDC_EDIT_ResultRecieverRoll)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultRecieverHeading);
			GetDlgItem(IDC_EDIT_ResultRecieverHeading)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultProbeX);
			GetDlgItem(IDC_EDIT_ResultProbeX)->SetWindowText(str);        //插头位置
			str.Format(youxiaoshuzi,ResultProbeY);
			GetDlgItem(IDC_EDIT_ResultProbeY)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultProbeZ);
			GetDlgItem(IDC_EDIT_ResultProbeZ)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultDrogueX);
			GetDlgItem(IDC_EDIT_ResultDrogueX)->SetWindowText(str);       //锥套位置
			str.Format(youxiaoshuzi,ResultDrogueY);
			GetDlgItem(IDC_EDIT_ResultDrogueY)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultDrogueZ);
			GetDlgItem(IDC_EDIT_ResultDrogueZ)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultImageProcessDrogueX);
			GetDlgItem(IDC_EDIT_ResultImageProcessDrogueX)->SetWindowText(str); //图像处理锥套位置
			str.Format(youxiaoshuzi,ResultImageProcessDrogueY);
			GetDlgItem(IDC_EDIT_ResultImageProcessDrogueY)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultImageProcessDrogueZ);
			GetDlgItem(IDC_EDIT_ResultImageProcessDrogueZ)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultPositioningAccuracyX);
			GetDlgItem(IDC_EDIT_ResultPositioningAccuracyX)->SetWindowText(str);//定位精度
			str.Format(youxiaoshuzi,ResultPositioningAccuracyY);
			GetDlgItem(IDC_EDIT_ResultPositioningAccuracyY)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultPositioningAccuracyZ);
			GetDlgItem(IDC_EDIT_ResultPositioningAccuracyZ)->SetWindowText(str);
			str.Format(youxiaoshuzi,ResultAtitudeHeading);
			GetDlgItem(IDC_EDIT_ResultAtitudeHeading)->SetWindowText(str);   //锥套相对摄像机姿态
			str.Format(youxiaoshuzi,ResultAtitudePitch);
			GetDlgItem(IDC_EDIT_ResultAtitudePitch)->SetWindowText(str);

		}else
			{
				if(VisionStartOverFlag == 1)
				{
					double x,y,z;//存储位置
					double h,p,r;//存储姿态

					pTanker = vpObject::find("c130");//加油机
					assert(pTanker);

					pReceiver = vpObject::find("f16");//受油机
					assert(pReceiver);

					pDrogue = vpObject::find("drogue");//受油机
					assert(pDrogue);

					Ring = vpObject::find("ring");//受油机
					assert(Ring);

					CameraTranform = vpTransform::find("farTransform");
					assert(CameraTranform);

					pReceiver->getTranslate(&x,&y,&z);
					pReceiver->getRotate(&h,&p,&r);

					ResultRecieverX = y;
					ResultRecieverY = x;
					ResultRecieverZ = -z;
					ResultRecieverPitch = r;
					ResultRecieverRoll = p;
					ResultRecieverHeading = h;

					vuVec3<double> CameraTranformpos;
					CameraTranform->getAbsolutePosition(&CameraTranformpos); 

					ResultProbeX = CameraTranformpos[1];
					ResultProbeY = CameraTranformpos[0];
					ResultProbeZ = -CameraTranformpos[2];


					pTanker->getTranslate(&x,&y,&z);
					pTanker->getRotate(&h,&p,&r);

					ResultTankerX = y;
					ResultTankerY = x;
					ResultTankerZ = -z;
					ResultTankerPitch = r;
					ResultTankerRoll = p;
					ResultTankerHeading = h;
					//得到锥管位置

					vuVec3<double> droguepos;
					pDrogue->getAbsolutePosition(&droguepos); 
					if(DisplayDroguelocateFlag == 1)
					{
						ResultDrogueX = DroguePosition[1];
						ResultDrogueY = DroguePosition[0];
						ResultDrogueZ = -DroguePosition[2];

						ResultImageProcessDrogueX = ImageDroguePositionY;
						ResultImageProcessDrogueY = ImageDroguePositionX;
						ResultImageProcessDrogueZ = -ImageDroguePositionZ;

						ResultPositioningAccuracyX = DrogueLocateAccurateY;
						ResultPositioningAccuracyY = DrogueLocateAccurateX;
						ResultPositioningAccuracyZ = DrogueLocateAccurateZ;

					}else
					{
						ResultDrogueX = DroguePositionY;
						ResultDrogueY = DroguePositionX;
						ResultDrogueZ = -DroguePositionZ;

					}
				}

				UpdateData(TRUE);
				CString str,youxiaoshuzi;
				if(m_Resultyouxiaoshuziweishu >= 0 && m_Resultyouxiaoshuziweishu <= 9)
				{
					char Math2char = m_Resultyouxiaoshuziweishu + 48;
					str.Format("%c",Math2char);
					youxiaoshuzi = _T("%.")+str+_T("f");
				}else
					MessageBox("请输入0-9之间的整数！！！");

				str.Format(youxiaoshuzi,ResultIntrinsicfx);
				GetDlgItem(IDC_EDIT_ResultIntrinsicfx)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultIntrinsicfy);
				GetDlgItem(IDC_EDIT_ResultIntrinsicfy)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultIntrinsiccx);
				GetDlgItem(IDC_EDIT_ResultIntrinsiccx)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultIntrinsiccy);
				GetDlgItem(IDC_EDIT_ResultIntrinsiccy)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultDistortionk1);
				GetDlgItem(IDC_EDIT_ResultDistortionk1)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultDistortionk2);
				GetDlgItem(IDC_EDIT_ResultDistortionk2)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultDistortionp1);
				GetDlgItem(IDC_EDIT_ResultDistortionp1)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultDistortionp2);
				GetDlgItem(IDC_EDIT_ResultDistortionp2)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultRotationr1);
				GetDlgItem(IDC_EDIT_ResultRotationr1)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultRotationr2);
				GetDlgItem(IDC_EDIT_ResultRotationr2)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultRotationr3);
				GetDlgItem(IDC_EDIT_ResultRotationr3)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultTranslationt1);
				GetDlgItem(IDC_EDIT_ResultTranslationt1)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultTranslationt2);
				GetDlgItem(IDC_EDIT_ResultTranslationt2)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultTranslationt3);
				GetDlgItem(IDC_EDIT_ResultTranslationt3)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultTankerX);
				GetDlgItem(IDC_EDIT_ResultTankerX)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultTankerY);
				GetDlgItem(IDC_EDIT_ResultTankerY)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultTankerZ);
				GetDlgItem(IDC_EDIT_ResultTankerZ)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultTankerPitch);
				GetDlgItem(IDC_EDIT_ResultTankerPitch)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultTankerRoll);
				GetDlgItem(IDC_EDIT_ResultTankerRoll)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultTankerHeading);
				GetDlgItem(IDC_EDIT_ResultTankerHeading)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultRecieverX);
				GetDlgItem(IDC_EDIT_ResultRecieverX)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultRecieverY);
				GetDlgItem(IDC_EDIT_ResultRecieverY)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultRecieverZ);
				GetDlgItem(IDC_EDIT_ResultRecieverZ)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultRecieverPitch);
				GetDlgItem(IDC_EDIT_ResultRecieverPitch)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultRecieverRoll);
				GetDlgItem(IDC_EDIT_ResultRecieverRoll)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultRecieverHeading);
				GetDlgItem(IDC_EDIT_ResultRecieverHeading)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultProbeX);
				GetDlgItem(IDC_EDIT_ResultProbeX)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultProbeY);
				GetDlgItem(IDC_EDIT_ResultProbeY)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultProbeZ);
				GetDlgItem(IDC_EDIT_ResultProbeZ)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultDrogueX);
				GetDlgItem(IDC_EDIT_ResultDrogueX)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultDrogueY);
				GetDlgItem(IDC_EDIT_ResultDrogueY)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultDrogueZ);
				GetDlgItem(IDC_EDIT_ResultDrogueZ)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultImageProcessDrogueX);
				GetDlgItem(IDC_EDIT_ResultImageProcessDrogueX)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultImageProcessDrogueY);
				GetDlgItem(IDC_EDIT_ResultImageProcessDrogueY)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultImageProcessDrogueZ);
				GetDlgItem(IDC_EDIT_ResultImageProcessDrogueZ)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultPositioningAccuracyX);
				GetDlgItem(IDC_EDIT_ResultPositioningAccuracyX)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultPositioningAccuracyY);
				GetDlgItem(IDC_EDIT_ResultPositioningAccuracyY)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultPositioningAccuracyZ);
				GetDlgItem(IDC_EDIT_ResultPositioningAccuracyZ)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultAtitudeHeading);
				GetDlgItem(IDC_EDIT_ResultAtitudeHeading)->SetWindowText(str);
				str.Format(youxiaoshuzi,ResultAtitudePitch);
				GetDlgItem(IDC_EDIT_ResultAtitudePitch)->SetWindowText(str);
		}
		

	}
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
}

//切换至飞控坐标
void CResultDisplay::OnBnClickedButtonChangedisplatcoordiate()
{
	if(ResultCoordiateChooseFlag == 0)
	{
		ResultCoordiateChooseFlag = 1;
		GetDlgItem(IDC_BUTTON_ChangeDisplatCoordiate)->SetWindowText("切换至视景坐标");
		return;
	}
	if(ResultCoordiateChooseFlag == 1)
	{
		ResultCoordiateChooseFlag = 0;
		GetDlgItem(IDC_BUTTON_ChangeDisplatCoordiate)->SetWindowText("切换至飞控坐标");
	}
	

	
	// TODO: Add your control notification handler code here
}

//暂停动态显示
void CResultDisplay::OnBnClickedButtonResultdynamicdisplay()
{
	if(ResultDisplayStatusFlag == 0)
	{
		GetDlgItem(IDC_BUTTON_ResultDynamicDisplay)->SetWindowText("开始动态显示");
		ResultDisplayStatusFlag = 1;
		KillTimer(1);
		return;
	}
	if(ResultDisplayStatusFlag == 1)
	{
		int TIMER_ID = 1;
		int timeout = 100;// Timeout of 33 means send a WM_TIMER message at 30Hz.
		int returnValue = SetTimer(TIMER_ID, timeout, NULL);    
		assert(returnValue == TIMER_ID);
		GetDlgItem(IDC_BUTTON_ResultDynamicDisplay)->SetWindowText("暂停动态显示");
		ResultDisplayStatusFlag = 0;
	}
	
	
	// TODO: Add your control notification handler code here
}

//输出结果报表
void CResultDisplay::OnBnClickedButtonResultreport()
{
	// TODO: Add your control notification handler code here
	CResultResport ResultReport;
	ResultReport.DoModal();

}

//地图显示
void CResultDisplay::OnBnClickedButtonMapdisplay()
{
	// TODO: Add your control notification handler code here
	CShowMap ShowMap;
	ShowMap.DoModal();
}

//经纬度设置
void CResultDisplay::OnBnClickedButtonLonglatsetting()
{
	CLongLatSetting LongLatSetting;
	LongLatSetting.DoModal();
	// TODO: Add your control notification handler code here
}
