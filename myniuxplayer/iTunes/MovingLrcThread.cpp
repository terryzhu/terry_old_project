// MovingLrcThread.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	CMovingLrcDlg *pcmld=new CMovingLrcDlg;
	pcmld->Create(IDD_MOVINGLRC);
	pcmld->ShowWindow(SW_SHOW);
	return TRUE;
}

int CMovingLrcThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMovingLrcThread, CWinThread)
END_MESSAGE_MAP()


// CMovingLrcThread ��Ϣ�������
