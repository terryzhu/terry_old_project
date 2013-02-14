#include "stdafx.h"
#include "Pencil.h"
CPencil::CPencil(int fnPenStyle,
				 int nWidth, 
				 COLORREF color,
				 CPtrArr& ptrTemp):CPloy(fnPenStyle,nWidth,color,ptrTemp)
{

}
CPencil::CPencil(CPencil &pencil)
{
	this->m_color=pencil.m_color;
	this->m_fnPenStyle=m_fnPenStyle;
	this->m_nWidth=pencil.m_nWidth;
	this->m_ptrArr.CopyFrom(pencil.m_ptrArr);
}
CPencil & CPencil::operator =(CPencil &pencil)
{
	this->m_color=pencil.m_color;
	this->m_fnPenStyle=pencil.m_fnPenStyle;
	this->m_nWidth=pencil.m_nWidth;
	POINT *pPt;
	POINT *p;
	m_ptrArr.ClearTheContent();
	m_ptrArr.Clear();
	for (int i=0;i<pencil.m_ptrArr.GetSize();i++)
	{
		pPt=(POINT*)pencil.m_ptrArr.GetAt(i);
		p=new POINT;
		p->x=pPt->x;
		p->y=pPt->y;
		m_ptrArr.Add(p);
	}
	return *this;
}
void CPencil::Draw(HDC hdc)
{
	AfxDrawPloy(hdc,m_fnPenStyle,m_nWidth,m_color,m_ptrArr);
}

RECT CPencil::GetItsRect()
{
	return CPloy::GetItsRect();
}
BOOL CPencil::IsSelected(POINT MousePos)
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
	return FALSE;
}

void CPencil::Move(int x,int y)
{
	for (int i=0;i<m_ptrArr.GetSize();i++)
	{
		AfxPointMove((POINT*)m_ptrArr.GetAt(i),x,y);
	}
}

void CPencil::Serialize(HANDLE hFile)
{
	int * temp=new int[CPENCIL_VALUE_SPACE];
	temp[0]=HAS_OBJ_FLAG;
	temp[1]=ISCPENCIL;
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
	WriteFile(hFile,temp,CPENCIL_VALUE_SPACE,&dwSize,NULL);
	delete [] temp;
}
void CPencil::Scaling(BasePoint Bp,float x_sacle,float y_scale)
{
	
	CPloy::Scaling(Bp,x_sacle,y_scale);
}
int CPencil::What_Am_I()
{
	return ISCPENCIL;
}