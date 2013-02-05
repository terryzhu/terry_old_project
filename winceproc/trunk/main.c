#include "afx.h"\

void CALLBACK WndProc(UINT32 message,UINT32 lParam,UINT32 wParam);
static UINT16 flag = 0;
static UINT16 tempX = 0;
static UINT16 tempY = 0;

int main(int argc,char ** argv)
{
	RegisterClass(WndProc);
	SendMessage(WM_CREATE,0,0);
	while (GetMessage())
	{
		TranslateMessage();
		DispatchMessage();
	}

	return 0;
}

void CALLBACK WndProc(UINT32 message,UINT32 lParam,UINT32 wParam)
{
	switch (message)
	{
	case WM_CREATE:
		{
			CDC *pDC=GetDC();
			pDC->LCD_Init();
			pDC->LCD_On();
			pDC->Fill_Screen(WHITE);
			pDC->SetPixel(20,40,BLACK);
			pDC->MoveTo(20,20);
			pDC->LineTo(220,20);
			pDC->MoveTo(20,20);
			pDC->LineTo(20,300);
			pDC->MoveTo(20,300);
			pDC->LineTo(220,300);
			pDC->MoveTo(220,20);
			pDC->LineTo(220,300);
			//pDC->PrintAuthors(1,2);
			pDC->PrintMenu(50,100);
			
			char b[100];
			IToA(b,154);
			PrintF("abc%d%def%d%s..",12,34,5,"nihao");
		}
		
		break;
	case WM_TIMER:
		PrintF("WM_TIMER\n");
		break;
	case WM_LBUTTONDOWN:

		if (flag == 1)
		{
			MoveTo(lParam,wParam);
		}
		if (flag == 2)
		{
			tempX = lParam;
			tempY = wParam;
		}

		//PrintF("WM_LBUTTONDOWN\n");
		//PrintF("x=%d   y=%d\n",lParam,wParam);
		break;
	case WM_LBUTTONUP:

		if (flag == 1)
		{
			LineTo(lParam,wParam);
		}
		if (flag == 2)
		{
			PrintF("%d,%d,%d\n",abs(wParam-tempY),tempX,tempY);
			if (abs(lParam - tempX) >= abs(wParam - tempY))
			{
				DrawCircle((abs(wParam-tempY)/2+tempX),(tempY+wParam)/2,(abs(wParam - tempY)/2),BLACK);
			}
			else
			{
				DrawCircle((tempX+lParam)/2,(abs(tempX-lParam)/2+tempY),(abs(lParam - tempX)/2),BLACK);
			}
		}

		if (lParam < 176 && lParam > 50 && wParam > 100 && wParam < 116)
		{	
			if(flag == 0)
			{
				flag = 1;
				Fill_Screen(WHITE);
			}
		}
		if (lParam < 200 && lParam > 40 && wParam > 173 && wParam < 183)
		{	
			if (flag == 0)
			{
				flag = 2;
				Fill_Screen(WHITE);
			}
		}
		//PrintF("WM_LBUTTONUP\n");
		//PrintF("x=%d   y=%d\n",lParam,wParam);
		break;
	}
}

