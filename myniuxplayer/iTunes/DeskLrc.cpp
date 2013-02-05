// DeskLrc.cpp : 实现文件
//

#include "stdafx.h"
#include "iTunes.h"
#include "DeskLrc.h"
#include "drawtext.h"
// CDeskLrc 对话框
IMPLEMENT_DYNAMIC(CDeskLrc, CDialog)

CDeskLrc::CDeskLrc(CWnd* pParent /*=NULL*/)
	: CDialog(CDeskLrc::IDD, pParent)
{
	m_hmemBmp=NULL;
	m_pBits=NULL;
	m_dwSize=0;
	m_dwEffWidth=0;
	m_rect.SetRectEmpty();
	_tcscpy_s(m_szText,_T(""));

	m_bEscape=FALSE;
}

CDeskLrc::~CDeskLrc()
{
}

void CDeskLrc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDeskLrc, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDeskLrc 消息处理程序

void CDeskLrc::SmoothShadow(LPTSTR lpszText,CRect rc,COLORREF clrText,
							UINT nFontSize,DWORD dwFlag,CFont* pFont)
{
	ASSERT(lpszText!=NULL);
	if(rc.IsRectEmpty())
		return;

	HDC hdc=::GetDC(NULL);

	CDC Mdc;
	if(Mdc.CreateCompatibleDC(CDC::FromHandle(hdc)))
	{
		HFONT hfont=NULL;
		if(pFont==NULL)
		{
			LOGFONT lf={0};
			::GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(LOGFONT),&lf);
			lf.lfWeight=FW_BOLD;
			lf.lfHeight = nFontSize;
			lstrcpyn(lf.lfFaceName,_T("幼圆"),_countof(lf.lfFaceName));
			hfont=::CreateFontIndirect(&lf);
		}
		else
			hfont=(HFONT)pFont->m_hObject;

		Mdc.SelectObject(hfont);	

		BYTE nTransparent,a1;

		MAT2 mat2 = {{0,1}, {0,0}, {0,0}, {0,1}};
		GLYPHMETRICS gm={0};

		bool bHaveOffsety=false;
		short cyOffset;

		SIZE size=CTextDrawer::GetDrawSize(Mdc,lpszText,dwFlag,bHaveOffsety,cyOffset);

		int left=rc.left;
		int top=rc.top;

		if(dwFlag & DT_CENTER)
		{
			left += ((rc.Width() - size.cx)>>1);
		}
		if(dwFlag & DT_VCENTER)
		{
			top += ((rc.Height() - size.cy)>>1);
		}

		CRect rc(0,0,size.cx,size.cy);
		rc.OffsetRect(left,top);

		BYTE tr=GetRValue(clrText),tg=GetGValue(clrText),tb=GetBValue(clrText);
		float f,f1;
		long slen=_tcslen(lpszText);
		UINT nChar;
		DWORD dwLen;
		BYTE* pBytes; 
		for(int n=0; n<slen; n++)
		{
			if(m_bEscape || left > rc.right)
				break;
			nChar=*(lpszText+n);
			if(nChar >= 0xa0)
			{//非英文
				//			nChar = (((nChar<<8)&0xff00) | (*(lpszText+ ++n) & 0x00ff)); //++n
			}
			dwLen=GetGlyphOutline(Mdc,nChar,GGO_GRAY8_BITMAP,&gm,0,NULL,&mat2);
			if(dwLen != GDI_ERROR)
			{
				pBytes=new BYTE[dwLen];
				if(pBytes)
				{
					dwLen=GetGlyphOutline(Mdc,nChar,GGO_GRAY8_BITMAP,&gm,dwLen,pBytes,&mat2);
					if(dwLen!=GDI_ERROR)
					{
						//对齐字符输出
						if(CTextDrawer::IsDoubleByteChar(nChar))
						{
							if(_istpunct(nChar) && nChar!=_T('—')) //标点符号，中文——
							{
								top=rc.bottom - gm.gmBlackBoxY - (bHaveOffsety && CTextDrawer::IsYOffset(nChar) ? cyOffset : 0);
							}
							else
							{//居中显示
								top=(rc.bottom + rc.top - gm.gmBlackBoxY)>>1;
							}
						}
						else if(nChar == _T('-'))
							top=(rc.bottom + rc.top - gm.gmBlackBoxY)>>1; //居中显示
						else
							top=rc.bottom - gm.gmBlackBoxY - (bHaveOffsety && CTextDrawer::IsYOffset(nChar) ? cyOffset : 0);

						int nByteCount = ((8 * gm.gmBlackBoxX +31) >> 5) << 2;
						//填充数据
						BYTE* dst,*src;
						if(nChar != _T(' '))
						{
							for(long y=0; y<(long)gm.gmBlackBoxY; y++)
							{
								if(m_bEscape)	break;
								dst=m_pBits + (m_rect.bottom - top - y - 1)*m_dwEffWidth + left*4;
								src= pBytes + y * nByteCount;
								for(long x=0; x<(long)gm.gmBlackBoxX; x++)
								{
									if(m_bEscape)	break;
									f=(float)(*src)/64;
									f1=1.0f-f;

									a1=*(dst+3);
									nTransparent=(BYTE)(~a1);
									*dst++=((BYTE)(tb*f + (*dst)*f1)*nTransparent + a1*(*dst))>>8;
									*dst++=((BYTE)(tg*f + (*dst)*f1)*nTransparent + a1*(*dst))>>8;
									*dst++=((BYTE)(tr*f + (*dst)*f1)*nTransparent + a1*(*dst))>>8;
									*dst++=((BYTE)(255*f)*nTransparent + a1*(*dst))>>8;

									src++;
								}
							}
						}
						left+=gm.gmBlackBoxX + CTextDrawer::m_cxSpacing;
						if(nChar == _T(' '))
							left+=CTextDrawer::m_cwBlank;
					}
					delete[]pBytes;
				}
			}
		}
		if(pFont==NULL)
			::DeleteObject(hfont);
	}
	VERIFY(::ReleaseDC(NULL,hdc)==1);
}
void CDeskLrc::OnSize(UINT nType, int cx, int cy)
{
	if(cx!=0 && cy!=0)
	{
		m_rect.SetRect(0,0,cx,cy);
		if(m_hmemBmp)
			::DeleteObject(m_hmemBmp);
		BITMAPINFO  bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth=cx;
		bmi.bmiHeader.biHeight=cy;
		bmi.bmiHeader.biPlanes=1;
		bmi.bmiHeader.biBitCount=32;
		bmi.bmiHeader.biCompression=BI_RGB;

		m_hmemBmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&m_pBits, 0, 0);
		if(m_hmemBmp && m_pBits)
		{
			m_dwSize=bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * 4;
			m_dwEffWidth = ((((32 * bmi.bmiHeader.biWidth) + 31) / 32) * 4);
		}
		UpdateLayered();
	}
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}
void CDeskLrc::SetLyric(LPTSTR lpszText)
{
	_ASSERT(lpszText!=NULL);
	_tcscpy_s(m_szText,lpszText);
	memset(m_pBits,0,m_dwSize);
	UpdateLayered();
}

void CDeskLrc::UpdateLayered()
{
	if(m_hWnd == NULL || !::IsWindow(m_hWnd))
		return;

	//	if(!UpdateLayeredWindow)
	//	return;

	CClientDC dc(this);
	CDC dcMemory;
	if(dcMemory.CreateCompatibleDC(&dc))
	{
		CRect rc(m_rect);
		//SmoothDrawTextEx(m_szText,&rc,RGB(100,50,50),RGB(202,100,200),60,DT_LEFT,NULL);
		SmoothDrawTextEx(m_szText,&rc,RGB(0,158,255),RGB(0,235,255),60,DT_LEFT,NULL);
		rc.OffsetRect(1,1);
		SmoothShadow(m_szText,&rc,RGB(10,10,10),60,DT_LEFT,NULL);

		if(m_hmemBmp)
		{
			dcMemory.SelectObject(m_hmemBmp);

			CRect rcWnd;
			GetWindowRect(&rcWnd);
			POINT ptWinPos={rcWnd.left,rcWnd.top};
			SIZE sizeWindow={rcWnd.Width(),rcWnd.Height()};
			POINT ptSrc={0,0};

			BLENDFUNCTION Blend={AC_SRC_OVER,0,255,AC_SRC_ALPHA};

			HDC hdcScreen=::GetDC(NULL);
			::UpdateLayeredWindow(m_hWnd,hdcScreen,&ptWinPos,&sizeWindow,dcMemory,&ptSrc,0,&Blend,2);
			VERIFY(::ReleaseDC(NULL,hdcScreen)==1);
		}
	}
}
void CDeskLrc::SmoothDrawTextEx(LPTSTR lpszText,CRect rc,COLORREF clr1,
								COLORREF clr2,UINT nFontSize/* =14 */,DWORD dwFlag,CFont* pFont)
{
	ASSERT(lpszText!=NULL);
	if(rc.IsRectEmpty())
		return;

	HDC hdc=::GetDC(NULL);
	CDC Mdc;
	if(Mdc.CreateCompatibleDC(CDC::FromHandle(hdc)))
	{
		HFONT hfont=NULL;
		if(pFont==NULL)
		{
			LOGFONT lf={0};
			::GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(LOGFONT),&lf);
			lf.lfWeight=FW_BOLD;
			lf.lfHeight = nFontSize;
			lstrcpyn(lf.lfFaceName,_T("幼圆"),_countof(lf.lfFaceName));	
			hfont=::CreateFontIndirect(&lf);
		}
		else
			hfont=(HFONT)pFont->m_hObject;

		Mdc.SelectObject(hfont);	

		MAT2 mat2 = {{0,1}, {0,0}, {0,0}, {0,1}};
		GLYPHMETRICS gm={0};

		bool bHaveOffsety=false;
		short cyOffset;

		SIZE size=CTextDrawer::GetDrawSize(Mdc,lpszText,dwFlag,bHaveOffsety,cyOffset);

		int left=rc.left;
		int top=rc.top;

		BYTE nTransparent,a1;

		if(dwFlag & DT_CENTER)
		{
			left += ((rc.Width() - size.cx)>>1);
		}
		if(dwFlag & DT_VCENTER)
		{
			top += ((rc.Height() - size.cy)>>1);
		}

		CRect rc(0,0,size.cx,size.cy);
		rc.OffsetRect(left,top);

		BYTE tr,tg,tb;
		float f,f1;
		long slen=_tcslen(lpszText);
		UINT nChar;
		DWORD dwLen;
		BYTE* pBytes;
		//		BYTE* dst,*src;
		for(int n=0; n<slen; n++)
		{
			if(m_bEscape || left > rc.right)
				break;
			nChar=*(lpszText+n);
			if(nChar >= 0xa0)
			{//非英文
				//	nChar = (((nChar<<8)&0xff00) | (*(lpszText+ ++n) & 0x00ff)); //++n
			}
			dwLen=GetGlyphOutline(Mdc,nChar,GGO_GRAY8_BITMAP,&gm,0,NULL,&mat2);
			if(dwLen != GDI_ERROR)
			{
				pBytes=new BYTE[dwLen];
				if(pBytes)
				{
					dwLen=GetGlyphOutline(Mdc,nChar,GGO_GRAY8_BITMAP,&gm,dwLen,pBytes,&mat2);
					if(dwLen!=GDI_ERROR)
					{
						//对齐字符输出
						if(CTextDrawer::IsDoubleByteChar(nChar))
						{
							if(_istpunct(nChar) && nChar!=_T('—')) //标点符号，中文——
							{
								top=rc.bottom - gm.gmBlackBoxY - (bHaveOffsety && CTextDrawer::IsYOffset(nChar) ? cyOffset : 0);
							}
							else
							{//居中显示
								top=(rc.bottom + rc.top - gm.gmBlackBoxY)>>1;
							}
						}
						else if(nChar == _T('-'))
							top=(rc.bottom + rc.top - gm.gmBlackBoxY)>>1; //居中显示
						else
							top=rc.bottom - gm.gmBlackBoxY - (bHaveOffsety && CTextDrawer::IsYOffset(nChar) ? cyOffset : 0);

						int nByteCount = ((8 * gm.gmBlackBoxX +31) >> 5) << 2;

						//填充数据

						float fr=(float)(GetRValue(clr2)-GetRValue(clr1))/((float)rc.Height()/2+1);
						float fg=(float)(GetGValue(clr2)-GetGValue(clr1))/((float)rc.Height()/2+1);
						float fb=(float)(GetBValue(clr2)-GetBValue(clr1))/((float)rc.Height()/2+1);

						BYTE* dst,*src;
						if(nChar != _T(' '))
						{
							int i=top-rc.top+1;
							if(i > rc.Height()/2)
							{
								i-=rc.Height()/2;
								tr=BYTE(GetRValue(clr2) - i * fr);
								tg=BYTE(GetGValue(clr2) - i * fg);
								tb=BYTE(GetBValue(clr2) - i * fb);
							}
							else
							{
								tr=BYTE(i * fr + GetRValue(clr1));
								tg=BYTE(i * fg + GetGValue(clr1));
								tb=BYTE(i * fb + GetBValue(clr1));
							}
							for(long y=0; y<(long)gm.gmBlackBoxY; y++)
							{
								if(m_bEscape)	break;
								dst=m_pBits + (m_rect.bottom - top - y - 1)*m_dwEffWidth + left*4;
								src= pBytes + y * nByteCount;
								if(top + y + 2 - rc.top> rc.Height()/2)
								{
									tr-=(BYTE)fr;
									tg-=(BYTE)fg;
									tb-=(BYTE)fb;
								}
								else
								{
									tr+=(BYTE)fr;
									tg+=(BYTE)fg;
									tb+=(BYTE)fb;
								}
								for(long x=0; x<(long)gm.gmBlackBoxX; x++)
								{
									if(m_bEscape)	break;
									f=(float)(*src)/64;
									f1=BYTE(1.0-f);
									a1=*(dst+3);
									nTransparent=(BYTE)(~a1);
									*dst++=((BYTE)(tb*f + (*dst)*f1)*nTransparent + a1*(*dst))>>8;
									*dst++=((BYTE)(tg*f + (*dst)*f1)*nTransparent + a1*(*dst))>>8;
									*dst++=((BYTE)(tr*f + (*dst)*f1)*nTransparent + a1*(*dst))>>8;
									*dst++=((BYTE)(255*f)*nTransparent + a1*(*dst))>>8;
									src++;
								}
							}
						}
						left+=gm.gmBlackBoxX + CTextDrawer::m_cxSpacing;
						if(nChar == _T(' '))
							left+=CTextDrawer::m_cwBlank;
					}
					delete[]pBytes;
				}
			}
		}
		if(pFont==NULL)
			::DeleteObject(hfont);
	}
	VERIFY(::ReleaseDC(NULL,hdc)==1);
}

int CDeskLrc::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE)|WS_EX_LAYERED);
	return 0;
}

BOOL CDeskLrc::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetTimer(202,500,NULL);
	UpdateLayered();
	SetWindowPos(&wndTopMost,50,650,0,0,SWP_NOSIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDeskLrc::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CString str=((CiTunesApp*)AfxGetApp())->strCurrentLrc;
	//SetLyric(m_arrLyric[n]);
	/*str.Remove(L'\n');*/
	TCHAR *p=str.GetBuffer(0);
str.ReleaseBuffer();
	SetLyric(p);

	CDialog::OnTimer(nIDEvent);
}

BOOL CDeskLrc::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.x=	100;
	cs.y=500;
	return CDialog::PreCreateWindow(cs);
}

LRESULT CDeskLrc::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rc;
	GetClientRect(rc);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);
}
