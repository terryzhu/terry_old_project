#include "stdafx.h"
#include "Ploy.h"
CPloy::CPloy(int fnPenStyle,
				 int nWidth, 
				 COLORREF color,
				 CPtrArr& ptrTemp)
{
	m_fnPenStyle=fnPenStyle;
	m_nWidth=nWidth;
	m_color=color;
	m_ptrArr=ptrTemp;
}
CPloy::CPloy(CPloy & ploy)
{
	this->m_color=ploy.m_color;
	this->m_fnPenStyle=m_fnPenStyle;
	this->m_nWidth=ploy.m_nWidth;
	//this->m_ptrArr=ploy.m_ptrArr;
	this->m_ptrArr.CopyFrom(ploy.m_ptrArr);
}
void CPloy::Draw(HDC hdc)
{
	HPEN hpen = CreatePen(m_fnPenStyle,m_nWidth,m_color);
	SelectObject(hdc, hpen);
	POINT *pPt;
	pPt=(POINT*)m_ptrArr.GetAt(0);
	MoveToEx(hdc,pPt->x,pPt->y,NULL);
	for (int i=1;i<m_ptrArr.GetSize();i++)
	{
		pPt=(POINT*)m_ptrArr.GetAt(i);
		LineTo(hdc,pPt->x,pPt->y);
	}
	pPt=(POINT*)m_ptrArr.GetAt(0);
	LineTo(hdc,pPt->x,pPt->y);
	DeleteObject(hpen);

}
RECT CPloy::GetItsRect()
{
	RECT rect;
	POINT pt1,pt2,ptTemp;;
	pt1=*((POINT*)m_ptrArr.GetAt(0));
	pt2=*((POINT*)m_ptrArr.GetAt(1));
	AfxTwoPointsToRect(rect,pt1,pt2);
	for (int i=2;i<m_ptrArr.GetSize();++i)
	{
		ptTemp=*((POINT*)m_ptrArr.GetAt(i));
		AfxRectAndPointToRect(rect,ptTemp);
	}
	rect.left-=m_nWidth>>1;
	rect.top-=m_nWidth>>1;
	rect.right+=m_nWidth>>1;
	rect.bottom+=m_nWidth>>1;
	return rect;
}
BOOL CPloy::IsSelected(POINT MousePos)
{
	POINT pt1,pt2;
	for (int i=1;i<m_ptrArr.GetSize();i++)
	{
		pt1=*((POINT*)m_ptrArr.GetAt(i-1));
		pt2=*((POINT*)m_ptrArr.GetAt(i));
		if ( AfxLineIsSelected(MousePos,pt1,pt2))
		{
			return TRUE;
		}
	}
	pt1=*((POINT*)m_ptrArr.GetAt(m_ptrArr.GetSize()-1));
	pt2=*((POINT*)m_ptrArr.GetAt(0));
	if ( AfxLineIsSelected(MousePos,pt1,pt2))
	{
		return TRUE;
	}
	return FALSE;
}

void CPloy::Move(int x,int y)
{
	for (int i=0;i<m_ptrArr.GetSize();i++)
	{
		AfxPointMove((POINT*)m_ptrArr.GetAt(i),x,y);
	}
}

void CPloy::Serialize(HANDLE hFile)
{
	int * temp=new int[CPLOY_VALUE_SPACE];
	temp[0]=HAS_OBJ_FLAG;
	temp[1]=ISCPLOY;
	temp[2]=m_color;
	temp[3]=m_fnPenStyle;
	temp[4]=m_nWidth;
	temp[5]=m_ptrArr.GetSize();
	POINT pt;
	for (int i=0;i<m_ptrArr.GetSize();i++)
	{
		pt=*((POINT*)m_ptrArr.GetAt(i));
		temp[5+i*2+1]=pt.x;
		temp[5+i*2+2]=pt.y;
	}
	DWORD dwSize;
	SetFilePointer(hFile,0,0,FILE_END);
	WriteFile(hFile,temp,CPLOY_VALUE_SPACE,&dwSize,NULL);
	delete [] temp;
}
void CPloy::Scaling(BasePoint Bp,float x_sacle,float y_scale)
{
	RECT rect;
	rect=GetItsRect();
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
	for (int i=0;i<m_ptrArr.GetSize();i++)
	{
		AfxPointScaling(basept,(POINT*)m_ptrArr.GetAt(i),x_sacle,y_scale);
	}
}
int CPloy::What_Am_I()
{
	return ISCPLOY;
}
CPloy & CPloy::operator =(CPloy &ploy)
{
	this->m_color=ploy.m_color;
	this->m_fnPenStyle=m_fnPenStyle;
	this->m_nWidth=ploy.m_nWidth;
	POINT *pPt;
	POINT *p;
	m_ptrArr.ClearTheContent();
	m_ptrArr.Clear();
	for (int i=0;i<ploy.m_ptrArr.GetSize();i++)
	{
		pPt=(POINT*)ploy.m_ptrArr.GetAt(i);
		p=new POINT;
		p->x=pPt->x;
		p->y=pPt->y;
		m_ptrArr.Add(p);
	}
	return *this;
}