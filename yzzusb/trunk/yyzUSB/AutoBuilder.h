#pragma once

#include <string>

using namespace std;

class CAutoBuilder
{
private:
	wstring m_strExecPath;
public:
	CAutoBuilder(void);
	~CAutoBuilder(void);

	// ���ÿ�ִ���ļ���·��
	void setExec(wstring path);

	// Ϊָ���̷�����Autorun�ļ�
	int buildAutorun(TCHAR diskLabel);
protected:
	int buildAutorun(wstring path);
	wstring getAutorunContext();
};

