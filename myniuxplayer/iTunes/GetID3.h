// GetID3.h : GetID3 DLL ����ͷ�ļ�
//

#pragma once

// CGetID3App
// �йش���ʵ�ֵ���Ϣ������� GetID3.cpp
//

class AFX_CLASS_EXPORT CGetID3App : public CWinApp
{
public:
	CGetID3App();
	CString CharToCString(char* a);

// ��д
public:
	virtual BOOL InitInstance();
	virtual BOOL GetInfo(CString MP3Address,MP3_INFO &MP3Info);
	DECLARE_MESSAGE_MAP()
};

extern "C" AFX_API_EXPORT CGetID3App* GetClass( );