// LrcWndThread.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��

	CLrcWnd *p=new CLrcWnd();

	p->Create(IDD_LRCWND);
	p->ShowWindow(SW_SHOW);
	p->UpdateWindow();
	return TRUE;
}

int CLrcWndThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CLrcWndThread, CWinThread)
END_MESSAGE_MAP()


// CLrcWndThread ��Ϣ�������
