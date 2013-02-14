#pragma once
#include "shape.h"
#define CLINE_VALUE_SPACE 36
class CLine :public CShape
{
public:
	CLine(void);
	CLine(POINT ,POINT,int,int,COLORREF);
	CLine(CLine &);
	virtual void Draw(HDC);
	virtual RECT GetItsRect();
	virtual BOOL IsSelected(POINT MousePos);
	virtual void Move(int x,int y);
	virtual void Serialize(HANDLE hFile);
	virtual void Scaling(BasePoint Bp,float x_sacle,float y_scale);
	~CLine(void);
	CLine operator=(CLine &line);
	virtual int What_Am_I();
	
protected:
	int m_fnPenStyle;
	int m_nWidth;
	POINT m_ptOrig;
	POINT m_ptDest;

};
