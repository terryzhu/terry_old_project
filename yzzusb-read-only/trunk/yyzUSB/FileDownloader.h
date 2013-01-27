#pragma once

#include <string>
#include <vector>

using namespace std;

// 这个类用于在后台下载其它文件
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

