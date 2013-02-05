#pragma once
class CTimerManager
{
private:
	CTimerManager(void);
	~CTimerManager(void);
public:
	static int getDownloadTimer();
	static int getCloseWindowTimer();

	static int DownloadTimerEclapse();
	static int CloseWindowTimerEclapse();
};

