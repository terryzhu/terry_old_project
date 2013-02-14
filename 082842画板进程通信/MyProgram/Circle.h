#pragma once
#include "shape.h"
#include "Rectangle.h"
#define CCIRCLE_VALUE_SPACE 36
class CCircle :public CRectangle
{
public:
	CCircle(RECT rect,int PenStyle,int nWidth,COLORREF col);
	CCircle();
	CCircle(CCircle &circle);
	void Draw(HDC);
	RECT GetItsRect();
	BOOL IsSelected(POINT MousePos);
	void Move(int x,int y);
	void Serialize(HANDLE hFile);
	void Scaling(BasePoint Bp,float x_sacle,float y_scale);
	int What_Am_I();
	CCircle & operator=(CCircle &circle);
private:
	int m_fnPenStyle;
	int m_nWidth;
	//RECT m_rect;
};