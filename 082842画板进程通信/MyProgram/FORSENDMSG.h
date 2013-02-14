#include "windows.h"
typedef  struct FORSENDMSG 
{
	LPARAM lParam;
	WPARAM wParam;
	UINT msg;
	HWND hWnd;
	void GetInfo(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
	{
		this->hWnd=hwnd;
		this->msg=msg;
		this->wParam=wp;
		this->lParam=lp;
	}
}*LPFORSENDMSG;