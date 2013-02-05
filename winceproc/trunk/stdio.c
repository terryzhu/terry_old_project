// 向UART1发送一个字符
#include "stdio.h"

void PutChar(char bData)
{
	// 等待发送缓冲区为空
	while(rUFSTAT1 & 0x200)
	{
		;
	}

	rUTXH1 = bData;
}

// 向UART1发送一个字符串
void PrintString(char *str)
{
	while (*str)
	{
		PutChar(*str);
		str++;
	}
}

// 从UART1接收一个字符
char GetChar(void)
{
	// 等待有字符可以接收
	while(!(rUFSTAT1 & 0x0f))
	{
		;
	}

	return rURXH1;

}

// 从UART1接收一个字符串
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

int AToI(char *str)
{
	int ret=0;
	int b_positive=true;
	if(*str=='-')
	{
		b_positive=false;
		str++;
	}
	while (*str)
	{
		ret*=10;
		ret+=*str-'0';
		str++;
	}
	return b_positive?ret:-ret;
}

void IToA(char *str,int i)
{
	if(i==0)
	{
		*str++='0';
		*str='\0';
		return ;
	}
	if(i<0)
	{
		*str++='-';
		i=-i;
	}
	int howlong=1;
	int temp=10;
	while (i>=temp)
	{
		howlong++;
		temp*=10;
	}
	int j=0;
	for (j=0;j<howlong;j++)
	{
		str[howlong-j-1]=i%10+'0';
		i/=10;
	}
	str[howlong]='\0';

}

void PrintF(const char * str,...)
{
	const char  **parg=&str+1;
	
	const char * p=str;
	char szbuf[100];
	while(*p!='\0')
	{
		if(*p!='%')
		{
			PutChar(*p);
			p++;
			continue;
		}
		p++;
		switch (*p)
		{
		case 'd':
			IToA(szbuf,*(int *)parg);
			PrintString(szbuf);
			parg++;
			break;
		case 's':
			PrintString(*(char**)parg);
			parg++;
			break;
		case 'c':
			PutChar(*(char*)parg);
			parg++;
			break;
		case '%':
			PutChar('%');
			break;
		}

		p++;
	}

}


int SetBit32(int bit,int pos,bool b)
{
	int mask=0xffffffff;
	if (b==true)
	{
		bit|=1<<pos;
	}
	else
	{
		bit&=~(1<<pos);
	}
	return bit;
}


void IToA16(char *str,unsigned int i)
{
	int k=0;
	for(k=0;k<11;k++)
	{
		str[k]='0';
	}
	*str++='0';
	*str++='x';
	if(i==0)
	{
		*str++='0';
		*str='\0';
		return ;
	}

	int howlong=9;
	int j=0;
	for (j=0;j<howlong;j++)
	{
		if (j==4)
		{
			str[j]=' ';
			continue;
		}
		short s=i%16;
		if (s<10)
		{
			str[howlong-j-1]=s+'0';
		}
		else
		{
			str[howlong-j-1]='A'+s-10;
		}

		i>>=4;
	}
	str[howlong]='\0';

}

int StrLen(char * str)
{
	int i=0;
	while (*str++)
	{
		i++;
	}
	return i;
}

void SetPortValue(int port,int value)
{
	(*(volatile unsigned *)port)=value;
}

int GetPortValue(int port)
{
	return *(volatile unsigned *)port;
}


int Abs(int x)
{
	if (x<0)
	{
		x=-x;
	}
	return x;
}
