#pragma once
#include "windows.h"

class CSingleton
{
public:
	CSingleton(TCHAR * singleName,BOOL isDebugMode=FALSE);
	~CSingleton(void);
private:
	HANDLE m_mutexOneInstance;
};

