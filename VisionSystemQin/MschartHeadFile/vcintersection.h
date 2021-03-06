#if !defined(AFX_VCINTERSECTION_H__27BA631F_D392_4719_B8B8_F03224BBA1E2__INCLUDED_)
#define AFX_VCINTERSECTION_H__27BA631F_D392_4719_B8B8_F03224BBA1E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CVcIntersection wrapper class

class CVcIntersection : public COleDispatchDriver
{
public:
	CVcIntersection() {}		// Calls COleDispatchDriver default constructor
	CVcIntersection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcIntersection(const CVcIntersection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetAuto();
	void SetAuto(BOOL bNewValue);
	double GetPoint();
	void SetPoint(double newValue);
	long GetAxisID();
	short GetIndex();
	BOOL GetLabelsInsidePlot();
	void SetLabelsInsidePlot(BOOL bNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCINTERSECTION_H__27BA631F_D392_4719_B8B8_F03224BBA1E2__INCLUDED_)
