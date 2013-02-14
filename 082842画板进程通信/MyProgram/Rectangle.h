#pragma once
#include "shape.h"
#include "Line.h"
#define CRECTANGLE_VALUE_SPACE 40
class CRectangle :
	public CLine
{
public:
	CRectangle(POINT,POINT,BOOL,COLORREF InnerColor,COLORREF OutlineColor,int nWidth);
	CRectangle();
	~CRectangle(void);
	CRectangle(CRectangle &);
	void Draw(HDC);
	virtual RECT GetItsRect();
	virtual BOOL IsSelected(POINT MousePos);
	virtual void Move(int x,int y);
	virtual void Serialize(HANDLE hFile);
	virtual void Scaling(BasePoint Bp,float x_sacle,float y_scale);
	CRectangle operator=(CRectangle & rectangle);
	virtual int What_Am_I();
private:
	//POINT m_ptOrig;
	//POINT m_ptdest;
	BOOL m_btransparent;
	COLORREF m_InnerColor;
	//int m_nWidth;

};
