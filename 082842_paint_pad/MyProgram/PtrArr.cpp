#include "StdAfx.h"
#include "PtrArr.h"
#define MAX_SIZE_INIT 500
CPtrArr::CPtrArr(void)
{
	p=new void*[MAX_SIZE_INIT];
	memset(p,0,MAX_SIZE_INIT);
	size=0;
	Max_size=MAX_SIZE_INIT;
}

CPtrArr::~CPtrArr(void)
{

	delete []p;

	
}
void CPtrArr::Add(void* pt)
{
	if (size==Max_size)
	{
		void **ptemp=new void*[2*size];
		memcpy(ptemp,p,size*sizeof(void*));
		delete []p;
		p=ptemp;
		Max_size=Max_size*2;
	}
	p[size++]=pt;
}
int CPtrArr::GetSize()
{
	return size;
}
void * CPtrArr::GetAt(int index)
{
	return p[index];
}
void CPtrArr::Clear()
{
	size=0;
}
CPtrArr & CPtrArr::operator=(CPtrArr& ptrArrTemp)
{
	this->Max_size=ptrArrTemp.Max_size;
	this->size=ptrArrTemp.size;
	this->p=new void*[Max_size];
	memcpy(this->p,ptrArrTemp.p,Max_size*sizeof(void*));
	return *this;
}
CPtrArr::CPtrArr(CPtrArr& ptrArrTemp)
{
	*this=ptrArrTemp;
}
void CPtrArr::ClearTheContent()
{
	POINT *pForDelete;
	for (int i=0;i<GetSize();i++)
	{
		if (NULL==GetAt(i))
		{
			continue;
		}
		pForDelete=(POINT*)GetAt(i);
		delete [] pForDelete;
	}
}
void CPtrArr::DeleteAt(int index)
{
	if (p[index])
	{
		p[index]=NULL;
	}
	
}
void  CPtrArr::CopyFrom(CPtrArr &ptrarr)
{
	this->Clear();
	POINT *pPt;
	POINT * p;
	for (int i=0;i<ptrarr.GetSize();i++)
	{
		p=new POINT;
		pPt=(POINT *)ptrarr.GetAt(i);
		p->x=pPt->x;
		p->y=pPt->y;
		this->Add(p);
	}
}

void CPtrArr::SetAt(int Index, CShape * psh)
{
	p[Index]=psh;
}