// iTunesDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "dshow.h"
#include "atltypes.h"
#include "afxcmn.h"
#include "DeskLrc.h"
#include "MyListBox.h"
#include "SimpleXML.h"
#include "SimpleXMLNodes.h"
#include "MovingLrcThread.h"
#include "LrcWndThread.h"
#include "MyButton.h"
#include "MySlider.h"
#define WM_PLAY (WM_USER+1)
#define WM_NC (WM_USER+1001)
#ifndef WM_MYSLIDER
#define WM_MYSLIDER (WM_USER+0x76)
#endif
#define TIMER_FOR_CHANGE_TITLE_ID 100
#define WM_FOR_LAYED  (WM_USER+2006)
// CiTunesDlg 对话框


class CiTunesDlg : public CDialog
{
// 构造
public:
	CiTunesDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ITUNES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg LRESULT OnPlayAgain(WPARAM,LPARAM);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPlayNext();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAdd();
	CMyListBox m_ListBox;
	afx_msg void OnLbnDblclkList1();
	//void PLayMusic(TCHAR * psz);
	void RandomPlayMusic(CString strInput);
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam,LPARAM IParam);
protected:
public:
	afx_msg void OnBnClickedButton1();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnDeviceChange(WPARAM,LPARAM);//用于接收U盘插入的消息
	CString m_WndTitle;
	CSliderCtrl m_slider;
	int m_siderValue;
	//CEdit m_MusicTitleEdit;
	CString m_MusicName;
	int m_nIndex;//计算歌词有几句
	SONG  m_SongArr[200];
	int m_nSecondsHasPast;
	int m_nLrcIndex;//唱到第几句了
	BOOL HasLrc;//此歌是否有歌词

	//  [9/12/2010 ZJ]
	CMySlider *m_pMySlider;
	CFont m_font;
	int m_titleBlank;

	CMyButton * pPlayBtn;
	CMyButton * pPauseBtn;

	CBitmap m_interface_bmp;

	BOOL isPlay;
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnSliderChange(WPARAM,LPARAM);
	//一些dx组件
	IMediaControl *m_media_control;
	IMediaEventEx *m_media_event;
	IGraphBuilder *m_graph_builder;
	IMediaSeeking *m_media_seeking;

	NOTIFYICONDATA NotifyIcon;//最小化到托盘
	LONGLONG m_MusicTime;

	CRect m_rect;//窗口位置的矩形
	BOOL m_IsSmall;

	INT m_minutesPast;

	CWinThread *pMovingLrcThread;
	CWinThread *pLrcWndThread;


	void FindAllMusic(CString p);
	void InitPlayModule(void);//初始化一些ddshow组件
	void InitVariable(void);//初始化一些变量
	void MaxAero(void);//最大化时候的aero效果
	void MinimiszeIntoTray(void);//最小化到托盘的效果
	void ChangeTrayText();

	LRESULT OnBeginLayed(WPARAM wParam,LPARAM IParam);

	void PlayMusicFromListBox(void);
	CDeskLrc * plrc;
	void LoadLrc(char *);//载入歌词
	afx_msg void OnClose();
	afx_msg void OnBnClickedMovelrcbtn();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnShowwnd();
	void BeginToDownLoadLrcUntilOK(CString strArtist,CString strSongName);//下载歌词一条龙服务
	afx_msg void OnBnClickedSavelist(CString);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedDeleteadll();
	BOOL m_IsLoop;
	afx_msg void OnBnClickedButton7();
	CButton m_LoopBtn;
	afx_msg void OnPlayNextMenu();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAddfile();
	afx_msg void OnAdddir();
	afx_msg void OnMvlrc();
	afx_msg void OnDesklrc();
	afx_msg void OnSavelist();
	afx_msg void OnOpenlist();
	afx_msg void OnPausePlay();
	afx_msg void OnClearlist();
	afx_msg LRESULT OnNcHitTest(CPoint point);

	void PressCloseBtn();
	void PressMinBtn();


	void PressPlayBtn();
	void PressPauseBtn();
	

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNormalLrc();
	afx_msg void OnDownLrc();
};
