

// UART 1�ĸ����Ĵ�������
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

// ��UART1����һ���ַ�
void PutChar(char bData)
{
	// �ȴ����ͻ�����Ϊ��
	while(rUFSTAT1 & 0x200)
	{
		;
	}
    
    rUTXH1 = bData;
}

// ��UART1����һ���ַ���
void PrintString(char *str)
{
    while (*str)
    {
        PutChar(*str);
        str++;
    }
}

// ��UART1����һ���ַ�
char GetChar(void)
{
	// �ȴ����ַ����Խ���
	while(!(rUFSTAT1 & 0x0f))
	{
		;
	}
	  
	return rURXH1;
 
}
  
// ��UART1����һ���ַ���
void GetString(char *pbString)
{ 
	char bC;
	
	while((bC = GetChar()) != '\r')
	{
		*pbString++ = bC;
		PutChar(bC);
	}
	*pbString='\0';
	PutChar('\n');
	
}




int main(int argc, char ** argv)
{
	char pStr[100];
	    
	PrintString("Please enter your name:\r\n");
	
	GetString(pStr);
	
	PrintString("Nice to meet you, ");
	PrintString(pStr);
	
	return 0;
}
