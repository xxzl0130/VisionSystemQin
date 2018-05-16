#include "LHM.h"
#include <iostream>
using namespace std;

#define rad2deg(x) ((x) / M_PI * 180)

double point2Angle(const cv::Point2f& point, const cv::Point2f& center)
{
	return atan2(point.x - center.x, point.y - center.y);
}

void sortPointsAnticlockXn(std::vector<cv::Point2f>& points, const cv::Point2f& center)
{
	for (auto i = 0; i < points.size(); ++i)
	{
		double maxAngle = -M_PI * 2;
		int maxIndex = i;
		for (auto j = i; j < points.size(); ++j)
		{
			auto tmp = point2Angle(points[j], center);
			if (tmp > maxAngle)
			{
				maxAngle = tmp;
				maxIndex = j;
			}
		}
		std::swap(points[i], points[maxIndex]);
	}
}

#define func(a,b,c,d) \
	if(points[a].y > points[b].y)\
	{\
		res[c] = points[a];\
		res[d] = points[b];\
	}\
	else\
	{\
		res[c] = points[b];\
		res[d] = points[a];\
	}

void ImagePointSortLeft(std::vector<cv::Point2f>& points)
{
	std::vector<cv::Point2f> res(points.size());
	for(auto i = 0;i < points.size();++i)
	{
		double minX = 1e8;
		int index = i;
		for (auto j = i; j < points.size(); ++j)
		{
			if (points[j].x < minX)
			{
				minX = points[j].x;
				index = j;
			}
		}
		swap(points[i], points[index]);
	}
	res[0] = points[0];
	res[6] = points[6];
	
	func(1, 2, 1, 11)
	func(3, 4, 2, 10)
	func(5, 6, 3, 9)
	func(7, 8, 4, 8)
	func(9, 10, 5, 7)

	points = res;
}

cv::Mat LHM_Locate(const std::vector<cv::Point2f>& centers, double centerX, double centerY, const cv::Rect& roi)
{
	cv::Point2f center(centerX, centerY);
	cv::Point2f offset(roi.x, roi.y);
	std::vector<cv::Point2f> points;
	static double Drogue3Dpotion[12][3] = { { -0.409481,0.000113,-0.58 },
	{ -0.355070,0.205000,-0.58 },{ -0.205000,0.355070,-0.58 },{ 0,0.41,-0.58 },{ 0.205000,0.355070,-0.58 },
	{ 0.355070,0.205000,-0.58 },{ 0.410073,-0.000065,-0.58 },{ 0.355070,-0.205000,-0.58 },{ 0.205000,-0.355070,-0.58 },
	{ 0,-0.41,-0.58 },{ -0.205000,-0.355070,-0.58 },{ -0.355070,-0.205000,-0.58 } };
	static cv::Mat drogue3DPotionMat(12, 3, CV_64FC1, Drogue3Dpotion);
	points.reserve(centers.size());
	for(auto& it:centers)
	{
		points.emplace_back(cv::Point2f(it + offset));
	}

	//sortPointsAnticlockXn(points, cv::Point2f(centerX, centerY));
	ImagePointSortLeft(points);
	cv::Mat drogue2DImagePotionMat(points.size(), 3, CV_64FC1);
	
	for (int i = 0; i < points.size(); ++i)
	{
		drogue2DImagePotionMat.at<double>(i, 0) = points[i].x;
		drogue2DImagePotionMat.at<double>(i, 1) = points[i].y;
		drogue2DImagePotionMat.at<double>(i, 2) = 1;
	}
	double InitRotate[3][3], pitch = 0, yaw = 0, roll = 0;
	InitRotate[0][0] = cos(roll)*cos(pitch);
	InitRotate[1][0] = sin(roll)*cos(pitch);
	InitRotate[2][0] = -sin(pitch);
	InitRotate[0][1] = cos(roll)*sin(pitch)*sin(yaw) - sin(yaw);
	InitRotate[1][1] = sin(roll)*sin(pitch) + cos(yaw)*cos(roll);
	InitRotate[2][1] = cos(yaw)*sin(yaw);
	InitRotate[0][2] = cos(roll)*sin(pitch) - sin(yaw)*sin(roll);
	InitRotate[1][2] = sin(roll)*sin(pitch)*cos(yaw) + cos(roll)*sin(yaw);
	InitRotate[2][2] = cos(pitch)*cos(yaw);
	cv::Mat initRotateMat(3, 3, CV_64FC1, InitRotate);
	cv::Mat translation = SolveAtitude_LHM(drogue2DImagePotionMat, drogue3DPotionMat, initRotateMat);
	//cv::Mat translation2 = cv::cvarrToMat(SolveAtitude_LHM2(drogue3DPotionMat, drogue2DImagePotionMat, initRotateMat));
	
	cv::Mat res(5, 1, CV_64FC1);

	//cout << translation.t() << endl << translation2.t() << endl << endl;
	//translation = translation2;
	res.at<double>(0) = translation.at<double>(0, 0); //x
	res.at<double>(1) = translation.at<double>(2, 0); //y
	res.at<double>(2) = -translation.at<double>(1, 0);//z
	res.at<double>(3) = atan2(res.at<double>(1),-res.at<double>(0));//heading
	res.at<double>(4) = atan2(res.at<double>(1), -res.at<double>(2));//pitch

	return res;
}

cv::Mat SolveAtitude_LHM(const cv::Mat& input2DPoints, const cv::Mat& input3DPoints, cv::Mat initRotation)
{
	int pointNum = input2DPoints.rows;
	double Error = 0.01;
	cv::Mat translation(3, 1, CV_64FC1);
	cv::Mat probeMatInv;
	cv::invert(probeMat, probeMatInv);
	int count = 0;

	while (true)
	{
		double ERt = 0;
		cv::Mat temp(3, 1, CV_64FC1, cv::Scalar::all(0));
		cv::Mat tempVi(3, 3, CV_64FC1, cv::Scalar::all(0));
		cv::Mat identity(3, 3, CV_64FC1);
		cv::setIdentity(identity);

		for (int i = 0; i < pointNum; ++i)
		{
			cv::Mat viMat(input2DPoints.row(i).t());

			viMat = probeMatInv * viMat;
			cv::Mat dst1 = viMat * viMat.t();
			cv::Mat dst2 = viMat.t() * viMat;
			cv::Mat viMat1(3, 3, CV_64FC1);
			dst1.convertTo(viMat1, CV_64FC1, 1 / dst2.at<double>(0, 0));

			cv::Mat piMat(input3DPoints.row(i).t());
			cv::Mat dst3 = initRotation * piMat;
			cv::Mat dst5 = viMat1 - identity;
			temp += dst5 * dst3;
			tempVi += viMat1;
			//cout << input3DPoints.row(i).t() << endl << piMat << endl;
		}

		cv::Mat temp2(3, 3, CV_64FC1);
		tempVi.convertTo(temp2, -1, 1.0 / pointNum);
		temp2 = identity - temp2;
		cv::invert(temp2, temp2);
		temp2.convertTo(temp2, -1, 1.0 / pointNum);
		translation = temp2 * temp;
		//cout << temp << endl << temp2 << endl << tempVi << endl << translation << endl;


		cv::Mat tempQMean(3, 1, CV_64FC1, cv::Scalar::all(0));
		cv::Mat tempPMean(3, 1, CV_64FC1, cv::Scalar::all(0));

		for (int i = 0; i < pointNum; ++i)
		{
			cv::Mat viMat(input2DPoints.row(i).t());
			viMat = probeMatInv * viMat;
			cv::Mat dst1 = viMat * viMat.t();
			cv::Mat dst2 = viMat.t() * viMat;
			cv::Mat viMat1(3, 3, CV_64FC1);
			dst1.convertTo(viMat1, CV_64FC1, 1 / dst2.at<double>(0, 0));

			cv::Mat piMat(input3DPoints.row(i).t());
			cv::Mat tempERt = initRotation * piMat + translation;
			tempERt = (identity - viMat1) * tempERt;
			ERt += cv::norm(tempERt);

			cv::Mat tempMedia = initRotation * piMat + translation;
			cv::Mat qiR = viMat1 * tempMedia;
			tempQMean += qiR;
			tempPMean += piMat;
		}

		if (++count > 10)
		{
			Error += 0.1;
		}
		if (ERt < Error)
		{
			break;
		}

		tempQMean.convertTo(tempQMean, -1, 1.0 / pointNum);
		tempPMean.convertTo(tempPMean, -1, 1.0 / pointNum);

		cv::Mat MR(3, 3, CV_64FC1, cv::Scalar::all(0));
		for (int i = 0; i < pointNum; ++i)
		{
			cv::Mat dQiR(3, 1, CV_64FC1);
			cv::Mat dPi(3, 1, CV_64FC1);
			cv::Mat viMat(input2DPoints.row(i).t());

			viMat = probeMatInv * viMat;
			cv::Mat dst1 = viMat * viMat.t();
			cv::Mat dst2 = viMat.t() * viMat;
			cv::Mat viMat1(3, 3, CV_64FC1);
			dst1.convertTo(viMat1, CV_64FC1, 1 / dst2.at<double>(0, 0));

			cv::Mat piMat(input3DPoints.row(i).t());
			cv::Mat tempMedia = initRotation * piMat + translation;
			cv::Mat qiR = viMat1 * tempMedia;

			dPi = piMat - tempPMean;
			dQiR = qiR - tempQMean;
			cv::Mat tempMR = dQiR * dPi.t();
			MR += tempMR;
		}

		cv::Mat W(3, 3, CV_64FC1), U(3, 3, CV_64FC1), V(3, 3, CV_64FC1);
		cv::SVD::compute(MR, W, U, V);
		initRotation = V * U.t();
	}

	return translation;
}

CvMat* SolveAtitude_LHM2(CvMat Input3DPoint, CvMat Input2DPoint, CvMat InitRotation)
{

	int PointNum = 12;
	double Error = 0.01;
	int count = 0;
	CvMat *translation = cvCreateMat(3, 1, CV_64FC1);
	double IntriMatrix[3][3] = { 0 };
	IntriMatrix[0][0] = 949.408;
	IntriMatrix[1][1] = 949.408;
	IntriMatrix[2][2] = 1;
	IntriMatrix[0][2] = 394.021;
	IntriMatrix[1][2] = 305.328;
	CvMat IntriMat = cvMat(3, 3, CV_64FC1, IntriMatrix);

	while (1)
	{
		double ERt = 0;
		CvMat *temp = cvCreateMat(3, 1, CV_64FC1);
		cvSetZero(temp);
		CvMat *tempVi = cvCreateMat(3, 3, CV_64FC1);
		cvSetZero(tempVi);

		for (int i = 0; i<PointNum; i++)
		{
			double viMatrix[3];
			viMatrix[0] = cvmGet(&Input2DPoint, i, 0);
			viMatrix[1] = cvmGet(&Input2DPoint, i, 1);
			viMatrix[2] = cvmGet(&Input2DPoint, i, 2);
			CvMat viMat = cvMat(3, 1, CV_64FC1, viMatrix);
			CvMat *IntriMatInvert = cvCreateMat(3, 3, CV_64FC1);
			cvInvert(&IntriMat, IntriMatInvert, CV_LU);
			cvGEMM(IntriMatInvert, &viMat, 1, NULL, 0, &viMat, 0);

			CvMat *Dst1 = cvCreateMat(3, 3, CV_64FC1);
			CvMat *Dst2 = cvCreateMat(1, 1, CV_64FC1);
			cvMulTransposed(&viMat, Dst1, 0);
			cvMulTransposed(&viMat, Dst2, -1);
			CvMat *Dst2Inv = cvCreateMat(1, 1, CV_64FC1);
			cvInvert(Dst2, Dst2Inv, CV_LU);
			CvMat *ViMat1 = cvCreateMat(3, 3, CV_64FC1);
			cvConvertScale(Dst1, ViMat1, cvmGet(Dst2Inv, 0, 0), 0);
			//cvGEMM(Dst1,Dst2Inv,1,NULL,0,ViMat1,0);

			double piMatrix[3];
			piMatrix[0] = cvmGet(&Input3DPoint, i, 0);
			piMatrix[1] = cvmGet(&Input3DPoint, i, 1);
			piMatrix[2] = cvmGet(&Input3DPoint, i, 2);
			CvMat piMat = cvMat(3, 1, CV_64FC1, piMatrix);
			CvMat *Dst3 = cvCreateMat(3, 1, CV_64FC1);
			CvMat *Dst4 = cvCreateMat(3, 1, CV_64FC1);
			CvMat *Dst5 = cvCreateMat(3, 3, CV_64FC1);
			CvMat *Tempresult = cvCreateMat(3, 1, CV_64FC1);
			CvMat *Indentity = cvCreateMat(3, 3, CV_64FC1);
			cvSetIdentity(Indentity);
			cvGEMM(&InitRotation, &piMat, 1, NULL, 0, Dst3, 0);
			cvSub(ViMat1, Indentity, Dst5);
			cvGEMM(Dst5, Dst3, 1, NULL, 0, Tempresult, 0);
			cvAdd(temp, Tempresult, temp);
			cvAdd(tempVi, ViMat1, tempVi);


			cvReleaseMat(&Dst1);
			cvReleaseMat(&Dst2);
			cvReleaseMat(&Dst2Inv);
			cvReleaseMat(&ViMat1);
			cvReleaseMat(&Dst3);
			cvReleaseMat(&Dst4);
			cvReleaseMat(&Dst5);
			cvReleaseMat(&Indentity);
			cvReleaseMat(&Tempresult);
			cvReleaseMat(&IntriMatInvert);

		}
		CvMat *temp1 = cvCreateMat(3, 3, CV_64FC1);
		CvMat *temp2 = cvCreateMat(3, 3, CV_64FC1);
		CvMat *tempdanwei = cvCreateMat(3, 3, CV_64FC1);
		cvSetIdentity(tempdanwei);
		cvConvertScale(tempVi, temp1, 1.0 / PointNum, 0);
		cvSub(tempdanwei, temp1, temp1);
		cvInvert(temp1, temp1, CV_LU);
		cvConvertScale(temp1, temp1, 1.0 / PointNum, 0);
		cvGEMM(temp1, temp, 1, NULL, 0, translation, 0);

		CvMat *tempqMean = cvCreateMat(3, 1, CV_64FC1);
		cvSetZero(tempqMean);
		CvMat *temppMean = cvCreateMat(3, 1, CV_64FC1);
		cvSetZero(temppMean);

		for (int i = 0; i<PointNum; i++)
		{
			double viMatrix[3];
			viMatrix[0] = cvmGet(&Input2DPoint, i, 0);
			viMatrix[1] = cvmGet(&Input2DPoint, i, 1);
			viMatrix[2] = cvmGet(&Input2DPoint, i, 2);
			CvMat viMat = cvMat(3, 1, CV_64FC1, viMatrix);
			CvMat *IntriMatInvert = cvCreateMat(3, 3, CV_64FC1);
			cvInvert(&IntriMat, IntriMatInvert, CV_LU);
			cvGEMM(IntriMatInvert, &viMat, 1, NULL, 0, &viMat, 0);


			CvMat *Dst1 = cvCreateMat(3, 3, CV_64FC1);
			CvMat *Dst2 = cvCreateMat(1, 1, CV_64FC1);
			cvMulTransposed(&viMat, Dst1, 0);
			cvMulTransposed(&viMat, Dst2, -1);
			CvMat *Dst2Inv = cvCreateMat(1, 1, CV_64FC1);
			cvInvert(Dst2, Dst2Inv, CV_LU);
			CvMat *ViMat1 = cvCreateMat(3, 3, CV_64FC1);
			cvConvertScale(Dst1, ViMat1, cvmGet(Dst2Inv, 0, 0), 0);
			//cvGEMM(Dst1,Dst2Inv,1,NULL,0,ViMat1,0);

			double piMatrix[3];
			piMatrix[0] = cvmGet(&Input3DPoint, i, 0);
			piMatrix[1] = cvmGet(&Input3DPoint, i, 1);
			piMatrix[2] = cvmGet(&Input3DPoint, i, 2);
			CvMat piMat = cvMat(3, 1, CV_64FC1, piMatrix);

			CvMat *tempERt = cvCreateMat(3, 1, CV_64FC1);
			cvGEMM(&InitRotation, &piMat, 1, NULL, 0, tempERt, 0);
			cvAdd(tempERt, translation, tempERt);

			CvMat *Indentity = cvCreateMat(3, 3, CV_64FC1);
			cvSetIdentity(Indentity);
			cvSub(Indentity, ViMat1, Indentity);
			cvGEMM(Indentity, tempERt, 1, NULL, 0, tempERt, 0);
			double tempvalue = cvNorm(tempERt, NULL, CV_L2);
			ERt += tempvalue;

			CvMat *qiR = cvCreateMat(3, 1, CV_64FC1);
			CvMat *tempMedian = cvCreateMat(3, 1, CV_64FC1);
			cvGEMM(&InitRotation, &piMat, 1, translation, 1, tempMedian, 0);
			cvGEMM(ViMat1, tempMedian, 1, NULL, 0, qiR, 0);
			cvAdd(qiR, tempqMean, tempqMean);

			cvAdd(&piMat, temppMean, temppMean);

			cvReleaseMat(&Dst1);
			cvReleaseMat(&Dst2);
			cvReleaseMat(&Dst2Inv);
			cvReleaseMat(&ViMat1);
			cvReleaseMat(&tempERt);
			cvReleaseMat(&Indentity);
			cvReleaseMat(&qiR);
			cvReleaseMat(&tempMedian);
			cvReleaseMat(&IntriMatInvert);
		}

		count++;
		if (count>10)
			Error += 0.1;
		if (ERt < Error)
		{
			cvReleaseMat(&temp);
			cvReleaseMat(&tempVi);
			cvReleaseMat(&temp1);
			cvReleaseMat(&temp2);
			cvReleaseMat(&tempdanwei);
			cvReleaseMat(&tempqMean);
			cvReleaseMat(&temppMean);
			break;
		}


		cvConvertScale(tempqMean, tempqMean, 1.0 / PointNum, 0);
		cvConvertScale(temppMean, temppMean, 1.0 / PointNum, 0);

		CvMat *MR = cvCreateMat(3, 3, CV_64FC1);
		cvSetZero(MR);
		for (int i = 0; i<PointNum; i++)
		{
			CvMat *DotqiR = cvCreateMat(3, 1, CV_64FC1);
			CvMat *Dotpi = cvCreateMat(3, 1, CV_64FC1);

			double viMatrix[3];
			viMatrix[0] = cvmGet(&Input2DPoint, i, 0);
			viMatrix[1] = cvmGet(&Input2DPoint, i, 1);
			viMatrix[2] = cvmGet(&Input2DPoint, i, 2);
			CvMat viMat = cvMat(3, 1, CV_64FC1, viMatrix);
			CvMat *IntriMatInvert = cvCreateMat(3, 3, CV_64FC1);
			cvInvert(&IntriMat, IntriMatInvert, CV_LU);
			cvGEMM(IntriMatInvert, &viMat, 1, NULL, 0, &viMat, 0);

			CvMat *Dst1 = cvCreateMat(3, 3, CV_64FC1);
			CvMat *Dst2 = cvCreateMat(1, 1, CV_64FC1);
			cvMulTransposed(&viMat, Dst1, 0);
			cvMulTransposed(&viMat, Dst2, -1);
			CvMat *Dst2Inv = cvCreateMat(1, 1, CV_64FC1);
			cvInvert(Dst2, Dst2Inv, CV_LU);
			CvMat *ViMat1 = cvCreateMat(3, 3, CV_64FC1);
			cvConvertScale(Dst1, ViMat1, cvmGet(Dst2Inv, 0, 0), 0);
			//cvGEMM(Dst1,Dst2Inv,1,NULL,0,ViMat1,0);
			double piMatrix[3];
			piMatrix[0] = cvmGet(&Input3DPoint, i, 0);
			piMatrix[1] = cvmGet(&Input3DPoint, i, 1);
			piMatrix[2] = cvmGet(&Input3DPoint, i, 2);
			CvMat piMat = cvMat(3, 1, CV_64FC1, piMatrix);

			CvMat *qiR = cvCreateMat(3, 1, CV_64FC1);
			CvMat *tempMedian = cvCreateMat(3, 1, CV_64FC1);
			cvGEMM(&InitRotation, &piMat, 1, translation, 1, tempMedian, 0);
			cvGEMM(ViMat1, tempMedian, 1, NULL, 0, qiR, 0);

			cvSub(&piMat, temppMean, Dotpi);
			cvSub(qiR, tempqMean, DotqiR);
			CvMat *tempMR = cvCreateMat(3, 3, CV_64FC1);

			cvGEMM(DotqiR, Dotpi, 1, NULL, 0, tempMR, CV_GEMM_B_T);
			cvAdd(MR, tempMR, MR);

			cvReleaseMat(&Dst1);
			cvReleaseMat(&Dst2);
			cvReleaseMat(&Dst2Inv);
			cvReleaseMat(&ViMat1);
			cvReleaseMat(&DotqiR);
			cvReleaseMat(&Dotpi);
			cvReleaseMat(&qiR);
			cvReleaseMat(&tempMedian);
			cvReleaseMat(&tempMR);
			cvReleaseMat(&IntriMatInvert);

		}
		CvMat *W = cvCreateMat(3, 3, CV_64FC1);
		CvMat *U = cvCreateMat(3, 3, CV_64FC1);
		CvMat *V = cvCreateMat(3, 3, CV_64FC1);
		cvSVD(MR, W, U, V);
		cvGEMM(V, U, 1, NULL, 0, &InitRotation, CV_GEMM_B_T);

		cvReleaseMat(&temp);
		cvReleaseMat(&tempVi);
		cvReleaseMat(&temp1);
		cvReleaseMat(&temp2);
		cvReleaseMat(&tempdanwei);
		cvReleaseMat(&tempqMean);
		cvReleaseMat(&temppMean);
		cvReleaseMat(&MR);
		cvReleaseMat(&U);
		cvReleaseMat(&V);
		cvReleaseMat(&W);

	}
	return translation;
	//cvReleaseMat(&translation);

}