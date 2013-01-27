#include "stdafx.h"
#include "SvrFrm.h"

CLog::CLogHeader CLog::s_logHeader;
CSvrFrm* CSvrFrm::ptr = NULL;

CSvrFrm::CSvrFrm(void)
	: m_hSvrStatus(NULL)
	, m_hEvtStopRequest(NULL)
	, m_hEvtPauseRequest(NULL)
	, m_hEvtResumeRequest(NULL)
	, m_dwSvrEvents(0)
	, m_cThreadCount(0)
	, m_hDevNotify(NULL)
{
	m_sSvrStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ptr = this;
}

CSvrFrm::~CSvrFrm(void)
{
}


void CSvrFrm::SetServiceEvent()
{
	SVRLOG("In SetServiceEvent")
		DWORD dwEvents = SvrSupportEvent();

	if (dwEvents & SE_CLEANUP)
		m_dwSvrEvents |= SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PRESHUTDOWN;
	if (dwEvents & SE_STOP)
		m_dwSvrEvents |= SERVICE_ACCEPT_STOP;
	if (dwEvents & SE_PAUSERESUME)
		m_dwSvrEvents |= SERVICE_ACCEPT_PAUSE_CONTINUE;
	if (dwEvents & SE_PARAMCHANGE)
		m_dwSvrEvents |= SERVICE_ACCEPT_PARAMCHANGE;
	if (dwEvents & SE_DEVICEEVENT)
	{
		LPVOID lpFilter = NULL;
		SVRLOG("Call SetDeviceFilter")
			SetDeviceFilter(lpFilter);
		if (lpFilter)
		{
			m_hDevNotify = ::RegisterDeviceNotification(m_hSvrStatus,lpFilter,DEVICE_NOTIFY_SERVICE_HANDLE);
			SVRLOG("Call RegisterDeviceNotification")
		}
	}
}

BOOL CSvrFrm::StartService()
{
	SVRLOG("In StartService")
		if (InitService())
		{
			// Set SERVICE_STATUS member to indicate service is running.
			m_sSvrStatus.dwCurrentState = SERVICE_RUNNING;
			m_sSvrStatus.dwControlsAccepted = m_dwSvrEvents;
			m_sSvrStatus.dwCheckPoint = 0;
			m_sSvrStatus.dwWin32ExitCode = NO_ERROR;
			m_sSvrStatus.dwServiceSpecificExitCode = 0;
			m_sSvrStatus.dwWaitHint = 0;
			::SetServiceStatus(m_hSvrStatus,&m_sSvrStatus);

			SVRLOG("Service Running")
				return TRUE;
		}
		else
		{
			// Error occurred, just stop service.
			SVRLOG("Init Failed")
				StopService(EC_START_ERROR);
			return FALSE;
		}
}

DWORD WINAPI CSvrFrm::InitServiceTrunk(LPVOID lpParameter)
{
	CSvrFrm* pSvr = (CSvrFrm*)lpParameter;
	SVRLOG("Before SvrInit...")
		DWORD dwRet = pSvr->SvrInit() ? 0 : 1;
	SVRLOG("After SvrInit...")
		return dwRet;
}

BOOL CSvrFrm::InitService()
{
	SVRLOG("In InitService")

		// Create EVENT used to get stop request.
		m_hEvtStopRequest = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	if (!m_hEvtStopRequest)
		return FALSE;

	// Create EVENT used to get pause resume request.
	if (m_dwSvrEvents & SERVICE_ACCEPT_PAUSE_CONTINUE)
	{
		m_hEvtPauseRequest = ::CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hEvtResumeRequest = ::CreateEvent(NULL,FALSE,FALSE,NULL);
		if (!m_hEvtStopRequest || !m_hEvtResumeRequest)
			return FALSE;
	}

	// Set SERVICE_STATUS member to indicate service is starting.
	m_sSvrStatus.dwCurrentState = SERVICE_START_PENDING;
	m_sSvrStatus.dwControlsAccepted = 0;
	m_sSvrStatus.dwCheckPoint = 1;
	m_sSvrStatus.dwWin32ExitCode = NO_ERROR;
	m_sSvrStatus.dwServiceSpecificExitCode = 0;
	m_sSvrStatus.dwWaitHint = 2000;

	// Start service initialization thread.
	SVRLOG("Start Init Thread...")
		HANDLE hInitThread = ::CreateThread(NULL,0,InitServiceTrunk,this,0,NULL);
	if (!hInitThread)
		return FALSE;

	// Setting service to SERVICE_START_PENDING until service initialization finished.
	while (TRUE)
	{
		::SetServiceStatus(m_hSvrStatus,&m_sSvrStatus);
		SVRLOG("Service Start Pending")
			DWORD dwFlag = ::WaitForSingleObject(hInitThread,1500);
		if (WAIT_TIMEOUT == dwFlag)
		{
			m_sSvrStatus.dwCheckPoint++;
		}
		else if (WAIT_OBJECT_0 == dwFlag)
		{
			break;
		}
		else
		{
			return FALSE;
		}
	}

	SVRLOG("End Init Thread...")
		DWORD dwThreadExitCode = 1;
	::GetExitCodeThread(hInitThread,&dwThreadExitCode);
	return dwThreadExitCode ? FALSE : TRUE;
}


void CSvrFrm::StopService(DWORD dwExitCode /*= EC_NO_ERROR*/)
{
	SVRLOG("In Stop Service")
		if (!FreeService())
			dwExitCode |= EC_STOP_ERROR;

	// Set SERVICE_STATUS member to indicate service is stopped.
	m_sSvrStatus.dwCurrentState = SERVICE_STOPPED;
	m_sSvrStatus.dwControlsAccepted = 0;
	m_sSvrStatus.dwCheckPoint = 0;
	m_sSvrStatus.dwWaitHint = 0;
	if (dwExitCode == EC_NO_ERROR)
	{
		m_sSvrStatus.dwWin32ExitCode = NO_ERROR;
		m_sSvrStatus.dwServiceSpecificExitCode = 0;
	}
	else
	{
		m_sSvrStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
		m_sSvrStatus.dwServiceSpecificExitCode = dwExitCode;
	}

	::SetServiceStatus(m_hSvrStatus,&m_sSvrStatus);
	SVRLOG("Service Stopped")
}

BOOL CSvrFrm::IsSvrToStop()
{
	if (!m_hEvtStopRequest) return FALSE;
	return WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtStopRequest,0);
}

void CSvrFrm::SvrStop()
{
	SVRLOG("Called SvrStop")
		if (IsSvrToStop())
			::SetEvent(m_hEvtStopRequest);
}

DWORD WINAPI CSvrFrm::FreeServiceTrunk(LPVOID lpParameter)
{	
	CSvrFrm* pSvr = (CSvrFrm*)lpParameter;

	// Wait for all handler thread to terminal.
	SVRLOG("Wait for threads to exit")
		while (pSvr->m_cThreadCount)
			::Sleep(0);

	SVRLOG("Before SvrFree...")
		pSvr->SvrFree();
	SVRLOG("After SvrFree...")
		return 0;
}

BOOL CSvrFrm::FreeService()
{
	SVRLOG("In Free Service")

		// Close EVENTs.
		if (m_hEvtStopRequest)
			::CloseHandle(m_hEvtStopRequest);
	if (m_hEvtPauseRequest)
		::CloseHandle(m_hEvtPauseRequest);
	if (m_hEvtResumeRequest)
		::CloseHandle(m_hEvtResumeRequest);

	// Close device notify handle.
	if (m_hDevNotify)
		::UnregisterDeviceNotification(m_hDevNotify);

	// Set SERVICE_STATUS member to indicate service is stopping.
	m_sSvrStatus.dwCurrentState = SERVICE_STOP_PENDING;
	m_sSvrStatus.dwControlsAccepted = 0;
	m_sSvrStatus.dwCheckPoint = 1;
	m_sSvrStatus.dwWin32ExitCode = NO_ERROR;
	m_sSvrStatus.dwServiceSpecificExitCode = 0;
	m_sSvrStatus.dwWaitHint = 2000;

	// Start service free thread.
	SVRLOG("Start Free Thread...")
		HANDLE hFreeThread = ::CreateThread(NULL,0,FreeServiceTrunk,this,0,NULL);
	if (!hFreeThread) return FALSE;

	// Setting service to SERVICE_START_PENDING until service initialization finished.
	while (TRUE)
	{
		::SetServiceStatus(m_hSvrStatus,&m_sSvrStatus);
		SVRLOG("Service Stop Pending")
			DWORD dwFlag = ::WaitForSingleObject(hFreeThread,1500);
		if (WAIT_TIMEOUT == dwFlag)
		{
			m_sSvrStatus.dwCheckPoint++;
		}
		else if (WAIT_OBJECT_0 == dwFlag)
		{
			SVRLOG("End Free Thread...")
				return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}


BOOL CSvrFrm::PerformService(DWORD dwArgc,LPTSTR* lpszArgv)
{
	SVRLOG("Before Perform Service...")
		if (SvrWork(dwArgc,lpszArgv))
		{
			SVRLOG("After Perform Service...")
				return TRUE;
		}
		else
		{
			SVRLOG("Error in Perform Serevice")
				StopService(EC_RUN_ERROR);
			return FALSE;
		}
}


void CSvrFrm::PauseService()
{
	// Set SERVICE_STATUS member to indicate service is paused.
	m_sSvrStatus.dwCurrentState = SERVICE_PAUSED;
	m_sSvrStatus.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PRESHUTDOWN;
	m_sSvrStatus.dwControlsAccepted |= (m_dwSvrEvents & SERVICE_ACCEPT_STOP) ? SERVICE_ACCEPT_STOP : 0;
	m_sSvrStatus.dwControlsAccepted |= (m_dwSvrEvents & SERVICE_ACCEPT_SHUTDOWN) ? SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PRESHUTDOWN : 0;
	m_sSvrStatus.dwCheckPoint = 0;
	m_sSvrStatus.dwWin32ExitCode = NO_ERROR;
	m_sSvrStatus.dwServiceSpecificExitCode = 0;
	m_sSvrStatus.dwWaitHint = 0;
	::SetServiceStatus(m_hSvrStatus,&m_sSvrStatus);
	SVRLOG("Service Paused")
}

BOOL CSvrFrm::IsSvrToPause()
{
	if (!m_hEvtPauseRequest) return FALSE;
	return WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtPauseRequest,0);
}

void CSvrFrm::SvrPause()
{
	SVRLOG("Called SvrPause")
		if (m_hEvtPauseRequest)
			::SetEvent(m_hEvtPauseRequest);
}

void CSvrFrm::ResumeService()
{
	// Set SERVICE_STATUS member to indicate service is paused.
	m_sSvrStatus.dwCurrentState = SERVICE_RUNNING;
	m_sSvrStatus.dwControlsAccepted = m_dwSvrEvents;
	m_sSvrStatus.dwCheckPoint = 0;
	m_sSvrStatus.dwWin32ExitCode = NO_ERROR;
	m_sSvrStatus.dwServiceSpecificExitCode = 0;
	m_sSvrStatus.dwWaitHint = 0;
	::SetServiceStatus(m_hSvrStatus,&m_sSvrStatus);
	SVRLOG("Service Resumed")
}

BOOL CSvrFrm::IsSvrToResume()
{
	if (!m_hEvtResumeRequest) return FALSE;
	return WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtResumeRequest,0);
}

void CSvrFrm::SvrResume()
{
	SVRLOG("Called SvrResume")
		if (m_hEvtResumeRequest)
			::SetEvent(m_hEvtResumeRequest);
}


VOID WINAPI CSvrFrm::ServiceMain(DWORD dwArgc,LPTSTR* lpszArgv)
{
	SVRLOG("In ServiceMain")

		// Set service event handler.
		SVRLOG("Call RegisterServiceCtrlHandlerEx")
		ptr->m_hSvrStatus = ::RegisterServiceCtrlHandlerEx(ptr->SvrName(),HandlerEx,NULL);
	if (!ptr->m_hSvrStatus) return;

	// Do service initialization.
	SVRLOG("Call StartService")
	ptr->SetServiceEvent();
	if (!ptr->StartService()) return;

	// Start service working.
	SVRLOG("Call PerformService")
		if (!ptr->PerformService(dwArgc,lpszArgv)) return;

	// Wait for stop of the service.
	SVRLOG("Wait for Stop Request")
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(ptr->m_hEvtStopRequest,INFINITE))
			ptr->StopService();
		else
			ptr->StopService(EC_RUN_ERROR);
}


DWORD WINAPI CSvrFrm::HandlerEx(DWORD dwControl,DWORD dwEventType,LPVOID lpEventData,LPVOID lpContext)
{
	SVRLOG("In HandlerEx")
		switch (dwControl)
	{
		case SERVICE_CONTROL_INTERROGATE:
			return NO_ERROR;

		case SERVICE_CONTROL_STOP:
			SVRLOG("Control Stop")
				ptr->SvrStop();
			return NO_ERROR;

		case SERVICE_CONTROL_PRESHUTDOWN:
		case SERVICE_CONTROL_SHUTDOWN:
			SVRLOG("Control Shutdown")
				ptr->SvrStop();
			return NO_ERROR;

		case SERVICE_CONTROL_PAUSE:
			{
				if (ptr->m_dwSvrEvents & SERVICE_ACCEPT_PAUSE_CONTINUE)
				{
					SVRLOG("Control Pause")
						ptr->SvrPause();
					return NO_ERROR;
				}
				else
					return ERROR_CALL_NOT_IMPLEMENTED;
			}

		case SERVICE_CONTROL_CONTINUE:
			{
				if (ptr->m_dwSvrEvents & SERVICE_ACCEPT_PAUSE_CONTINUE)
				{
					SVRLOG("Control Resume")
						ptr->SvrResume();
					return NO_ERROR;
				}
				else
					return ERROR_CALL_NOT_IMPLEMENTED;
			}

		case SERVICE_CONTROL_PARAMCHANGE:
			return ERROR_CALL_NOT_IMPLEMENTED;

		case SERVICE_CONTROL_DEVICEEVENT:
			if (dwEventType == DBT_DEVICEARRIVAL || dwEventType == DBT_DEVICEREMOVECOMPLETE)
			{
				SVRLOG("Control Device Event")
					DEVICECHANGE_PARAM* param = new DEVICECHANGE_PARAM;
				param->dc = dwEventType == DBT_DEVICEARRIVAL ? DC_INSERT : DC_REMOVE;
				param->pData = lpEventData;
				param->pClass = ptr;
				::CreateThread(NULL,0,DeviceChangeTrunk,param,0,NULL);
			}
			return NO_ERROR;
	}

	return ERROR_CALL_NOT_IMPLEMENTED;
}


DWORD WINAPI CSvrFrm::DeviceChangeTrunk(LPVOID lpParameter)
{
	DEVICECHANGE_PARAM* param = (DEVICECHANGE_PARAM*)lpParameter;
	::InterlockedIncrement(&param->pClass->m_cThreadCount);
	SVRLOG("Before DeviceEventHdler")
		param->pClass->DeviceEventHdler(param->dc,param->pData);
	SVRLOG("After DeviceEventHdler")
		delete param;
	::InterlockedDecrement(&param->pClass->m_cThreadCount);
	return 0;
}