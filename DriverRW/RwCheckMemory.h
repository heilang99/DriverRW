#pragma once
#include <ntifs.h>
NTSTATUS RwCheckMemory(HANDLE pid, PVOID checkAddress, OUT PVOID MemoryInformation);

//�Զ����ڴ���Ϣ�ṹ Ϊ���ݶ�ȡ32λ���� �� 64λ���̵��ڴ���Ϣ��
typedef struct _MYMEMORY_BASIC_INFORMATION {
	ULONG64 BaseAddress;
	ULONG64 AllocationBase;
	ULONG64 AllocationProtect;
	ULONG64 RegionSize;
	ULONG64 State;
	ULONG64 Protect;
	ULONG64 Type;
} MYMEMORY_BASIC_INFORMATION, * PMYMEMORY_BASIC_INFORMATION;