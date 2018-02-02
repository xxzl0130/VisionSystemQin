// ShowMap.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSystemQin.h"
#include "ShowMap.h"
#include "MAPX.H"
#include "LongLatSetting.h"
#include "MyApp.h"

double LeftLongtitude,RightLongtitude,LeftLattitude,RightLattitude;
int StartTrackingLocationFlag,ShowFlightLineFlag,MapDisplayModeFlag;

// CShowMap dialog

IMPLEMENT_DYNAMIC(CShowMap, CDialog)

CShowMap::CShowMap(CWnd* pParent /*=NULL*/)
	: CDialog(CShowMap::IDD, pParent)
{

}

CShowMap::~CShowMap()
{
}

void CShowMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowMap, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OnLarge, &CShowMap::OnBnClickedButtonOnlarge)
	ON_BN_CLICKED(IDC_BUTTON_OnSmall, &CShowMap::OnBnClickedButtonOnsmall)
	ON_BN_CLICKED(IDC_BUTTON_OnMove, &CShowMap::OnBnClickedButtonOnmove)
	ON_BN_CLICKED(IDC_BUTTON_OnMiddle, &CShowMap::OnBnClickedButtonOnmiddle)
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MenuOnLarge, &CShowMap::OnMenuonlarge)
	ON_COMMAND(ID_MenuOnSmall, &CShowMap::OnMenuonsmall)
	ON_COMMAND(ID_MenuOnMove, &CShowMap::OnMenuonmove)
	ON_COMMAND(ID_MenuOnMiddle, &CShowMap::OnMenuonmiddle)
	ON_COMMAND(ID_MenuOnProjection, &CShowMap::OnMenuonprojection)
	ON_COMMAND(ID_MENU_ShowWholeMap, &CShowMap::OnMenuShowwholemap)
	ON_COMMAND(ID_MENUMouseControl, &CShowMap::OnMenumousecontrol)
	ON_BN_CLICKED(IDC_BUTTON_test, &CShowMap::OnBnClickedButtontest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_StartTrackingLocation, &CShowMap::OnBnClickedButtonStarttrackinglocation)
	ON_BN_CLICKED(IDC_BUTTON_ShowFlightLine, &CShowMap::OnBnClickedButtonShowflightline)
	ON_BN_CLICKED(IDC_RADIO_LockLocation, &CShowMap::OnBnClickedRadioLocklocation)
	ON_BN_CLICKED(IDC_RADIO_ShowFree, &CShowMap::OnBnClickedRadioShowfree)
END_MESSAGE_MAP()


// CShowMap message handlers

BOOL CShowMap::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CString strLic="uQnZi2sFw22L0-MRa8pYX-2E1P1085-3N9M0463-2C632204-41647-34517-52483";
	//BSTR bstrLic=strLic.AllocSysString();
	//RECT windRect;
	//GetClientRect(&windRect);
	//if (!m_ctrMapX.Create(NULL, WS_VISIBLE, windRect, this,IDC_RealTimeMAP,NULL,FALSE,bstrLic))
	//{
	//	::SysFreeString(bstrLic);
	//	return TRUE;
	//}
	//::SysFreeString(bstrLic);


	m_ctrMap = (CMapX*)GetDlgItem(IDC_RealTimeMAP);
	if(LongLatCoordiateSettingFlag != 1)
	{
		MessageBox("δ���о�γ�����귶Χ���ã����Ƚ������ã�");
		EndDialog(0);
	}
	StartTrackingLocationFlag = 0;
	ShowFlightLineFlag = 0;
	MapDisplayModeFlag = 1;
	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO_ShowFree);
	radio->SetCheck(1);
	LeftLongtitude = DUFenMiao2DotDu(LongLatSettingLongLeftDu,
		LongLatSettingLongLeftFen,LongLatSettingLongLeftMiao);
	LeftLattitude = DUFenMiao2DotDu(LongLatSettingLatLeftDu,
		LongLatSettingLatLeftFen,LongLatSettingLatLeftMiao);
	RightLongtitude = DUFenMiao2DotDu(LongLatSettingLongRightDu,
		LongLatSettingLongRightFen,LongLatSettingLongRightMiao);
	RightLattitude = DUFenMiao2DotDu(LongLatSettingLatRightDu,
		LongLatSettingLatRightFen,LongLatSettingLatRightMiao);
	//F16PositionCount = 1;
	SetTimer(1,1000,NULL);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShowMap::OnBnClickedButtonOnlarge()
{
	m_ctrMap->SetCurrentTool(miZoomInTool);
	// TODO: Add your control notification handler code here
}

void CShowMap::OnBnClickedButtonOnsmall()
{
	m_ctrMap->SetCurrentTool(miZoomOutTool);
	// TODO: Add your control notification handler code here
}

void CShowMap::OnBnClickedButtonOnmove()
{
	m_ctrMap->SetCurrentTool(miPanTool);
	
	// TODO: Add your control notification handler code here
}



void CShowMap::OnBnClickedButtonOnmiddle()
{
	m_ctrMap->SetCurrentTool(miCenterTool);
	
	// TODO: Add your control notification handler code here
}



void CShowMap::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CRect rect;//�����������
	GetDlgItem(IDC_RealTimeMAP)->GetWindowRect(&rect);//��ȡ�ؼ�����ľ���
	if(rect.PtInRect(point))
	{
		CMenu   menu;  
		VERIFY(menu.LoadMenu(IDR_MENU_ShowMap));  

		CMenu*   pPopup=menu.GetSubMenu(0);  
		ASSERT(pPopup!=NULL);  

		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this); //����굱ǰλ����ʾָ���˵�
	}
	// TODO: Add your message handler code here
}

void CShowMap::OnMenuonlarge()
{
	m_ctrMap->SetCurrentTool(miZoomInTool);
	// TODO: Add your command handler code here
}

void CShowMap::OnMenuonsmall()
{
	m_ctrMap->SetCurrentTool(miZoomOutTool);
	// TODO: Add your command handler code here
}

void CShowMap::OnMenuonmove()
{
	m_ctrMap->SetCurrentTool(miPanTool);
	// TODO: Add your command handler code here
}

void CShowMap::OnMenuonmiddle()
{
	m_ctrMap->SetCurrentTool(miCenterTool);
	// TODO: Add your command handler code here
}

void CShowMap::OnMenuonprojection()
{
	m_ctrMap->PropertyPage();
	// TODO: Add your command handler code here
}
void CShowMap::OnMenuShowwholemap()
{
	try
	{
		m_ctrMap->SetZoom(m_ctrMap->GetGeoSetWidth());
	}
	catch (COleDispatchException *e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (COleException *e)
	{
		e->ReportError();
		e->Delete();
	}

	// TODO: Add your command handler code here
}

void CShowMap::OnMenumousecontrol()
{
	m_ctrMap->SetMousewheelSupport(miMousewheelNoAutoScroll);
	// TODO: Add your command handler code here
}
void CShowMap::OnBnClickedButtontest()
{
	/*float screenX,screenY;
	double mapX,mapY;
	CMapXPoints pts;
	CMapXPoint pt;
	CMapXLayer layer ;
	pts.CreateDispatch(pts.GetClsid() );
	pt.CreateDispatch(pt.GetClsid() );

	//screenX = X;
	//screenY = Y; 
	//m_ctrMap->ConvertCoord(&screenX,&screenY,&mapX,&mapY,miScreenToMap);
	pt.Set( 116 , 40 ) ;
	pts.Add(pt);
	layer = m_ctrMap->GetLayers().CreateLayer("tmpLayer");
	
	CMapXFeatureFactory ff = m_ctrMap->GetFeatureFactory();

	COleVariant vtPoints;
	vtPoints.vt = VT_DISPATCH;
	vtPoints.pdispVal = pts.m_lpDispatch;
	vtPoints.pdispVal->AddRef();

	CMapXFeature fe = ff.CreateMultipoint( vtPoints );
	layer.AddFeature( fe ) ;*/
	

	/*m_ctrMap->SetCenterX(116.4623);          //������������ 	
	m_ctrMap->SetCenterY(40.2360);
	m_ctrMap->SetTitleText("");
	m_ctrMap->SetZoom(5);*/

	/*CMapXPoints  Pnts; 
	CMapXFeatureFactory FeaFac; 
	CMapXLayer   Layer;
	CMapXFeature Feature;
	CMapXStyle   Style;
	double centerX,centerY;
	centerX=m_ctrMap->GetCenterX();
	centerY=m_ctrMap->GetCenterY();
	Pnts.CreateDispatch(Pnts.GetClsid());
	CMapXLayers layers=m_ctrMap->GetLayers();
	BOOL Flag=false;
	for(int i=0;i<layers.GetCount();i++)
	{
		Layer=layers.Item(i+1);
		if(Layer.GetName()=="tempLayer") 
		{
			Flag=true;  
			break;
		} 
	}

	if (Flag==false)
	{
		CMapXLayer lyr=m_ctrMap->GetLayers().CreateLayer("tempLayer");
		m_ctrMap->GetLayers().SetAnimationLayer(lyr); 
	}
	Layer=m_ctrMap->GetLayers().Item("tempLayer");
	FeaFac=m_ctrMap->GetFeatureFactory();

	Pnts.AddXY(centerX,centerY);
	Pnts.AddXY(centerX+5,centerY+5);
	

	COleVariant vtPoints;
	vtPoints.vt=VT_DISPATCH;
	vtPoints.pdispVal=Pnts.m_lpDispatch;
	vtPoints.pdispVal->AddRef();

	Feature=FeaFac.CreateLine(vtPoints);
	Style=Feature.GetStyle();
	Style.SetLineColor(miColorRed);
	Style.SetLineWidth(2);
	Feature.SetStyle(Style.m_lpDispatch);
	Layer.AddFeature(Feature);
	Layer.Refresh();*/


	CMapXLayers allLr=m_ctrMap->GetLayers();                  //ȡ��ͼ�㼯��
	CMapXLayer Lr=allLr.CreateLayer("Stlayer",NULL, 2, 32); //�½������һ���µ�ͼ�㵽ͼ�㼯�ϵ���(��ʱͼ��)
	Lr.BeginAccess(miAccessReadWrite);		
	CMapXPoint ptd;
	ptd.CreateDispatch( ptd.GetClsid( ) );
	ptd.Set( 116, 46 );

	CMapXFeature ftd;
	ftd.CreateDispatch( ftd.GetClsid( ) );
	ftd.Attach( m_ctrMap->GetDispatch( FALSE ) );

	ftd.SetType( miFeatureTypeSymbol );//miFeatureTypeSymbol
	ftd.GetStyle( ).SetSymbolType( 1 );
	ftd.GetStyle( ).SetSymbolBitmapName("PIN4-32.bmp");//PIN4-32

	ftd.GetStyle( ).SetSymbolBitmapSize( 25 );
	ftd.GetStyle( ).SetSymbolBitmapColor( 0x000000 );
	//ָ����Ҫ��ʾλͼ�Ĵ��λ�� 
	
	//����Ϊ͸�� 
	ftd.GetStyle( ).SetSymbolBitmapTransparent(TRUE); 
	ftd.SetPoint( ptd );		

	Lr.AddFeature((LPDISPATCH)ftd);
	Lr.EndAccess();

	// TODO: Add your control notification handler code here
}



double CShowMap::DUFenMiao2DotDu(int Du, int Fen, int Miao)
{
	double DotDu;
	DotDu = Du + double(Fen + double(Miao)/60.0)/60.0;
	return DotDu;
}

vuVec3<int> CShowMap::DotDu2DuFenMiao(double DotDu)
{
	int Du,Fen,Miao;
	vuVec3<int> temp;
	Du = floor(DotDu);
	Fen = floor((DotDu - Du)*60);
	double DotFen = (DotDu - Du)*60;
	Miao = floor((DotFen - Fen)*60.0);
	temp[0] = Du;
	temp[1] = Fen;
	temp[2] = Miao;
	return temp;
}

void CShowMap::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		if(F16PositionCount > 0)
		{
			if(StartTrackingLocationFlag == 1)
			{
				CMapXLayer   Layer;
				CMapXLayers layers=m_ctrMap->GetLayers();
				BOOL Flag=false;
				for(int i=0;i<layers.GetCount();i++)
				{
					Layer=layers.Item(i+1);
					if(Layer.GetName()=="Stlayer") 
					{
						Flag=true;  
						break;
					} 
				}
				//û��tempLayerͼ�㣬���½�
				if (Flag==true)
				{
					Layer = m_ctrMap->GetLayers().Item("Stlayer");
					long Count;
					Layer.SetEditable(TRUE);
					Count = Layer.AllFeatures().GetCount();
					while(Count > 0)
					{
						Layer.DeleteFeature(Layer.AllFeatures().Item(Count));
						Layer.Refresh();
						Count = Layer.AllFeatures().GetCount();
						m_ctrMap->GetLayers().Remove("Stlayer");
					}


				}
				
				double CurrentLongtitude = LeftLongtitude + double(RightLongtitude - LeftLongtitude
					)/(LongLatSettingXRight - LongLatSettingXLeft)*(F16PositionX[F16PositionCount-1] - LongLatSettingXLeft);
				double CurrentLattitude = LeftLattitude + double(RightLattitude - LeftLattitude
					)/(LongLatSettingYRight - LongLatSettingYLeft)*(F16PositionY[F16PositionCount-1] - LongLatSettingYLeft);
				int LongtitudeDu,LongtitudeFen,LongtitudeMiao,LattitudeDu,LattitudeFen,LattitudeMiao;
				vuVec3<int> tempLongtitude,tempLattitude;
				tempLongtitude = DotDu2DuFenMiao(CurrentLongtitude);
				tempLattitude = DotDu2DuFenMiao(CurrentLattitude);
				LongtitudeDu = tempLongtitude[0];
				LongtitudeFen = tempLongtitude[1];
				LongtitudeMiao  = tempLongtitude[2];
				LattitudeDu = tempLattitude[0];
				LattitudeFen = tempLattitude[1];
				LattitudeMiao = tempLattitude[2];
				CString strDu,strFen,strMiao,str_height;
				strDu.Format("%d",LongtitudeDu);
				strFen.Format("%2d",LongtitudeFen);
				strMiao.Format("%2d",LongtitudeMiao);
				GetDlgItem(IDC_EDIT_ShowMap_RecieverLong)->SetWindowText(strDu+"��"+strFen+"��"+strMiao+"��");
				strDu.Format("%d",LattitudeDu);
				strFen.Format("%2d",LattitudeFen);
				strMiao.Format("%2d",LattitudeMiao);
				GetDlgItem(IDC_EDIT_ShowMap_RecieverLat)->SetWindowText(strDu+"��"+strFen+"��"+strMiao+"��");
				str_height.Format("%f",F16PositionZ[F16PositionCount-1]);
				GetDlgItem(IDC_EDIT_ShowMap_RecieverHeight)->SetWindowText(str_height);
				GetDlgItem(IDC_EDIT_ShowMap_RecieverSpeed)->SetWindowText("0");

				CMapXLayers allLr=m_ctrMap->GetLayers();                  //ȡ��ͼ�㼯��
				CMapXLayer Lr=allLr.CreateLayer("Stlayer",NULL, 2, 32); //�½������һ���µ�ͼ�㵽ͼ�㼯�ϵ���(��ʱͼ��)
				Lr.BeginAccess(miAccessReadWrite);		
				CMapXPoint ptd;
				ptd.CreateDispatch( ptd.GetClsid( ) );
				ptd.Set( CurrentLongtitude, CurrentLattitude );

				CMapXFeature ftd;
				ftd.CreateDispatch( ftd.GetClsid( ) );
				ftd.Attach( m_ctrMap->GetDispatch( FALSE ) );

				ftd.SetType( miFeatureTypeSymbol );//miFeatureTypeSymbol
				ftd.GetStyle( ).SetSymbolType( 1 );
				ftd.GetStyle( ).SetSymbolBitmapName("PIN4-32.BMP");

				ftd.GetStyle( ).SetSymbolBitmapSize( 25 );
				ftd.GetStyle( ).SetSymbolBitmapColor( 0x000000 );
				//ָ����Ҫ��ʾλͼ�Ĵ��λ�� 

				//����Ϊ͸�� 
				ftd.GetStyle( ).SetSymbolBitmapTransparent(TRUE); 
				ftd.SetPoint( ptd );		

				Lr.AddFeature((LPDISPATCH)ftd);
				Lr.EndAccess();
				if(MapDisplayModeFlag == 2)
				{
					m_ctrMap->SetCenterX(CurrentLongtitude);          //������������ 	
					m_ctrMap->SetCenterY(CurrentLattitude);
					m_ctrMap->SetTitleText("");
					//m_ctrMap->SetZoom(5);
				}
				if(MapDisplayModeFlag == 1)
				{
					//m_ctrMap->SetZoom(500);

				}

				
			}else
			{
				CMapXLayer   Layer;
				CMapXLayers layers=m_ctrMap->GetLayers();
				BOOL Flag=false;
				for(int i=0;i<layers.GetCount();i++)
				{
					Layer=layers.Item(i+1);
					if(Layer.GetName()=="Stlayer") 
					{
						Flag=true;  
						break;
					} 
				}
				//û��tempLayerͼ�㣬���½�
				if (Flag==true)
				{
					Layer = m_ctrMap->GetLayers().Item("Stlayer");
					long Count;
					Layer.SetEditable(TRUE);
					Count = Layer.AllFeatures().GetCount();
					while(Count > 0)
					{
						Layer.DeleteFeature(Layer.AllFeatures().Item(Count));
						Layer.Refresh();
						Count = Layer.AllFeatures().GetCount();
						m_ctrMap->GetLayers().Remove("Stlayer");
					}
				}
			}
			if(ShowFlightLineFlag == 1)
			{
				if(F16PositionCount > 1)
				{
					CMapXPoints  Pnts;          //�㼯����
					CMapXFeatureFactory FeaFac; 
					CMapXLayer   Layer;
					CMapXFeature Feature;
					CMapXStyle   Style;
					// �ж��Ƿ����tempLayerͼ��
					CMapXLayers layers=m_ctrMap->GetLayers();
					BOOL Flag=false;
					for(int i=0;i<layers.GetCount();i++)
					{
						Layer=layers.Item(i+1);
						if(Layer.GetName()=="tempLayer") 
						{
							Flag=true;  
							break;
						} 
					}
					//û��tempLayerͼ�㣬���½�
					if (Flag==true)
					{
						Layer = m_ctrMap->GetLayers().Item("tempLayer");
						long Count;
						Layer.SetEditable(TRUE);
						Count = Layer.AllFeatures().GetCount();
						while(Count > 0)
						{
							Layer.DeleteFeature(Layer.AllFeatures().Item(Count));
							Layer.Refresh();
							Count = Layer.AllFeatures().GetCount();
							m_ctrMap->GetLayers().Remove("tempLayer");
						}


					}

					double CurrentLongtitude[20000],CurrentLattitude[20000];
					for( int i =0;i < F16PositionCount;i++)
					{
						CurrentLongtitude[i] = LeftLongtitude + double(RightLongtitude - LeftLongtitude
							)/(LongLatSettingXRight - LongLatSettingXLeft)*(F16PositionX[i] - LongLatSettingXLeft);
						CurrentLattitude[i] = LeftLattitude + double(RightLattitude - LeftLattitude
							)/(LongLatSettingYRight - LongLatSettingYLeft)*(F16PositionY[i] - LongLatSettingYLeft);

					}

					Pnts.CreateDispatch(Pnts.GetClsid());
					// �ж��Ƿ����tempLayerͼ��
					Flag=false;
					for(int i=0;i<layers.GetCount();i++)
					{
						Layer=layers.Item(i+1);
						if(Layer.GetName()=="tempLayer") 
						{
							Flag=true;  
							break;
						} 
					}
					//û��tempLayerͼ�㣬���½�
					if (Flag==false)
					{
						CMapXLayer lyr=m_ctrMap->GetLayers().CreateLayer("tempLayer");
						m_ctrMap->GetLayers().SetAnimationLayer(lyr); //��Ϊ��̬ͼ��  
					}
					Layer=m_ctrMap->GetLayers().Item("tempLayer");
					FeaFac=m_ctrMap->GetFeatureFactory();
					//�ӵ�
					for(int i = 0; i < F16PositionCount; i++)
					{
						Pnts.AddXY(CurrentLongtitude[i],CurrentLattitude[i]);
					}

					//
					COleVariant vtPoints;
					vtPoints.vt=VT_DISPATCH;
					vtPoints.pdispVal=Pnts.m_lpDispatch;
					vtPoints.pdispVal->AddRef();

					Feature=FeaFac.CreateLine(vtPoints); //���յ㼯����
					Style=Feature.GetStyle();
					Style.SetLineColor(miColorRed); //������ɫ
					Style.SetLineWidth(2);          //�������
					Feature.SetStyle(Style.m_lpDispatch);
					Layer.AddFeature(Feature);  //����Feature
					Layer.Refresh();            //����ͼ��

				}else
				{
					MessageBox("������С������,�޷���ʾ�����������ԣ�");
					
				}
				
			}else
			{
				CMapXPoints  Pnts;          //�㼯����
				CMapXFeatureFactory FeaFac; 
				CMapXLayer   Layer;
				CMapXFeature Feature;
				CMapXStyle   Style;
				// �ж��Ƿ����tempLayerͼ��
				CMapXLayers layers=m_ctrMap->GetLayers();
				BOOL Flag=false;
				for(int i=0;i<layers.GetCount();i++)
				{
					Layer=layers.Item(i+1);
					if(Layer.GetName()=="tempLayer") 
					{
						Flag=true;  
						break;
					} 
				}
				//û��tempLayerͼ�㣬���½�
				if (Flag==true)
				{
					Layer = m_ctrMap->GetLayers().Item("tempLayer");
					long Count;
					Layer.SetEditable(TRUE);
					Count = Layer.AllFeatures().GetCount();
					while(Count > 0)
					{
						Layer.DeleteFeature(Layer.AllFeatures().Item(Count));
						Layer.Refresh();
						Count = Layer.AllFeatures().GetCount();
						m_ctrMap->GetLayers().Remove("tempLayer");
					}

				}

			}
			
			
		}else
		{
			MessageBox("δ��⵽�������ݣ�");
			KillTimer(1);
		}

	}

	CDialog::OnTimer(nIDEvent);
}

void CShowMap::OnBnClickedButtonStarttrackinglocation()
{
	if(StartTrackingLocationFlag == 0)
	{
		GetDlgItem(IDC_BUTTON_StartTrackingLocation)->SetWindowText("ֹͣ��λ");
		StartTrackingLocationFlag = 1;

	}else
	{
		GetDlgItem(IDC_BUTTON_StartTrackingLocation)->SetWindowText("��ʼ��λ");
		StartTrackingLocationFlag = 0;

	}
	// TODO: Add your control notification handler code here
}


void CShowMap::OnBnClickedButtonShowflightline()
{
	if(ShowFlightLineFlag == 0)
	{
		GetDlgItem(IDC_BUTTON_ShowFlightLine)->SetWindowText("���غ���");
		ShowFlightLineFlag = 1;
	}else
	{
		GetDlgItem(IDC_BUTTON_ShowFlightLine)->SetWindowText("��ʾ����");
		ShowFlightLineFlag = 0;

	}
	// TODO: Add your control notification handler code here
}
/*�������   miAddLineTool   ������ͼԪ��ӵ�����ͼ���С�
��ӵ�  miAddPointTool   �����ù��߿ɽ���ͼԪ��ӵ�����ͼ���С�
�������   miAddPolyLineTool  ������ͼԪ��ӵ�����ͼ���С�
�������   miAddRegionTool   ������ͼԪ��ӵ�����ͼ���С�
��ͷ    miArrowTool    ���������ע�ʹ��⣬�ڿɱ༭ͼ�����ƶ�ѡ��ͼԪ�����ѡ��ͼԪ�Ĵ�С��
����    miCenterTool   �����ù��߿������½���ͼ���С�
�ӱ�ǩ   miLabelTool    ��һ��ͼԪ�ϵ������Ը���ͼԪ�ӱ�ǩ��
ƽ��    miPanTool   �϶��ù��߿������½���ͼ���С�
�����ѡ��    miPolygonSelectTool   �����ù��߿��Ի���һ������Σ��ö�����ڵĶ��󽫱�ѡ����
�뾶ѡ��  miRadiusSelectTool   �϶��ù��߿���ѡ��뾶�ڵ�ͼԪ��
����ѡ��  miRectSelectTool   �϶��ù��߿���ѡ������ڵ�ͼԪ��
ѡ�񹤾�  miSelectTool   �����ù��߿���ѡ��ͼԪ��
����   miSymbolTool   ���÷���ע�͡�
�ı�   miTextTool   �����ı�ע�͡�
�Ŵ�   miZoomInTool   �Ŵ�
��С   miZoomOutTool   ��С��*/
void CShowMap::OnBnClickedRadioLocklocation()
{
	MapDisplayModeFlag = 2;
	// TODO: Add your control notification handler code here
}

void CShowMap::OnBnClickedRadioShowfree()
{
	MapDisplayModeFlag = 1;
	// TODO: Add your control notification handler code here
}
