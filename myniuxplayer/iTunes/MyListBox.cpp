// MyListBox.cpp : 实现文件
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



// CMyListBox 消息处理程序



void CMyListBox::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int DropCount=DragQueryFile(hDropInfo,-1,NULL,0);//取得被拖动文件的数目       
	for(int i=0;i< DropCount;i++)   
	{      //取得第i个拖动文件名所占字节数      
		int NameSize=2*DragQueryFile(hDropInfo,i,NULL,0);      
		HANDLE hHeap=GetProcessHeap();      //根据字节数分配缓冲区      //char *pName=new char[++NameSize];     
		TCHAR *pName=(LPTSTR)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,(NameSize++)*2);
		if (pName==NULL)
		{       
			MessageBox(L"给文件名分配暂存空间时出错!",L"错误信息",MB_ICONERROR);       
			return;     
		}
		DragQueryFile(hDropInfo,i,pName,NameSize);//把文件名拷贝到缓冲区     
		AddString(pName);//文件名加入listbox中显示                     //delete [] pName;//释放缓冲区      
		HeapFree(hHeap,HEAP_ZERO_MEMORY,pName);     
	} 
	DragFinish(hDropInfo);  //拖放结束后,释放内存
	CListBox::OnDropFiles(hDropInfo);
}
