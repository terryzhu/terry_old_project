#ifndef _INTERRUPT
#define _INTERRUPT
#include "type.h"
//---------------------------------------------------------------------------
#define     _ISR_STARTADDRESS        ((volatile unsigned *)0x33ffff00)
#define VICVectAddr _ISR_STARTADDRESS





#define    rADCDAT0 (*(volatile unsigned * )0x5800000C) 
#define    rADCDAT1 (*(volatile unsigned * )0x58000010) 




//--------------------------------------------------------------------------

#define    rINTOFFSET (*(volatile unsigned * )0x4A000014) 
#define    rTCFG0 (*(volatile unsigned * )0x51000000) 
#define    rTCFG1 (*(volatile unsigned * )0x51000004) 

#define    rTCNTB1 (*(volatile unsigned * )0x51000018) 
#define    rTCMPB1 (*(volatile unsigned * )0x5100001C) 

#define    rTCNTB4 (*(volatile unsigned * )0x5100003C) 
#define    rTCON (*(volatile unsigned * )0x51000008) 
#define    rINTMSK (*(volatile unsigned * )0X4A000008) 
#define    rSRCPND (*(volatile unsigned * )0X4A000000) 
#define    rINTPND (*(volatile unsigned * )0X4A000010) 


#define    rTCNTB1 (*(volatile unsigned * )0x51000018) 
#define    rTCMPB1 (*(volatile unsigned * )0x5100001C) 

#define BIT_TIMER4 (0x1<<14)
#define BIT_TIMER1 (0x1<<11)
//----------------------------------------------------------------------------
#define rADCDLY (*(volatile unsigned * )0x58000008) 
#define rADCTSC (*(volatile unsigned * )0x58000004) 
#define rADCCON (*(volatile unsigned * )0x58000000) 
#define rSUBSRCPND (*(volatile unsigned * )0X4A000018) 
#define rINTSUBMSK (*(volatile unsigned * )0X4A00001C) 

#define BIT_ADC (0x1<<31)
#define BIT_SUB_ADC                (0x1<<10)
#define BIT_SUB_TC                (0x1<<9)

//-----------------------------------------------------------------------------

#define rGPFCON (*(volatile unsigned *)0x56000050)
#define rGPECON (*(volatile unsigned *)0x56000040)
#define rEXTINT0 (*(volatile unsigned *)0x56000088)
#define rEINTPEND (*(volatile unsigned *)0x560000A8)
#define rEINTMASK (*(volatile unsigned *)0x560000A4)
#define rSPCON1 (*(volatile unsigned *)0x59000020)
#define rSPPRE1 (*(volatile unsigned *)0x5900002C)


#define BIT_EINT4_7 ((0x1<<4)/*|(0x1<<7)*/)


#define WM_TOUCH 31


//
//extern bool bHasMessage;
//extern MSG msg;
extern WinProcFunc pWinProcFunc;
#endif
