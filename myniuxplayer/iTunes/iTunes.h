// iTunes.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
class CiTunesDlg;

// CiTunesApp:
// �йش����ʵ�֣������ iTunes.cpp
//

class CiTunesApp : public CWinApp
{
public:
	CiTunesApp();
	CString strCurrentLrc;
// ��д
	public:
	virtual BOOL InitInstance();
	CiTunesDlg * m_piTunesdlg;
// ʵ��

	DECLARE_MESSAGE_MAP()
};
struct SONG
{
	LONG time;
	CString lrc;
};
extern CiTunesApp theApp;