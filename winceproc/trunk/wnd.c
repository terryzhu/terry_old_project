#include "wnd.h"
#include "gdi.h"
#include "interrupt.h"
static CDC dc;
static CDC * pDC=&dc;

void RegisterClass(WinProcFunc p)
{
	pDC->PrintAuthors=PrintAuthors;
	pDC->PrintMenu=PrintMenu;
	pDC->LCD_Init=LCD_Init;
	pDC->SetPixel=SetPixel;
	pDC->LCD_On=LCD_On;
	pDC->Fill_Screen=Fill_Screen;
	pDC->MoveTo=MoveTo;
	pDC->LineTo=LineTo;
	pDC->DrawText=DrawText;
	pDC->DrawChar=DrawChar;
	pDC->SetColor=SetColor;
	pDC->DrawCircle=DrawCircle;
	pWinProcFunc=p;
}
CDC *GetDC()
{
	return pDC;
}

bool GetMessage()
{
	return 1;
}

void TranslateMessage()
{

}
void DispatchMessage()
{

}
void SendMessage(UINT32 message,UINT32 lParam,UINT32 wParam)
{
	pWinProcFunc(message,lParam,wParam);
}
