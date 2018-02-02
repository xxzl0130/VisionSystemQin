// VisionSystemQin.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SimStructDefine.h"
#include "SimVisionSystem.h"
#include "VisionSystemQin.h"
#include "VisionSystemQinDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVisionSystemQinApp

BEGIN_MESSAGE_MAP(CVisionSystemQinApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVisionSystemQinApp ����

CVisionSystemQinApp::CVisionSystemQinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CVisionSystemQinApp ����

CVisionSystemQinApp theApp;


// CVisionSystemQinApp ��ʼ��

BOOL CVisionSystemQinApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	XTPSkinManager()->SetApplyOptions(  
		XTPSkinManager()->GetApplyOptions() |   
		xtpSkinApplyMetrics | xtpSkinApplyFrame | xtpSkinApplyColors | xtpSkinApplyMenus);  
	//XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);
	//�ú����ĵ�����������Ӧ�õ���Щ������Ҫʹ��Ƥ����
	XTPSkinManager()->LoadSkin( _T("Office2010.cjstyles")); //Codejock,ETag,Office2007,Office2010,Vista,WinXP.Luna,WinXP.Royale.


	CVisionSystemQinDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
