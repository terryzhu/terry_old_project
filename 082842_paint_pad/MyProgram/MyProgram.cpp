// MyProgram.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MyProgram.h"
#include "ShapeWnd.h"
TCHAR CShapeWnd::szWindowClass[]=L"";
int CShapeWnd::g_PenWidth=10;
HWND CShapeWnd::hwndTemp=NULL;
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;
	static CShapeWnd csw;
	csw.ShapeWndReg(hInstance);
	csw.CreatShapeWindow(hInstance);
	csw.OnCreateToolBar();
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROGRAM));
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
