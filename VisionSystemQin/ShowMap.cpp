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
		MessageBox("未进行经纬度坐标范围设置，请先进行设置！");
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
	CRect rect;//定义矩形区域
	GetDlgItem(IDC_RealTimeMAP)->GetWindowRect(&rect);//获取控件区域的矩形
	if(rect.PtInRect(point))
	{
		CMenu   menu;  
		VERIFY(menu.LoadMenu(IDR_MENU_ShowMap));  

		CMenu*   pPopup=menu.GetSubMenu(0);  
		ASSERT(pPopup!=NULL);  

		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this); //在鼠标当前位置显示指定菜单
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
	

	/*m_ctrMap->SetCenterX(116.4623);          //设置中心坐标 	
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


	CMapXLayers allLr=m_ctrMap->GetLayers();                  //取得图层集合
	CMapXLayer Lr=allLr.CreateLayer("Stlayer",NULL, 2, 32); //新建并添加一个新的图层到图层集合当中(临时图层)
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
	//指定所要显示位图的存放位置 
	
	//设置为透明 
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
				//没有tempLayer图层，就新建
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
				GetDlgItem(IDC_EDIT_ShowMap_RecieverLong)->SetWindowText(strDu+"°"+strFen+"′"+strMiao+"″");
				strDu.Format("%d",LattitudeDu);
				strFen.Format("%2d",LattitudeFen);
				strMiao.Format("%2d",LattitudeMiao);
				GetDlgItem(IDC_EDIT_ShowMap_RecieverLat)->SetWindowText(strDu+"°"+strFen+"′"+strMiao+"″");
				str_height.Format("%f",F16PositionZ[F16PositionCount-1]);
				GetDlgItem(IDC_EDIT_ShowMap_RecieverHeight)->SetWindowText(str_height);
				GetDlgItem(IDC_EDIT_ShowMap_RecieverSpeed)->SetWindowText("0");

				CMapXLayers allLr=m_ctrMap->GetLayers();                  //取得图层集合
				CMapXLayer Lr=allLr.CreateLayer("Stlayer",NULL, 2, 32); //新建并添加一个新的图层到图层集合当中(临时图层)
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
				//指定所要显示位图的存放位置 

				//设置为透明 
				ftd.GetStyle( ).SetSymbolBitmapTransparent(TRUE); 
				ftd.SetPoint( ptd );		

				Lr.AddFeature((LPDISPATCH)ftd);
				Lr.EndAccess();
				if(MapDisplayModeFlag == 2)
				{
					m_ctrMap->SetCenterX(CurrentLongtitude);          //设置中心坐标 	
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
				//没有tempLayer图层，就新建
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
					CMapXPoints  Pnts;          //点集对象
					CMapXFeatureFactory FeaFac; 
					CMapXLayer   Layer;
					CMapXFeature Feature;
					CMapXStyle   Style;
					// 判断是否存在tempLayer图层
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
					//没有tempLayer图层，就新建
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
					// 判断是否存在tempLayer图层
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
					//没有tempLayer图层，就新建
					if (Flag==false)
					{
						CMapXLayer lyr=m_ctrMap->GetLayers().CreateLayer("tempLayer");
						m_ctrMap->GetLayers().SetAnimationLayer(lyr); //设为动态图层  
					}
					Layer=m_ctrMap->GetLayers().Item("tempLayer");
					FeaFac=m_ctrMap->GetFeatureFactory();
					//加点
					for(int i = 0; i < F16PositionCount; i++)
					{
						Pnts.AddXY(CurrentLongtitude[i],CurrentLattitude[i]);
					}

					//
					COleVariant vtPoints;
					vtPoints.vt=VT_DISPATCH;
					vtPoints.pdispVal=Pnts.m_lpDispatch;
					vtPoints.pdispVal->AddRef();

					Feature=FeaFac.CreateLine(vtPoints); //按照点集画线
					Style=Feature.GetStyle();
					Style.SetLineColor(miColorRed); //线条颜色
					Style.SetLineWidth(2);          //线条宽度
					Feature.SetStyle(Style.m_lpDispatch);
					Layer.AddFeature(Feature);  //加入Feature
					Layer.Refresh();            //更新图层

				}else
				{
					MessageBox("航点数小于两个,无法显示航迹，请重试！");
					
				}
				
			}else
			{
				CMapXPoints  Pnts;          //点集对象
				CMapXFeatureFactory FeaFac; 
				CMapXLayer   Layer;
				CMapXFeature Feature;
				CMapXStyle   Style;
				// 判断是否存在tempLayer图层
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
				//没有tempLayer图层，就新建
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
			MessageBox("未检测到飞行数据！");
			KillTimer(1);
		}

	}

	CDialog::OnTimer(nIDEvent);
}

void CShowMap::OnBnClickedButtonStarttrackinglocation()
{
	if(StartTrackingLocationFlag == 0)
	{
		GetDlgItem(IDC_BUTTON_StartTrackingLocation)->SetWindowText("停止定位");
		StartTrackingLocationFlag = 1;

	}else
	{
		GetDlgItem(IDC_BUTTON_StartTrackingLocation)->SetWindowText("开始定位");
		StartTrackingLocationFlag = 0;

	}
	// TODO: Add your control notification handler code here
}


void CShowMap::OnBnClickedButtonShowflightline()
{
	if(ShowFlightLineFlag == 0)
	{
		GetDlgItem(IDC_BUTTON_ShowFlightLine)->SetWindowText("隐藏航迹");
		ShowFlightLineFlag = 1;
	}else
	{
		GetDlgItem(IDC_BUTTON_ShowFlightLine)->SetWindowText("显示航迹");
		ShowFlightLineFlag = 0;

	}
	// TODO: Add your control notification handler code here
}
/*添加线条   miAddLineTool   将线条图元添加到插入图层中。
添加点  miAddPointTool   单击该工具可将点图元添加到插入图层中。
添加折线   miAddPolyLineTool  将折线图元添加到插入图层中。
添加区域   miAddRegionTool   将区域图元添加到插入图层中。
箭头    miArrowTool    单击标题或注释此外，在可编辑图层中移动选定图元或调整选定图元的大小。
居中    miCenterTool   单击该工具可以重新将地图居中。
加标签   miLabelTool    在一个图元上单击可以给该图元加标签。
平移    miPanTool   拖动该工具可以重新将地图居中。
多边形选择    miPolygonSelectTool   单击该工具可以绘制一个多边形；该多边形内的对象将被选定。
半径选择  miRadiusSelectTool   拖动该工具可以选择半径内的图元。
矩形选择  miRectSelectTool   拖动该工具可以选择矩形内的图元。
选择工具  miSelectTool   单击该工具可以选择图元。
符号   miSymbolTool   放置符号注释。
文本   miTextTool   放置文本注释。
放大   miZoomInTool   放大。
缩小   miZoomOutTool   缩小。*/
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
