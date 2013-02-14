#pragma once
#include "Shape.h"
#include "PtrArr.h"
#define CPLOY_VALUE_SPACE (24+m_ptrArr.GetSize()*8)
class CPloy:public CShape
{
public:
	CPloy(int fnPenStyle,
		int nWidth, COLORREF color,CPtrArr&);
	CPloy(){}
	CPloy(CPloy & ploy);
	CPloy & operator=(CPloy &ploy);
	virtual void Draw(HDC hdc);
	virtual RECT GetItsRect();
	virtual BOOL IsSelected(POINT MousePos);
	virtual void Move(int x,int y);
	virtual void Serialize(HANDLE hFile);
	virtual void Scaling(BasePoint Bp,float x_sacle,float y_scale);
	virtual int What_Am_I();
protected:
	CPtrArr m_ptrArr;
	int m_fnPenStyle;
	int m_nWidth;
};