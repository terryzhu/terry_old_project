#pragma once
#include "windows.h"
#define ARCHIIVE_MAXSIZE 100
class CShapeArchive
{
public:
	CShapeArchive()
	{
		p=new BYTE[ARCHIIVE_MAXSIZE];
		pCurrent=p;
		size=0;
	}
	~CShapeArchive()
	{
		delete [] p;
	}
	CShapeArchive & operator>>(POINT );
	CShapeArchive & operator<<(POINT pt)
	{
		if (size+sizeof(POINT)>ARCHIIVE_MAXSIZE)
		{
			BYTE * pNew=new BYTE[2*size];
			memcpy(pNew,p,size);
			delete [] p;
			p=pNew;
			pCurrent=p+size;
		}
		memcpy(pCurrent,&pt,sizeof(POINT));
		pCurrent+=sizeof(POINT);
		size+=sizeof(POINT);
		return *this;
	}
	CShapeArchive & operator<<(int x)
	{
		if (size+sizeof(POINT)>ARCHIIVE_MAXSIZE)
		{
			BYTE * pNew=new BYTE[2*size];
			memcpy(pNew,p,size);
			delete [] p;
			p=pNew;
			pCurrent=p+size;
		}
		memcpy(pCurrent,&x,sizeof(int));
		pCurrent+=sizeof(int);
		size+=sizeof(int);
		return *this;
	}
	BYTE * GetPointer()
	{
		return p;
	}
//	CShapeArchive & operator<<();
protected:
private:
	BYTE * p;
	int size;
	BYTE * pCurrent;

};