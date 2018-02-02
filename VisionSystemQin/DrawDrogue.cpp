#include "stdafx.h"
#include "DrawDrogue.h"
#include "MyApp.h"


CDrawDrogue* Drawdrogue;
float VisionChangeX = -30,VisionChangeY = -70,VisionChangeZ = 5,
	  VisionChangeP = -5,VisionChangeH = -25,VisionChangeR = 0;
float DroguePositionX,DroguePositionY,DroguePositionZ;


CDrawDrogue::CDrawDrogue(void)
{
}

CDrawDrogue::~CDrawDrogue(void)
{
}

void CDrawDrogue::DrogueCoordinate(vuVec3f PointCenter_a, vuVec3f PointCenter_b, float r)
{
	Point_a_up[0]=PointCenter_a[0]; Point_a_up[1]=PointCenter_a[1];Point_a_up[2]=PointCenter_a[2]+r;
	Point_a_Leftup[0]=PointCenter_a[0]-r*cos(vuDeg2Rad(45.0)); Point_a_Leftup[1]=PointCenter_a[1];Point_a_Leftup[2]=PointCenter_a[2]+r*cos(vuDeg2Rad(45.0));
	Point_a_Rightup[0]=PointCenter_a[0]+r*cos(vuDeg2Rad(45.0)); Point_a_Rightup[1]=PointCenter_a[1];Point_a_Rightup[2]=PointCenter_a[2]+r*cos(vuDeg2Rad(45.0));
	Point_a_Left[0]=PointCenter_a[0]-r; Point_a_Left[1]=PointCenter_a[1];Point_a_Left[2]=PointCenter_a[2];
	Point_a_Right[0]=PointCenter_a[0]+r; Point_a_Right[1]=PointCenter_a[1];Point_a_Right[2]=PointCenter_a[2];
	Point_a_LeftDown[0]=PointCenter_a[0]-r*cos(vuDeg2Rad(45.0)); Point_a_LeftDown[1]=PointCenter_a[1];Point_a_LeftDown[2]=PointCenter_a[2]-r*cos(vuDeg2Rad(45.0));
	Point_a_RightDown[0]=PointCenter_a[0]+r*cos(vuDeg2Rad(45.0)); Point_a_RightDown[1]=PointCenter_a[1];Point_a_RightDown[2]=PointCenter_a[2]-r*cos(vuDeg2Rad(45.0));
	Point_a_Down[0]=PointCenter_a[0]; Point_a_Down[1]=PointCenter_a[1];Point_a_Down[2]=PointCenter_a[2]-r;
	//点周围八个点的坐标
	Point_b_up[0]=PointCenter_b[0]; Point_b_up[1]=PointCenter_b[1];Point_b_up[2]=PointCenter_b[2]+r;
	Point_b_Leftup[0]=PointCenter_b[0]-r*cos(vuDeg2Rad(45.0)); Point_b_Leftup[1]=PointCenter_b[1];Point_b_Leftup[2]=PointCenter_b[2]+r*cos(vuDeg2Rad(45.0));
	Point_b_Rightup[0]=PointCenter_b[0]+r*cos(vuDeg2Rad(45.0)); Point_b_Rightup[1]=PointCenter_b[1];Point_b_Rightup[2]=PointCenter_b[2]+r*cos(vuDeg2Rad(45.0));
	Point_b_Left[0]=PointCenter_b[0]-r; Point_b_Left[1]=PointCenter_b[1];Point_b_Left[2]=PointCenter_b[2];
	Point_b_Right[0]=PointCenter_b[0]+r; Point_b_Right[1]=PointCenter_b[1];Point_b_Right[2]=PointCenter_b[2];
	Point_b_LeftDown[0]=PointCenter_b[0]-r*cos(vuDeg2Rad(45.0)); Point_b_LeftDown[1]=PointCenter_b[1];Point_b_LeftDown[2]=PointCenter_b[2]-r*cos(vuDeg2Rad(45.0));
	Point_b_RightDown[0]=PointCenter_b[0]+r*cos(vuDeg2Rad(45.0)); Point_b_RightDown[1]=PointCenter_b[1];Point_b_RightDown[2]=PointCenter_b[2]-r*cos(vuDeg2Rad(45.0));
	Point_b_Down[0]=PointCenter_b[0]; Point_b_Down[1]=PointCenter_b[1];Point_b_Down[2]=PointCenter_b[2]-r;
}


void CDrawDrogue::DrawDrogue(vuVec3f point1,vuVec3f point2,vuVec3f point3,vuVec3f point4,vuVec3f point5,vuVec3f point6,
		 vuVec3f point7,vuVec3f point8,vuVec3f point9,vuVec3f point10,vuVec3f point11,vuVec3f point12,
		 vuVec3f point13,vuVec3f point14,vuVec3f point15,vuVec3f point16,vuVec3f point17,vuVec3f point18,
		 vuVec3f point19,vuVec3f point20,vuVec3f point21,vuVec3f point22,vuVec3f point23,vuVec3f point24,
		 vuVec4f color,vuVec3f settranslate,vuVec3f rotate,float raius)
{
	vpTransform *Total =new vpTransform();

	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point2[0]-point1[0],point2[1]-point1[1],point2[2]-point1[2]),
		vuVec3f(point1[0],point1[1],point1[2]),color,vuVec3f(0,0,0),raius,pTrans1,1);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point3[0]-point2[0],point3[1]-point2[1],point3[2]-point2[2]),
		vuVec3f(point2[0],point2[1],point2[2]),color,vuVec3f(0,0,0),raius,pTrans2,2);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point4[0]-point3[0],point4[1]-point3[1],point4[2]-point3[2]),
		vuVec3f(point3[0],point3[1],point3[2]),color,vuVec3f(0,0,0),raius,pTrans3,3);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point5[0]-point4[0],point5[1]-point4[1],point5[2]-point4[2]),
		vuVec3f(point4[0],point4[1],point4[2]),color,vuVec3f(0,0,0),raius,pTrans4,4);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point6[0]-point5[0],point6[1]-point5[1],point6[2]-point5[2]),
		vuVec3f(point5[0],point5[1],point5[2]),color,vuVec3f(0,0,0),raius,pTrans5,5);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point7[0]-point6[0],point7[1]-point6[1],point7[2]-point6[2]),
		vuVec3f(point6[0],point6[1],point6[2]),color,vuVec3f(0,0,0),raius,pTrans6,6);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point8[0]-point7[0],point8[1]-point7[1],point8[2]-point7[2]),
		vuVec3f(point7[0],point7[1],point7[2]),color,vuVec3f(0,0,0),raius,pTrans7,7);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point9[0]-point8[0],point9[1]-point8[1],point9[2]-point8[2]),
		vuVec3f(point8[0],point8[1],point8[2]),color,vuVec3f(0,0,0),raius,pTrans8,8);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point10[0]-point9[0],point10[1]-point9[1],point10[2]-point9[2]),
		vuVec3f(point9[0],point9[1],point9[2]),color,vuVec3f(0,0,0),raius,pTrans9,9);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point11[0]-point10[0],point11[1]-point10[1],point11[2]-point10[2]),
		vuVec3f(point10[0],point10[1],point10[2]),color,vuVec3f(0,0,0),raius,pTrans10,10);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point12[0]-point11[0],point12[1]-point11[1],point12[2]-point11[2]),
		vuVec3f(point11[0],point11[1],point11[2]),color,vuVec3f(0,0,0),raius,pTrans11,11);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point13[0]-point12[0],point13[1]-point12[1],point13[2]-point12[2]),
		vuVec3f(point12[0],point12[1],point12[2]),color,vuVec3f(0,0,0),raius,pTrans12,12);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point14[0]-point13[0],point14[1]-point13[1],point14[2]-point13[2]),
		vuVec3f(point13[0],point13[1],point13[2]),color,vuVec3f(0,0,0),raius,pTrans13,13);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point15[0]-point14[0],point15[1]-point14[1],point15[2]-point14[2]),
		vuVec3f(point14[0],point14[1],point14[2]),color,vuVec3f(0,0,0),raius,pTrans14,14);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point16[0]-point15[0],point16[1]-point15[1],point16[2]-point15[2]),
		vuVec3f(point15[0],point15[1],point15[2]),color,vuVec3f(0,0,0),raius,pTrans15,15);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point17[0]-point16[0],point17[1]-point16[1],point17[2]-point16[2]),
		vuVec3f(point16[0],point16[1],point16[2]),color,vuVec3f(0,0,0),raius,pTrans16,16);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point18[0]-point17[0],point18[1]-point17[1],point18[2]-point17[2]),
		vuVec3f(point17[0],point17[1],point17[2]),color,vuVec3f(0,0,0),raius,pTrans17,17);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point19[0]-point18[0],point19[1]-point18[1],point19[2]-point18[2]),
		vuVec3f(point18[0],point18[1],point18[2]),color,vuVec3f(0,0,0),raius,pTrans18,18);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point20[0]-point19[0],point20[1]-point19[1],point20[2]-point19[2]),
		vuVec3f(point19[0],point19[1],point19[2]),color,vuVec3f(0,0,0),raius,pTrans19,19);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point21[0]-point20[0],point21[1]-point20[1],point21[2]-point20[2]),
		vuVec3f(point20[0],point20[1],point20[2]),color,vuVec3f(0,0,0),raius,pTrans20,20);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point22[0]-point21[0],point22[1]-point21[1],point22[2]-point21[2]),
		vuVec3f(point21[0],point21[1],point21[2]),color,vuVec3f(0,0,0),raius,pTrans21,21);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point23[0]-point22[0],point23[1]-point22[1],point23[2]-point22[2]),
		vuVec3f(point22[0],point22[1],point22[2]),color,vuVec3f(0,0,0),raius,pTrans22,22);
	VSGDrogue(vuVec3f(0,0,0),vuVec3f(point24[0]-point23[0],point24[1]-point23[1],point24[2]-point23[2]),
		vuVec3f(point23[0],point23[1],point23[2]),color,vuVec3f(0,0,0),raius,pTrans23,23);

	

	Total->addChild(pTrans1);Total->addChild(pTrans2);Total->addChild(pTrans3);Total->addChild(pTrans4);
	Total->addChild(pTrans5);Total->addChild(pTrans6);Total->addChild(pTrans7);Total->addChild(pTrans8);
	Total->addChild(pTrans9);Total->addChild(pTrans10);Total->addChild(pTrans11);Total->addChild(pTrans12);
	Total->addChild(pTrans13);Total->addChild(pTrans14);Total->addChild(pTrans15);Total->addChild(pTrans16);
	Total->addChild(pTrans17);Total->addChild(pTrans18);Total->addChild(pTrans19);Total->addChild(pTrans20);
	Total->addChild(pTrans21);Total->addChild(pTrans22);Total->addChild(pTrans23);//Total->addChild(pTrans24);
	drogue->addChild(Ring);
	Ring->setTranslate(0,-0.592,0);
	drogue->setTranslate(point24[0],point24[1],point24[2]);	
	Total->addChild(drogue);
	Objectc130->addChild(Total);
	//Objectc130->addChild(ChessBoard);
	//ChessBoard->setTranslate(-13,-20,0);
	//Total->setTranslate(-13.029466,-1,0.223951);
	Total->setTranslate(-13.029466-point1[0],-1.6-point1[1],0.28-point1[2]);
	Total->setRotate(0,0,0);

	vpTransform* f16Transform=(vpTransform*)vpTransform::find("f16Transform");
	f16Transform->setTranslate(VisionChangeX,VisionChangeY,VisionChangeZ);
	f16Transform->setRotate(VisionChangeH,VisionChangeP,VisionChangeR);

	Objectc130->setTranslate(settranslate[0],settranslate[1],settranslate[2]);
	Objectc130->setRotate(rotate[0],rotate[1],rotate[2]);

	vpObject* pDrogue = (vpObject*)vpObject::find("drogue");//受油机
	assert(pDrogue);
	vuVec3<double> droguepos;
	pDrogue->getAbsolutePosition(&droguepos); 
	DroguePositionX = droguepos[0],DroguePositionY = droguepos[1],DroguePositionZ =droguepos[2];

	//vpObject* f16 = (vpObject*)vpObject::find("f16");
	//f16->setTranslate(settranslate[0]-13.029466,settranslate[1] - 30,settranslate[2] - 10);
	//f16->setRotate(rotate[0],rotate[1],rotate[2]);

	

	int result = pVpApp->endFrame();
	assert(result == vsgu::SUCCESS);
	Total->removeChild(pTrans1);Total->removeChild(pTrans2);Total->removeChild(pTrans3);Total->removeChild(pTrans4);
	Total->removeChild(pTrans5);Total->removeChild(pTrans6);Total->removeChild(pTrans7);Total->removeChild(pTrans8);
	Total->removeChild(pTrans9);Total->removeChild(pTrans10);Total->removeChild(pTrans11);Total->removeChild(pTrans12);
	Total->removeChild(pTrans13);Total->removeChild(pTrans14);Total->removeChild(pTrans15);Total->removeChild(pTrans16);
	Total->removeChild(pTrans17);Total->removeChild(pTrans18);Total->removeChild(pTrans19);Total->removeChild(pTrans20);
	Total->removeChild(pTrans21);Total->removeChild(pTrans22);Total->removeChild(pTrans23);
	drogue->removeChild(Ring);
	Total->removeChild(drogue);
	Objectc130->removeChild(Total);

}

void CDrawDrogue::VSGDrogue(vuVec3f pointa,vuVec3f pointb,vuVec3f settraslatepoint,vuVec4f Color,vuVec3f Rotate,float radius,vpTransform *Trans,int ID)
{

	switch(ID)
	{
	case 1:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_1,pFrustum1_1,pFrustum2_1,pFrustum3_1,
				pSGeom_1,pSGeom1_1,pSGeom2_1,pSGeom3_1);
			break;
		}
	case 2:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_2,pFrustum1_2,pFrustum2_2,pFrustum3_2,
				pSGeom_2,pSGeom1_2,pSGeom2_2,pSGeom3_2);
			break;
		}
	case 3:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_3,pFrustum1_3,pFrustum2_3,pFrustum3_3,
				pSGeom_3,pSGeom1_3,pSGeom2_3,pSGeom3_3);
			break;
		}
	case 4:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_4,pFrustum1_4,pFrustum2_4,pFrustum3_4,
				pSGeom_4,pSGeom1_4,pSGeom2_4,pSGeom3_4);
			break;
		}
	case 5:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_5,pFrustum1_5,pFrustum2_5,pFrustum3_5,
				pSGeom_5,pSGeom1_5,pSGeom2_5,pSGeom3_5);
			break;
		}
	case 6:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_6,pFrustum1_6,pFrustum2_6,pFrustum3_6,
				pSGeom_6,pSGeom1_6,pSGeom2_6,pSGeom3_6);
			break;
		}
	case 7:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_7,pFrustum1_7,pFrustum2_7,pFrustum3_7,
				pSGeom_7,pSGeom1_7,pSGeom2_7,pSGeom3_7);
			break;
		}
	case 8:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_8,pFrustum1_8,pFrustum2_8,pFrustum3_8,
				pSGeom_8,pSGeom1_8,pSGeom2_8,pSGeom3_8);
			break;
		}
	case 9:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_9,pFrustum1_9,pFrustum2_9,pFrustum3_9,
				pSGeom_9,pSGeom1_9,pSGeom2_9,pSGeom3_9);
			break;
		}
	case 10:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_10,pFrustum1_10,pFrustum2_10,pFrustum3_10,
				pSGeom_10,pSGeom1_10,pSGeom2_10,pSGeom3_10);
			break;
		}
	case 11:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_11,pFrustum1_11,pFrustum2_11,pFrustum3_11,
				pSGeom_11,pSGeom1_11,pSGeom2_11,pSGeom3_11);
			break;
		}
	case 12:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_12,pFrustum1_12,pFrustum2_12,pFrustum3_12,
				pSGeom_12,pSGeom1_12,pSGeom2_12,pSGeom3_12);
			break;
		}
	case 13:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_13,pFrustum1_13,pFrustum2_13,pFrustum3_13,
				pSGeom_13,pSGeom1_13,pSGeom2_13,pSGeom3_13);
			break;
		}
	case 14:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_14,pFrustum1_14,pFrustum2_14,pFrustum3_14,
				pSGeom_14,pSGeom1_14,pSGeom2_14,pSGeom3_14);
			break;
		}
	case 15:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_15,pFrustum1_15,pFrustum2_15,pFrustum3_15,
				pSGeom_15,pSGeom1_15,pSGeom2_15,pSGeom3_15);
			break;
		}
	case 16:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_16,pFrustum1_16,pFrustum2_16,pFrustum3_16,
				pSGeom_16,pSGeom1_16,pSGeom2_16,pSGeom3_16);
			break;
		}
	case 17:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_17,pFrustum1_17,pFrustum2_17,pFrustum3_17,
				pSGeom_17,pSGeom1_17,pSGeom2_17,pSGeom3_17);
			break;
		}
	case 18:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_18,pFrustum1_18,pFrustum2_18,pFrustum3_18,
				pSGeom_18,pSGeom1_18,pSGeom2_18,pSGeom3_18);
			break;
		}
	case 19:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_19,pFrustum1_19,pFrustum2_19,pFrustum3_19,
				pSGeom_19,pSGeom1_19,pSGeom2_19,pSGeom3_19);
			break;
		}
	case 20:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_20,pFrustum1_20,pFrustum2_20,pFrustum3_20,
				pSGeom_20,pSGeom1_20,pSGeom2_20,pSGeom3_20);
			break;
		}
	case 21:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_21,pFrustum1_21,pFrustum2_21,pFrustum3_21,
				pSGeom_21,pSGeom1_21,pSGeom2_21,pSGeom3_21);
			break;
		}
	case 22:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_22,pFrustum1_22,pFrustum2_22,pFrustum3_22,
				pSGeom_22,pSGeom1_22,pSGeom2_22,pSGeom3_22);
			break;
		}
	case 23:
		{
			VSGDrogueone(pointa,pointb,settraslatepoint,Color,Rotate,radius,Trans,
				pFrustum_23,pFrustum1_23,pFrustum2_23,pFrustum3_23,
				pSGeom_23,pSGeom1_23,pSGeom2_23,pSGeom3_23);
			break;
		}
	default:
		break;
	}
}


vuVec3f CDrawDrogue::SolvePlane(vuVec3f point1,vuVec3f point2,vuVec3f point3)
{
	vuVec3f xishu;
	xishu[0] = float(-(point1[1]*point2[2] - point1[2]*point2[1] - point1[1]*point3[2] + point1[2]*point3[1] + 
		point2[1]*point3[2] - point2[2]*point3[1])/(point1[0]*point2[1]*point3[2] - point1[0]*point2[2]*point3[1]
	- point1[1]*point2[0]*point3[2] + point1[1]*point3[0]*point2[2] + point2[0]*point1[2]*point3[1] - 
		point1[2]*point2[1]*point3[0]));
	xishu[1]=float((point1[0]*point2[2] - point2[0]*point1[2] - point1[0]*point3[2] + point1[2]*point3[0] + 
		point2[0]*point3[2] - point3[0]*point2[2])/(point1[0]*point2[1]*point3[2] - point1[0]*point2[2]*point3[1]
	- point1[1]*point2[0]*point3[2] + point1[1]*point3[0]*point2[2] + point2[0]*point1[2]*point3[1] - 
		point1[2]*point2[1]*point3[0]));
	xishu[2]=float(-((point1[0]*point2[1] - point1[1]*point2[0] - point1[0]*point3[1] + point1[1]*point3[0] + 
		point2[0]*point3[1]- point2[1]*point3[0]))/(point1[0]*point2[1]*point3[2] - point1[0]*point2[2]*point3[1]
	- point1[1]*point2[0]*point3[2] + point1[1]*point3[0]*point2[2] + point2[0]*point1[2]*point3[1] - 
		point1[2]*point2[1]*point3[0]));

	return xishu;
}

float CDrawDrogue::SovleOffset(vuVec3f pointCenter,vuVec3f plane)
{
	float OffSet;
	OffSet=float(fabs(plane[0]*pointCenter[0]+plane[1]*pointCenter[1]+plane[2]*pointCenter[2]+1)/
		sqrt(pow(plane[0],2)+pow(plane[1],2)+pow(plane[2],2)));
	return OffSet;

}


void CDrawDrogue::VSGDrogueone(vuVec3f pointa,vuVec3f pointb,vuVec3f settraslatepoint,vuVec4f Color,vuVec3f Rotate,float radius,vpTransform *Trans,
				  vrFrustum* pFrustum,vrFrustum* pFrustum1,vrFrustum* pFrustum2,vrFrustum* pFrustum3,
				  vsGeometry* pSGeom,vsGeometry* pSGeom1,vsGeometry* pSGeom2,vsGeometry* pSGeom3)

{
	vuVec3f faxiangliang,faxiangliang1;
	float offset,offset1;
	float zjbl;
	vuFrustum <float>vuf,vuf1,vuf2,vuf3;



	DrogueCoordinate(pointa,pointb,radius);
	faxiangliang = SolvePlane(Point_a_up,Point_a_Leftup,Point_b_Leftup);
	faxiangliang1 = SolvePlane(Point_a_Leftup,Point_a_Down,Point_b_Down);
	offset = SovleOffset(pointa,faxiangliang);
	offset1 = SovleOffset(pointa,faxiangliang1);
	zjbl = sqrt(pow(faxiangliang[0],2)+pow(faxiangliang[1],2)+pow(faxiangliang[2],2));
	faxiangliang[0]=float(faxiangliang[0]/zjbl);
	faxiangliang[1]=float(faxiangliang[1]/zjbl);
	faxiangliang[2]=float(faxiangliang[2]/zjbl);
	zjbl = sqrt(pow(faxiangliang1[0],2)+pow(faxiangliang1[1],2)+pow(faxiangliang1[2],2));
	faxiangliang1[0]=float(faxiangliang1[0]/zjbl);
	faxiangliang1[1]=float(faxiangliang1[1]/zjbl);
	faxiangliang1[2]=float(faxiangliang1[2]/zjbl);
	SetupFax=faxiangliang[0]; SetupFay=faxiangliang[1]; SetupFaz=faxiangliang[2];
	SetLeftFax=faxiangliang1[0]; SetLeftFay=faxiangliang1[1]; SetLeftFaz=faxiangliang1[2];//faxiangliang1[2];
	offseta = offset1;
	offsetb = offset;

	vuf.makeAsymmetricFOV(offseta,offseta,offsetb,offsetb,fabs(pointb[1]-pointa[1]),0);


	pFrustum->setFrustum(vuf);
	pFrustum->setColor(Color[0],Color[1],Color[2],Color[3]);
	pFrustum->ref();

	pSGeom->setGeometryBase(pFrustum);
	pSGeom->setState(pState);
	pSGeom->ref();

	Trans->setTranslate(settraslatepoint[0],settraslatepoint[1],settraslatepoint[2]);
	Trans->setRotate(Rotate[0],Rotate[1],Rotate[2]);
	Trans->setScale(1,1,1);
	Trans->push_back_child(pSGeom);
	Trans->ref();

	DrogueCoordinate(pointa,pointb,radius);
	faxiangliang = SolvePlane(Point_a_up,Point_a_Rightup,Point_b_Rightup);
	faxiangliang1 = SolvePlane(Point_a_up,Point_a_LeftDown,Point_b_LeftDown);
	offset = SovleOffset(pointa,faxiangliang);
	offset1 = SovleOffset(pointa,faxiangliang1);
	zjbl = sqrt(pow(faxiangliang[0],2)+pow(faxiangliang[1],2)+pow(faxiangliang[2],2));
	faxiangliang[0]=float(faxiangliang[0]/zjbl);
	faxiangliang[1]=float(faxiangliang[1]/zjbl);
	faxiangliang[2]=float(faxiangliang[2]/zjbl);
	zjbl = sqrt(pow(faxiangliang1[0],2)+pow(faxiangliang1[1],2)+pow(faxiangliang1[2],2));
	faxiangliang1[0]=float(faxiangliang1[0]/zjbl);
	faxiangliang1[1]=float(faxiangliang1[1]/zjbl);
	faxiangliang1[2]=float(faxiangliang1[2]/zjbl);
	SetupFax=faxiangliang[0]; SetupFay=faxiangliang[1]; SetupFaz=faxiangliang[2];
	SetLeftFax=faxiangliang1[0]; SetLeftFay=faxiangliang1[1]; SetLeftFaz=faxiangliang1[2];//faxiangliang1[2];
	offseta = offset1;
	offsetb = offset;

	vuf1.makeAsymmetricFOV(offseta,offseta,offsetb,offsetb,fabs(pointb[1]-pointa[1]),0);

	pFrustum1->setFrustum(vuf1);
	pFrustum1->setColor(Color[0],Color[1],Color[2],Color[3]);
	pFrustum1->ref();

	pSGeom1->setGeometryBase(pFrustum1);
	pSGeom1->setState(pState);
	pSGeom1->ref();

	Trans->setTranslate(settraslatepoint[0],settraslatepoint[1],settraslatepoint[2]);
	Trans->setRotate(Rotate[0],Rotate[1],Rotate[2]);
	Trans->setScale(1,1,1);
	Trans->push_back_child(pSGeom1);
	Trans->ref();


	DrogueCoordinate(pointa,pointb,radius);
	faxiangliang = SolvePlane(Point_a_Left,Point_a_Leftup,Point_b_Leftup);
	faxiangliang1 = SolvePlane(Point_a_Left,Point_a_RightDown,Point_b_RightDown);
	offset = SovleOffset(pointa,faxiangliang);
	offset1 = SovleOffset(pointa,faxiangliang1);
	zjbl = sqrt(pow(faxiangliang[0],2)+pow(faxiangliang[1],2)+pow(faxiangliang[2],2));
	faxiangliang[0]=float(faxiangliang[0]/zjbl);
	faxiangliang[1]=float(faxiangliang[1]/zjbl);
	faxiangliang[2]=float(faxiangliang[2]/zjbl);
	zjbl = sqrt(pow(faxiangliang1[0],2)+pow(faxiangliang1[1],2)+pow(faxiangliang1[2],2));
	faxiangliang1[0]=float(faxiangliang1[0]/zjbl);
	faxiangliang1[1]=float(faxiangliang1[1]/zjbl);
	faxiangliang1[2]=float(faxiangliang1[2]/zjbl);
	SetupFax=faxiangliang[0]; SetupFay=faxiangliang[1]; SetupFaz=faxiangliang[2];
	SetLeftFax=faxiangliang1[0]; SetLeftFay=faxiangliang1[1]; SetLeftFaz=faxiangliang1[2];//faxiangliang1[2];
	offseta = offset1;
	offsetb =offset;

	vuf2.makeAsymmetricFOV(offseta,offseta,offsetb,offsetb,fabs(pointb[1]-pointa[1]),0);

	pFrustum2->setFrustum(vuf2);
	pFrustum2->setColor(Color[0],Color[1],Color[2],Color[3]);
	pFrustum2->ref();

	pSGeom2->setGeometryBase(pFrustum2);
	pSGeom2->setState(pState);
	pSGeom2->ref();

	Trans->setTranslate(settraslatepoint[0],settraslatepoint[1],settraslatepoint[2]);
	Trans->setRotate(Rotate[0],Rotate[1],Rotate[2]);
	Trans->setScale(1,1,1);
	Trans->push_back_child(pSGeom2);
	Trans->ref();



	DrogueCoordinate(pointa,pointb,radius);
	faxiangliang = SolvePlane(Point_a_Rightup,Point_a_Right,Point_b_Right);
	faxiangliang1 = SolvePlane(Point_a_Rightup,Point_a_Left,Point_b_Left);
	offset = SovleOffset(pointa,faxiangliang);
	offset1 = SovleOffset(pointa,faxiangliang1);
	zjbl = sqrt(pow(faxiangliang[0],2)+pow(faxiangliang[1],2)+pow(faxiangliang[2],2));
	faxiangliang[0]=float(faxiangliang[0]/zjbl);
	faxiangliang[1]=float(faxiangliang[1]/zjbl);
	faxiangliang[2]=float(faxiangliang[2]/zjbl);
	zjbl = sqrt(pow(faxiangliang1[0],2)+pow(faxiangliang1[1],2)+pow(faxiangliang1[2],2));
	faxiangliang1[0]=float(faxiangliang1[0]/zjbl);
	faxiangliang1[1]=float(faxiangliang1[1]/zjbl);
	faxiangliang1[2]=float(faxiangliang1[2]/zjbl);
	SetupFax=faxiangliang[0]; SetupFay=faxiangliang[1]; SetupFaz=faxiangliang[2];
	SetLeftFax=faxiangliang1[0]; SetLeftFay=faxiangliang1[1]; SetLeftFaz=faxiangliang1[2];//faxiangliang1[2];
	offseta = offset1;
	offsetb =offset;

	vuf3.makeAsymmetricFOV(offseta,offseta,offsetb,offsetb,fabs(pointb[1]-pointa[1]),0);

	pFrustum3->setFrustum(vuf3);
	pFrustum3->setColor(Color[0],Color[1],Color[2],Color[3]);
	pFrustum3->ref();

	pSGeom3->setGeometryBase(pFrustum3);
	pSGeom3->setState(pState);
	pSGeom3->ref();

	Trans->setTranslate(settraslatepoint[0],settraslatepoint[1],settraslatepoint[2]);
	Trans->setRotate(Rotate[0],Rotate[1],Rotate[2]);
	Trans->setScale(1,1,1);
	Trans->push_back_child(pSGeom3);
	Trans->ref();

}
void CDrawDrogue::initialize()
{
	pTrans1=new vpTransform();pTrans2=new vpTransform();pTrans3=new vpTransform();
	pTrans4=new vpTransform();pTrans5=new vpTransform();pTrans6=new vpTransform();
	pTrans7=new vpTransform();pTrans8=new vpTransform();pTrans9=new vpTransform();
	pTrans10=new vpTransform();pTrans11=new vpTransform();pTrans12=new vpTransform();
	pTrans13=new vpTransform();pTrans14=new vpTransform();pTrans15=new vpTransform();
	pTrans16=new vpTransform();pTrans17=new vpTransform();pTrans18=new vpTransform();
	pTrans19=new vpTransform();pTrans20=new vpTransform();pTrans21=new vpTransform();
	pTrans22=new vpTransform();pTrans23=new vpTransform();

	pFrustum_1= new vrFrustum();pFrustum1_1= new vrFrustum();pFrustum2_1= new vrFrustum();pFrustum3_1= new vrFrustum();pFrustum_2= new vrFrustum();pFrustum1_2= new vrFrustum();pFrustum2_2= new vrFrustum();pFrustum3_2= new vrFrustum();
	pFrustum_3= new vrFrustum();pFrustum1_3= new vrFrustum();pFrustum2_3= new vrFrustum();pFrustum3_3= new vrFrustum();pFrustum_4= new vrFrustum();pFrustum1_4= new vrFrustum();pFrustum2_4= new vrFrustum();pFrustum3_4= new vrFrustum();
	pFrustum_5= new vrFrustum();pFrustum1_5= new vrFrustum();pFrustum2_5= new vrFrustum();pFrustum3_5= new vrFrustum();pFrustum_6= new vrFrustum();pFrustum1_6= new vrFrustum();pFrustum2_6= new vrFrustum();pFrustum3_6= new vrFrustum();
	pFrustum_7= new vrFrustum();pFrustum1_7= new vrFrustum();pFrustum2_7= new vrFrustum();pFrustum3_7= new vrFrustum();pFrustum_8= new vrFrustum();pFrustum1_8= new vrFrustum();pFrustum2_8= new vrFrustum();pFrustum3_8= new vrFrustum();
	pFrustum_9= new vrFrustum();pFrustum1_9= new vrFrustum();pFrustum2_9= new vrFrustum();pFrustum3_9= new vrFrustum();pFrustum_10= new vrFrustum();pFrustum1_10= new vrFrustum();pFrustum2_10= new vrFrustum();pFrustum3_10= new vrFrustum();
	pFrustum_11= new vrFrustum();pFrustum1_11= new vrFrustum();pFrustum2_11= new vrFrustum();pFrustum3_11= new vrFrustum();pFrustum_12= new vrFrustum();pFrustum1_12= new vrFrustum();pFrustum2_12= new vrFrustum();pFrustum3_12= new vrFrustum();
	pFrustum_13= new vrFrustum();pFrustum1_13= new vrFrustum();pFrustum2_13= new vrFrustum();pFrustum3_13= new vrFrustum();pFrustum_14= new vrFrustum();pFrustum1_14= new vrFrustum();pFrustum2_14= new vrFrustum();pFrustum3_14= new vrFrustum();
	pFrustum_15= new vrFrustum();pFrustum1_15= new vrFrustum();pFrustum2_15= new vrFrustum();pFrustum3_15= new vrFrustum();pFrustum_16= new vrFrustum();pFrustum1_16= new vrFrustum();pFrustum2_16= new vrFrustum();pFrustum3_16= new vrFrustum();
	pFrustum_17= new vrFrustum();pFrustum1_17= new vrFrustum();pFrustum2_17= new vrFrustum();pFrustum3_17= new vrFrustum();pFrustum_18= new vrFrustum();pFrustum1_18= new vrFrustum();pFrustum2_18= new vrFrustum();pFrustum3_18= new vrFrustum();
	pFrustum_19= new vrFrustum();pFrustum1_19= new vrFrustum();pFrustum2_19= new vrFrustum();pFrustum3_19= new vrFrustum();pFrustum_20= new vrFrustum();pFrustum1_20= new vrFrustum();pFrustum2_20= new vrFrustum();pFrustum3_20= new vrFrustum();
	pFrustum_21= new vrFrustum();pFrustum1_21= new vrFrustum();pFrustum2_21= new vrFrustum();pFrustum3_21= new vrFrustum();pFrustum_22= new vrFrustum();pFrustum1_22= new vrFrustum();pFrustum2_22= new vrFrustum();pFrustum3_22= new vrFrustum();
	pFrustum_23= new vrFrustum();pFrustum1_23= new vrFrustum();pFrustum2_23= new vrFrustum();pFrustum3_23= new vrFrustum();pFrustum_24= new vrFrustum();pFrustum1_24= new vrFrustum();pFrustum2_24= new vrFrustum();pFrustum3_24= new vrFrustum();


	pSGeom_1= new vsGeometry;pSGeom1_1= new vsGeometry;pSGeom2_1= new vsGeometry;pSGeom3_1= new vsGeometry;pSGeom_2= new vsGeometry;pSGeom1_2= new vsGeometry;pSGeom2_2= new vsGeometry;pSGeom3_2= new vsGeometry;
	pSGeom_3= new vsGeometry;pSGeom1_3= new vsGeometry;pSGeom2_3= new vsGeometry;pSGeom3_3= new vsGeometry;pSGeom_4= new vsGeometry;pSGeom1_4= new vsGeometry;pSGeom2_4= new vsGeometry;pSGeom3_4= new vsGeometry;
	pSGeom_5= new vsGeometry;pSGeom1_5= new vsGeometry;pSGeom2_5= new vsGeometry;pSGeom3_5= new vsGeometry;pSGeom_6= new vsGeometry;pSGeom1_6= new vsGeometry;pSGeom2_6= new vsGeometry;pSGeom3_6= new vsGeometry;
	pSGeom_7= new vsGeometry;pSGeom1_7= new vsGeometry;pSGeom2_7= new vsGeometry;pSGeom3_7= new vsGeometry;pSGeom_8= new vsGeometry;pSGeom1_8= new vsGeometry;pSGeom2_8= new vsGeometry;pSGeom3_8= new vsGeometry;
	pSGeom_9= new vsGeometry;pSGeom1_9= new vsGeometry;pSGeom2_9= new vsGeometry;pSGeom3_9= new vsGeometry;pSGeom_10= new vsGeometry;pSGeom1_10= new vsGeometry;pSGeom2_10= new vsGeometry;pSGeom3_10= new vsGeometry;
	pSGeom_11= new vsGeometry;pSGeom1_11= new vsGeometry;pSGeom2_11= new vsGeometry;pSGeom3_11= new vsGeometry;pSGeom_12= new vsGeometry;pSGeom1_12= new vsGeometry;pSGeom2_12= new vsGeometry;pSGeom3_12= new vsGeometry;
	pSGeom_13= new vsGeometry;pSGeom1_13= new vsGeometry;pSGeom2_13= new vsGeometry;pSGeom3_13= new vsGeometry;pSGeom_14= new vsGeometry;pSGeom1_14= new vsGeometry;pSGeom2_14= new vsGeometry;pSGeom3_14= new vsGeometry;
	pSGeom_15= new vsGeometry;pSGeom1_15= new vsGeometry;pSGeom2_15= new vsGeometry;pSGeom3_15= new vsGeometry;pSGeom_16= new vsGeometry;pSGeom1_16= new vsGeometry;pSGeom2_16= new vsGeometry;pSGeom3_16= new vsGeometry;
	pSGeom_17= new vsGeometry;pSGeom1_17= new vsGeometry;pSGeom2_17= new vsGeometry;pSGeom3_17= new vsGeometry;pSGeom_18= new vsGeometry;pSGeom1_18= new vsGeometry;pSGeom2_18= new vsGeometry;pSGeom3_18= new vsGeometry;
	pSGeom_19= new vsGeometry;pSGeom1_19= new vsGeometry;pSGeom2_19= new vsGeometry;pSGeom3_19= new vsGeometry;pSGeom_20= new vsGeometry;pSGeom1_20= new vsGeometry;pSGeom2_20= new vsGeometry;pSGeom3_20= new vsGeometry;
	pSGeom_21= new vsGeometry;pSGeom1_21= new vsGeometry;pSGeom2_21= new vsGeometry;pSGeom3_21= new vsGeometry;pSGeom_22= new vsGeometry;pSGeom1_22= new vsGeometry;pSGeom2_22= new vsGeometry;pSGeom3_22= new vsGeometry;
	pSGeom_23= new vsGeometry;pSGeom1_23= new vsGeometry;pSGeom2_23= new vsGeometry;pSGeom3_23= new vsGeometry;pSGeom_24= new vsGeometry;pSGeom1_24= new vsGeometry;pSGeom2_24= new vsGeometry;pSGeom3_24= new vsGeometry;
	vrTexture::State *m_textureState = new vrTexture::State;
	m_textureState->ref();
	vrTextureFactory *textureFactory = new vrTextureFactory();
	vrTexture* m_texture = textureFactory->read("drogue.rgb");
	vrTexture::Element m_textureElement;
	if (m_texture != NULL) m_texture->ref();
	m_textureState->setEnable(true);
	m_textureState->setTexture(m_texture);
	textureFactory->unref();
	m_textureElement.push_back_texture_state(m_textureState);

	vrLight::Element m_lightElement;
	m_lightElement.m_enable = false;

	alphaBlendElement.m_enable=true;
	pState=new vrState;
	pState->setElement(vrTexture::Element::Id,&m_textureElement);
	//pState->setElement(vrLight::Element::Id,&m_lightElement);

	drogue=(vpObject*)vpObject::find("drogue");
	Objectc130=(vpObject*)vpObject::find("c130");
	Ring = (vpObject*)vpObject::find("ring");
}
