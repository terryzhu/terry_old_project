#pragma once



// CLrcWndThread

class CLrcWndThread : public CWinThread
{
	DECLARE_DYNCREATE(CLrcWndThread)

public:
	CLrcWndThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLrcWndThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


