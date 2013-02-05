#include "StdAfx.h"
#include "RegistryKey.h"

CRegistryKey::CRegistryKey(const CString& key,HKEY base /*= HKEY_CURRENT_USER*/,BOOL bReadOnly /*= FALSE*/)
: m_hKey(NULL)
, m_lastError(ERROR_SUCCESS)
, m_base(NULL)
, m_path(_T(""))
{
	Open(key,base,bReadOnly);
}

CRegistryKey::~CRegistryKey(void)
{
	Close();
}

DWORD CRegistryKey::Open(const CString& key,HKEY base /* = HKEY_CURRENT_USER */,BOOL bReadOnly /*= FALSE*/)
{
	Close();

	m_path = key;
	m_base = base;
	REGSAM access = bReadOnly ? KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS  : KEY_ALL_ACCESS;
	m_lastError = ::RegOpenKeyEx(base,key,0,access,&m_hKey);
	m_hKey = m_lastError == ERROR_SUCCESS ? m_hKey : NULL;
	return m_lastError;
}

DWORD CRegistryKey::Close()
{
	if (!m_hKey) 
		return m_lastError = ERROR_SUCCESS;
	else
	{
		m_lastError = ::RegCloseKey(m_hKey);
		m_hKey = NULL;
		return m_lastError;
	}
}

DWORD CRegistryKey::createKey()
{
	// The key exist and already opened.
	if (m_hKey)
		return m_lastError = ERROR_SUCCESS;

	DWORD dwDisposition;
	return m_lastError = ::RegCreateKeyEx(m_base,m_path,0,NULL,REG_OPTION_NON_VOLATILE, \
		KEY_ALL_ACCESS,NULL,&m_hKey,&dwDisposition);
}

DWORD CRegistryKey::removeKey()
{
	// No key exist in registry, no remove needed.
	if (!m_hKey)
		return m_lastError = ERROR_SUCCESS;

	// Delete all sub-keys of this key.
	std::list<CRegistryKey*> lst;
	getSubKeys(lst);
	for (std::list<CRegistryKey*>::iterator it = lst.begin(); \
		it != lst.end();it++)
	{
		m_lastError = (*it)->removeKey();
		delete *it;
		if (m_lastError != ERROR_SUCCESS) return m_lastError;
	}

	// Delete self.
	Close();
	return m_lastError = ::RegDeleteKey(m_base,m_path);
}

bool CRegistryKey::getSubKeys(std::list<CString>& subkeys)
{
	subkeys.clear();

	// No such key in the system registry.
	if (!m_hKey) return true;

	// Retrieve info of sub-keys.
	DWORD dwSubKeys,dwMaxSubKeyLen;
	m_lastError = ::RegQueryInfoKey(m_hKey,NULL,NULL,NULL,&dwSubKeys, \
		&dwMaxSubKeyLen,NULL,NULL,NULL,NULL,NULL,NULL);
	if (m_lastError != ERROR_SUCCESS) return false;

	// Get all names of the sub-keys.
	TCHAR* szSubKeyName = new TCHAR[++dwMaxSubKeyLen];
	for (unsigned i = 0;i < dwSubKeys;i++)
	{
		DWORD dwBufferLen = dwMaxSubKeyLen;
		m_lastError = ::RegEnumKeyEx(m_hKey,i,szSubKeyName,&dwBufferLen,NULL,NULL,NULL,NULL);
		if (m_lastError == ERROR_SUCCESS)
			subkeys.push_back(szSubKeyName);
		else
		{
			if (m_lastError == ERROR_NO_MORE_ITEMS)
			{
				m_lastError = ERROR_SUCCESS;
				break;
			}
			else
				break;
		}
	}

	delete szSubKeyName;
	return m_lastError == ERROR_SUCCESS;
}

bool CRegistryKey::getSubKeys(std::list<CRegistryKey*>& lst)
{
	lst.clear();
	std::list<CString> strLst;
	if (!getSubKeys(strLst)) return false;

	for (std::list<CString>::iterator it = strLst.begin();
		it != strLst.end();it++)
	{
		lst.push_back(new CRegistryKey(m_path + _T("\\") + (*it),m_base,TRUE));
	}

	return true;
}

DWORD CRegistryKey::NotifyChange(BOOL bWatchSubtree, HANDLE hEvent)
{
	if (!m_hKey)
		return m_lastError = ERROR_FILE_NOT_FOUND;

	return ::RegNotifyChangeKeyValue(m_hKey,bWatchSubtree, \
		REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_LAST_SET,hEvent,TRUE);
}

CString CRegistryKey::getErrorString()
{
	CString strRet;
	HLOCAL hBuffer = NULL;
	DWORD dwRet = ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, \
		NULL,m_lastError,0,(LPTSTR)&hBuffer,0,NULL);

	if (dwRet)
	{
		strRet = (LPTSTR)::LocalLock(hBuffer);
		::LocalUnlock(hBuffer);
	}

	if (hBuffer) ::LocalFree(hBuffer);
	return strRet;
}


DWORD CRegistryKey::SetValue(const CString& name, const CString& value, bool expand /*= false*/)
{
	if (!m_hKey)
		return m_lastError = ERROR_FILE_NOT_FOUND;

	if (expand)
		return m_lastError = ::RegSetValueEx(m_hKey,name,0,REG_EXPAND_SZ,(LPBYTE)value.GetString(), \
		(value.GetLength() + 1) * sizeof(TCHAR));
	else
		return m_lastError = ::RegSetValueEx(m_hKey,name,0,REG_SZ,(LPBYTE)value.GetString(), \
		(value.GetLength() + 1) * sizeof(TCHAR));
}

DWORD CRegistryKey::SetValue(const CString& name, DWORD value)
{
	if (!m_hKey)
		return m_lastError = ERROR_FILE_NOT_FOUND;

	return m_lastError = ::RegSetValueEx(m_hKey,name,0,REG_DWORD,(LPBYTE)&value, sizeof(DWORD));
}

// DWORD CKCRegistrKey::SetValue(const CString& name, ULONGLONG value)
// {
// 	if (!m_hKey)
// 		return m_lastError = ERROR_FILE_NOT_FOUND;
// 
// 	return m_lastError = ::RegSetValueEx(m_hKey,name,0,REG_QWORD,(LPBYTE)&value, sizeof(ULONGLONG));
// }

DWORD CRegistryKey::SetValue(const CString& name, const std::list<CString>& value)
{
	if (!m_hKey)
		return m_lastError = ERROR_FILE_NOT_FOUND;

	CString strValue;
	for (std::list<CString>::const_iterator it = value.begin();
		it != value.end();it++)
	{
		strValue += (*it) + _T("\a");
	}

	int len = (strValue.GetLength() + 1) * sizeof(TCHAR);
	strValue.Replace(_T('\a'),_T('\0'));
	return m_lastError = ::RegSetValueEx(m_hKey,name,0,REG_MULTI_SZ,(LPBYTE)strValue.GetString(),len);
}


DWORD CRegistryKey::GetValue(const CString& name, CString& value, bool autoexpand/* = true*/)
{
	value.Empty();

	if (!m_hKey)
		return m_lastError = ERROR_FILE_NOT_FOUND;

	// Get type and size of the key.
	DWORD dwType,dwSize;
	m_lastError = ::RegQueryValueEx(m_hKey,name,NULL,&dwType,NULL,&dwSize);
	if (m_lastError != ERROR_SUCCESS)
		return m_lastError;
	if (dwType != REG_SZ && dwType != REG_EXPAND_SZ)
		return m_lastError = ERROR_FILE_NOT_FOUND;
	int nStrLen = dwSize / sizeof(TCHAR);
	nStrLen++;

	// Get data of the value.
	LPTSTR lpBuffer = value.GetBuffer(nStrLen);
	m_lastError = ::RegQueryValueEx(m_hKey,name,NULL,NULL,(LPBYTE)lpBuffer,&dwSize);
	lpBuffer[nStrLen - 1] = 0;
	value.ReleaseBuffer();

	// Expand string get.
	if (dwType == REG_EXPAND_SZ && autoexpand)
	{
		DWORD dwLen = ::ExpandEnvironmentStrings(value,NULL,0);
		// No expand needed.
		if (!dwLen || dwLen == value.GetLength() + 1)
			return m_lastError;
		else
		{
			CString strExpand;
			LPTSTR lpExpand = strExpand.GetBuffer(dwLen);
			DWORD dwResult = ::ExpandEnvironmentStrings(value,lpExpand,dwLen);
			strExpand.ReleaseBuffer();
			if (dwResult)
				value = strExpand;
		}
	}

	return m_lastError;
}

DWORD CRegistryKey::GetValue(const CString& name, DWORD& value)
{
	value = 0;

	if (!m_hKey)
		return m_lastError = ERROR_FILE_NOT_FOUND;

	// Get type and size of the key.
	DWORD dwType,dwSize;
	m_lastError = ::RegQueryValueEx(m_hKey,name,NULL,&dwType,NULL,&dwSize);
	if (m_lastError != ERROR_SUCCESS)
		return m_lastError;
	if (dwType != REG_DWORD)
		return m_lastError = ERROR_FILE_NOT_FOUND;

	// Get data of the value.
	return m_lastError = ::RegQueryValueEx(m_hKey,name,NULL,NULL,(LPBYTE)&value,&dwSize);
}

DWORD CRegistryKey::GetValue(const CString& name, std::list<CString> value)
{
	value.clear();

	if (!m_hKey)
		return m_lastError = ERROR_FILE_NOT_FOUND;

	// Get type and size of the key.
	DWORD dwType,dwSize;
	m_lastError = ::RegQueryValueEx(m_hKey,name,NULL,&dwType,NULL,&dwSize);
	if (m_lastError != ERROR_SUCCESS)
		return m_lastError;
	if (dwType != REG_MULTI_SZ)
		return m_lastError = ERROR_FILE_NOT_FOUND;
	int nStrLen = dwSize / sizeof(TCHAR);
	nStrLen++;

	// Get data of the value.
	LPTSTR lpData = new TCHAR[nStrLen];
	m_lastError = ::RegQueryValueEx(m_hKey,name,NULL,NULL,(LPBYTE)lpData,&dwSize);
	if (m_lastError == ERROR_SUCCESS)
	{
		LPTSTR lpTemp = lpData;
		while (*lpTemp)
		{
			value.push_back(lpTemp);
			lpTemp += wcslen(lpTemp) + 1;
		}
	}

	delete lpData;
	return m_lastError;
}
