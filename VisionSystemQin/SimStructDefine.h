#ifndef SIMSTRUCTDEFINE_H
#define SIMSTRUCTDEFINE_H

//λ�ýṹ��
typedef struct POSTION3D
{
	double x;
	double y;
	double z;
}POSTION3D;

//���ͻ���ʼ�������ṹ��
typedef struct PLANE_INIT_PARAM
{
	double P0;				//��ƽ����
	double deltap0;			//��ƽ���ſ���,%
	double alpha0;			//��ƽӭ��
	double deltaz0;			//��ƽ������ƫ��
	double cy0;				//ƽ��ʱ������ϵ��
	double cx0;				//ƽ��ʱ������ϵ��
	double mz0;				//ƽ��ʱ�ĸ�������ϵ��
	double theta0;			//��ƽ������
	double H0;				//��ƽ�߶�
	double V0;				//��ƽ�ٶ�
	double Xd0;				//��ʼxd
	double Zd0;				//��ʼZd

	double extra1;			//���Ӳ���

}PLANE_INIT_PARAM;

//���ͻ���������
typedef struct RECEIVER_SIM_DATA
{
	double deltax;
	double deltay;
	double deltaz;
	double deltap;
	double xd;
	double yd;
	double zd;
	double vk;
	double vxd;
	double vyd;
	double vzd;
	double alpha;
	double beta;
	double gamma;
	double psi;
	double theta;
	double wx;
	double wy;
	double wz;
	double M;
	double P;
}RECEIVER_SIM_DATA;

//���ͻ���������
typedef struct TANKER_SIM_DATA
{
	double tv;		//����
	double psi_s;	//�����
	double txd;		//���ͻ�λ��
	double tyd;
	double tzd;
	double dxd;		//׶��λ��
	double dyd;
	double dzd;
}TANKER_SIM_DATA;

//���������
typedef struct WIND_SIM_DATA
{
	double windx;
	double windy;
	double windz;
	double windyx;
	double windyz;
	double windzx;
}WIND_SIM_DATA;

/*------------------------------------------------------*/
//��ɢ�ɻ�ר�����ݽṹ��
typedef struct DISCRETE_PLANE_STATE
{
	double V;//����
	double alpha;//ӭ��
	double wz;
	double theta;
	double H;
	double Xd;
	double dletaz;
	double deltap;
	double beta;
	double wx;
	double wy;
	double gamma;
	double psi;
	double Zd;
	double deltax;
	double deltay;
}DISCRETE_PLANE_STATE;

typedef struct DISCRETE_PLANE_INPUT
{
	double deltazc;
	double deltapc;
	double deltaxc;
	double deltayc;
	double windx;
	double windy;
	double windz;
	double windyx;
	double windyz;
	double windzx;
}DISCRETE_PLANE_INPUT;
/*------------------------------------------------------*/
#endif

#ifndef R2D
#define R2D 57.2957805	//180/PI
#endif

#ifndef D2R
#define D2R 0.017453292 //PI/180
#endif

#ifndef Pi
#define Pi 3.14159265
#endif