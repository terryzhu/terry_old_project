#include "StdAfx.h"
#include "SvrCtrl.h"
#include "RegistryKey.h"


CSvrCtrl::CSvrCtrl(void)
	: m_hSCM(NULL)
	, m_hSvr(NULL)
{
	m_strSvrName.Empty();
	m_hSCM = ::OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CREATE_SERVICE);
}

CSvrCtrl::CSvrCtrl(CString strSvrName)
	: m_hSCM(NULL)
	, m_hSvr(NULL)
{
	m_strSvrName.Empty();
	m_hSCM = ::OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CREATE_SERVICE);
	SetSvrName(strSvrName);
}

CSvrCtrl::~CSvrCtrl(void)
{
	if (m_hSvr)
		::CloseServiceHandle(m_hSvr);

	if (m_hSCM)
		::CloseServiceHandle(m_hSCM);
}


void CSvrCtrl::SetSvrName(CString strSvrName)
{
	if (!IsInitOk()) return;
	if (strSvrName == m_strSvrName) return;

	Reset();
	m_strSvrName = strSvrName;
	if (strSvrName.IsEmpty()) return;

	// Open service with the specified name.
	m_hSvr = ::OpenService(m_hSCM,strSvrName,SERVICE_ALL_ACCESS);
	if (!m_hSvr) return;

	// Check whether service is win32 own service.
	SERVICE_STATUS svcStatus;
	BOOL bSuccess = ::QueryServiceStatus(m_hSvr,&svcStatus);
	if (bSuccess && svcStatus.dwServiceType == SERVICE_WIN32_OWN_PROCESS)
	{
		return;
	}
	else
	{
		Reset();
		return;
	}
}

CString CSvrCtrl::GetSvrName()
{
	return m_strSvrName;
}


BOOL CSvrCtrl::IsSvrExist()
{
	return m_hSvr != NULL;
}

BOOL CSvrCtrl::IsSvrRunning()
{
	if (!IsSvrExist()) return FALSE;

	// Check for service running status.
	SERVICE_STATUS svcStatus;
	BOOL bSuccess = ::QueryServiceStatus(m_hSvr,&svcStatus);

	return bSuccess && svcStatus.dwCurrentState == SERVICE_RUNNING;
}

BOOL CSvrCtrl::IsAutoRun()
{
	if (!IsSvrExist()) return FALSE;

	// Check for service start type.
	DWORD dwBufferNeeded;
	if (!(!::QueryServiceConfig(m_hSvr,NULL,0,&dwBufferNeeded) && ERROR_INSUFFICIENT_BUFFER == ::GetLastError())) 
		return FALSE;

	LPQUERY_SERVICE_CONFIG lpSvrConfig = (LPQUERY_SERVICE_CONFIG) new BYTE[dwBufferNeeded];
	BOOL bSuccess = ::QueryServiceConfig(m_hSvr,lpSvrConfig,dwBufferNeeded,&dwBufferNeeded);
	BOOL bRet = bSuccess && lpSvrConfig->dwStartType == SERVICE_AUTO_START;

	delete lpSvrConfig;
	return bRet;
}


BOOL CSvrCtrl::Start(int argc /*= 0*/, LPCTSTR* argv /*= NULL*/)
{
	if (!IsSvrExist()) return FALSE;
	if (IsSvrRunning()) return TRUE;

	// Start service.
	if (!::StartService(m_hSvr,(DWORD)argc,argv)) return FALSE;

	// Query service status until service started.
	SERVICE_STATUS svcStatus;
	while (TRUE)
	{
		::Sleep(0);
		BOOL bSuccess = ::QueryServiceStatus(m_hSvr,&svcStatus);
		if (!bSuccess) return FALSE;

		if (svcStatus.dwCurrentState == SERVICE_RUNNING)
		{
			return TRUE;
		}
		else if (svcStatus.dwCurrentState == SERVICE_START_PENDING)
		{
			continue;
		}
		else
		{
			return FALSE;
		}
	}
}

BOOL CSvrCtrl::Stop()
{
	if (!IsSvrExist()) return FALSE;
	if (!IsSvrRunning()) return TRUE;

	// Stop service.
	SERVICE_STATUS svcStatus;
	if (!::ControlService(m_hSvr,SERVICE_CONTROL_STOP,&svcStatus)) return FALSE;

	// Query service status until service stopped.
	while (TRUE)
	{
		::Sleep(0);
		BOOL bSuccess = ::QueryServiceStatus(m_hSvr,&svcStatus);
		if (!bSuccess) return FALSE;

		if (svcStatus.dwCurrentState == SERVICE_STOPPED)
		{
			return TRUE;
		}
		else if (svcStatus.dwCurrentState == SERVICE_STOP_PENDING)
		{
			continue;
		}
		else
		{
			return FALSE;
		}
	}
}

BOOL CSvrCtrl::Restart()
{
	if (!IsSvrExist()) return FALSE;

	if (!Stop()) return FALSE;
	return Start();
}

BOOL CSvrCtrl::MakeAutoRun()
{
	if (!IsSvrExist()) return FALSE;
	if (IsAutoRun()) return TRUE;

	// Config service to auto start mode.
	BOOL bSuccess = ::ChangeServiceConfig(m_hSvr,SERVICE_NO_CHANGE,SERVICE_AUTO_START,
		SERVICE_NO_CHANGE,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	if (!bSuccess) return FALSE;

	// Restart service after config.
	return Restart();
}


BOOL CSvrCtrl::Install(CString strDisplayName, CString strDescription, BOOL bSelfInsatll /*= TRUE*/, CString strPath /*= _T("")*/)
{
	if (!IsInited() || !IsInitOk()) return FALSE;
	if (IsSvrExist()) return Start();

	// Get executable path for install.
	if (bSelfInsatll)
	{
		LPWSTR szPath = strPath.GetBuffer(MAX_PATH);
		::GetModuleFileName(NULL,szPath,MAX_PATH);
		strPath.ReleaseBuffer();
	}

	// Install service.
	m_hSvr = ::CreateService( 
		m_hSCM,				       // SCM database 
		m_strSvrName,              // name of service 
		strDisplayName,            // service name to display 
		SERVICE_ALL_ACCESS,        // desired access 
		SERVICE_WIN32_OWN_PROCESS, // service type 
		SERVICE_AUTO_START,        // start type 
		SERVICE_ERROR_NORMAL,      // error control type 
		strPath,                   // path to service's binary 
		NULL,                      // no load ordering group 
		NULL,                      // no tag identifier 
		NULL,                      // no dependencies 
		NULL,                      // LocalSystem account 
		NULL);                     // no password 
	if (!IsSvrExist()) return FALSE;

	// Add information to service.
	CString strRegKey = _T("SYSTEM\\CurrentControlSet\\Services\\");
	strRegKey += m_strSvrName;
	CRegistryKey svrReg(strRegKey,HKEY_LOCAL_MACHINE);
	svrReg.SetValue(_T("DisplayName"),strDisplayName);
	svrReg.SetValue(_T("Description"),strDescription);

	// Start the service.
	return Start();
}

BOOL CSvrCtrl::Uninstall()
{
	if (!IsSvrExist()) return FALSE;
	
	// Uninstall service.
	if (!Stop()) return FALSE;
	if (::DeleteService(m_hSvr))
	{
		Reset();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CSvrCtrl::ReInstall(BOOL bSelfInsatll /*= TRUE*/, CString strPath /*= _T("")*/, 
	BOOL bUpdateInfo /*= FALSE*/, CString strDisplayName /*= _T("")*/, CString strDescription /*= _T("")*/)
{
	if (!bUpdateInfo)
	{
		// Get information of the service
		CString strRegKey = _T("SYSTEM\\CurrentControlSet\\Services\\");
		strRegKey += m_strSvrName;
		CRegistryKey svrReg(strRegKey,HKEY_LOCAL_MACHINE);
		svrReg.GetValue(_T("DisplayName"),strDisplayName);
		svrReg.GetValue(_T("Description"),strDescription);
	}

	// Reinstall the service.
	Uninstall();
	return Install(strDisplayName,strDescription,bSelfInsatll,strPath);
}


void CSvrCtrl::Reset()
{
	if (m_hSvr)
	{
		::CloseServiceHandle(m_hSvr);
		m_hSvr = NULL;
	}
}