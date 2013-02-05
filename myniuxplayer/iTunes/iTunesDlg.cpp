// iTunesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "iTunes.h"
#include "iTunesDlg.h"
#include "drawtext.h"
#include "MovingLrcDlg.h"
#include "GetLrc.h"
#include "LrcWnd.h"
#include "skinscrollwnd.h"
#include "MovingLrcThread.h"
#include "Dbt.h"
#include "MP3Struct.h"
#include "GetID3.h"
#include <dwmapi.h>
#pragma comment (lib, "dwmapi.lib")
#pragma comment (lib, "UxTheme.lib")

//#include "Mmsystem.h"
#define TIMER_FOR_CAL_TIMES 965
#define CLOSE_BTN_POS 288,5,306,21
#define MIN_BTN_POS 266,5,284,21
#define PLAY_BTN_POS 100,124,160,184
#define PAUSE_BTN_POS 99,123,159,183
#define OFFSET_SLIDER -1
#define SLIDER_RECT 11,120+OFFSET_SLIDER,297,131+OFFSET_SLIDER

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CiTunesDlg::CiTunesDlg(CWnd* pParent /*=NULL*/)
: CDialog(CiTunesDlg::IDD, pParent)
, m_MusicName(_T(""))
, m_IsSmall(FALSE)
, m_WndTitle(_T(""))
, m_siderValue(0)
, m_MusicTime(0)
, m_IsLoop(FALSE)
{
	m_hIcon=AfxGetApp()->LoadIcon(IDI_ICON1);
	m_minutesPast=0;
	pMovingLrcThread=NULL;
	pLrcWndThread=NULL;
	m_titleBlank=1;
	m_font.CreateFont(   
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
		m_interface_bmp.LoadBitmap(IDB_MAININTERFACE);
}

void CiTunesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	//DDX_Control(pDX, IDC_EDIT1, m_MusicTitleEdit);
	//DDX_Text(pDX, IDC_EDIT1, m_MusicName);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Slider(pDX, IDC_SLIDER1, m_siderValue);
	//	DDX_Control(pDX, IDC_BUTTON1, m_btnSelDir);
	DDX_Control(pDX, IDC_BUTTON7, m_LoopBtn);
}

BEGIN_MESSAGE_MAP(CiTunesDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CiTunesDlg::OnBnClickedAdd)
	ON_LBN_DBLCLK(IDC_LIST1, &CiTunesDlg::OnLbnDblclkList1)
	ON_MESSAGE(WM_PLAY,&CiTunesDlg::OnPlayAgain)
	ON_MESSAGE(WM_NC,OnNotifyIcon)
	ON_MESSAGE(WM_MYSLIDER,OnSliderChange)
	ON_MESSAGE(WM_DEVICECHANGE,OnDeviceChange)
	ON_MESSAGE(WM_FOR_LAYED,OnBeginLayed)
	ON_BN_CLICKED(IDC_BUTTON1, &CiTunesDlg::OnBnClickedButton1)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CiTunesDlg::OnNMReleasedcaptureSlider1)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MOVELRCBTN, &CiTunesDlg::OnBnClickedMovelrcbtn)
	ON_BN_CLICKED(IDC_BUTTON3, &CiTunesDlg::OnBnClickedButton3)
	ON_COMMAND(ID_SHOWWND, &CiTunesDlg::OnShowwnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DELETEADLL, &CiTunesDlg::OnBnClickedDeleteadll)
	ON_BN_CLICKED(IDC_BUTTON7, &CiTunesDlg::OnBnClickedButton7)
	ON_COMMAND(ID_PLAY_NEXT, &CiTunesDlg::OnPlayNextMenu)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDM_ADDFILE, &CiTunesDlg::OnAddfile)
	ON_COMMAND(IDM_ADDDIR, &CiTunesDlg::OnAdddir)
	ON_COMMAND(IDM_MVLRC, &CiTunesDlg::OnMvlrc)
	ON_COMMAND(IDM_DESKLRC, &CiTunesDlg::OnDesklrc)
	ON_COMMAND(IDM_SAVELIST, &CiTunesDlg::OnSavelist)
	ON_COMMAND(IDM_OPENLIST, &CiTunesDlg::OnOpenlist)
	ON_COMMAND(IDM_CLEARLIST, &CiTunesDlg::OnClearlist)
	ON_COMMAND(IDM_PAUSE_PLAY, &CiTunesDlg::OnPausePlay)
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_NORMAL_LRC, &CiTunesDlg::OnNormalLrc)
	ON_COMMAND(IDM_DOWN_LRC, &CiTunesDlg::OnDownLrc)
END_MESSAGE_MAP()

// CiTunesDlg 消息处理程序

BOOL CiTunesDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
	//OSVERSIONINFO osvi;
	//ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	//osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	//GetVersionEx(&osvi);
	//if(osvi.dwMajorVersion >= 6)
	//{
	//	MARGINS m = {-1,-1,-1,-1};
	//	DwmExtendFrameIntoClientArea(m_hWnd, &m);
	//}
	//
	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING,IDM_ABOUTBOX, strAboutMenu);
			pSysMenu->AppendMenu(MF_STRING,IDM_NEXTMUSIC,L"播放下一首");
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CoInitialize(0);
	InitVariable();
	InitPlayModule();
	SetTimer(TIMER_FOR_CHANGE_TITLE_ID,999,NULL);
	SetTimer(TIMER_FOR_CAL_TIMES,60000,NULL);
	HANDLE hMutex=CreateMutex(0, TRUE, L"MYPLAYER");
	if (GetLastError()==ERROR_ALREADY_EXISTS)
	{
		
		::MessageBox(NULL,L"已经有一个在运行",NULL,MB_OK);
		PostQuitMessage(0);

	}
	HBITMAP SrcBitmap=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_SCROLL));
	//SrcBitmap=(HBITMAP)LoadImage(NULL,  //应用程序实例   
	//	L"scrollbar.bmp",     //路径
	//	IMAGE_BITMAP,    
	//	0,        //如果不置0,则此参数指定bitmap的宽(位图被拉伸)  
	//	0,        //如果不置0,则此参数指定bitmap的高(位图被拉伸)  
	//	LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);
	m_ListBox.ModifyStyleEx(WS_EX_CLIENTEDGE,0);
	SkinWndScroll(&m_ListBox,SrcBitmap);
	CString str(AfxGetApp()->m_lpCmdLine);
	if (str.GetLength()!=0)
	{
		str.Remove(L'\"');
		m_ListBox.InsertString(0,str);
	}
	CSimpleXML xml;
	CSimpleXMLNodes xmlNodes;
	BOOL ret=xml.Open(L"NiuxPlayer.xml");

	if (ret)
	{

		xml.SelectNodes(L"NiuxPlayer",xmlNodes);
		for (int i=0;i<xmlNodes.GetCount();i++)
		{
			CString str;
			xmlNodes.GetNumberedItem(i,str);
			m_ListBox.AddString(str);
		}

	}
	SetLayeredWindowAttributes(RGB(0,0,0),0, LWA_ALPHA);
	//SetWindowPos(&wndNoTopMost,0,0,309,556,SWP_NOMOVE);
	CRgn rgn;
	rgn.CreateRoundRectRgn(0,0,309,556,7,7);
	SetWindowRgn(rgn,FALSE);
	PostMessage(WM_FOR_LAYED);
	CMyButton * pMybtn=new CMyButton(IDB_CLOSENORMAL,IDB_CLOSEMOVE,IDB_CLOSEPRESS,
		&CiTunesDlg::PressCloseBtn,this);
	pMybtn->Create(AfxRegisterWndClass(
		CS_VREDRAW | CS_HREDRAW), NULL,WS_CHILD, CRect(CLOSE_BTN_POS), 
		this,997, NULL);
	pMybtn->ShowWindow(SW_SHOW);
	pMybtn->UpdateWindow();


	pMybtn=new CMyButton(IDB_MIN_NORMAL,IDB_MIN_MOVE,IDB_MIN_PRESS,
		&CiTunesDlg::PressMinBtn,this);
	pMybtn->Create(AfxRegisterWndClass(
		CS_VREDRAW | CS_HREDRAW), NULL,WS_CHILD, CRect(MIN_BTN_POS), 
		this,998, NULL);
	pMybtn->ShowWindow(SW_SHOW);
	pMybtn->UpdateWindow();

	pPlayBtn=new CMyButton(IDB_PLAY_NORMAL,IDB_PLAY_MOVE,IDB_PLAY_PRESS,
		&CiTunesDlg::PressPlayBtn,this);
	
	pPlayBtn->Create(AfxRegisterWndClass(
		CS_VREDRAW | CS_HREDRAW), NULL,WS_CHILD, CRect(PLAY_BTN_POS), 
		this,999, NULL);
	pPlayBtn->ShowWindow(SW_SHOW);
	pPlayBtn->UpdateWindow();

	pPauseBtn=new CMyButton(IDB_PAUSE_NORMAL,IDB_PAUSE_MOVE,IDB_PAUSE_PRESS,
		&CiTunesDlg::PressPauseBtn,this);
	pPauseBtn->Create(AfxRegisterWndClass(
		CS_VREDRAW | CS_HREDRAW), NULL,WS_CHILD, CRect(PAUSE_BTN_POS), 
		this,1023, NULL);
	pPauseBtn->ShowWindow(SW_HIDE);
	pPauseBtn->UpdateWindow();

	//progress bar:
	m_pMySlider=new CMySlider();
	m_pMySlider->Create(AfxRegisterWndClass(
		CS_VREDRAW | CS_HREDRAW),NULL,WS_CHILD,CRect(SLIDER_RECT),this,965,NULL);
	m_pMySlider->ShowWindow(SW_SHOW);
	m_pMySlider->UpdateWindow();
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE



}
void CiTunesDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID==SC_MAXIMIZE)
	{
		MaxAero();
		return;
	}
	if (nID==SC_MINIMIZE)
	{
		MinimiszeIntoTray();
		return;
	}
	if ((nID & 0xFFFF)==IDM_NEXTMUSIC)
	{
		OnPlayNext();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CiTunesDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{


		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CRect   clientRect;   
		GetClientRect( &clientRect );


		
		
		CDC MainInterfaceDC;
		MainInterfaceDC.CreateCompatibleDC(&dc);
		MainInterfaceDC.SelectObject(&m_interface_bmp);
		dc.BitBlt(0,0,clientRect.Width(),clientRect.Height(),&MainInterfaceDC,0,0,SRCCOPY);
		dc.SelectObject(&m_font);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(255,255,255));
		dc.DrawText(m_WndTitle,CRect(10+2*m_titleBlank,5,240,30),DT_LEFT);
		//dc.TextOut(2*m_titleBlank,5,m_WndTitle);
		CDialog::OnPaint();
	}



}

HCURSOR CiTunesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CiTunesDlg::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码

	CFileDialog cfd(TRUE,NULL,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT);
	cfd.m_ofn.nMaxFile=12444;
	TCHAR *p=new TCHAR[12444];
	p[0]=NULL;
	cfd.m_ofn.lpstrFile=p;
	if (cfd.DoModal()==IDOK)
	{
		POSITION mPos=cfd.GetStartPosition();
		while (mPos)
		{
			CString str=cfd.GetNextPathName(mPos);
			m_ListBox.AddString(str);

		}
	}
	delete [] p;

}


void CiTunesDlg::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	pPauseBtn->ShowWindow(SW_SHOW);
	pPlayBtn->ShowWindow(SW_HIDE);
	CString str;
	if (m_ListBox.GetCurSel()==-1)
	{
		return;
	}
	m_ListBox.GetText(m_ListBox.GetCurSel(),str);
	RandomPlayMusic(str);
	//CString     FullPath;   

}

LRESULT CiTunesDlg::OnPlayAgain(WPARAM wp,LPARAM lp)
{
	long evCode, param1, param2;

	HRESULT hr;
	if (m_media_event == NULL)
		return 0;
	while (hr = m_media_event->GetEvent(&evCode, &param1, &param2, 0),SUCCEEDED(hr))
	{
		if (evCode==EC_COMPLETE)
		{
			PlayMusicFromListBox();
		}

	}
	return 0;
}

void CiTunesDlg::RandomPlayMusic(CString strInput)
{
	if (m_graph_builder)
	{

		m_media_control->Stop();

		m_graph_builder->Release();
		m_media_control->Release();
		m_media_event->Release();
		m_media_seeking->Release();

		InitPlayModule();

		HRESULT hr;
		if(hr=m_graph_builder->RenderFile(strInput,NULL))
		{
			PlayMusicFromListBox();
			return;
		}
		m_MusicName=strInput;
		//	m_MusicTitleEdit.UpdateData();
		//	m_MusicTitleEdit.SetWindowText(m_MusicName);
		SetWindowText(strInput);
		m_WndTitle=strInput;
		m_media_seeking->GetDuration(&m_MusicTime);
		long l=(long)(m_MusicTime/10000000);
		CString strTime;
		strTime.Format(L"%d:%d   ",l/60,l%60);
		m_slider.SetPos(0);
		if (m_media_control->Run()==FALSE)
		{
			MessageBox(L"Play Music Error!");
		}

		CString strLocation=L"D:\\music\\歌曲歌词\\";

		int m_nIndexChar=strInput.ReverseFind(L'.');
		CString name=strInput.Left(strInput.GetLength()-3);
		name+=L"lrc";
		CFileFind cff;
		if (cff.FindFile(name)==TRUE)
		{
			strLocation=name;
		}
		else
		{
			m_nIndexChar=name.ReverseFind(L'\\');
			name=name.Right(name.GetLength()-m_nIndexChar);

			strLocation+=name;
		}
		cff.Close();

		char p[255]={0};
		WideCharToMultiByte(CP_ACP,0,strLocation,strLocation.GetLength(),p,255,NULL,NULL);
		LoadLrc(p);


	}
}


LRESULT CiTunesDlg::OnNotifyIcon(WPARAM wParam,LPARAM IParam)
{
	if (IParam == WM_RBUTTONDOWN)
	{
		SetForegroundWindow();
		CMenu menuPopup;                  //声明菜单对象
		if(menuPopup.CreatePopupMenu())      //创建一个弹出式菜单
		{
			menuPopup.AppendMenu(MF_STRING,ID_SHOWWND,L"还原窗口");//向菜单menuPopup中添加菜单项
			menuPopup.AppendMenu(MF_STRING,ID_PLAY_NEXT,L"下一首");
			//显示弹出式菜单，并对用户选择的菜单项作出响应
			CPoint pt;
			GetCursorPos(&pt);
			menuPopup.TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
		}
		menuPopup.DestroyMenu();
	}
	return 0;
}

void CiTunesDlg::FindAllMusic(CString p)
{
	CFileFind cff;
	CString str;
	//str.Format(L"%s\\*.*",p);
	str=p+L"\\*.*";
	BOOL ret=cff.FindFile(str);
	if (ret==FALSE)
	{
		cff.Close();
		return;
	}

	while (ret)
	{
		ret = cff.FindNextFile();
		if (cff.IsDots())
			continue;

		if (cff.IsDirectory())
		{

			CString strFLod = cff.GetFilePath();
			FindAllMusic(strFLod);
		}
		else
		{
			CString strFilePath = cff.GetFilePath();
			m_ListBox.AddString(strFilePath);
		}


	}

	cff.Close();



}

void CiTunesDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//可以去除标题栏
	//SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)^WS_CAPTION);
	BROWSEINFO bi;//BROWSEINFO结构体
	TCHAR Buffer[512]=L"";
	TCHAR FullPath[512]=L"";
	bi.hwndOwner=m_hWnd;//m_hWnd你的程序主窗口
	bi.pidlRoot=NULL;
	bi.pszDisplayName=Buffer;//返回选择的目录名的缓冲区
	bi.lpszTitle=L"请选择MP3文件";//弹出的窗口的文字提示
	bi.ulFlags=BIF_BROWSEINCLUDEFILES;//BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_BROWSEFORCOMPUTER;//只返回目录。其他标志看MSDN
	bi.lpfn=NULL;//回调函数，有时很有用
	bi.lParam=0;
	bi.iImage=0;
	ITEMIDLIST*pidl=::SHBrowseForFolder(&bi);//显示弹出窗口，ITEMIDLIST很重要
	::SHGetPathFromIDList(pidl,FullPath);
	if (FullPath[0]!=NULL)
	{
		MessageBox(FullPath);
		FindAllMusic(FullPath);
	}

}

int  CiTunesDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0,250,LWA_ALPHA);
	HANDLE hFile=CreateFile(L"D:\\C++\\iTunes\\Debug\\cal.dat",GENERIC_WRITE | GENERIC_READ ,
		NULL,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	DWORD dwRead;
	int minute=0;
	ReadFile(hFile,&minute,4,&dwRead,NULL);
	CloseHandle(hFile);
	m_minutesPast+=minute;
	return 0;
}

void CiTunesDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==TIMER_FOR_CAL_TIMES)
	{
		m_minutesPast++;
		
	}
	if (isPlay==FALSE)
	{
		return;
	}
	
	if (m_MusicTime!=0)
	{

		LONGLONG pos;
		m_media_seeking->GetCurrentPosition(&pos);
		double d=double(pos*100.0/m_MusicTime);
		if ( m_pMySlider->m_LeftBtnDown==FALSE)
		{
			m_pMySlider->SetProgress(d);
		}
		
		InvalidateRect(CRect(SLIDER_RECT));	
	}
	
	
	if (m_ListBox.GetCount()>0&&m_MusicTime!=0)
	{

		for (m_nLrcIndex=1;m_nLrcIndex<m_nIndex&&HasLrc==TRUE;m_nLrcIndex++)
		{

			if (m_nSecondsHasPast>=m_SongArr[m_nLrcIndex-1].time&&m_nSecondsHasPast<m_SongArr[m_nLrcIndex].time)
			{
				m_WndTitle=m_SongArr[m_nLrcIndex-1].lrc;
				SetWindowText(m_WndTitle);
				

				if (((CiTunesApp*)AfxGetApp())->strCurrentLrc!=m_WndTitle)
				{
					m_titleBlank=0;
				}
				else
				{
					m_titleBlank++;
				}
				
				InvalidateRect(CRect(0,0,300,30));
				((CiTunesApp*)AfxGetApp())->strCurrentLrc=m_WndTitle;
				break;
			}
		}
		if (m_nSecondsHasPast>m_SongArr[m_nLrcIndex].time&&HasLrc==TRUE)
		{
			if (m_nLrcIndex>m_nIndex)
			{
				m_nLrcIndex--;
			}
		}
	}
m_nSecondsHasPast++;


	
}

void CiTunesDlg::OnPlayNext()
{
	CString str;
	int count=m_ListBox.GetCount();
	srand( (unsigned)time( NULL ) );
	int j=rand()%count;
	m_ListBox.GetText(j,str);
	RandomPlayMusic(str);
	m_ListBox.SetCurSel(j);
	ChangeTrayText();
}

void CiTunesDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码


	UpdateData();
	if (m_MusicTime!=0)
	{
		LONGLONG ll;
		ll=m_MusicTime/100*m_siderValue;
		m_media_seeking->SetPositions(&ll,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
	}
	*pResult = 0;
}
void CiTunesDlg::LoadLrc(char * pszFileName)
{
	FILE *fp;
	TCHAR szP[300]={0};
	TCHAR *pszTemp;
	if((fopen_s(&fp,pszFileName,"r"))!=0)//读取相应的LRC文件
	{
		HasLrc=FALSE;
		//BeginToDownLoadLrcUntilOK(,);
		return;
	}
	HasLrc=TRUE;
	char p[300]={0};
	ZeroMemory(p,300);
	m_nIndex=0;
	m_nLrcIndex=0;
	m_nSecondsHasPast=0;
	while (fgets(p,200,fp))//存入结构体
	{
		MultiByteToWideChar(CP_ACP,0,p,-1,szP,200);
		CString str;
		str=szP;
		if (str.GetAt(0)!=L'[')
		{
			continue;
		}
		if (str.GetAt(1)<L'0'||str.GetAt(1)>L'9')
		{
			//m_nIndex++;
			continue;
		}
		pszTemp=szP;
		int nCountTheSame=0;
		for (int i=0;i<str.GetLength();i++)
		{
			if (str.GetAt(i)==L'[')
			{
				nCountTheSame++;
			}
		}
		int minute=0;
		int sec=0;
		for (int i=0;i<nCountTheSame;i++)
		{
			minute=_wtoi(szP+1+10*i);
			sec=_wtoi(szP+4+10*i);
			m_SongArr[m_nIndex].time=60*minute+sec;
			pszTemp=szP+10*nCountTheSame;
			str=pszTemp;
			str.Remove(L'\n');
			m_SongArr[m_nIndex].lrc=str;
			m_nIndex++;
		}

	}
	for (int i=0;i<m_nIndex;i++)
	{
		for (int j=0;j<m_nIndex-1;j++)
		{
			if (m_SongArr[j].time>m_SongArr[j+1].time)
			{
				SONG temp;
				temp.lrc=m_SongArr[j].lrc;
				temp.time=m_SongArr[j].time;
				m_SongArr[j].time=m_SongArr[j+1].time;
				m_SongArr[j].lrc=m_SongArr[j+1].lrc;
				m_SongArr[j+1].time=temp.time;
				m_SongArr[j+1].lrc=temp.lrc;
			}
		}
	}
	fclose(fp);
}
void CiTunesDlg::InitPlayModule(void)
{

	if(FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, 
		(void**)&m_graph_builder)))
	{

		return ;
	}
	m_graph_builder->QueryInterface(IID_IMediaControl, (void**)&m_media_control);
	m_graph_builder->QueryInterface(IID_IMediaEvent, (void**)&m_media_event);
	m_graph_builder->QueryInterface(IID_IMediaSeeking,(void**)&m_media_seeking);
	if(m_media_event->SetNotifyWindow((OAHWND)m_hWnd,WM_PLAY,NULL)!=S_OK)
	{
		MessageBox(L"ERROR");
	}
}

void CiTunesDlg::InitVariable(void)
{
	m_media_control=NULL;
	m_media_event=NULL;
	m_graph_builder=NULL;
	m_media_seeking=NULL;
	m_nLrcIndex=0;
	HasLrc=FALSE;
	m_nSecondsHasPast=0;
	plrc=NULL;
	isPlay=TRUE;
	((CiTunesApp*)AfxGetApp())->m_piTunesdlg=this;
}

void CiTunesDlg::MaxAero(void)
{
	if (m_IsSmall==FALSE)
	{
		GetWindowRect(m_rect);
		/*	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE)|0x80000L|WS_EX_TOPMOST);*/
		for (int i=0;i<30;i++)
		{
			Sleep(30);
			SetLayeredWindowAttributes(RGB(0,0,0),255-8*i, LWA_ALPHA);
		}
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		MoveWindow(122,0,278,25);
		m_IsSmall=!m_IsSmall;
		SetLayeredWindowAttributes(RGB(0,0,0),255, LWA_ALPHA);

		{
			/*CMovingLrcDlg *pcmld=new CMovingLrcDlg;
			pcmld->Create(IDD_MOVINGLRC);
			pcmld->ShowWindow(SW_SHOW);
			pcmld->UpdateData();*/
		}
		return;
	}
	else
	{
		SetLayeredWindowAttributes(RGB(0,0,0),5, LWA_ALPHA);
		MoveWindow(m_rect);
		UpdateWindow();
		for (int i=0;i<30;i++)
		{
			SetLayeredWindowAttributes(RGB(0,0,0),8*i+15, LWA_ALPHA);
			Sleep(30);
		}
		m_IsSmall=!m_IsSmall;
	}
}

void CiTunesDlg::MinimiszeIntoTray(void)
{
	CRect rect;
	GetWindowRect(rect);
	// TODO: 在此处添加消息处理程序代码
#define HOW_MANY_AERO 30
#define X_DST 1146
#define Y_DST 769
	CPoint ptCenter=rect.CenterPoint();
	int cx=rect.Width();
	int cy=rect.Height();
	for (int i=0;i<HOW_MANY_AERO;i++)
	{
		CPoint ptctyemp;
		ptctyemp.x=ptCenter.x+(X_DST-ptCenter.x)*i/HOW_MANY_AERO;
		ptctyemp.y=ptCenter.y+(Y_DST-ptCenter.y)*i/HOW_MANY_AERO;
		CRect changeRc;
		changeRc.left=ptctyemp.x-(HOW_MANY_AERO-i)*cx/HOW_MANY_AERO/2;
		changeRc.right=(HOW_MANY_AERO-i)*cx/HOW_MANY_AERO/2+ptctyemp.x;
		changeRc.top=ptctyemp.y-(HOW_MANY_AERO-i)*cy/HOW_MANY_AERO/2;
		changeRc.bottom=(HOW_MANY_AERO-i)*cy/HOW_MANY_AERO/2+ptctyemp.y;
		CString str;
		SetWindowPos(&wndTopMost,changeRc.left,changeRc.top,
			changeRc.Width(),changeRc.Height(),SWP_SHOWWINDOW);
		/*CClientDC dc(this);
		
		CRect   clientRect;   
		GetClientRect( &clientRect );


		CBitmap interface_bmp;
		interface_bmp.LoadBitmap(IDB_STRBLT);
		CDC MainInterfaceDC;
		MainInterfaceDC.CreateCompatibleDC(&dc);
		MainInterfaceDC.SelectObject(&interface_bmp);
		dc.SetStretchBltMode(HALFTONE);
		dc.StretchBlt(0,0,clientRect.Width(),clientRect.Height(),&MainInterfaceDC,0,0,309,556,SRCCOPY);
*/
		int layed_num=255-8*i;
		if (layed_num>0)
		{
			SetLayeredWindowAttributes(RGB(0,0,0),layed_num, LWA_ALPHA);
		}
		
		Sleep(10);
	}
	


	NotifyIcon.cbSize=sizeof(NOTIFYICONDATA);
	NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hWnd=m_hWnd;
	NotifyIcon.uCallbackMessage=WM_NC;
	NotifyIcon.uFlags=NIF_ICON | NIF_MESSAGE | NIF_TIP;
	ChangeTrayText();

	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);


	ShowWindow(SW_HIDE);
}

void CiTunesDlg::PlayMusicFromListBox(void)
{
	CString str;
	if (m_IsLoop==TRUE)
	{
		m_ListBox.GetText(m_ListBox.GetCurSel(),str);
		RandomPlayMusic(str);
		//m_ListBox.SetCurSel(j);
		return;
	}

	int count=m_ListBox.GetCount();
	srand( (unsigned)time( NULL ) );
	int j=rand()%count;
	m_ListBox.GetText(j,str);
	RandomPlayMusic(str);
	m_ListBox.SetCurSel(j);
	ChangeTrayText();
}

LRESULT CiTunesDlg::OnDeviceChange(WPARAM wParam,LPARAM lParam)
{
	//CString str;
	//
	//if (wParam==DBT_DEVICEARRIVAL)
	//{
	//	DEV_BROADCAST_HDR* p=(DEV_BROADCAST_HDR*)lParam;
	//	str.Format(L"%d %d %d",p->dbch_devicetype,
	//		p->dbch_reserved,p->dbch_size);
	//}
	//MessageBox(str);
	return 0;
}

void CiTunesDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CoUninitialize();
	CDialog::OnClose();
}

void CiTunesDlg::OnBnClickedMovelrcbtn()
{
	if (pMovingLrcThread==NULL)
	{
		pMovingLrcThread=AfxBeginThread(RUNTIME_CLASS(CMovingLrcThread));
		
	}
	else
	{
		HANDLE hp=pMovingLrcThread->m_hThread;
		if (hp) 
		{
			if (WaitForSingleObject(hp, 1) != WAIT_OBJECT_0)
			{
				TerminateThread(hp,0);
				pMovingLrcThread=NULL;
			}
			
			CloseHandle(hp);
		}
	}
}

void CiTunesDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	HINSTANCE hInst=LoadLibrary(L"GetID3.dll");
	typedef CGetID3App * (*pFnGetMp3Info)();
	pFnGetMp3Info pfn=(pFnGetMp3Info)GetProcAddress(hInst,"GetClass");
	MP3_INFO mp3info;
	CGetID3App * pClass=pfn();
	CString strPath;

	CGetLrc *p=NULL;
	if (m_ListBox.GetCurSel()==-1)
	{
		p=new CGetLrc(L"",L"",L"");
	}
	else
	{
		m_ListBox.GetText(m_ListBox.GetCurSel(),strPath);
		CString lrcpath;
		CString temp=strPath.Right(strPath.GetLength()-strPath.ReverseFind(L'\\')-1);
		lrcpath=L"D:\\music\\歌曲歌词\\"+temp.Left(temp.GetLength()-3)+L"lrc";
		pClass->GetInfo(strPath,mp3info);
		p=new CGetLrc(mp3info.song_name,mp3info.singer_name,lrcpath);
	}
	
	p->Create(IDD_GETLRC_DLG);
	p->ShowWindow(SW_SHOW);
	p->UpdateWindow();


}


void CiTunesDlg::OnShowwnd()
{
	// TODO: 在此添加命令处理程序代码
	ModifyStyleEx(0,WS_EX_TOPMOST);
	SetWindowPos(&wndTopMost,482,88,482+309,88+556,SWP_SHOWWINDOW);
	SetLayeredWindowAttributes(RGB(0,0,0),255, LWA_ALPHA);
	ShowWindow(SW_SHOW);
	Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);
}


void CiTunesDlg::BeginToDownLoadLrcUntilOK(CString strArtist,CString strSongName)
{
	DOWNLOADLRC *pd=new DOWNLOADLRC;

	CString str;
	if (m_ListBox.GetCurSel()==-1)
	{
		return;
	}
	m_ListBox.GetText(m_ListBox.GetCurSel(),str);
	int m_nIndexChar=str.ReverseFind(L'.');
	CString name=str.Left(str.GetLength()-3);
	name+=L"lrc";
	m_nIndexChar=name.ReverseFind(L'\\');
	name=name.Right(name.GetLength()-m_nIndexChar);
	CString strLocation=L"D:\\music\\歌曲歌词\\";
	strLocation+=name;
	pd->m_strSingerName=strArtist;
	pd->m_strSongName=strSongName;
	pd->MusicLrcPath=strLocation;
	CWinThread*pThread=AfxBeginThread(CGetLrc::GetLrcUntilOK,pd);
}

void CiTunesDlg::OnBnClickedSavelist(CString fileName)
{
	// TODO: 在此添加控件通知处理程序代码
	CSimpleXML xml;
	xml.InsertMainItem();

	for (int i=0;i<m_ListBox.GetCount();i++)
	{
		CString strPath;
		m_ListBox.GetText(i,strPath);
		xml.InsertOneNewMusic(strPath);
	}
	xml.Save(fileName);

}

void CiTunesDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	HANDLE hFile=CreateFile(L"D:\\C++\\iTunes\\Debug\\cal.dat",GENERIC_WRITE | GENERIC_READ ,
		NULL,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	DWORD dwRead;
	WriteFile(hFile,&m_minutesPast,4,&dwRead,NULL);
	CloseHandle(hFile);
}

void CiTunesDlg::OnBnClickedDeleteadll()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListBox.ResetContent();
}

void CiTunesDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码

	m_IsLoop=!m_IsLoop;
	if (m_IsLoop==TRUE)
	{
		m_LoopBtn.SetWindowText(L"∽");
	}
	else
	{
		m_LoopBtn.SetWindowText(L"↓");
	}
}

void CiTunesDlg::OnPlayNextMenu()
{
	// TODO: 在此添加命令处理程序代码
	OnPlayNext();
}


void CiTunesDlg::ChangeTrayText()
{
	CString songName;
	if (m_ListBox.GetCurSel()>-1)
	{
		m_ListBox.GetText(m_ListBox.GetCurSel(),songName);
		CString s;
		s=songName.Right(songName.GetLength()-songName.ReverseFind(L'\\')-1);
		memcpy(NotifyIcon.szTip,(int*)s.GetBuffer(),sizeof(TCHAR)*s.GetLength()+2);
		Shell_NotifyIcon(NIM_MODIFY,&NotifyIcon);
	}
	else
	{
		NotifyIcon.szTip[0]=NULL;
		Shell_NotifyIcon(NIM_MODIFY,&NotifyIcon);
	}

}
HBRUSH CiTunesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	COLORREF   clr; 
	HBRUSH   m_brMine; 
	switch(nCtlColor) 
	{ 
	case   CTLCOLOR_LISTBOX: 
		clr   =   RGB(117,120,171); 
		pDC-> SetTextColor(clr);       //设置黑色的文本 
		clr   =   RGB(42,43,63); 
		pDC-> SetBkColor(clr);           //设置淡蓝色的背景 
		m_brMine   =   ::CreateSolidBrush(clr); 
		return   m_brMine;	 //返回对应的刷子句柄 
	default: 
		HBRUSH   hbr   =   CDialog::OnCtlColor(pDC,   pWnd,   nCtlColor); 
		return   hbr; 
	}
}

void CiTunesDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	//CRect rcClose(288,6,300,19);
	//if(rcClose.PtInRect(point))
	//{
	//	for (int i=0;i<30;i++)
	//	{
	//		Sleep(30);
	//		SetLayeredWindowAttributes(RGB(0,0,0),255-8*i, LWA_ALPHA);
	//	}
	//	PostMessage(WM_CLOSE);
	//	return ;
	//}


	/*CRect rcMin(266,5,278,18);
	if(rcMin.PtInRect(point))
	{
		MinimiszeIntoTray();
		return ;
	}*/
	CRect menuPt(28,213,45,230);
	if(menuPt.PtInRect(point))
	{
		SetForegroundWindow();
		CMenu menuPopup;                  //声明菜单对象
		if(menuPopup.CreatePopupMenu())      //创建一个弹出式菜单
		{
			menuPopup.AppendMenu(MF_STRING,IDM_ADDFILE,L"添加文件");//向菜单menuPopup中添加菜单项
			menuPopup.AppendMenu(MF_STRING,IDM_ADDDIR,L"添加文件夹");
			menuPopup.AppendMenu(MF_STRING,IDM_MVLRC,L"移动歌词");
			menuPopup.AppendMenu(MF_STRING,IDM_NORMAL_LRC,L"普通歌词");
			menuPopup.AppendMenu(MF_STRING,IDM_DESKLRC,L"桌面歌词");
			menuPopup.AppendMenu(MF_STRING,IDM_SAVELIST,L"保存列表");
			menuPopup.AppendMenu(MF_STRING,IDM_OPENLIST,L"打开列表");
			menuPopup.AppendMenu(MF_STRING,IDM_CLEARLIST,L"清除列表");
			menuPopup.AppendMenu(MF_STRING,IDM_DOWN_LRC,L"下载歌词");
			
			//显示弹出式菜单，并对用户选择的菜单项作出响应
			CPoint pt;
			GetCursorPos(&pt);
			menuPopup.TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
		}
		menuPopup.DestroyMenu();
		return ;
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CiTunesDlg::OnAddfile()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedAdd();
}

void CiTunesDlg::OnAdddir()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedButton1();
}

void CiTunesDlg::OnMvlrc()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedMovelrcbtn();
}

void CiTunesDlg::OnDesklrc()
{
	// TODO: 在此添加命令处理程序代码
	if (!plrc)
	{
		plrc=new CDeskLrc;
		plrc->Create(IDD_DESKLRC);
		plrc->ShowWindow(SW_SHOW);
	}
	else
	{
		plrc->DestroyWindow();
		plrc=NULL;
	}
}

void CiTunesDlg::OnSavelist()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog cfd(FALSE,L"xml",NULL,OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		L"Music List (*.xml)|*.xml||",this,sizeof(OPENFILENAME));
	
	cfd.DoModal();
	if (cfd.GetFileTitle()==L"")
	{
		return ;
	}
	OnBnClickedSavelist(cfd.GetFileTitle()+L".xml");
	
}

void CiTunesDlg::OnPausePlay()
{
	// TODO: 在此添加命令处理程序代码
	if (isPlay)
	{
		m_media_control->Pause();
		isPlay=FALSE;
	}
	else
	{
		isPlay=TRUE;
		m_media_control->Run();
	}
}


LRESULT CiTunesDlg::OnBeginLayed(WPARAM wParam,LPARAM IParam)
{

	SetLayeredWindowAttributes(RGB(0,0,0),5, LWA_ALPHA);
	//UpdateWindow();
	for (int i=0;i<15;i++)
	{
		SetLayeredWindowAttributes(RGB(0,0,0),16*i+15, LWA_ALPHA);
		Sleep(30);
	}
	SetLayeredWindowAttributes(RGB(0,0,0),255, LWA_ALPHA);
	return S_OK;
}
LRESULT CiTunesDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rc(0,0,243,20);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);

}

void CiTunesDlg::PressCloseBtn()
{
	for (int i=0;i<30;i++)
	{
		Sleep(30);
		SetLayeredWindowAttributes(RGB(0,0,0),255-8*i, LWA_ALPHA);
	}
	PostMessage(WM_CLOSE);
}
void CiTunesDlg::PressMinBtn()
{
	MinimiszeIntoTray();
}
BOOL CiTunesDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CiTunesDlg::OnSliderChange(WPARAM wp,LPARAM)
{
	int progress=(int)wp;
	if (progress>99)
	{
		progress=99;
	}
	if (m_MusicTime!=0)
	{
		LONGLONG ll;
		ll=m_MusicTime/100*wp;
		m_media_seeking->SetPositions(&ll,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
	}
	return TRUE;
}

void CiTunesDlg::OnOpenlist()
{
	
	CFileDialog cfd(TRUE,L"xml",NULL,OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		L"Music List (*.xml)|*.xml||",this,sizeof(OPENFILENAME));

	cfd.DoModal();
	CString xmlFile=cfd.GetPathName();
	if (xmlFile==L"")
	{
		return ;
	}
	
	MessageBox(xmlFile);
	CSimpleXML xml;
	CSimpleXMLNodes xmlNodes;
	BOOL ret=xml.Open(xmlFile);
	if (ret)
	{
		m_ListBox.ResetContent();
		xml.SelectNodes(L"NiuxPlayer",xmlNodes);
		for (int i=0;i<xmlNodes.GetCount();i++)
		{
			CString str;
			xmlNodes.GetNumberedItem(i,str);
			m_ListBox.AddString(str);
		}

	}
}

void CiTunesDlg::OnClearlist()
{
m_ListBox.ResetContent();
}
void CiTunesDlg::OnNormalLrc()
{
	if (pLrcWndThread==NULL)
	{
		pLrcWndThread=AfxBeginThread(RUNTIME_CLASS(CLrcWndThread));
	}
	else
	{
		HANDLE hp=pLrcWndThread->m_hThread;
		if (hp) 
		{
			if (WaitForSingleObject(hp, 1) != WAIT_OBJECT_0)
			{
				TerminateThread(hp,0);
				pLrcWndThread=NULL;
			}
			CloseHandle(hp);
		}
	}
}

void CiTunesDlg::PressPlayBtn()
{
	pPlayBtn->ShowWindow(SW_HIDE);
	pPauseBtn->ShowWindow(SW_SHOW);
	OnPausePlay();
}

void CiTunesDlg::PressPauseBtn()
{
	pPauseBtn->ShowWindow(SW_HIDE);
	pPlayBtn->ShowWindow(SW_SHOW);
	
	OnPausePlay();
}
void CiTunesDlg::OnDownLrc()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedButton3();
}
