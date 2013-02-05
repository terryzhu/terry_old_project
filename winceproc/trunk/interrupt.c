#include "interrupt.h"

WinProcFunc pWinProcFunc;
#define TSP_MINX	 53
#define TSP_MINY	 67
#define TSP_MAXX	 920
#define TSP_MAXY	 933
#define TSP_LCDX     240
#define TSP_LCDY     320


void InitTimer4()
{

	rTCFG0=rTCFG0&~(0xffffff)|0x00ff0f;
	rTCFG1=rTCFG1&~(0xffffff)|0x030000;//1/16
	rTCNTB4=11718;//查阅资料得知pclk为12MHz,所以12000000/4/256=11718
	rTCON=rTCON&~ (0xffffff)|0x500000;
	rSRCPND|=BIT_TIMER4;
	rINTMSK&=~BIT_TIMER4;

	PrintF("InitTimer4 finished\n");

}
void InitTouch()
{
	if (rSRCPND&BIT_ADC)
	{
		rSRCPND|=BIT_ADC;
	}
	if (rINTPND&BIT_ADC)
	{
		rINTPND|=BIT_ADC;
	}
	if (rSUBSRCPND&BIT_SUB_ADC)
	{
		rSUBSRCPND|=BIT_SUB_ADC;
	}
	if (rSUBSRCPND&BIT_SUB_TC)
	{
		rSUBSRCPND|=BIT_SUB_TC;
	}
	rADCDLY=0x5000;
	rADCTSC=(0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
	rADCCON=(1<<14)|(49<<6)|(0<<3)|(0<<2)|(0<<1)|(0);
	rSUBSRCPND|=BIT_SUB_TC;
	rINTMSK&=~(BIT_ADC);
	rINTSUBMSK&=~(BIT_SUB_TC);
	PrintF("InitTouch finished\n");
}
#define ONEBIT (0x1)


void InitIRQ()
{
	InitTimer4();
	InitTouch();
}


void TSP_TransXY(int *px, int *py)
{

	*px = (*px >= TSP_MAXX) ? (TSP_MAXX) : *px;
	*py = (*py >= TSP_MAXY) ? (TSP_MAXY) : *py;
	*px = (*px - TSP_MINX);
	*py = (*py - TSP_MINY);
	*px = (*px >= 0) ? *px : 0;
	*py = (*py >= 0) ? *py : 0;
	*px = *px * TSP_LCDY / (TSP_MAXX - TSP_MINX);
	*py = *py * TSP_LCDX / (TSP_MAXY - TSP_MINY);
	*px = (*px >= TSP_LCDY)? (TSP_LCDY - 1) : *px;
	*py = (*py >= TSP_LCDX)? (TSP_LCDX - 1) : *py;
	*px = TSP_LCDY - *px - 1;
	*py = TSP_LCDX - *py - 1;
}
#define TSP_SAMPLE_NUM 4

void GetXY(int* px,int* py)
{
	int i;
	int xsum, ysum;
	int x, y;
	int dx, dy;
	xsum = ysum = 0;
	for (i = 0; i < TSP_SAMPLE_NUM; i++)
	{
		rADCTSC =   (0      <<  8) |        /* UD_Sen*/
			(1      <<  7) |        /* YMON  1 (YM = GND)*/
			(1      <<  6) |        /* nYPON 1 (YP Connected AIN[n])*/
			(0      <<  5) |        /* XMON  0 (XM = Z)*/
			(1      <<  4) |        /* nXPON 1 (XP = AIN[7])*/
			(1      <<  3) |        /* Pull Up Enable*/
			(1      <<  2) |        /* Auto ADC Conversion Mode*/
			(0      <<  0);         /* No Operation Mode*/
		rADCCON |= (1 << 0);  /* Start Auto conversion*/
		while (rADCCON & 0x1);               /* check if Enable_start is low*/
		while (!(rADCCON & (1 << 15)));      /* Check ECFLG*/
		y = 0x3ff-(0x3ff & rADCDAT1);
		x = (0x3ff & rADCDAT0);
		xsum += x;
		ysum += y;
	}

	*px = xsum / TSP_SAMPLE_NUM;
	*py = ysum / TSP_SAMPLE_NUM;
	rADCTSC =    (1      <<  8) |            /* UD_Sen*/
		(1      <<  7) |            /* YMON  1 (YM = GND)*/
		(1      <<  6) |            /* nYPON 1 (YP Connected AIN[n])*/
		(0      <<  5) |            /* XMON  0 (XM = Z)*/
		(1      <<  4) |            /* nXPON 1 (XP = AIN[7])*/
		(0      <<  3) |            /* Pull Up Disable*/
		(0      <<  2) |            /* Normal ADC Conversion Mode*/
		(3      <<  0);             /* Waiting Interrupt*/
	dx = (*px > x) ? (*px - x) : (x - *px);
	dy = (*py > y) ? (*py - y) : (y - *py);
}
void InterruptHandler()
{
	volatile unsigned int offset=rINTOFFSET;
	int x,y;
	PrintF("interrupt:%d\n",offset);
	if (offset==WM_TIMER)
	{
	//	PrintF("Timer4 Interrupt\n");
		pWinProcFunc(WM_TIMER,0,0);
		rSRCPND|=BIT_TIMER4;
		rINTPND|=BIT_TIMER4;
		rINTMSK&=~(BIT_TIMER4);
	}
	if (offset==WM_TOUCH)
	{
		rINTSUBMSK |= (BIT_SUB_ADC|BIT_SUB_TC);
		GetXY(&y,&x);
		TSP_TransXY(&y,&x);


		if(!(rADCDAT1>>15))
		{ 
			//rADCTSC &= 0x0ff;        	
			pWinProcFunc(WM_LBUTTONDOWN,x,y);	
		}
		else
		{
			pWinProcFunc(WM_LBUTTONUP,x,y);
		}
		
		rSRCPND|=BIT_ADC;
		rINTPND|=BIT_ADC;
		rSUBSRCPND|=BIT_SUB_TC;
		rINTMSK&=~(BIT_ADC);
		rINTSUBMSK&=~(BIT_SUB_TC);
		rINTSUBMSK&=~(BIT_SUB_ADC);
	}
}


