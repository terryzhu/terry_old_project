#ifndef TYPE_H
#define TYPE_H

typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned short COLOR;
typedef int bool;



#define TRUE 1
#define true 1
#define FALSE 0
#define false 0
#define NULL (void*)0

//it means this par is used to rceive the result
#define _ret_ 

typedef struct _POS 
{
	UINT16 x;
	UINT16 y;
}POS;
typedef struct _MSG 
{
	UINT32 message;
	UINT32 lParam;
	UINT32 wParam;
}MSG;

#define   BLACK          0x0000     
#define   NAVY          0x000F     
#define   DGREEN         0x03E0  
#define   DCYAN          0x03EF  
#define   MAROON         0x7800  
#define   PURPLE         0x780F  
#define   OLIVE          0x7BE0     
#define   LGRAY          0xC618    
#define   DGRAY          0x7BEF  
#define   BLUE          0x001F  
#define   GREEN          0x07E0  
#define   CYAN             0x07FF   
#define   RED          0xF800  
#define   MAGENTA         0xF81F  
#define   YELLOW         0xFFE0  
#define   WHITE          0xFFFF 

#define WM_TIMER 14

#define WM_LBUTTONDOWN 311
#define WM_LBUTTONUP 310
#define WM_CREATE 312

#define CALLBACK 
typedef void (*WinProcFunc)(UINT32 message,UINT32 lParam,UINT32 wParam);
#endif
