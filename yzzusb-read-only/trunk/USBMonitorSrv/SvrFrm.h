#pragma once
#include "Log.h"

// Template function used to start a new service
template <class T>
void SvrMain()
{
// #ifdef _DEBUG
// 	::DebugBreak();
// #endif // _DEBUG
	SVRLOG("Start Running Service...")
	T svr;
	CSvrFrm* p = dynamic_cast<CSvrFrm*>(&svr);
	if (!p) return;

	SERVICE_TABLE_ENTRY DispatchTable[] = 
	{ 
		{ (LPTSTR)svr.SvrName(), (LPSERVICE_MAIN_FUNCTION) T::ServiceMain }, 
		{ NULL, NULL } 
	}; 
	
	SVRLOG("Before StartServiceCtrlDispatcher...")
	::StartServiceCtrlDispatcher(DispatchTable);
	SVRLOG("After StartServiceCtrlDispatcher...")
}


class CSvrFrm
{
public:
	enum ExitCode
	{
		EC_NO_ERROR		= 0,
		EC_START_ERROR	= 1,
		EC_STOP_ERROR	= 2,
		EC_RUN_ERROR	= 4,
	};

	enum SupportEvent
	{
		SE_STOP			= 1,
		SE_PAUSERESUME	= 2,
		SE_PARAMCHANGE	= 4,
		SE_DEVICEEVENT	= 8,
		SE_CLEANUP		= 16,
	};

	enum DevChange
	{
		DC_INSERT	= 0,
		DC_REMOVE	= 1,
	};

public:
	CSvrFrm(void);
	virtual ~CSvrFrm(void);


public:
	// Check whether the service is asked to stop.
	// It must be used periodically when SvrWork is working in a infinite loop to check for stop request and jump out the loop.
	BOOL IsSvrToStop();

	// Control the service to stop.
	void SvrStop();

	// Check whether the service is asked to pause.
	// It must be used periodically when SvrWork is working in a infinite loop and support pause&resume to check for pause request and pause its working.
	BOOL IsSvrToPause();

	// Control the service to pause.
	void SvrPause();

	// Check whether the service is asked to resume.
	// It must be used in SvrWork when it has get pause request to wait for request to resume work.
	BOOL IsSvrToResume();

	// Control the service to resume.
	void SvrResume();

public:
	// Returning the name of the service.
	virtual LPCTSTR SvrName() = 0;

	// Set the event supported by the service.
	virtual DWORD SvrSupportEvent() = 0;
	virtual void SetDeviceFilter(LPVOID& lpFilter) {}

	// Doing initialization work in a separate thread.
	virtual BOOL SvrInit() = 0;

	// Doing free work in a separate thread.
	virtual void SvrFree() = 0;

	// Doing work that the service actually done.
	virtual BOOL SvrWork(DWORD dwArgc,LPTSTR* lpszArgv) = 0;

	// Handle device change event in a separate thread.
	virtual void DeviceEventHdler(DevChange dc, LPVOID lpData) {}

public:
	static VOID WINAPI ServiceMain(DWORD dwArgc,LPTSTR* lpszArgv);
	static DWORD WINAPI HandlerEx(DWORD dwControl,DWORD dwEventType,LPVOID lpEventData,LPVOID lpContext);

protected:
	void SetServiceEvent();
	BOOL StartService();
	void StopService(DWORD dwExitCode = EC_NO_ERROR);
	BOOL InitService();
	BOOL FreeService();
	BOOL PerformService(DWORD dwArgc,LPTSTR* lpszArgv);
	void PauseService();
	void ResumeService();
	

protected:
	static DWORD WINAPI InitServiceTrunk(LPVOID lpParameter);
	static DWORD WINAPI FreeServiceTrunk(LPVOID lpParameter);

	struct DEVICECHANGE_PARAM 
	{
		CSvrFrm* pClass;
		DevChange dc;
		LPVOID pData;
	};
	static DWORD WINAPI DeviceChangeTrunk(LPVOID lpParameter);


protected:
	SERVICE_STATUS_HANDLE m_hSvrStatus;
	SERVICE_STATUS m_sSvrStatus;
	volatile HANDLE m_hEvtStopRequest;
	volatile HANDLE m_hEvtPauseRequest;
	volatile HANDLE m_hEvtResumeRequest;
	volatile DWORD m_dwSvrEvents;
	volatile ULONG m_cThreadCount;
	HDEVNOTIFY m_hDevNotify;

private:
	static CSvrFrm* ptr;
};


