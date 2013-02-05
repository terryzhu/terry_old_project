// LrcWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "iTunes.h"
#include "LrcWnd.h"
#include "iTunesDlg.h"
#include "drawtext.h"
// CLrcWnd 对话框

IMPLEMENT_DYNAMIC(CLrcWnd, CDialog)

CLrcWnd::CLrcWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CLrcWnd::IDD, pParent)
{

}

CLrcWnd::~CLrcWnd()
{
}

void CLrcWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLrcWnd, CDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CLrcWnd 消息处理程序

//BOOL CLrcWnd::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	// TODO:  在此添加额外的初始化
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//	// 异常: OCX 属性页应返回 FALSE
//}

int CLrcWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetTimer(12,100,NULL);
	 CiTunesDlg *pdlg=((CiTunesApp*)AfxGetApp())->m_piTunesdlg;
	 for (int i=0;i<pdlg->m_nIndex;i++)
	 {
		 if (pdlg->m_nSecondsHasPast<pdlg->m_SongArr[i].time)
		 {
			 ipast=(float)i*14;
			 break;
		 }
	 }

	SetWindowPos(&wndTopMost,829,73,0,0,SWP_NOSIZE);
	

	 
	 
	
	
	return 0;
}

void CLrcWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CiTunesDlg *pdlg=((CiTunesApp*)AfxGetApp())->m_piTunesdlg;
	if (pdlg->HasLrc==FALSE)
	{
		return;
	}
	CClientDC dc(this);
	CRect   clientRect;   
	GetClientRect( &clientRect );   
	CDC   MemDC;   
	CBitmap   Membitmap;
	 m_alllength=100+14*pdlg->m_nIndex;
	Membitmap.CreateCompatibleBitmap(&dc,clientRect.Width(),m_alllength);
	MemDC.CreateCompatibleDC(&dc);
	CBitmap*pOldBitmap=MemDC.SelectObject(&Membitmap);
	MemDC.FillSolidRect(0,0,clientRect.Width(),m_alllength,RGB(37,38,57));
//nHeight   nWidth   nEscapement   nOrientation   nWeight bItalic   bUnderline   cStrikeOut   nCharSet   nOutPrecision 
	//nClipPrecision   nQuality   nPitchAndFamily   
	CFont m_hFont;
	m_hFont.CreateFont(12,0,0, 0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("宋体"));
	MemDC.SelectObject(&m_hFont);
	MemDC.SetTextColor(RGB(117,120,171));
	//////////////////////////////////////////////////////////////////////////
	
	
	int index=pdlg->m_nLrcIndex;
	static int lastindex=0;
	static float  ipastsingle=0.0f;
	static int TextOutWidthPix=0;
	static float TextOutWidthPixSingle=0.0f;
	static int nMoveToNextLrc=-1;
	if (pdlg->m_nLrcIndex==0)
	{
		lastindex=0;
		ipast=0;
		ipastsingle=0;
	}
	
	if (index+1<pdlg->m_nIndex)
	{
		
		
		int interval=0;
		if (index==0)
		{
			interval=(pdlg->m_SongArr[index]).time;
		}
		else
		{
			interval=(pdlg->m_SongArr[index]).time-(pdlg->m_SongArr[index-1]).time;
		}
		if (lastindex==index)
		{
			ipast+=ipastsingle;
			TextOutWidthPix+=(int)TextOutWidthPixSingle;
		}
		else
		{
			lastindex=index;
			if (interval!=0)
			{
				nMoveToNextLrc=0;
				TextOutWidthPix=0;
				TextOutWidthPixSingle=(float)CTextDrawer::GetTextWidthPix(MemDC.m_hDC,(pdlg->m_SongArr[index-1]).lrc)/interval/10.0f+1;
				ipastsingle=14.0f/10.0f/interval+0.05f;
			}
		}
		for (int i=0;i<pdlg->m_nIndex;++i)
		{
			MemDC.TextOut(10,100+14*i,pdlg->m_SongArr[i].lrc);
			if (i==pdlg->m_nLrcIndex-1&&pdlg->m_nLrcIndex>0)
			{
				if (nMoveToNextLrc!=-1)
				{
					if (nMoveToNextLrc==6)
					{
						nMoveToNextLrc=-1;
					}
					else
					{
						MemDC.SetTextColor(RGB(209-11*nMoveToNextLrc,
							209-11*nMoveToNextLrc,219-5*nMoveToNextLrc));
						
						if (i>0)
						{
							MemDC.TextOut(10,100+14*(i-1),pdlg->m_SongArr[i-1].lrc);
						}

						nMoveToNextLrc++;
					}
				}
				
				MemDC.SetTextColor(RGB(255,255,255));
				CRect rc(10,100+14*i,10+TextOutWidthPix,130+14*i);
				MemDC.DrawText(pdlg->m_SongArr[i].lrc,rc,DT_LEFT);
				//MemDC.TextOut(10,100+14*i,pdlg->m_SongArr[i].lrc);
				MemDC.SetTextColor(RGB(117,120,171));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	dc.BitBlt(0,0,clientRect.Width(),clientRect.Height(),&MemDC,0,(int)ipast,SRCCOPY);   
	MemDC.DeleteDC();   
	Membitmap.DeleteObject(); 

	CDialog::OnTimer(nIDEvent);
}

void CLrcWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect clientRect;
	GetClientRect(clientRect);
	dc.FillSolidRect(0,0,clientRect.Width(),clientRect.Height(),RGB(37,38,57));
}


BOOL CLrcWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CiTunesDlg *pdlg=((CiTunesApp*)AfxGetApp())->m_piTunesdlg;
	FLOAT interval=FLOAT(pdlg->m_SongArr[pdlg->m_nLrcIndex].time-pdlg->m_SongArr[pdlg->m_nLrcIndex-1].time);
	float yOffset=14.0f/interval;
	static int SecondOffset=0;
	if (pdlg->HasLrc==FALSE)
	{
		return CDialog::OnMouseWheel(nFlags, zDelta, pt);
	}
	if (zDelta>0)//滚轮向上,歌词太慢
	{
		SecondOffset++;
		if (ipast+yOffset<m_alllength)
		{
			ipast+=yOffset;
		}
	
		for (int i=0;i<pdlg->m_nIndex;i++)
		{
			pdlg->m_SongArr[i].time--;
		}
	}
	else
	{
		SecondOffset--;
		ipast-=yOffset;
		for (int i=0;i<pdlg->m_nIndex;i++)
		{
			pdlg->m_SongArr[i].time++;
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

LRESULT CLrcWnd::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rc;
	GetClientRect(rc);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);
}
