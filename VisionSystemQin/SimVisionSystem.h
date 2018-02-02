#pragma once
//三维坐标
class CPOSTION3D
{
public:
	CPOSTION3D(void);
	CPOSTION3D(double tx,double ty,double tz);
	CPOSTION3D& operator=(const CPOSTION3D& other);//重载等号运算符
	CPOSTION3D& operator+(const CPOSTION3D& other);//重载加号运算符
	CPOSTION3D& operator-(const CPOSTION3D& other);//重载减号运算符
	double x;
	double y;
	double z;

};

//三维姿态
class CHPR
{
public:

	CHPR(void);
	CHPR(double th,double tp,double tr);
	CHPR& operator=(const CHPR& other);//重载等号运算符
	CHPR& operator+(const CHPR& other);//重载加号运算符
	CHPR& operator-(const CHPR& other);//重载减号运算符

	double h;
	double p;
	double r;

};