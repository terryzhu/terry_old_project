#include "StdAfx.h"
#include "Singleton.h"


CSingleton::CSingleton(TCHAR * singleName,BOOL isDebugMode)
{
	m_mutexOneInstance = CreateMutex(NULL,TRUE,singleName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if (isDebugMode == TRUE)
		{
			MessageBox(NULL,_T("One Instance Exists"),_T("Error"),MB_OK);
		}
		ExitProcess (0);
	}

}


CSingleton::~CSingleton(void)
{
	if (m_mutexOneInstance != NULL)
	{
		ReleaseMutex (m_mutexOneInstance);
	}
}
