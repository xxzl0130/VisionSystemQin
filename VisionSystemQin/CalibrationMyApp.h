#pragma once



class CCalibrationMyApp :
	public vpApp,public vsChannel::Subscriber
{
public:
	CCalibrationMyApp(void);
	~CCalibrationMyApp(void);
	int configure(); //ϵͳ����
	virtual void notify(vsChannel::Event, const vsChannel *channel,vsTraversalCull *){}//����pre/post cull��Ϣ������
	virtual void notify(vsChannel::Event, const vsChannel *channel,vrDrawContext *context);//����pre / post draw��Ϣ������
	virtual void onKeyInput(vrWindow::Key key, int mod);//���ؼ�����Ϣ������
	virtual int endFrame();


public:

	bool Calibrationm_bCapture;
	//��ʾģʽ

	vpChannel *CalibrationpMainChannel;	//��ͨ��
	vpTransform *CalibrationfarTransform;	//��ͨ��

	unsigned char *Calibrationm_data;
	vuField<vuImageFactory*> Calibrationm_factory;


};

extern CCalibrationMyApp* CalibrationVpApp;
extern int VisionPictureCount;