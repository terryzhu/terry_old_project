#pragma once
#include <list>

class CFileFinder
{
public:
	CFileFinder(CString strSearchPath, BOOL bRecursive = TRUE, int nLevel = 0);
	~CFileFinder(void);

	BOOL SetNameFilter(CString strSearchName);
	BOOL SetSizeFilter(ULONGLONG ulMinSize, ULONGLONG ulMaxSize);
	BOOL SetTypeFilter(DWORD dwFileType);

	BOOL IsSearching();
	BOOL StartSearching();

	BOOL HasNext();
	BOOL GetNextResult(CString& strResult);

private:
	void SearchOneLevel(CString strLevel);
	BOOL IsFilterMatching(LPWIN32_FIND_DATA findData);
	static int PathLevel(CString strPath);
	static BOOL IsDot(CString strPath);

	void PushReult(CString strResult);
	CString PopResult();

	static DWORD WINAPI SearchTrunk(LPVOID lpParameter);

private:
	CFileFinder(const CFileFinder&);
	CFileFinder& operator= (const CFileFinder&);

private:
	CString m_strBasePath;
	CString m_strSearchName;
	std::list<CString> m_lstSubDir;
	std::list<CString> m_lstResult;
	CRITICAL_SECTION m_csResult;
	ULONGLONG m_ulMinSize;
	ULONGLONG m_ulMaxSize;
	DWORD m_dwAttribute;
	BOOL m_bRecursive;
	int m_nLevel;
	HANDLE m_hSearch;
};

