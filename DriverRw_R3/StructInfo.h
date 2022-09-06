#pragma once
#include <Windows.h>
//================ 约定通信结构体 ======================
typedef enum _COMMUNICATE_NUMBER
{
	IsCr3ToCr0 = 0x1234,
	DriverRead,
	DriverWrite,
	GetMMType,
	InstallProtect,
	UnstallProtect,
	ModifyHeaderFlag,
	RestoreHeaderFlag,
	InJectThread
}COMMUNICATE_NUMBER, * PCOMMUNICATE_NUMBER;

typedef struct _PACKAGE
{
	COMMUNICATE_NUMBER communicateID;
	COMMUNICATE_NUMBER communicateFun;
	ULONG64 content;
	ULONG64 length;
	ULONG64 result;
}PACKAGE, * PPACKAGE;

//================ 数据通信结构体 ======================
//读数据通信 结构体
typedef struct _MMREAD
{
	ULONG ProcessId;
	PVOID readAddress;
	ULONG readSize;
	PVOID readBuff;
}MMREAD, * PMMREAD;

//写数据通信 结构体
typedef struct _MMWRITE
{
	ULONG64 ProcessId;
	PVOID writeAddress;
	ULONG64 writeSize;
	PVOID targetWriteAddr;
}MMWRITE, * PMMWRITE;

//内存信息通信 结构体
typedef struct _MMTYPE {
	ULONG64 ProcessId;
	PVOID checkAddress;
	PVOID MMInfoBuff;
} MMTYPE, * PMMTYPE;

//进程保护通信 结构体
typedef struct _INPROTECT {
	HANDLE pid;
} INPROTECT, * PINPROTECT;

//自定义内存信息结构
typedef struct _MYMEMORY_BASIC_INFORMATION {
	ULONG64 BaseAddress;
	ULONG64 AllocationBase;
	ULONG64 AllocationProtect;
	ULONG64 RegionSize;
	ULONG64 State;
	ULONG64 Protect;
	ULONG64 Type;
} MYMEMORY_BASIC_INFORMATION, * PMYMEMORY_BASIC_INFORMATION;


//线程劫持通信 结构体
typedef struct _INJECTTHREAD {
	HANDLE pid;
	ULONG Argc;
	ULONG Arg[4];
} INJECTTHREAD, *PINJECTTHREAD;


//================ 通信函数 ======================
//Win10驱动通信
typedef ULONG64(_fastcall* _FuncPos)(char, PVOID, PVOID, PVOID);

//Win7 驱动通信
typedef struct _IO_STATUS_BLOCK {
	union {
		PVOID Status;
		PVOID Pointer;
	};
	ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

typedef ULONG(*_NtQueryInformationFile)(
	HANDLE FileHandle,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID  FileInformation,
	ULONG Length,
	ULONG FileInformationClass
);