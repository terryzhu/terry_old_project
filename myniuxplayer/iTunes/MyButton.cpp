// MyButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "iTunes.h"
#include "MyButton.h"


// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CWnd)

CMyButton::CMyButton(UINT normal_id,UINT move_id,UINT press_id
					 ,pTypeFnLBtnUp pFnLBtnUp,CiTunesDlg * pDlg)
{
	m_normal_id=normal_id;
	m_move_id=move_id;
	m_press_id=press_id;
	m_pFnLBtnUp=pFnLBtnUp;
	m_pDlg=pDlg;
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CWnd)
	ON_WM_NCHITTEST()
	//	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CMyButton ��Ϣ�������



LRESULT CMyButton::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	FillMove();
	return CWnd::OnNcHitTest(point);


}
int CMyButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CMyButton::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	FillNormal();
	CWnd::OnMouseLeave();
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TRACKMOUSEEVENT te;
	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_LEAVE;
	te.hwndTrack = this->GetSafeHwnd();
	te.dwHoverTime = 0;
	TrackMouseEvent(&te);

	CWnd::OnMouseMove(nFlags, point);
}

void CMyButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	FillNormal();
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	FillPress();
	CWnd::OnLButtonDown(nFlags, point);
}

void CMyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	FillMove();
	(m_pDlg->*m_pFnLBtnUp)();
	CWnd::OnLButtonUp(nFlags, point);
}
void CMyButton::FillNormal()
{
	CRect rc;
	GetClientRect(&rc);
	CClientDC dc(this);
	CBitmap bmp;
	bmp.LoadBitmap(m_normal_id);
	CDC dct;
	dct.CreateCompatibleDC(&dc);
	dct.SelectObject(&bmp);
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dct,0,0,SRCCOPY);
}
void CMyButton::FillMove()
{
	CRect rc;
	GetClientRect(&rc);
	CClientDC dc(this);
	CBitmap bmp;
	bmp.LoadBitmap(m_move_id);
	CDC dct;
	dct.CreateCompatibleDC(&dc);
	dct.SelectObject(&bmp);
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dct,0,0,SRCCOPY);
}
void CMyButton::FillPress()
{
	CRect rc;
	GetClientRect(&rc);
	CClientDC dc(this);
	CBitmap bmp;
	bmp.LoadBitmap(m_press_id);
	CDC dct;
	dct.CreateCompatibleDC(&dc);
	dct.SelectObject(&bmp);
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dct,0,0,SRCCOPY);
}