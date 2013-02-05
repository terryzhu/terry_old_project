// MyListBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "iTunes.h"
#include "MyListBox.h"


// CMyListBox

IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{
	

}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CMyListBox ��Ϣ�������



void CMyListBox::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	int DropCount=DragQueryFile(hDropInfo,-1,NULL,0);//ȡ�ñ��϶��ļ�����Ŀ       
	for(int i=0;i< DropCount;i++)   
	{      //ȡ�õ�i���϶��ļ�����ռ�ֽ���      
		int NameSize=2*DragQueryFile(hDropInfo,i,NULL,0);      
		HANDLE hHeap=GetProcessHeap();      //�����ֽ������仺����      //char *pName=new char[++NameSize];     
		TCHAR *pName=(LPTSTR)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,(NameSize++)*2);
		if (pName==NULL)
		{       
			MessageBox(L"���ļ��������ݴ�ռ�ʱ����!",L"������Ϣ",MB_ICONERROR);       
			return;     
		}
		DragQueryFile(hDropInfo,i,pName,NameSize);//���ļ���������������     
		AddString(pName);//�ļ�������listbox����ʾ                     //delete [] pName;//�ͷŻ�����      
		HeapFree(hHeap,HEAP_ZERO_MEMORY,pName);     
	} 
	DragFinish(hDropInfo);  //�ϷŽ�����,�ͷ��ڴ�
	CListBox::OnDropFiles(hDropInfo);
}
