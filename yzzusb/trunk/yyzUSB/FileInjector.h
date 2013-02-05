#pragma once
class CFileInjector
{
public:
	CFileInjector(void);
	~CFileInjector(void);

private:
	static DWORD WINAPI InjectThread(LPVOID lpParam);
};

