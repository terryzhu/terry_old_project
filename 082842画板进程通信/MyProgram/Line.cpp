#include "StdAfx.h"
#include "Line.h"
CLine::CLine(void)
{
}
CLine::CLine(CLine & line)
{
	this->m_color=line.m_color;
	this->m_fnPenStyle=line.m_fnPenStyle;
	this->m_nWidth=line.m_nWidth;
	this->m_ptDest=line.m_ptDest;
	this->m_ptOrig=line.m_ptOrig;
}

CLine::~CLine(void)
{
}
CLine::CLine(POINT pt1, POINT pt2,int fnPenStyle,
			 int nWidth, COLORREF color)
{
	m_fnPenStyle=fnPenStyle;
	m_nWidth=nWidth;
	m_ptOrig=pt1;
	m_ptDest=pt2;
	m_color=color;
}
void CLine::Draw(HDC hdc)
{
	AfxDrawLine(hdc,m_ptOrig,m_ptDest,m_fnPenStyle,m_nWidth,m_color);

}
RECT CLine::GetItsRect()
{
	RECT rect;
	AfxTwoPointsToRect(rect,m_ptOrig,m_ptDest);
	rect.left-=((m_nWidth>>1)+1);
	rect.top-=((m_nWidth>>1)+1);
	rect.right+=((m_nWidth>>1)+1);
	rect.bottom+=((m_nWidth>>1)+1);
	return rect;
}
BOOL CLine::IsSelected(POINT MousePos)
{
	return  AfxLineIsSelected(MousePos,m_ptOrig,m_ptDest);
}
void CLine::Move(int x,int y)
{
	AfxPointMove(&m_ptOrig,x,y);
	AfxPointMove(&m_ptDest,x,y);
}

void CLine::Serialize(HANDLE hFile)
{
	int temp[CLINE_VALUE_SPACE>>2];
	temp[0]=HAS_OBJ_FLAG;
	temp[1]=ISCLINE;
	temp[2]=m_color;
	temp[3]=m_fnPenStyle;
	temp[4]=m_nWidth;
	temp[5]=m_ptOrig.x;
	temp[6]=m_ptOrig.y;
	temp[7]=m_ptDest.x;
	temp[8]=m_ptDest.y;
	DWORD dwSize;
	SetFilePointer(hFile,0,0,FILE_END);
	WriteFile(hFile,temp,CLINE_VALUE_SPACE,&dwSize,NULL);

}
void CLine::Scaling(BasePoint Bp,float x_sacle,float y_scale)
{
	RECT rect;
	AfxTwoPointsToRect(rect,m_ptOrig,m_ptDest);
	POINT basept;
	switch (Bp)
	{
	case LEFTTOP:
		basept.x=rect.left;
		basept.y=rect.top;
		break;
	case RIGHTTOP:
		basept.x=rect.right;
		basept.y=rect.top;
		break;
	case RIGHTBOTTOM:
		basept.x=rect.right;
		basept.y=rect.bottom;
		break;
	case LEFTBOTTOM:
		basept.x=rect.left;
		basept.y=rect.bottom;
		break;
	}
	AfxPointScaling(basept,m_ptOrig,x_sacle,y_scale);
	AfxPointScaling(basept,m_ptDest,x_sacle,y_scale);
}
CLine CLine::operator =(CLine &line)
{
	this->m_color=line.m_color;
	this->m_fnPenStyle=line.m_fnPenStyle;
	this->m_nWidth=line.m_nWidth;
	this->m_ptDest=line.m_ptDest;
	this->m_ptOrig=line.m_ptOrig;
	return *this;
}
int CLine::What_Am_I()
{
	return ISCLINE;
}