#pragma once
#include <ntifs.h>
NTSTATUS RwCheckMemory(HANDLE pid, PVOID checkAddress, OUT PVOID MemoryInformation);

//自定义内存信息结构 为兼容读取32位进程 和 64位进程的内存信息；
typedef struct _MYMEMORY_BASIC_INFORMATION {
	ULONG64 BaseAddress;
	ULONG64 AllocationBase;
	ULONG64 AllocationProtect;
	ULONG64 RegionSize;
	ULONG64 State;
	ULONG64 Protect;
	ULONG64 Type;
} MYMEMORY_BASIC_INFORMATION, * PMYMEMORY_BASIC_INFORMATION;