#pragma once
//��ά����
class CPOSTION3D
{
public:
	CPOSTION3D(void);
	CPOSTION3D(double tx,double ty,double tz);
	CPOSTION3D& operator=(const CPOSTION3D& other);//���صȺ������
	CPOSTION3D& operator+(const CPOSTION3D& other);//���ؼӺ������
	CPOSTION3D& operator-(const CPOSTION3D& other);//���ؼ��������
	double x;
	double y;
	double z;

};

//��ά��̬
class CHPR
{
public:

	CHPR(void);
	CHPR(double th,double tp,double tr);
	CHPR& operator=(const CHPR& other);//���صȺ������
	CHPR& operator+(const CHPR& other);//���ؼӺ������
	CHPR& operator-(const CHPR& other);//���ؼ��������

	double h;
	double p;
	double r;

};