#pragma once
#include <string>
#include <vector>
#include "ntshell.h"

using namespace std;

typedef NTSYSAPI NTSTATUS (NTAPI *ZwQuerySystemInformation)(IN ULONG SystemInformationClass,
	OUT PVOID SystemInformation, IN ULONG SystemInformationLength, OUT PULONG ReturnLength);

typedef NTSYSAPI NTSTATUS (NTAPI *pZwClose)(IN HANDLE Handle);

typedef NTSYSAPI NTSTATUS (NTAPI *pZwOpenSection)(OUT PHANDLE SectionHandle, 
	IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);

typedef NTSYSAPI VOID (NTAPI *pRtlInitUnicodeString)(PUNICODE_STRING 
	DestinationString, PCWSTR SourceString);

typedef NTSYSAPI NTSTATUS (NTAPI *pNtVdmControl)(IN ULONG ControlCode, IN PVOID ControlData);

typedef NTSYSAPI NTSTATUS (NTAPI *pZwTerminateThread)(IN HANDLE ThreadHandle, IN NTSTATUS ExitStatus);

DWORD WINAPI TaskmgrCloser(LPVOID lpParam);

// 这个类用于关闭指定的程序
class CProgramKiller
{
	friend DWORD WINAPI TaskmgrCloser(LPVOID lpParam);
private:
	vector<wstring> stringVector;
	vector<wstring> targetProcessName;
	HANDLE hThread;
	DWORD threadId;

	ZwQuerySystemInformation pZwQuerySystemInformation;
	pZwClose ZwClose;
	pZwOpenSection ZwOpenSection;
	pRtlInitUnicodeString RtlInitUnicodeString;
	pNtVdmControl NtVdmControl;
	pZwTerminateThread ZwTerminateThread;
public:
	CProgramKiller(void);
	~CProgramKiller(void);

protected:
	bool CloseWindow();
	bool CloseProcess();

	BOOL ProMgrcForceKillProcess();

	bool isProcessTarget(UNICODE_STRING ProcessName);
	bool EnumAllThreads(IN ULONG UniqueProcessId, OUT PRING0_KILLTHREAD ThreadId);
	NTSTATUS Ring0Call(
		IN PRING0_ROUTINE Ring0Routine,
		IN PVOID Ring0Argument
		);
	PVOID GetModuleBase(PCSTR name);
	HANDLE OpenPhysicalMemory();
};

