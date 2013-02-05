// LrcWndThread.cpp : 实现文件
//

#include "stdafx.h"
#include "iTunes.h"
#include "LrcWndThread.h"
#include "LrcWnd.h"

// CLrcWndThread

IMPLEMENT_DYNCREATE(CLrcWndThread, CWinThread)

CLrcWndThread::CLrcWndThread()
{
}

CLrcWndThread::~CLrcWndThread()
{
}

BOOL CLrcWndThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化

	CLrcWnd *p=new CLrcWnd();

	p->Create(IDD_LRCWND);
	p->ShowWindow(SW_SHOW);
	p->UpdateWindow();
	return TRUE;
}

int CLrcWndThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CLrcWndThread, CWinThread)
END_MESSAGE_MAP()


// CLrcWndThread 消息处理程序
