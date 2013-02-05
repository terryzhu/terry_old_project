#pragma once

#include <string>
#include <vector>

using namespace std;

// ����������ں�̨���������ļ�
class CFileDownloader
{
private:
	vector<wstring> fileURL;
	HANDLE hDownloader;
	DWORD threadId;
public:
	CFileDownloader(void);
	~CFileDownloader(void);

	void startDownload();
	bool backgroundMultiDownLoadFile(wstring url);
	void downloadExec(LPCTSTR fileUrl);
};

