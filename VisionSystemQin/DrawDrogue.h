#pragma once


class CDrawDrogue
{
public:

	float offseta,offsetb;

	vuVec3f Point_a_up,Point_a_Leftup,Point_a_Left,Point_a_LeftDown,Point_a_Down,Point_a_RightDown,
		Point_a_Right,Point_a_Rightup;
	vuVec3f Point_b_up,Point_b_Leftup,Point_b_Left,Point_b_LeftDown,Point_b_Down,Point_b_RightDown,
		Point_b_Right,Point_b_Rightup;
	vrAlphaBlend::Element alphaBlendElement;
	vrState *pState;

	vpTransform *pTrans1;vpTransform *pTrans2;vpTransform *pTrans3;
	vpTransform *pTrans4;vpTransform *pTrans5;vpTransform *pTrans6;
	vpTransform *pTrans7;vpTransform *pTrans8;vpTransform *pTrans9;
	vpTransform *pTrans10;vpTransform *pTrans11;vpTransform *pTrans12;
	vpTransform *pTrans13;vpTransform *pTrans14;vpTransform *pTrans15;
	vpTransform *pTrans16;vpTransform *pTrans17;vpTransform *pTrans18;
	vpTransform *pTrans19;vpTransform *pTrans20;vpTransform *pTrans21;
	vpTransform *pTrans22;vpTransform *pTrans23;
	vrFrustum *pFrustum_1,*pFrustum1_1,*pFrustum2_1,*pFrustum3_1,*pFrustum_2,*pFrustum1_2,*pFrustum2_2,*pFrustum3_2,
		*pFrustum_3,*pFrustum1_3,*pFrustum2_3,*pFrustum3_3,*pFrustum_4,*pFrustum1_4,*pFrustum2_4,*pFrustum3_4,
		*pFrustum_5,*pFrustum1_5,*pFrustum2_5,*pFrustum3_5,*pFrustum_6,*pFrustum1_6,*pFrustum2_6,*pFrustum3_6,
		*pFrustum_7,*pFrustum1_7,*pFrustum2_7,*pFrustum3_7,*pFrustum_8,*pFrustum1_8,*pFrustum2_8,*pFrustum3_8,
		*pFrustum_9,*pFrustum1_9,*pFrustum2_9,*pFrustum3_9,*pFrustum_10,*pFrustum1_10,*pFrustum2_10,*pFrustum3_10,
		*pFrustum_11,*pFrustum1_11,*pFrustum2_11,*pFrustum3_11,*pFrustum_12,*pFrustum1_12,*pFrustum2_12,*pFrustum3_12,
		*pFrustum_13,*pFrustum1_13,*pFrustum2_13,*pFrustum3_13,*pFrustum_14,*pFrustum1_14,*pFrustum2_14,*pFrustum3_14,
		*pFrustum_15,*pFrustum1_15,*pFrustum2_15,*pFrustum3_15,*pFrustum_16,*pFrustum1_16,*pFrustum2_16,*pFrustum3_16,
		*pFrustum_17,*pFrustum1_17,*pFrustum2_17,*pFrustum3_17,*pFrustum_18,*pFrustum1_18,*pFrustum2_18,*pFrustum3_18,
		*pFrustum_19,*pFrustum1_19,*pFrustum2_19,*pFrustum3_19,*pFrustum_20,*pFrustum1_20,*pFrustum2_20,*pFrustum3_20,
		*pFrustum_21,*pFrustum1_21,*pFrustum2_21,*pFrustum3_21,*pFrustum_22,*pFrustum1_22,*pFrustum2_22,*pFrustum3_22,
		*pFrustum_23,*pFrustum1_23,*pFrustum2_23,*pFrustum3_23,*pFrustum_24,*pFrustum1_24,*pFrustum2_24,*pFrustum3_24;


	vsGeometry *pSGeom_1,*pSGeom1_1,*pSGeom2_1,*pSGeom3_1,*pSGeom_2,*pSGeom1_2,*pSGeom2_2,*pSGeom3_2,
		*pSGeom_3,*pSGeom1_3,*pSGeom2_3,*pSGeom3_3,*pSGeom_4,*pSGeom1_4,*pSGeom2_4,*pSGeom3_4,
		*pSGeom_5,*pSGeom1_5,*pSGeom2_5,*pSGeom3_5,*pSGeom_6,*pSGeom1_6,*pSGeom2_6,*pSGeom3_6,
		*pSGeom_7,*pSGeom1_7,*pSGeom2_7,*pSGeom3_7,*pSGeom_8,*pSGeom1_8,*pSGeom2_8,*pSGeom3_8,
		*pSGeom_9,*pSGeom1_9,*pSGeom2_9,*pSGeom3_9,*pSGeom_10,*pSGeom1_10,*pSGeom2_10,*pSGeom3_10,
		*pSGeom_11,*pSGeom1_11,*pSGeom2_11,*pSGeom3_11,*pSGeom_12,*pSGeom1_12,*pSGeom2_12,*pSGeom3_12,
		*pSGeom_13,*pSGeom1_13,*pSGeom2_13,*pSGeom3_13,*pSGeom_14,*pSGeom1_14,*pSGeom2_14,*pSGeom3_14,
		*pSGeom_15,*pSGeom1_15,*pSGeom2_15,*pSGeom3_15,*pSGeom_16,*pSGeom1_16,*pSGeom2_16,*pSGeom3_16,
		*pSGeom_17,*pSGeom1_17,*pSGeom2_17,*pSGeom3_17,*pSGeom_18,*pSGeom1_18,*pSGeom2_18,*pSGeom3_18,
		*pSGeom_19,*pSGeom1_19,*pSGeom2_19,*pSGeom3_19,*pSGeom_20,*pSGeom1_20,*pSGeom2_20,*pSGeom3_20,
		*pSGeom_21,*pSGeom1_21,*pSGeom2_21,*pSGeom3_21,*pSGeom_22,*pSGeom1_22,*pSGeom2_22,*pSGeom3_22,
		*pSGeom_23,*pSGeom1_23,*pSGeom2_23,*pSGeom3_23,*pSGeom_24,*pSGeom1_24,*pSGeom2_24,*pSGeom3_24;
	vpObject* drogue;
	vpObject* Objectc130;
	vpObject* Ring;
	CDrawDrogue(void);
	~CDrawDrogue(void);
	vuVec3f SolvePlane(vuVec3f point1, vuVec3f point2, vuVec3f point3);
	void DrogueCoordinate(vuVec3f PointCenter_a, vuVec3f PointCenter_b, float r);
	void initialize();
	void VSGDrogue(vuVec3f pointa,vuVec3f pointb,vuVec3f settraslatepoint,vuVec4f Color,vuVec3f Rotate,float radius,vpTransform *Trans,int ID);
	void DrawDrogue(vuVec3f point1,vuVec3f point2,vuVec3f point3,vuVec3f point4,vuVec3f point5,vuVec3f point6,
		vuVec3f point7,vuVec3f point8,vuVec3f point9,vuVec3f point10,vuVec3f point11,vuVec3f point12,
		vuVec3f point13,vuVec3f point14,vuVec3f point15,vuVec3f point16,vuVec3f point17,vuVec3f point18,
		vuVec3f point19,vuVec3f point20,vuVec3f point21,vuVec3f point22,vuVec3f point23,vuVec3f point24,
		vuVec4f color,vuVec3f settranslate,vuVec3f rotate,float raius);
	void VSGDrogueone(vuVec3f pointa,vuVec3f pointb,vuVec3f settraslatepoint,vuVec4f Color,vuVec3f Rotate,float radius,vpTransform *Trans,
		vrFrustum* pFrustum,vrFrustum* pFrustum1,vrFrustum* pFrustum2,vrFrustum* pFrustum3,
		vsGeometry* pSGeom,vsGeometry* pSGeom1,vsGeometry* pSGeom2,vsGeometry* pSGeom3);
	float SovleOffset(vuVec3f pointCenter,vuVec3f plane);
};
	extern CDrawDrogue* Drawdrogue;
	extern float VisionChangeX,VisionChangeY,VisionChangeZ,VisionChangeP,VisionChangeH,VisionChangeR;
	extern float DroguePositionX,DroguePositionY,DroguePositionZ;