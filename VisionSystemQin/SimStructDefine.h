#ifndef SIMSTRUCTDEFINE_H
#define SIMSTRUCTDEFINE_H

//位置结构体
typedef struct POSTION3D
{
	double x;
	double y;
	double z;
}POSTION3D;

//受油机初始化参数结构体
typedef struct PLANE_INIT_PARAM
{
	double P0;				//配平推力
	double deltap0;			//配平油门开度,%
	double alpha0;			//配平迎角
	double deltaz0;			//配平升降舵偏角
	double cy0;				//平衡时的升力系数
	double cx0;				//平衡时的阻力系数
	double mz0;				//平衡时的俯仰力矩系数
	double theta0;			//配平俯仰角
	double H0;				//配平高度
	double V0;				//配平速度
	double Xd0;				//初始xd
	double Zd0;				//初始Zd

	double extra1;			//附加参数

}PLANE_INIT_PARAM;

//受油机仿真数据
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

//加油机仿真数据
typedef struct TANKER_SIM_DATA
{
	double tv;		//地速
	double psi_s;	//航向角
	double txd;		//加油机位置
	double tyd;
	double tzd;
	double dxd;		//锥管位置
	double dyd;
	double dzd;
}TANKER_SIM_DATA;

//风仿真数据
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
//离散飞机专用数据结构体
typedef struct DISCRETE_PLANE_STATE
{
	double V;//地速
	double alpha;//迎角
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