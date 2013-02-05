// MySlider.cpp : 实现文件
//

#include "stdafx.h"
#include "iTunes.h"
#include "MySlider.h"


// CMySlider

IMPLEMENT_DYNAMIC(CMySlider, CWnd)

CMySlider::CMySlider()
{
	m_roundSlider.LoadBitmap(IDB_SLIDER);
	m_iProgress=0;
	m_LeftBtnDown=FALSE;
}

CMySlider::~CMySlider()
{
}


BEGIN_MESSAGE_MAP(CMySlider, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CMySlider 消息处理程序

void CMySlider::SetProgress(double process)
{
	m_iProgress=process;
	Invalidate();
}

void CMySlider::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()

	CRect rc;
	GetClientRect(&rc);
	
	int x=int((rc.Width()-13)*m_iProgress/100);
	CBrush br;
	br.CreateSolidBrush(RGB(50,161,224));
	CRgn rgn;
	//rgn.CreateRoundRectRgn(2,3,x,4,3,3);
	rgn.CreateRoundRectRgn(2,3,2+x,7,3,3);
	CDC dct;
	dct.CreateCompatibleDC(&dc);
	dct.SelectObject(&m_roundSlider);
	dc.FillRgn(&rgn,&br);
	dc.BitBlt(x,0,rc.Width(),rc.Height(),&dct,0,0,SRCCOPY);
}

BOOL CMySlider::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void CMySlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_LeftBtnDown=TRUE;
	
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CMySlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_LeftBtnDown)
	{
		OnMouseMove(nFlags,point);
		
		GetParent()->PostMessage(WM_MYSLIDER,m_DragProgress);
		m_LeftBtnDown=FALSE;
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CMySlider::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_LeftBtnDown=FALSE;
	
	CWnd::OnMouseLeave();
}

void CMySlider::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TRACKMOUSEEVENT te;
	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_LEAVE;
	te.hwndTrack = this->GetSafeHwnd();
	te.dwHoverTime = 0;
	TrackMouseEvent(&te);
	if (m_LeftBtnDown==TRUE)
	{
		CRect rc;
		GetClientRect(rc);
	
		if (point.x<4)
		{
			point.x=4;
			m_DragProgress=0;
		}
		if (point.x>280)
		{
			SetProgress(100);
			m_DragProgress=99;

		}
		else
		{
			SetProgress((point.x)*100/280);
			m_DragProgress=(point.x)*100/280;
		}

		if (GetParent())
		{
			GetParent()->InvalidateRect(CRect(11,120-1,297,131-1));
			if (m_DragProgress<0)
			{
				m_DragProgress=0;
			}
			if (m_DragProgress>100)
			{
				m_DragProgress=99;
			}
			
		}
		m_iProgress=m_DragProgress;
	}
	CWnd::OnMouseMove(nFlags, point);
}
