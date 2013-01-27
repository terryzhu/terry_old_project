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

	// 设置可执行文件的路径
	void setExec(wstring path);

	// 为指定盘符创建Autorun文件
	int buildAutorun(TCHAR diskLabel);
protected:
	int buildAutorun(wstring path);
	wstring getAutorunContext();
};

