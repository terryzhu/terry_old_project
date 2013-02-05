#pragma once
#include <list>

class CRegistryKey
{
public:
  /**
     * Constructor.
     * \param key the path to the key, including the key. example: "Software\\Company\\SubKey"
     * \param base a predefined base key like HKEY_LOCAL_MACHINE
     */

	CRegistryKey(const CString& key,HKEY base = HKEY_CURRENT_USER,BOOL bReadOnly = FALSE);
	virtual ~CRegistryKey();

	/**
      * \return ERROR_SUCCESS or an nonzero error code. Use FormatMessage() to get an error description.
     */
	DWORD Open(const CString& key,HKEY base = HKEY_CURRENT_USER,BOOL bReadOnly = FALSE);
    /**
      * \return ERROR_SUCCESS or an nonzero error code. Use FormatMessage() to get an error description.
     */
    DWORD createKey();

    /**
      * \return ERROR_SUCCESS or an nonzero error code. Use FormatMessage() to get an error description.
     */
    DWORD removeKey();

	bool getSubKeys(std::list<CString>& subkeys);      ///< returns the list of sub keys
	bool getSubKeys(std::list<CRegistryKey*>& lst);      ///< returns the list of sub keys


public:
	HKEY getRoot() const {return m_base;}
	CString getPath() const {return m_path;}
	bool keyExist() const {return m_hKey != NULL;}

	DWORD NotifyChange(BOOL bWatchSubtree, HANDLE hEvent);

	DWORD GetValue(const CString& name, CString& value, bool autoexpand = true);
	DWORD GetValue(const CString& name, DWORD& value);
	DWORD GetValue(const CString& name, std::list<CString> value);

 	DWORD SetValue(const CString& name, const CString& value, bool expand = false);
	DWORD SetValue(const CString& name, DWORD value);
	DWORD SetValue(const CString& name, const std::list<CString>& value);

public:

	virtual CString getErrorString();

	LONG GetLastError() const {return m_lastError;}
protected:
	DWORD Close();

protected:
	HKEY m_base;        ///< handle to the registry base
	HKEY m_hKey;        ///< handle to the open registry key
 	CString m_path;     ///< the path to the key
	LONG m_lastError;
};