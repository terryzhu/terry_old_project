#ifndef GDI_H
#define GDI_H
   
#include "type.h"

#define rGPCCON    (*(volatile unsigned *)0x56000020)      //Port C control
#define rGPCDAT    (*(volatile unsigned *)0x56000024)       //Port C data
#define rGPCUP     (*(volatile unsigned *)0x56000028) //Pull-up control C

#define rGPDCON    (*(volatile unsigned *)0x56000030)      //Port D control
#define rGPDDAT    (*(volatile unsigned *)0x56000034)       //Port D data
#define rGPDUP     (*(volatile unsigned *)0x56000038)       //Pull-up control D

#define rGPGCON    (*(volatile unsigned *)0x56000060)      //Port G control
#define rGPGDAT    (*(volatile unsigned *)0x56000064)       //Port G data
#define rGPGUP     (*(volatile unsigned *)0x56000068)       //Pull-up control G


// LCD Register:
#define rLCDCON1 (*(volatile unsigned *)0X4D000000) //LCD control 1 register
#define rLCDCON2 (*(volatile unsigned *)0X4D000004)//LCD Control 2
#define rLCDCON3 (*(volatile unsigned *)0X4D000008)// LCD Control 3
#define rLCDCON4 (*(volatile unsigned *)0X4D00000C)// LCD Control 4
#define rLCDCON5 (*(volatile unsigned *)0X4D000010)//

#define rTPAL (*(volatile unsigned *)0x4d000050) 

#define rLCDSADDR1 (*(volatile unsigned *)0X4D000014) //STN/TFT: Frame buffer start address 1 register
#define rLCDSADDR2  (*(volatile unsigned *)0x4d000018)    //STN/TFT Frame buffer start address 2
#define rLCDSADDR3  (*(volatile unsigned *)0x4d00001c)    //STN/TFT Virtual screen address set
#define rLCDINTMSK  (*(volatile unsigned *)0x4d00005c)    //LCD Interrupt mask
#define rLPCSEL     (*(volatile unsigned *)0x4d000060)   //LPC3600 Control --- edited by junon

//TFT_SIZE
#define  GUI_LCM_XMAX  240     
#define  GUI_LCM_YMAX  320



#define M5D(n)                          ((n) & 0x1fffff)     // To get lower 21bits

#define LCD_XSIZE (240)
#define LCD_YSIZE (320)
#define SCR_XSIZE (LCD_XSIZE*2)
#define SCR_YSIZE (LCD_YSIZE*2)
#define HOZVAL (LCD_XSIZE-1)
#define LINEVAL (LCD_YSIZE-1)
#define CLKVAL (4)
#define MVAL (13)
#define LCD_MVAL (6)


#define LCD_VBPD ((2-1)&0xff)
#define LCD_VFPD ((3-1)&0xff)
#define LCD_VSPW ((2-1)&0x3f)
#define LCD_HBPD ((7-1)&0x7f)
#define LCD_HFPD ((3-1)&0xff)
#define LCD_HSPW ((4-1)&0xff)

#define LCD_VIDEO_ADDR 0x33800000

void PrintAuthors(UINT32 x,UINT32 y);
void PrintMenu(UINT32 x,UINT32 y);

void LCD_Init();
void SetPixel(UINT32 x,UINT32 y,COLOR c);
void LCD_On();
void Fill_Screen(UINT16 c);


//draw function
void MoveTo(UINT16 x,UINT16 y);
void LineTo(UINT16 x0,UINT16 y0);

void DrawCircle(UINT16 xc,UINT16 yc,UINT16 radius,COLOR c);

void DrawChar(UINT32 x,UINT32 y,char c);
void DrawText(UINT32 x,UINT32 y,char * str);
void SetColor(COLOR c);














#endif
