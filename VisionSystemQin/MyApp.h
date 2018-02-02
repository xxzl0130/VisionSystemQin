#pragma once

class CMyApp :
	public vpApp,public vsChannel::Subscriber
{
public:
	CMyApp(void);
	~CMyApp(void);
	int configure(); //ϵͳ����
	virtual void notify(vsChannel::Event, const vsChannel *channel,vsTraversalCull *){}//����pre/post cull��Ϣ������
	virtual void notify(vsChannel::Event, const vsChannel *channel,vrDrawContext *context);//����pre / post draw��Ϣ������
	virtual void onKeyInput(vrWindow::Key key, int mod);//���ؼ�����Ϣ������
	virtual int endFrame();
	void postWeather(int ID);
	static DWORD WINAPI ImageProcess(LPVOID lpParameter);
private:
	//������ʾ�ַ����Ķ���
	vuField<vrFont2D *> m_pFont;
	vuString m_ReceiverPosxText,m_ReceiverPosyText,m_ReceiverPoszText;//λ����ʾ����
	vuString m_ReceiverHText,m_ReceiverPText,m_ReceiverRText;//��̬��ʾ����

	vuString m_TankerPosxText,m_TankerPosyText,m_TankerPoszText;//λ����ʾ����
	vuString m_TankerHText,m_TankerPText,m_TankerRText;//��̬��ʾ����

	vuString m_DroguePosxText,m_DroguePosyText,m_DroguePoszText;//׶��λ����ʾ����
	vuString m_ProbePosxText,m_ProbePosyText,m_ProbePoszText;//׶��λ����ʾ����

	vuString m_commonText;//�������ֱ���


public:
	//����λ����ʾ�����Ŀ̶���
	//void DrawLineLeft(vrDrawContext *context,double fSpeed=0,double fSetSpeed=0);
	//����λ����ʾ���Ҳ�Ŀ̶���
	//void DrawLineRight(vrDrawContext *context,double fHeight=0,double fSetHeight=0);
public:
	int m_iDisplayMode;//�۲�ģʽ����
	float m_fSpeed;//��ǰ�ٶ�
	float m_fSetSpeed;//�趨�ٶ�
	float m_fHeight;//��ǰ�߶�
	float m_fSetHeight;//��ǰ�趨�߶�.
	int OriginX,OriginY,WidthX,HeightY;

	//��Ļ��ʾ״̬
	//�����ͨ����ʾλ
	//��ʾģʽ

	
	vpObject *pTanker;		//���ͻ�
	vpObject *pDrogue;	//���ͻ�
	vpObject *Ring;
	vpObserver *pMainObserver;	//���۲���
	vpTransform *pReceiverHeadPoint,*CameraTranform;//���ͻ�ǰ��ĳ��
	vpChannel *pMainChannel;	//��ͨ��
	vpChannel *CameraChannel;	//��ͨ��
	vpChannel *CutPictureChannel;	//��ͨ��
	vpLight *MyLightSetting;	//��ͨ��

	GLubyte* buffer;

	bool Calibrationm_bCapture;
	//��ʾģʽ

	vpChannel *CalibrationpMainChannel;	//��ͨ��
	vpTransform *CalibrationfarTransform;	//��ͨ��

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
extern bool m_bScreenDisplay,m_bCamaraChannelDisplay;//������ʾλ
extern double DrogueRalativeCameraDistanceX,DrogueRalativeCameraDistanceY,DrogueRalativeCameraDistanceZ;
extern vuVec3<double> DroguePosition,CameraTranformpos;
extern double F16PositionX[20000],F16PositionY[20000],F16PositionZ[20000];
extern int F16PositionCount;
extern vpObject *pReceiver;	//���ͻ�
