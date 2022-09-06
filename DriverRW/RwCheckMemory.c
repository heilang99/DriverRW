#include "RwCheckMemory.h"

NTSTATUS RwCheckMemory(HANDLE pid, PVOID checkAddress, OUT PVOID MemoryInformation)
{
	NTSTATUS status = STATUS_SUCCESS;
	if (MemoryInformation == NULL)
	{
		return STATUS_INVALID_PARAMETER_3;
	}
	PEPROCESS targetProcess = NULL;
	status = PsLookupProcessByProcessId(pid,&targetProcess);
	if (!NT_SUCCESS(status))
	{
		return STATUS_UNSUCCESSFUL;
	}
	ObDereferenceObject(targetProcess);

	//�����ڴ�
	SIZE_T realSize = 0;
	PMEMORY_BASIC_INFORMATION pMemInfo = NULL;
	pMemInfo = ExAllocatePool(NonPagedPool, sizeof(MEMORY_BASIC_INFORMATION));
	if (pMemInfo == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}
	RtlZeroMemory(pMemInfo, sizeof(MEMORY_BASIC_INFORMATION));

	//���̹ҿ�  ������������-1����ǰ�������ֹ�������
	KAPC_STATE apcState = { 0 };
	KeStackAttachProcess(targetProcess,&apcState);
	status = ZwQueryVirtualMemory(ZwCurrentProcess(), checkAddress, MemoryBasicInformation, pMemInfo, sizeof(MEMORY_BASIC_INFORMATION), &realSize);
	if (!NT_SUCCESS(status))
	{
		ExFreePool(pMemInfo);
		KeUnstackDetachProcess(&apcState);
		return status;
	}
	KeUnstackDetachProcess(&apcState);

	//ȡ���ҿ����Ƶ�����
	((PMYMEMORY_BASIC_INFORMATION)MemoryInformation)->AllocationBase = pMemInfo->AllocationBase;
	((PMYMEMORY_BASIC_INFORMATION)MemoryInformation)->AllocationProtect = pMemInfo->AllocationProtect;
	((PMYMEMORY_BASIC_INFORMATION)MemoryInformation)->BaseAddress = pMemInfo->BaseAddress;
	((PMYMEMORY_BASIC_INFORMATION)MemoryInformation)->Protect = pMemInfo->Protect;
	((PMYMEMORY_BASIC_INFORMATION)MemoryInformation)->RegionSize = pMemInfo->RegionSize;
	((PMYMEMORY_BASIC_INFORMATION)MemoryInformation)->Type = pMemInfo->Type;

	//�ͷ��ڴ�
	ExFreePool(pMemInfo);
	return status;
}