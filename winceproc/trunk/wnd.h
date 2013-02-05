#ifndef _WND
#define _WND
#include "type.h"




typedef void (*GDIFunc) (void);
typedef void (*GDIFunc16_16) (UINT16,UINT16);
typedef void (*GDIFunc16) (UINT16);
typedef void (*GDIFunc32_32) (UINT32,UINT32);
typedef void (*GDIFunc32_32_16) (UINT32,UINT32,COLOR);
typedef void (*GDIFunc32_32_8) (UINT32,UINT32,char);
typedef void (*GDIFunc32_32_32) (UINT32,UINT32,char*);
typedef void (*GDIFunc16_16_16_16) (UINT16,UINT16,UINT16,COLOR);

typedef struct _CDC 
{
	GDIFunc32_32 PrintAuthors;
	GDIFunc32_32 PrintMenu;
	GDIFunc LCD_Init;
	GDIFunc32_32_16 SetPixel;
	GDIFunc LCD_On;
	GDIFunc16 Fill_Screen;
	GDIFunc16_16 MoveTo;
	GDIFunc16_16 LineTo;
	GDIFunc32_32_8 DrawChar;
	GDIFunc32_32_32 DrawText;
	GDIFunc16  SetColor;
	GDIFunc16_16_16_16 DrawCircle;
}CDC;


CDC *GetDC();
void RegisterClass(WinProcFunc p);

bool GetMessage();
void TranslateMessage();
void DispatchMessage();










#endif
