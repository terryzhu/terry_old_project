#pragma once



// CMovingLrcThread

class CMovingLrcThread : public CWinThread
{
	DECLARE_DYNCREATE(CMovingLrcThread)

public:
	CMovingLrcThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMovingLrcThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


