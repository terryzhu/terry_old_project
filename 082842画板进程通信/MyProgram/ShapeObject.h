#pragma once
#include "windows.h"
#include "PtrArr.h"
#include "windows.h"
#include "math.h"//为了计算平方根
#include "commdlg.h"
#define SELECTDISTANCE 10
#define PI 3.14
class CShapeObject 
{
public:
	static void AfxDrawLine(
		HDC hdc,
		POINT pt_Orig,
		POINT pt_dest,
		int fnPenStyle,
		int nWidth, 
		COLORREF color);
	static void AfxDrawEllipse(HDC hdc,RECT rect,int PenStyle,int nWidth,COLORREF col);
	static void AfxDrawRect(
		HDC hdc,
		POINT pt_Orig,
		POINT pt_dest,
		BOOL b_transparent,
		COLORREF InnerColor,
		COLORREF OutlineColor,
		int nWidth);
	static void AfxDrawPloy(HDC hdc,
		int fnPenStyle,
		int nWidth, 
		COLORREF color,
		CPtrArr& ptrTemp);
	static void AfxTwoPointsToRect(RECT & rect,const POINT pt1,POINT pt2);
	static void AfxRectToTwoPoints(RECT rect,POINT & pt1,POINT &pt2);
	static void AfxFixMaxRect(RECT &rectMax,RECT &rect);
	static void AfxRectAndPointToRect(RECT &rect,POINT pt);
	static float AfxTwoPointDistance(POINT pt1,POINT pt2);
	static BOOL AfxLineIsSelected(POINT MousePos,POINT pt1,POINT pt2);
	static BOOL AfxEllipseIsSelected(POINT MousePos,RECT rect);
	static void AfxDrawSelectedRect(HDC hdc,RECT rect);
	static void AfxEnlargeRect(RECT &rect,int n=0);
	static void AfxPointMove(POINT *pPt,int x,int y);
	static void AfxRectMove(RECT *pRect,int x,int y);
	static BOOL AfxPointIsInRect(POINT pt,RECT rect);
	static void AfxInit_OPENFILENAME(OPENFILENAME& ofn,HWND hWnd,TCHAR *buf,int nMaxFile,TCHAR * lpstrFilter);
	static void AfxAddStringStar(TCHAR* str1,TCHAR * str2);
	static void AfxPointScaling(POINT basept,POINT &Scalpt,float x_scale,float y_scale);
	static void AfxPointScaling(POINT basept,POINT *pScalpt,float x_scale,float y_scale);
	static BOOL AfxMsgCanPass(UINT message ,UINT * UintArray,INT Num);
	static BOOL AfxLastCharIsStar(TCHAR * p);
};