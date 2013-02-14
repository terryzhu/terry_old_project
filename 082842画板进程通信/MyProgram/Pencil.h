#pragma once
#include "Shape.h"
#include "PtrArr.h"
#include "Ploy.h"
#define CPENCIL_VALUE_SPACE (24+m_ptrArr.GetSize()*8)
class CPencil:public CPloy
{
public:
	CPencil(){}
	CPencil(int fnPenStyle,
		int nWidth, COLORREF color,CPtrArr&);
	CPencil & operator=(CPencil & pencil);
	CPencil(CPencil &);
	void Draw(HDC hdc);
	RECT GetItsRect();
	BOOL IsSelected(POINT MousePos);
	void Move(int x,int y);
	void Serialize(HANDLE hFile);
	void Scaling(BasePoint Bp,float x_sacle,float y_scale);
	int What_Am_I();
//protected:
//	CPtrArr m_ptrArr;
//	int m_fnPenStyle;
//	int m_nWidth;
};