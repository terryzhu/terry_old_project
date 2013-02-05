#pragma once



// CLrcWndThread

class CLrcWndThread : public CWinThread
{
	DECLARE_DYNCREATE(CLrcWndThread)

public:
	CLrcWndThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CLrcWndThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


