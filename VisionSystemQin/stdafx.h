// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��


// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "afxbutton.h"

#include "XTToolkitPro.h"

#include "math.h"
#include <vector>

#include <opencv/cv.h> 
#include <opencv/highgui.h>

#include <opencv2/opencv.hpp>  
#include <opencv2/features2d/features2d.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
//#include <opencv2/nonfree/nonfree.hpp>  


//vega prime��
#include <vrMaterial.h>
#include "vp.h"
#include "vpModule.h"
#include "vpKernel.h"
#include "vpSearchPath.h"
#include "vpPipeline.h"
#include "vpWindow.h"
#include "vpChannel.h"
#include "vpObserver.h"
#include "vpScene.h"
#include "vpTransform.h"
#include "vpObject.h"
#include "vpIsectorXYZPR.h"
#include "vpIsectorServiceAsynchronous.h"
#include "vpRecyclingService.h"
#include "vpEnvGlobals.h"
#include "vpEnv.h"
#include "vpEnvSun.h"
#include "vpEnvMoon.h"
#include "vpEnvSkyDome.h"
#include "vpEnvCloudLayer.h"
#include "vpEnvStars.h"
#include "vpEnvWind.h"
#include "vpEnvSnow.h"
#include "vpEnvRain.h"
#include "vpApp.h"
#include "vpInputKeyboard.h"
#include "vrmode.h"
#include  "vpLight.h"


#include <vuImage.h>
#include <vuImageFactory.h>
#include <vuAllocTracer.h>

#include <GL/gl.h>

#include "vsDOF.h"
#include "vrWindow.h"
#include "vrState.h"
#include "vrGeometry.h"
#include "vrGeometryBase.h"
#include "vsGeometry.h"
#include "vrBox.h"
#include "vrFrustum.h"
#include "vrLightPoint.h"
#include "vrPlane.h"
#include "vrSphere.h"
#include "vrString.h"
#include "vrElement.h"
#include "vuFrustum.h"




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#define _CRT_SECURE_NO_WARNINGS
