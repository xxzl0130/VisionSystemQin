//欢迎界面
// SplasScreen.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSystemQin.h"
#include "SplasScreen.h"



// CSplasScreen dialog

IMPLEMENT_DYNAMIC(CSplasScreen, CDialog)

CSplasScreen::CSplasScreen(CWnd* pParent /*=NULL*/)
	: CDialog(CSplasScreen::IDD, pParent)
{

}

CSplasScreen::~CSplasScreen()
{
}

void CSplasScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplasScreen, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CSplasScreen::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	return TRUE;
}


// CSplasScreen message handlers

void CSplasScreen::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
}

void CSplasScreen::OnPaint()
{
	CPaintDC   dc(this);   
	CRect   rect;   
	GetClientRect(&rect);   
	CDC   dcMem;   
	dcMem.CreateCompatibleDC(&dc);   
	CBitmap   bmpBackground;   
	bmpBackground.LoadBitmap(IDB_BITMAP_AirRefuelling);   
	//IDB_BITMAP是你自己的图对应的ID   
	BITMAP   bitmap;   
	bmpBackground.GetBitmap(&bitmap);   
	CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
		bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);   
}

BOOL CSplasScreen::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYUP)
	{
		if(pMsg->wParam ==  VK_RETURN)
		{
			SendMessage(WM_CLOSE);
			return true;
		}

	}

	return CDialog::PreTranslateMessage(pMsg);
}
