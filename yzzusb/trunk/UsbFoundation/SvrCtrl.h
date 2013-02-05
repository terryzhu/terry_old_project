#pragma once
class CSvrCtrl
{
public:
	CSvrCtrl(void);
	CSvrCtrl(CString strSvrName);
	~CSvrCtrl(void);

	void SetSvrName(CString strSvrName);
	CString GetSvrName();

	BOOL IsSvrExist();
	BOOL IsSvrRunning();
	BOOL IsAutoRun();

	BOOL Start(int argc = 0, LPCTSTR* argv = NULL);
	BOOL Stop();
	BOOL Restart();
	BOOL MakeAutoRun();

	BOOL Install(CString strDisplayName, CString strDescription, BOOL bSelfInsatll = TRUE, CString strPath = _T(""));
	BOOL Uninstall();
	BOOL ReInstall(BOOL bSelfInsatll = TRUE, CString strPath = _T(""), 
		BOOL bUpdateInfo = FALSE, CString strDisplayName = _T(""), CString strDescription = _T(""));

private:
	CSvrCtrl(const CSvrCtrl&);
	CSvrCtrl& operator= (const CSvrCtrl&);

	void Reset();
	BOOL IsInitOk() {return m_hSCM != NULL;}
	BOOL IsInited() {return !m_strSvrName.IsEmpty();}

private:
	SC_HANDLE m_hSCM;
	SC_HANDLE m_hSvr;

	CString m_strSvrName;
};

