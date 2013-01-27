#pragma once
#include "windows.h"
#include "resource.h"
DWORD WINAPI DoubleProtectThread(LPVOID);
class CDaemon
{
public:
	static bool unDread;
	CDaemon(void);
	~CDaemon(void);
	void Init();
private:
	
};

