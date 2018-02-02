#pragma once

class CMyApp :
	public vpApp,public vsChannel::Subscriber
{
public:
	CMyApp(void);
	~CMyApp(void);
	int configure(); //系统配置
	virtual void notify(vsChannel::Event, const vsChannel *channel,vsTraversalCull *){}//重载pre/post cull消息处理函数
	virtual void notify(vsChannel::Event, const vsChannel *channel,vrDrawContext *context);//重载pre / post draw消息处理函数
	virtual void onKeyInput(vrWindow::Key key, int mod);//重载键盘消息处理函数
	virtual int endFrame();
	void postWeather(int ID);
	static DWORD WINAPI ImageProcess(LPVOID lpParameter);
private:
	//用来显示字符串的东西
	vuField<vrFont2D *> m_pFont;
	vuString m_ReceiverPosxText,m_ReceiverPosyText,m_ReceiverPoszText;//位置显示文字
	vuString m_ReceiverHText,m_ReceiverPText,m_ReceiverRText;//姿态显示文字

	vuString m_TankerPosxText,m_TankerPosyText,m_TankerPoszText;//位置显示文字
	vuString m_TankerHText,m_TankerPText,m_TankerRText;//姿态显示文字

	vuString m_DroguePosxText,m_DroguePosyText,m_DroguePoszText;//锥管位置显示文字
	vuString m_ProbePosxText,m_ProbePosyText,m_ProbePoszText;//锥管位置显示文字

	vuString m_commonText;//公用文字变量


public:
	//绘制位于显示框左侧的刻度线
	//void DrawLineLeft(vrDrawContext *context,double fSpeed=0,double fSetSpeed=0);
	//绘制位于显示框右侧的刻度线
	//void DrawLineRight(vrDrawContext *context,double fHeight=0,double fSetHeight=0);
public:
	int m_iDisplayMode;//观察模式控制
	float m_fSpeed;//当前速度
	float m_fSetSpeed;//设定速度
	float m_fHeight;//当前高度
	float m_fSetHeight;//当前设定高度.
	int OriginX,OriginY,WidthX,HeightY;

	//屏幕显示状态
	//摄像机通道显示位
	//显示模式

	
	vpObject *pTanker;		//加油机
	vpObject *pDrogue;	//受油机
	vpObject *Ring;
	vpObserver *pMainObserver;	//主观察者
	vpTransform *pReceiverHeadPoint,*CameraTranform;//受油机前方某点
	vpChannel *pMainChannel;	//主通道
	vpChannel *CameraChannel;	//主通道
	vpChannel *CutPictureChannel;	//主通道
	vpLight *MyLightSetting;	//主通道

	GLubyte* buffer;

	bool Calibrationm_bCapture;
	//显示模式

	vpChannel *CalibrationpMainChannel;	//主通道
	vpTransform *CalibrationfarTransform;	//主通道

	unsigned char *Calibrationm_data;
	vuField<vuImageFactory*> Calibrationm_factory;
	HBITMAP CopyScreen2BitMap(CRect& Rect);
	void HBitmap2Ipl(HBITMAP hBmp, IplImage* dst);
};


extern CMyApp* pVpApp;	
extern double SetupFax,SetupFay,SetupFaz,SetLeftFax,SetLeftFay,SetLeftFaz;
extern int VisionMode;
extern bool Detect_Drogue_Image_Flag;
extern int VisionPictureCount,m_WheatherMode;
extern bool m_bScreenDisplay,m_bCamaraChannelDisplay;//屏显显示位
extern double DrogueRalativeCameraDistanceX,DrogueRalativeCameraDistanceY,DrogueRalativeCameraDistanceZ;
extern vuVec3<double> DroguePosition,CameraTranformpos;
extern double F16PositionX[20000],F16PositionY[20000],F16PositionZ[20000];
extern int F16PositionCount;
extern vpObject *pReceiver;	//受油机
