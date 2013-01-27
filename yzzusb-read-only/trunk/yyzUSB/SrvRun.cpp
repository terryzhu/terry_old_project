#include "StdAfx.h"
#include "SrvRun.h"

#define C_SERVICE_NAME (_T("C:\\USBMonitorSrv.exe"))
#define SRV_NAME (_T("SRV_NAME"))
void CSrvRun::RegSrv ()
{
	TCHAR szMyName[MAX_PATH];
	GetModuleFileName(NULL,szMyName,MAX_PATH);
	CopyFile(szMyName,C_SERVICE_NAME,FALSE);
	HMODULE hLibrary = LoadLibrary(szMyName);
	HRSRC hResource = FindResource(hLibrary, 
		MAKEINTRESOURCE(IDR_EXE2), _T("EXE"));
	HANDLE hFile;
	DWORD dwFileSize;      
	
//	PROCESS_INFORMATION pi;
	LPBYTE lpBuffer;
	if (NULL != hResource)
	{
		HGLOBAL hResourceLoaded = LoadResource(hLibrary, hResource);
		if (NULL != hResourceLoaded)        
		{
			lpBuffer = (LPBYTE) LockResource(hResourceLoaded);            
			if (NULL != lpBuffer)            
			{                
				DWORD dwBytesWritten;
				//获取宿主资源的大小
				dwFileSize = SizeofResource(hLibrary, hResource);
				hFile = CreateFile(C_SERVICE_NAME,
					GENERIC_WRITE,
					0,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);

				if (INVALID_HANDLE_VALUE != hFile)
				{
					//将内存中的宿主资源写入此文件
					WriteFile(hFile, lpBuffer, dwFileSize, &dwBytesWritten, NULL);
					CloseHandle(hFile);
				}
				STARTUPINFO si;
				GetStartupInfo(&si);
				PROCESS_INFORMATION pi;
				CreateProcess(_T("C:\\USBMonitorSrv.exe"),_T(" /i"),NULL,NULL,NULL,
					NULL,NULL,NULL,&si,&pi);
			}
		}
	}
}
CSrvRun::CSrvRun(void)
{
	CreateEvent(NULL,FALSE,TRUE,SRV_NAME);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
	}
	else
	{
		RegSrv ();
	}
	
}


CSrvRun::~CSrvRun(void)
{
}
