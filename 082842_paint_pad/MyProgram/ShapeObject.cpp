#include "stdafx.h"
#include "ShapeObject.h"

void		CShapeObject::AfxDrawLine(
				 HDC hdc,
				 POINT pt_Orig,
				 POINT pt_dest,
				 int fnPenStyle,
				 int nWidth, 
				 COLORREF color)
{
	HPEN hpen = CreatePen(fnPenStyle,nWidth,color);
	SelectObject(hdc, hpen);
	MoveToEx(hdc,pt_Orig.x,pt_Orig.y,NULL);
	LineTo(hdc,pt_dest.x,pt_dest.y);
	DeleteObject(hpen);

}
void		CShapeObject::AfxDrawEllipse(HDC hdc,RECT rect,int PenStyle,int nWidth,COLORREF col)
{
	HPEN hpen=CreatePen(PenStyle,nWidth,col);
	SelectObject(hdc, hpen);
	LOGBRUSH   logBrush;   
	logBrush.lbStyle=BS_NULL;   
	HBRUSH   hnewBrush=CreateBrushIndirect(&logBrush);//创建透明画刷^_^ 
	SelectObject(hdc,hnewBrush);
	Ellipse(hdc,rect.left,rect.top,rect.right,rect.bottom);
	DeleteObject(hpen);
	DeleteObject(hnewBrush);
}
void		CShapeObject::AfxDrawRect(
				 HDC hdc,
				 POINT pt_Orig,
				 POINT pt_dest,
				 BOOL b_transparent,
				 COLORREF InnerColor,
				 COLORREF OutlineColor,
				 int nWidth)
{
	HPEN hpen=CreatePen(PS_SOLID,nWidth,OutlineColor);
	SelectObject(hdc,hpen);
	HBRUSH hbrush;

	if (b_transparent==TRUE)
	{
		MoveToEx(hdc,pt_Orig.x,pt_Orig.y,NULL);
		LineTo(hdc,pt_dest.x,pt_Orig.y);
		LineTo(hdc,pt_dest.x,pt_dest.y);
		LineTo(hdc,pt_Orig.x,pt_dest.y);
		LineTo(hdc,pt_Orig.x,pt_Orig.y);
	}
	else
	{
		hbrush = CreateSolidBrush(InnerColor);
		SelectObject(hdc,hbrush);
		Rectangle(hdc,pt_Orig.x,pt_Orig.y,pt_dest.x,pt_dest.y);
		DeleteObject(hbrush);
	}

	DeleteObject(hpen);

}

void		CShapeObject::AfxDrawPloy(HDC hdc,
				 int fnPenStyle,
				 int nWidth, 
				 COLORREF color,
				 CPtrArr& ptrTemp)
{
	HPEN hpen = CreatePen(fnPenStyle,nWidth,color);
	SelectObject(hdc, hpen);
	POINT *pPt;
	pPt=(POINT*)ptrTemp.GetAt(0);
	MoveToEx(hdc,pPt->x,pPt->y,NULL);
	for (int i=1;i<ptrTemp.GetSize();i++)
	{
		pPt=(POINT*)ptrTemp.GetAt(i);
		LineTo(hdc,pPt->x,pPt->y);
	}
	DeleteObject(hpen);
}


//两点确定一个矩形,并且使得此矩形非空
void		CShapeObject::AfxTwoPointsToRect(RECT & rect,const POINT pt1,POINT pt2)
{
	rect.left=min(pt1.x,pt2.x);
	rect.right=max(pt2.x,pt1.x)+1;
	rect.bottom=max(pt1.y,pt2.y)+1;
	rect.top=min(pt1.y,pt2.y);
}
void		CShapeObject::AfxRectToTwoPoints(RECT rect,POINT & pt1,POINT &pt2)
{
	pt1.x=rect.left;
	pt1.y=rect.top;
	pt2.x=rect.right;
	pt2.y=rect.bottom;
}
//确定两个矩形组合成的最大矩形,前者为被转换的矩形
void		CShapeObject::AfxFixMaxRect(RECT &rectMax,RECT &rect)
{
	rectMax.left=min(rect.left,rectMax.left);
	rectMax.right=max(rect.right,rectMax.right);
	rectMax.top=min(rect.top,rectMax.top);
	rectMax.bottom=max(rect.bottom,rectMax.bottom);
}
//确定一个矩形和一个点形成的矩形
void		CShapeObject::AfxRectAndPointToRect(RECT &rect,POINT pt)
{
	rect.left=min(rect.left,pt.x);
	rect.right=max(rect.right,pt.x);
	rect.top=min(rect.top,pt.y);
	rect.bottom=max(rect.bottom,pt.y);
}
float		CShapeObject::AfxTwoPointDistance(POINT pt1,POINT pt2)
{
	float result;
	result=sqrt((float)((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y)));
	return result;
}

BOOL		CShapeObject::AfxLineIsSelected(POINT MousePos,POINT pt1,POINT pt2)//鼠标是否选中这条直线
{
	float a,b,c;
	float ang1,ang2,ang;
	a=AfxTwoPointDistance(MousePos,pt1);
	if (a==0)
	{
		return TRUE;
	}
	b=AfxTwoPointDistance(MousePos,pt2);
	if (b==0)
	{
		return TRUE;
	}
	c=AfxTwoPointDistance(pt1,pt2);
	if (a<b)
	{
		if (pt1.y==pt2.y)
		{
			if (pt1.x<pt2.x)
			{
				ang1=0;
			}
			else
			{
				ang1=(float)PI;
			}
		}
		else
		{
			ang1=(float)acos((pt2.x-pt1.x)/c);
			if (pt1.y>pt2.y)
			{
				ang1=(float)PI*2-ang1;
			}
		}
		ang2=(float)acos((MousePos.x-pt1.x)/a);
		if (pt1.y>MousePos.y)
		{
			ang2=(float)PI*2-ang2;
		}
		ang=ang2-ang1;
		if (ang<0)
		{
			ang=-ang;
		}
		if (ang>PI)
		{
			ang=(float)PI*2-ang;
		}
		if (ang>PI/2)
		{
			return a<SELECTDISTANCE;
		}
		else
		{
			return (a*(float)sin(ang))<SELECTDISTANCE;
		}
	}
	else
	{
		if (pt1.y==pt2.y)
		{
			if (pt1.x<pt2.x)
			{
				ang1=(float)PI;
			}
			else
			{
				ang1=0;
			}
		}
		else
		{
			ang1=(float)acos((pt1.x-pt2.x)/c);
			if (pt2.y>pt1.y)
			{
				ang1=(float)PI*2-ang1;
			}
		}
		ang2=(float)acos((MousePos.x-pt2.x)/b);
		if (pt2.y>MousePos.y)
		{
			ang2=(float)PI*2-ang2;
		}
		ang=ang2-ang1;
		if (ang<0)
		{
			ang=-ang;
		}
		if (ang>PI)
		{
			ang=(float)PI*2-ang;
		}
		if (ang>PI/2)
		{
			return b<SELECTDISTANCE;
		}
		else
		{
			return (b*(float)sin(ang))<SELECTDISTANCE;
		}
	}
}
BOOL		CShapeObject::AfxEllipseIsSelected(POINT MousePos,RECT rect)
{
	POINT middle;
	middle.x=(rect.left+rect.right)>>1;
	middle.y=(rect.top+rect.bottom)>>1;
	int a=abs((rect.left-rect.right)>>1);
	int b=abs((rect.top-rect.bottom)>>1);
	int c=(int)sqrt((float)abs(a*a-b*b));
	POINT focus1;
	POINT focus2;
	int k=a>b?a:b;
	if (a>b)
	{//左右长椭圆
		focus1.x=middle.x-c;
		focus2.x=middle.x+c;
		focus1.y=focus2.y=middle.y;
	} 
	else
	{
		focus1.x=focus2.x=middle.x;
		focus1.y=middle.y-c;
		focus2.y=middle.y+c;
	}
	int len=(int)AfxTwoPointDistance(MousePos,focus1)+(int)AfxTwoPointDistance(MousePos,focus2);
	if (abs(len-(k<<1))<SELECTDISTANCE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}


}
//绘制被选中的那个图形的外部矩形
void		CShapeObject::AfxDrawSelectedRect(HDC hdc,RECT rect)
{

	HPEN hpen = CreatePen(PS_DOT,1,RGB(0,0,0));
	SelectObject(hdc, hpen);
	MoveToEx(hdc,rect.left,rect.top,NULL);
	LineTo(hdc,rect.right,rect.top);
	LineTo(hdc,rect.right,rect.bottom);
	LineTo(hdc,rect.left,rect.bottom);
	LineTo(hdc,rect.left,rect.top);
	DeleteObject(hpen);

}
//为了边缘地带,使得矩形必须大一个像素
void		CShapeObject::AfxEnlargeRect(RECT &rect,int n)
{
	rect.right+=1+n;
	rect.bottom+=1+n;
	rect.left-=1+n;
	rect.top-=1+n;
}

void		CShapeObject::AfxPointMove(POINT *pPt,int x,int y)
{
	pPt->x+=x;
	pPt->y+=y;
}

void		CShapeObject::AfxRectMove(RECT *pRect,int x,int y)
{
	pRect->left+=x;
	pRect->right+=x;
	pRect->bottom+=y;
	pRect->top+=y;
}
BOOL		CShapeObject::AfxPointIsInRect(POINT pt,RECT rect)
{
	if (pt.x<rect.right&&pt.x>rect.left&&pt.y<rect.bottom&&pt.y>rect.top)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void		CShapeObject::AfxInit_OPENFILENAME(OPENFILENAME& ofn,HWND hWnd,TCHAR *buf,int nMaxFile,TCHAR * lpstrfilter)
{
	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.hwndOwner=hWnd;
	ofn.lpstrFile=buf;
	ofn.nMaxFile =nMaxFile;
	ofn.lpstrFilter=lpstrfilter;
	ofn.Flags=OFN_EXPLORER|OFN_OVERWRITEPROMPT ;
}
//让一个字符串结尾增加一个*号,为改变窗口标题
void		CShapeObject::AfxAddStringStar(TCHAR* str1,TCHAR * str2)
{
	TCHAR *p=str1;
	int len=0;
	while (*p!=NULL)
	{
		p++;
		len++;
	}
	memcpy(str2,str1,2*len);
	p=str2;
	p+=len;
	*p='*';
	p++;
	*p=NULL;

}
void		CShapeObject::AfxPointScaling(POINT basept,POINT &Scalpt,float x_scale,float y_scale)
{
	Scalpt.x=int((Scalpt.x-basept.x)*x_scale+basept.x);
	Scalpt.y=int((Scalpt.y-basept.y)*y_scale+basept.y);
}
void		CShapeObject::AfxPointScaling(POINT basept,POINT *pScalpt,float x_scale,float y_scale)
{
	pScalpt->x=int((pScalpt->x-basept.x)*x_scale+basept.x);
	pScalpt->y=int((pScalpt->y-basept.y)*y_scale+basept.y);
}
//message这个消息是否在UintArray这个数组中
BOOL		CShapeObject::AfxMsgCanPass(UINT message ,UINT * UintArray,INT Num)
{
	for (int i=0;i<Num;i++)
	{
		if (message==UintArray[i])
		{
			return TRUE;
		}

	}
	return FALSE;
}

BOOL		CShapeObject::AfxLastCharIsStar(TCHAR * p)
{
	while (*p)
	{
		p++;
	}
	p--;
	return (*p=='*');
}