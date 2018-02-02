
#include "stdafx.h"
#include "SimStructDefine.h"
#include "SimVisionSystem.h"

CPOSTION3D::CPOSTION3D(void)
{
	x=y=z=0;
}
CPOSTION3D::CPOSTION3D(double tx,double ty,double tz)
{
	x=tx;
	y=ty;
	z=tz;
}

CPOSTION3D& CPOSTION3D::operator=(const CPOSTION3D& other)//重载等号运算符
{
	if(&other!=this)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}
	return *this;
}

CPOSTION3D& CPOSTION3D::operator+(const CPOSTION3D& other)//重载加号运算符
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

CPOSTION3D& CPOSTION3D::operator-(const CPOSTION3D& other)//重载减号运算符
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

CHPR::CHPR(void)
{
	h=p=r=0;
}
CHPR::CHPR(double th,double tp,double tr)
{
	h=th;
	p=tp;
	r=tr;
}

CHPR& CHPR::operator=(const CHPR& other)//重载等号运算符
{
	if(&other!=this)
	{
		h = other.h;
		p = other.p;
		r = other.r;
	}
	return *this;
}

CHPR& CHPR::operator+(const CHPR& other)//重载加号运算符
{
	h += other.h;
	p += other.p;
	r += other.r;
	return *this;
}

CHPR& CHPR::operator-(const CHPR& other)//重载减号运算符
{
	h -= other.h;
	p -= other.p;
	r -= other.r;
	return *this;
}