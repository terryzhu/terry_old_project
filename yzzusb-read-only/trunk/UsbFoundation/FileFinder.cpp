#include "StdAfx.h"
#include "FileFinder.h"

CFileFinder::CFileFinder(CString strSearchPath, BOOL bRecursive /*= TRUE*/, int nLevel /*= 0*/)
	: m_strBasePath(strSearchPath)
	, m_strSearchName(_T("*.*"))
	, m_bRecursive(bRecursive)
	, m_nLevel(nLevel > 0 ? nLevel : 0)
	, m_ulMinSize(0)
	, m_ulMaxSize(0)
	, m_dwAttribute(0)
	, m_hSearch(NULL)
{
	::InitializeCriticalSection(&m_csResult);
	if (m_strBasePath.Right(1) != _T('\\'))
		m_strBasePath += _T("\\");
}

CFileFinder::~CFileFinder(void)
{
	::DeleteCriticalSection(&m_csResult);
	if (IsSearching())
	{
		::WaitForSingleObject(m_hSearch,INFINITE);
	}
}


BOOL CFileFinder::SetNameFilter(CString strSearchName)
{
	if (IsSearching()) return FALSE;
	if (strSearchName.IsEmpty()) return FALSE;
	if (strSearchName.Find(_T('\\')) != -1) return FALSE;

	m_strSearchName = strSearchName;
	return TRUE;
}

BOOL CFileFinder::SetSizeFilter(ULONGLONG ulMinSize, ULONGLONG ulMaxSize)
{
	if (IsSearching()) return FALSE;
	if (ulMinSize > ulMaxSize) return FALSE;

	m_ulMaxSize = ulMaxSize;
	m_ulMinSize = ulMinSize;
	return TRUE;
}

BOOL CFileFinder::SetTypeFilter(DWORD dwFileType)
{
	if (IsSearching()) return FALSE;

	m_dwAttribute = dwFileType;
	return FALSE;
}


BOOL CFileFinder::IsSearching()
{
	if (!m_hSearch) return FALSE;
	
	DWORD dwWait = ::WaitForSingleObject(m_hSearch,0);
	if (dwWait == WAIT_OBJECT_0)
	{
		m_hSearch = NULL;
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CFileFinder::HasNext()
{
	if (IsSearching()) return TRUE;
	if (!m_lstSubDir.empty()) return TRUE;
	
	BOOL bRet = FALSE;
	::EnterCriticalSection(&m_csResult);
	if (!m_lstResult.empty())
		bRet = TRUE;
	::LeaveCriticalSection(&m_csResult);

	return bRet;
}

BOOL CFileFinder::GetNextResult(CString& strResult)
{
	strResult = PopResult();
	return strResult != _T("");
}

BOOL CFileFinder::StartSearching()
{
	if (IsSearching()) return TRUE;
	
	m_hSearch = ::CreateThread(NULL,0,SearchTrunk,this,0,NULL);
	return m_hSearch ? TRUE : FALSE;
}

DWORD WINAPI CFileFinder::SearchTrunk(LPVOID lpParameter)
{
	CFileFinder* pThis = (CFileFinder*)lpParameter;
	pThis->SearchOneLevel(pThis->m_strBasePath);
	while (!pThis->m_lstSubDir.empty())
	{
		CString strSearch = pThis->m_lstSubDir.front();
		pThis->m_lstSubDir.pop_front();
		pThis->SearchOneLevel(strSearch);
	}

	return 0;
}


void CFileFinder::SearchOneLevel(CString strLevel)
{
	// Generate search filter
	CString strFindFilter = strLevel;
	if (strFindFilter.Right(1) != _T('\\'))
		strFindFilter += _T("\\");

	CString strFilter = strFindFilter;
	if (!m_bRecursive)
		strFilter += m_strSearchName;
	else
		strFilter += _T("*.*");

	BOOL bResult = TRUE;
	WIN32_FIND_DATA findData;
	::SetLastError(0);
	HANDLE hFileFinder = ::FindFirstFile(strFilter,&findData);
	if (hFileFinder == INVALID_HANDLE_VALUE) return;
	
	while (bResult && ::GetLastError() != ERROR_NO_MORE_FILES)
	{
		if (m_bRecursive)
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && !IsDot(findData.cFileName))
			{
				CString strFullPath = strFindFilter;
				strFullPath += findData.cFileName;
				strFullPath += _T("\\");
				if (m_nLevel)
				{
					if (PathLevel(strFullPath) - PathLevel(m_strBasePath) <= m_nLevel)
					{
						m_lstSubDir.push_back(strFullPath);
					}
				}
				else
				{
					m_lstSubDir.push_back(strFullPath);
				}
			}
		}
		else
		{
			if (IsFilterMatching(&findData))
			{
				CString strFullPath = strFindFilter;
				strFullPath += findData.cFileName;
				PushReult(strFullPath);
			}
		}

		bResult = ::FindNextFile(hFileFinder,&findData);
	}

	::FindClose(hFileFinder);


	// Second finding for recursive finding.
	if (m_bRecursive)
	{
		CString strSubFilter = strFindFilter;
		strSubFilter += m_strSearchName;
		bResult = TRUE;
		::SetLastError(0);
		hFileFinder = ::FindFirstFile(strSubFilter,&findData);
		if (hFileFinder == INVALID_HANDLE_VALUE) return;

		while (bResult && ::GetLastError() != ERROR_NO_MORE_FILES)
		{
			if (IsFilterMatching(&findData))
			{
				CString strFullPath = strFindFilter;
				strFullPath += findData.cFileName;
				PushReult(strFullPath);
			}
			bResult = ::FindNextFile(hFileFinder,&findData);
		}
	}

	::FindClose(hFileFinder);
}

BOOL CFileFinder::IsFilterMatching(LPWIN32_FIND_DATA findData)
{
	if (m_ulMinSize && m_ulMaxSize)
	{
		ULONGLONG ulTemp = findData->nFileSizeHigh;
		ULONGLONG ulSize = findData->nFileSizeLow;
		ulSize += ulTemp << 32;
		if (ulSize < m_ulMinSize || ulSize > m_ulMaxSize)
		{
			return FALSE;
		}
	}

	if (m_dwAttribute)
	{
		if (!(findData->dwFileAttributes & m_dwAttribute))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

int CFileFinder::PathLevel(CString strPath)
{
	int nRet = -1;
	int nPos = -1;
	do 
	{
		nPos = strPath.Find(_T('\\'),nPos + 1);
		if (nPos != -1) nRet++;
	} while (nPos != -1);

	return nRet;
}

BOOL CFileFinder::IsDot(CString strPath)
{
	CString strCompare = strPath;
	int nPos = strPath.ReverseFind(_T('\\'));
	if (nPos != -1)
		strCompare = strPath.Right(strPath.GetLength() - nPos);

	return (!strCompare.Compare(_T(".")) || !strCompare.Compare(_T("..")));
}

void CFileFinder::PushReult(CString strResult)
{
	::EnterCriticalSection(&m_csResult);
	m_lstResult.push_back(strResult);
	::LeaveCriticalSection(&m_csResult);
}

CString CFileFinder::PopResult()
{
	CString strResult = _T("");

	::EnterCriticalSection(&m_csResult);
	if (!m_lstResult.empty())
	{
		strResult = m_lstResult.front();
		m_lstResult.pop_front();
	}
	::LeaveCriticalSection(&m_csResult);
	return strResult;
}