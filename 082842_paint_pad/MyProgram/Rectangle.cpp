#include "StdAfx.h"
#include "Rectangle.h"
CRectangle::CRectangle(POINT pt_Orig,
					   POINT pt_dest,
					   BOOL b_transparent,
					   COLORREF InnerColor,
					   COLORREF OutlineColor,
					   int nWidth)
{
	m_ptOrig=pt_Orig;
	m_ptDest=pt_dest;
	m_btransparent=b_transparent;
	m_InnerColor=InnerColor;
	m_color=OutlineColor;
	m_nWidth=nWidth;
}

CRectangle::CRectangle(CRectangle & rect)
{
	this->m_btransparent=rect.m_btransparent;
	this->m_color=rect.m_color;
	this->m_InnerColor=rect.m_InnerColor;
	this->m_nWidth=rect.m_nWidth;
	this->m_ptDest=rect.m_ptDest;
	this->m_ptOrig=rect.m_ptOrig;
}
void CRectangle::Draw(HDC hdc)
{
	AfxDrawRect(hdc,m_ptOrig,m_ptDest,m_btransparent,m_InnerColor,m_color,m_nWidth);
}
RECT CRectangle::GetItsRect()
{
	RECT rect;
	AfxTwoPointsToRect(rect,m_ptOrig,m_ptDest);
	rect.left=rect.left-(m_nWidth>>1);
	rect.right=rect.right+(m_nWidth>>1);
	rect.top-=m_nWidth>>1;
	rect.bottom+=m_nWidth>>1;

	return rect;
}
BOOL CRectangle::IsSelected(POINT MousePos)
{
	POINT pt1,pt2;
	pt1.x=m_ptOrig.x;
	pt1.y=m_ptDest.y;
	pt2.x=m_ptDest.x;
	pt2.y=m_ptOrig.y;
	if ( AfxLineIsSelected(MousePos,pt1,m_ptDest))
	{
		return TRUE;
	}
	if ( AfxLineIsSelected(MousePos,pt2,m_ptDest))
	{
		return TRUE;
	}
	if ( AfxLineIsSelected(MousePos,pt1,m_ptOrig))
	{
		return TRUE;
	}
	if ( AfxLineIsSelected(MousePos,pt2,m_ptOrig))
	{
		return TRUE;
	}
	return FALSE;
}

void CRectangle::Move(int x,int y)
{
	AfxPointMove(&m_ptOrig,x,y);
	AfxPointMove(&m_ptDest,x,y);
}

void CRectangle::Serialize(HANDLE hFile)
{
	int temp[CRECTANGLE_VALUE_SPACE];
	temp[0]=HAS_OBJ_FLAG;
	temp[1]=ISCRECTANGLE;
	temp[2]=m_color;
	temp[3]=m_btransparent;
	temp[4]=m_InnerColor;
	temp[5]=m_nWidth;
	temp[6]=m_ptOrig.x;
	temp[7]=m_ptOrig.y;
	temp[8]=m_ptDest.x;
	temp[9]=m_ptDest.y;
	DWORD dwSize;
	SetFilePointer(hFile,0,0,FILE_END);
	WriteFile(hFile,temp,CRECTANGLE_VALUE_SPACE,&dwSize,NULL);
}
void CRectangle::Scaling(BasePoint Bp,float x_sacle,float y_scale)
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
CRectangle::CRectangle(){}
CRectangle::~CRectangle(void){}
CRectangle CRectangle::operator =(CRectangle &rectangle)
{
	this->m_btransparent=rectangle.m_btransparent;
	this->m_color=rectangle.m_color;
	this->m_InnerColor=rectangle.m_InnerColor;
	this->m_nWidth=rectangle.m_nWidth;
	this->m_ptDest=rectangle.m_ptDest;
	this->m_ptOrig=rectangle.m_ptOrig;
	return *this;
}
int CRectangle::What_Am_I()
{
	return ISCRECTANGLE;
}