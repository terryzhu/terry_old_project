// MovingLrcThread.cpp : 实现文件
//

#include "stdafx.h"
#include "iTunes.h"
#include "MovingLrcDlg.h"
#include "MovingLrcThread.h"


// CMovingLrcThread

IMPLEMENT_DYNCREATE(CMovingLrcThread, CWinThread)

CMovingLrcThread::CMovingLrcThread()
{
}

CMovingLrcThread::~CMovingLrcThread()
{
}

BOOL CMovingLrcThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	CMovingLrcDlg *pcmld=new CMovingLrcDlg;
	pcmld->Create(IDD_MOVINGLRC);
	pcmld->ShowWindow(SW_SHOW);
	return TRUE;
}

int CMovingLrcThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMovingLrcThread, CWinThread)
END_MESSAGE_MAP()


// CMovingLrcThread 消息处理程序
