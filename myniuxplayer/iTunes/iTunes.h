// iTunes.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
class CiTunesDlg;

// CiTunesApp:
// 有关此类的实现，请参阅 iTunes.cpp
//

class CiTunesApp : public CWinApp
{
public:
	CiTunesApp();
	CString strCurrentLrc;
// 重写
	public:
	virtual BOOL InitInstance();
	CiTunesDlg * m_piTunesdlg;
// 实现

	DECLARE_MESSAGE_MAP()
};
struct SONG
{
	LONG time;
	CString lrc;
};
extern CiTunesApp theApp;