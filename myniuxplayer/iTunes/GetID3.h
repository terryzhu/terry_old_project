// GetID3.h : GetID3 DLL 的主头文件
//

#pragma once

// CGetID3App
// 有关此类实现的信息，请参阅 GetID3.cpp
//

class AFX_CLASS_EXPORT CGetID3App : public CWinApp
{
public:
	CGetID3App();
	CString CharToCString(char* a);

// 重写
public:
	virtual BOOL InitInstance();
	virtual BOOL GetInfo(CString MP3Address,MP3_INFO &MP3Info);
	DECLARE_MESSAGE_MAP()
};

extern "C" AFX_API_EXPORT CGetID3App* GetClass( );