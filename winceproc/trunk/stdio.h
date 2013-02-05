
#ifndef STDIO_H
#define STDIO_H
#include "type.h"

// UART 1的各个寄存器定义
#define rULCON1		(*(volatile unsigned *)0x50004000) //UART 1 Line control
#define rUCON1		(*(volatile unsigned *)0x50004004) //UART 1 Control
#define rUFCON1		(*(volatile unsigned *)0x50004008) //UART 1 FIFO control
#define rUMCON1		(*(volatile unsigned *)0x5000400c) //UART 1 Modem control
#define rUTRSTAT1	(*(volatile unsigned *)0x50004010) //UART 1 Tx/Rx status
#define rUERSTAT1	(*(volatile unsigned *)0x50004014) //UART 1 Rx error status
#define rUFSTAT1	(*(volatile unsigned *)0x50004018) //UART 1 FIFO status
#define rUMSTAT1	(*(volatile unsigned *)0x5000401c) //UART 1 Modem status
#define rUBRDIV1	(*(volatile unsigned *)0x50004028) //UART 1 Baud rate divisor
#define rUTXH1 		(*(volatile unsigned char *)0x50004020) //UART 1 Transmission Hold
#define rURXH1 		(*(volatile unsigned char *)0x50004024) //UART 1 Receive buffer





int GetPortValue(int port);
void SetPortValue(int port,int value);
void PrintString(char * pstr);
void GetString(_ret_ char * pstr);
char GetChar();
void PutChar(char bData);
int AToI(char *str);
void IToA(_ret_ char *str,int i);
int SetBit32(int bit, int pos,bool  b);
void PrintF(const char * str,...);
void IToA16(_ret_ char *str,unsigned int i);
int StrLen(char * str);

int Abs(int x);

#define SWAP(x,y) (x=x+y;y=x-y;x=x-y;)


#endif
