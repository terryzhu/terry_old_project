#include "StdAfx.h"
#include "Circle.h"
CCircle::CCircle(RECT rect,int PenStyle,int nWidth,COLORREF col)
{
	m_fnPenStyle=PenStyle;
	m_ptOrig.x=rect.bottom;
	AfxRectToTwoPoints(rect,m_ptOrig,m_ptDest);
	m_nWidth=nWidth;
	m_color=col;
}
CCircle::CCircle(){}
CCircle::CCircle(CCircle & circle)
{
	this->m_color=circle.m_color;
	this->m_fnPenStyle=circle.m_fnPenStyle;
	this->m_nWidth=circle.m_nWidth;
	this->m_ptDest=circle.m_ptDest;
	this->m_ptOrig=circle.m_ptOrig;
}
void CCircle::Draw(HDC hdc)
{
	RECT rect;
	AfxTwoPointsToRect(rect,m_ptOrig,m_ptDest);
	AfxDrawEllipse(hdc,rect,m_fnPenStyle,m_nWidth,m_color);
}
RECT CCircle::GetItsRect()
{
	RECT rect;
	AfxTwoPointsToRect(rect,m_ptOrig,m_ptDest);
	AfxEnlargeRect(rect,(m_nWidth>>1)+1);
	return rect;
}
BOOL CCircle::IsSelected(POINT MousePos)
{
	RECT rect;
	AfxTwoPointsToRect(rect,m_ptOrig,m_ptDest);
	 if(TRUE==AfxEllipseIsSelected(MousePos,rect))
	 {
		OutputDebugString(L"SELEct");
		return TRUE;
	 }
	return FALSE;
	
}
void CCircle::Move(int x, int y)
{
	m_ptOrig.x+=x;
	m_ptDest.x+=x;
	m_ptOrig.y+=y;
	m_ptDest.y+=y;
}
void CCircle::Serialize(HANDLE hFile)
{
	int temp[CCIRCLE_VALUE_SPACE>>2];
	temp[0]=HAS_OBJ_FLAG;
	temp[1]=ISCCIRCLE;
	temp[2]=m_color;
	temp[3]=m_fnPenStyle;
	temp[4]=m_nWidth;
	temp[5]=m_ptOrig.x;
	temp[6]=m_ptOrig.y;
	temp[7]=m_ptDest.x;
	temp[8]=m_ptDest.y;
	DWORD dwSize;
	SetFilePointer(hFile,0,0,FILE_END);
	WriteFile(hFile,temp,CCIRCLE_VALUE_SPACE,&dwSize,NULL);
}
void CCircle::Scaling(BasePoint Bp,float x_sacle,float y_scale)
{

	//POINT basept;
	//POINT pt1,pt2,pt3,pt4;
	//pt4.x=pt1.x=m_rect.left;
	//pt1.y=pt2.y=m_rect.top;
	//pt2.x=pt3.x=m_rect.right;
	//pt3.y=pt4.y=m_rect.bottom;
	//switch (Bp)
	//{
	//case LEFTTOP:
	//	basept.x=m_rect.left;
	//	basept.y=m_rect.top;
	//	break;
	//case RIGHTTOP:
	//	basept.x=m_rect.right;
	//	basept.y=m_rect.top;
	//	break;
	//case RIGHTBOTTOM:
	//	basept.x=m_rect.right;
	//	basept.y=m_rect.bottom;
	//	break;
	//case LEFTBOTTOM:
	//	basept.x=m_rect.left;
	//	basept.y=m_rect.bottom;
	//	break;
	//}
	//AfxPointScaling(basept,pt1,x_sacle,y_scale);
	//AfxPointScaling(basept,pt3,x_sacle,y_scale);
	//m_rect.bottom=pt3.y;
	//m_rect.left=pt1.x;
	//m_rect.right=pt3.x;
	//m_rect.top=pt1.y;
	CRectangle::Scaling(Bp,x_sacle,y_scale);
}

int CCircle::What_Am_I()
{
	return ISCCIRCLE;
}
CCircle & CCircle::operator =(CCircle &circle)
{
	this->m_color=circle.m_color;
	this->m_fnPenStyle=circle.m_fnPenStyle;
	this->m_nWidth=circle.m_nWidth;
	this->m_ptDest=circle.m_ptDest;
	this->m_ptOrig=circle.m_ptOrig;
	return *this;
}