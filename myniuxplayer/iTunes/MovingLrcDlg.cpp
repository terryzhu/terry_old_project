// MovingLrcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "iTunes.h"
#include "MovingLrcDlg.h"
#include "drawtext.h"
#include "iTunesDlg.h"
// CMovingLrcDlg 对话框
#define STRANGE_NUM 65.0f
#define TIMER_INTERVAL 10
//
//#define STRANGE_NUM 16.2f
//#define TIMER_INTERVAL 50
IMPLEMENT_DYNAMIC(CMovingLrcDlg, CDialog)

CMovingLrcDlg::CMovingLrcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMovingLrcDlg::IDD, pParent)
{

}

CMovingLrcDlg::~CMovingLrcDlg()
{
}

void CMovingLrcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMovingLrcDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CMovingLrcDlg 消息处理程序

BOOL CMovingLrcDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialog::PreCreateWindow(cs);
}

int CMovingLrcDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO:  在此添加您专用的创建代码
	MoveWindow(CRect(400,0,800,25));
	SetTimer(1,TIMER_INTERVAL,NULL);
	ipast=0.0f;
	m_Font.CreateFont(   
		12,    //nHeight   //_T("Tahoma")); 14
		0,                              //nWidth   
		0,    //nEscapement   
		0,    //nOrientation   
		FW_NORMAL,   //nWeight 
		//FW_MEDIUM,
		//FW_SEMIBOLD,
		FALSE,                          //bItalic   
		FALSE,                          //bUnderline   
		0,                              //cStrikeOut   
		//   ANSI_CHARSET,     
		DEFAULT_CHARSET,  //nCharSet   
		OUT_DEFAULT_PRECIS,             //nOutPrecision   
		CLIP_DEFAULT_PRECIS,            //nClipPrecision   
		DEFAULT_QUALITY,                //nQuality   
		DEFAULT_PITCH   |   FF_SWISS,   //nPitchAndFamily   
		_T("宋体"));
	SetWindowPos(&wndTopMost,400,0,0,0,SWP_NOSIZE);
	CRgn rgn;
	rgn.CreateRoundRectRgn(0,0,400,25,10,10);
	SetWindowRgn(rgn,TRUE);
	ShowWindow(SW_SHOW);
	UpdateWindow();
	return 0;
}

void CMovingLrcDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CiTunesDlg *pdlg=((CiTunesApp*)AfxGetApp())->m_piTunesdlg;
	if (pdlg->HasLrc==FALSE)
	{
		return ;
	}
	if (pdlg->isPlay==FALSE)
	{
		return;
	}
	int index=pdlg->m_nLrcIndex;
	CClientDC dc(this);
	
	CRect   clientRect;   
	GetClientRect( &clientRect );
	CDC   MemDC;   
	CBitmap   Membitmap;
	Membitmap.CreateCompatibleBitmap(&dc,clientRect.Width()*3,clientRect.Height());
	MemDC.CreateCompatibleDC(&dc);
	CBitmap*pOldBitmap=MemDC.SelectObject(&Membitmap);
	MemDC.FillSolidRect(0,0,clientRect.Width()*3,clientRect.Height(),RGB(37,38,57));

	
	
	MemDC.SelectObject(&m_Font);

	MemDC.SetTextColor(RGB(117,120,171));

	static float  ipastsingle=0.0f;
 	static int lastindex=0;
	static int nMoveToNextLrc=-1;
 	if (index<pdlg->m_nIndex)
 	{
 		CString str;
 		int interval=0;
 		if (index==0)
 		{
 			interval=(pdlg->m_SongArr[index]).time;
 		}
 		else
 		{
 			interval=(pdlg->m_SongArr[index]).time-(pdlg->m_SongArr[index-1]).time;
 		}
		//dsadsa
 		if (index>0)
 		{
 			str=(pdlg->m_SongArr[index-1]).lrc;
 		}
 	
 		bool b;
 		short s;
 		SIZE szthis=CTextDrawer::GetDrawSize(MemDC.m_hDC,str,DT_LEFT,b,s);
 		if (lastindex==index)
 		{
 			ipast+=ipastsingle;
 		}
 		else
 		{
 			lastindex=index;
			if (interval!=0)
			{
				nMoveToNextLrc=0;
				ipastsingle=szthis.cx/STRANGE_NUM/interval;
			}
 			ipast=0;
 		}
		//dsadsa
 		if (index==0||index==1)
 		{
			if (index==0)
			{
				CString str=(pdlg->m_SongArr[0]).lrc+L"   "+(pdlg->m_SongArr[1]).lrc;
			}
 			/*ipast=0;
			CString str=(pdlg->m_SongArr[0]).lrc+L"   "+(pdlg->m_SongArr[1]).lrc;
			MemDC.TextOut(20,3,str);*/
 		}
		int x=2;
		int pix=0;
		if (index>2)
		{
			pix=CTextDrawer::GetTextWidthPix(MemDC.m_hDC,(pdlg->m_SongArr[index-2]).lrc);
			int i=0;
			while (200-ipast-pix>0&&i<6)
			{
				i++;
				if (index-1-x>-1)
				{
					if (x==2&&nMoveToNextLrc!=-1)
					{
						if (nMoveToNextLrc==6)
						{
							nMoveToNextLrc=-1;
						}
						else
						{
							MemDC.SetTextColor(RGB(209-11*nMoveToNextLrc,
								209-11*nMoveToNextLrc,219-5*nMoveToNextLrc));
						}					
					}
					MemDC.TextOut(int(200-ipast-pix),3,(pdlg->m_SongArr[index-x]).lrc);
					if (x==2&&nMoveToNextLrc!=-1)
					{
						nMoveToNextLrc++;
						MemDC.SetTextColor(RGB(117,120,171));
					}
					pix+=CTextDrawer::GetTextWidthPix(MemDC.m_hDC,(pdlg->m_SongArr[index-1-x]).lrc);
					x++;
				}
			}
			if (index-1-x>-1)
			{
				
				MemDC.TextOut(int(200-ipast-pix),3,(pdlg->m_SongArr[index-x]).lrc);
				
			}
		}
		x=0;
		pix=0;
		//dsadsa
		if (index>0)
		{
			MemDC.TextOut(int(200-ipast),3,str);//当前这句
			MemDC.SetTextColor(RGB(255,255,255));
			//**********************卡拉ok效果
			MemDC.DrawText(str,CRect(int(200-ipast),3,200,30),DT_LEFT);
			//**********************卡拉ok效果
			MemDC.SetTextColor(RGB(117,120,171));
			int i=0;
			while (200-ipast+pix<400&&index>0&&i<6)
			{
				pix+=CTextDrawer::GetTextWidthPix(MemDC.m_hDC,(pdlg->m_SongArr[index-1+x]).lrc);
				MemDC.TextOut(int(200+pix-ipast),3,(pdlg->m_SongArr[index+x]).lrc);
				i++;
				x++;
			}
		}
		
 
 	}
	dc.BitBlt(0,0,clientRect.Width(),clientRect.Height(),&MemDC,0,0,SRCCOPY);   
	MemDC.DeleteDC();   
	Membitmap.DeleteObject(); 
	CDialog::OnTimer(nIDEvent);
}

BOOL CMovingLrcDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnEraseBkgnd(pDC);
}

int CMovingLrcDlg::CalLrcWidthPix(int Num)
{
	CClientDC dc(this);
	
	dc.SelectObject(&m_Font);
	CiTunesDlg *pdlg=((CiTunesApp*)AfxGetApp())->m_piTunesdlg;
	int widthpix=0;
	bool b;
	short s;
	if (Num==-1)
	{
		Num=pdlg->m_nIndex;
	}
	for (int i=0;i<Num;++i)
	{
		widthpix+=(CTextDrawer::GetDrawSize(dc.m_hDC,pdlg->m_SongArr[i].lrc,DT_LEFT,b,s)).cx;
	}
	return widthpix;
}

BOOL CMovingLrcDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nFlags==MK_CONTROL)
	{
		CiTunesDlg *pdlg=((CiTunesApp*)AfxGetApp())->m_piTunesdlg;
		FLOAT interval=FLOAT(pdlg->m_SongArr[pdlg->m_nLrcIndex].time-
			pdlg->m_SongArr[pdlg->m_nLrcIndex-1].time);
		CClientDC dc(this);
		dc.SelectObject(&m_Font);
		int widthpix=CTextDrawer::GetTextWidthPix(dc.m_hDC,
			pdlg->m_SongArr[pdlg->m_nLrcIndex-1].lrc);
		float xOffset=((FLOAT)widthpix)/interval;
		if (pdlg->HasLrc==FALSE)
		{
			return CDialog::OnMouseWheel(nFlags, zDelta, pt);
		}
		if (zDelta>0)//滚轮向上,歌词太慢
		{
			for (int i=0;i<pdlg->m_nIndex;i++)
			{
				pdlg->m_SongArr[i].time--;
			}
			ipast+=xOffset;
		}
		else
		{
			for (int i=0;i<pdlg->m_nIndex;i++)
			{
				pdlg->m_SongArr[i].time++;
			}
			ipast-=xOffset;
		}
	}
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

LRESULT CMovingLrcDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rc;
	GetClientRect(rc);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);
}
