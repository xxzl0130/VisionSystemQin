#pragma once
#include "afxwin.h"


// CCalibrateCamera dialog

class CCalibrateCamera : public CDialog
{
	DECLARE_DYNAMIC(CCalibrateCamera)

public:
	CCalibrateCamera(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalibrateCamera();
	int OriginOfChessBoardPicture_Flag;
	int ColorConvert_Flag;
	int FPP_Flag,UIG_Flag,FAR_Flag,ZTD_Flag,LoadPictureFlag,LoadVisionFlag,FrameProPRocessMethodFlag;
	int code;
	CString youxiaoshuzi;
	int ExitCalibrationFlag;

	//标定的结果保存
	CvMat *intrinsics;//内参数
	CvMat *distortion_coeff;//畸变参数
	CvMat *rotation_vectors;//旋转向量
	CvMat *translation_vectors;//平移向量
	CvMat *object_points;
	CvMat *point_counts;
	CvMat *image_points;
	CvMat *image_points2;

	IplImage     *current_frame_rgb; //连接棋盘格角点图像（缩小大小后的，便于显示）

	CButton* radio;
// Dialog Data
	enum { IDD = IDD_CALIBRATECAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	//void CalibrateCamera(void);
	void InitCorners3D(CvMat* Corners3D, CvSize ChessBoardSize, int NImages, float SquareSize);
	afx_msg void OnBnClickedRadioVisionchessboard();
	afx_msg void OnBnClickedRadioLocalchessboard();
	afx_msg void OnBnClickedRadioRgb2gray();
	afx_msg void OnBnClickedRadioRgb2hsv();
	afx_msg void OnBnClickedRadioRgb2ycrcb();
	afx_msg void OnBnClickedRadioRgb2hls();
	afx_msg void OnBnClickedRadioRgb2luv();
	afx_msg void OnBnClickedRadioRgb2xyz();
	afx_msg void OnBnClickedCheckFpp();
	afx_msg void OnBnClickedCheckUig();
	afx_msg void OnBnClickedCheckFar();
	afx_msg void OnBnClickedCheckZtd();
	afx_msg void OnBnClickedButtonSettingbysystem();
	CComboBox m_Combox;
	int m_XCornorCount;
	int m_YCornorCount;
	float m_SquareSize;
	int m_ChessBoardTimes;
	afx_msg void OnBnClickedButtonStartcalibration();
	void CalibrationResult(float intr[][3], float* dist, float* rotv, float* tranv,int Pointcount);
	int m_youxiaoshuzi;
	afx_msg void OnEnChangeEdityouxiaoshuziweishu();
	void CamaeraCalibration(CvSize ImageSize, CvSize ChessBoardSize, float SquareSize, int NImages,int ChessBoardTimes);
	afx_msg void OnBnClickedButtonLoadlocalchessboard();
	void MaskActiveCottrol(void);
	void RecoverActiveControl(void);
	void LoadImage2Control(IplImage* img, UINT ID);
	afx_msg void OnBnClickedButtonExitcalibration();
	afx_msg void OnPaint();
	static DWORD WINAPI CalibrationIng(LPVOID lpParameter);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void PassPama2ResultDisplay(float intr[][3],float* dist, float* rotv, float* tranv);
	afx_msg void OnCbnSelchangeComboMethodchoose();
};
