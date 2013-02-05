#pragma once



// CMovingLrcThread

class CMovingLrcThread : public CWinThread
{
	DECLARE_DYNCREATE(CMovingLrcThread)

public:
	CMovingLrcThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CMovingLrcThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


