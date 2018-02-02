#pragma once



class CCalibrationMyApp :
	public vpApp,public vsChannel::Subscriber
{
public:
	CCalibrationMyApp(void);
	~CCalibrationMyApp(void);
	int configure(); //系统配置
	virtual void notify(vsChannel::Event, const vsChannel *channel,vsTraversalCull *){}//重载pre/post cull消息处理函数
	virtual void notify(vsChannel::Event, const vsChannel *channel,vrDrawContext *context);//重载pre / post draw消息处理函数
	virtual void onKeyInput(vrWindow::Key key, int mod);//重载键盘消息处理函数
	virtual int endFrame();


public:

	bool Calibrationm_bCapture;
	//显示模式

	vpChannel *CalibrationpMainChannel;	//主通道
	vpTransform *CalibrationfarTransform;	//主通道

	unsigned char *Calibrationm_data;
	vuField<vuImageFactory*> Calibrationm_factory;


};

extern CCalibrationMyApp* CalibrationVpApp;
extern int VisionPictureCount;