#pragma once
#include "ShapeObject.h"
#define ISCLINE 1
#define ISCPENCIL 2
#define ISCPLOY 3
#define ISCRECTANGLE 4
#define ISCCIRCLE 5
#define HAS_OBJ_FLAG 0x0f0f0f0f

//表明放缩时的基准点
enum BasePoint{NOPT,LEFTTOP,RIGHTTOP,RIGHTBOTTOM,LEFTBOTTOM};
class CShape:public CShapeObject
{
public:
	virtual void Draw(HDC)=0;
	virtual RECT GetItsRect()=0;//返回能包围此图形的最小矩形
	virtual BOOL IsSelected(POINT MousePos)=0;
	virtual void Move(int x,int y)=0;
	virtual void Serialize(HANDLE hFile)=0;
	virtual void Scaling(BasePoint Bp,float x_sacle,float y_scale)=0;
	virtual INT What_Am_I()=0;
protected:
	COLORREF m_color;
};

