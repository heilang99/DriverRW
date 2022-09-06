#include "MmRead.h"

// ============================= ���̹ҿ���ֱ�Ӷ�д =====================================
NTSTATUS ReadR3Memory(HANDLE processId, PVOID readAddress, ULONG64 readSize, PVOID readBuff)
{
	if ((ULONG64)readAddress >= MM_HIGHEST_USER_ADDRESS || ((ULONG64)readAddress + readSize) >= MM_HIGHEST_USER_ADDRESS)
	{
		return STATUS_ACCESS_VIOLATION;
	}
	if (readAddress == NULL || readSize == 0 || readBuff == NULL)
	{
		return STATUS_INVALID_PARAMETER;
	}
	PEPROCESS eProcess = NULL;
	NTSTATUS status = STATUS_SUCCESS;
	PVOID allocBuff = NULL;
	KAPC_STATE ApcState = { 0 };
	status = PsLookupProcessByProcessId(processId, &eProcess);
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	ObDereferenceObject(eProcess);
	if (PsGetProcessExitStatus(eProcess) != 0x103)	//eprocess+0x444
	{
		return STATUS_INVALID_PARAMETER_1;
	}
	allocBuff = ExAllocatePool(NonPagedPool, readSize);
	if (allocBuff == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}
	memset(allocBuff, 0, readSize);
	//�ҿ�
	KeStackAttachProcess(eProcess, &ApcState);
	if (!(MmIsAddressValid(readAddress) || !MmIsAddressValid((PVOID)((ULONG64)readAddress + readSize - 1))))
	{
		KeUnstackDetachProcess(&ApcState);
		ExFreePool(allocBuff);
		return STATUS_ACCESS_VIOLATION;
	}
	//���Ƶ�0��
	memcpy(allocBuff, readAddress, readSize);
	KeUnstackDetachProcess(&ApcState);
	//���Ƶ�3��
	memcpy(readBuff, allocBuff, readSize);
	ExFreePool(allocBuff);
	return status;
}

// ============================= ͨ��API��ȡ�ڴ� =====================================
NTSTATUS ReadR3MemoryByApi(HANDLE processId, PVOID readAddress, ULONG64 readSize, PVOID readBuff)
{
	if ((ULONG64)readAddress >= MM_HIGHEST_USER_ADDRESS || ((ULONG64)readAddress + readSize) >= MM_HIGHEST_USER_ADDRESS)
	{
		return STATUS_ACCESS_VIOLATION;
	}
	if (readAddress == NULL || readSize == 0 || readBuff == NULL)
	{
		return STATUS_INVALID_PARAMETER;
	}
	PEPROCESS eProcess = NULL;
	NTSTATUS status = STATUS_SUCCESS;
	PVOID allocBuff = NULL;
	KAPC_STATE ApcState = { 0 };
	status = PsLookupProcessByProcessId(processId, &eProcess);
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	//MmCopyVirtualMemory
	SIZE_T size = 0;
	status = MmCopyVirtualMemory(eProcess, readAddress, IoGetCurrentProcess(), readBuff, readSize, UserMode, &size);

	ObDereferenceObject(eProcess);
	return status;
}

// ============================= ͨ��MDL��ȡ�ڴ� =====================================
NTSTATUS ReadR3MemoryByMDL(HANDLE processId, PVOID readAddress, ULONG64 readSize, PVOID readBuff)
{
	if ((ULONG64)readAddress >= MM_HIGHEST_USER_ADDRESS || ((ULONG64)readAddress + readSize) >= MM_HIGHEST_USER_ADDRESS)
	{
		return STATUS_ACCESS_VIOLATION;
	}
	if (readAddress == NULL || readSize == 0 || readBuff == NULL)
	{
		return STATUS_INVALID_PARAMETER;
	}
	PEPROCESS eProcess = NULL;
	NTSTATUS status = STATUS_SUCCESS;
	PVOID allocBuff = NULL;
	KAPC_STATE ApcState = { 0 };
	status = PsLookupProcessByProcessId(processId, &eProcess);
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	ObDereferenceObject(eProcess);
	if (PsGetProcessExitStatus(eProcess) != 0x103)	//eprocess+0x444
	{
		return STATUS_INVALID_PARAMETER_1;
	}
	//�ҿ�
	KeStackAttachProcess(eProcess, &ApcState);
	if (!(MmIsAddressValid(readAddress) && MmIsAddressValid((PVOID)((ULONG64)readAddress + readSize - 1))))
	{
		KeUnstackDetachProcess(&ApcState);
		return STATUS_ACCESS_VIOLATION;
	}
	//MDLӳ��
	PMDL mdl = NULL;
	PVOID mappedAddr = NULL;
	mappedAddr = MmMdlMapped(readAddress, readSize, &mdl);
	if (mappedAddr == NULL)
	{
		KeUnstackDetachProcess(&ApcState);
		return STATUS_INVALID_PARAMETER_2;
	}
	KeUnstackDetachProcess(&ApcState);
	//���Ƶ�3��
	memcpy(readBuff, mappedAddr, readSize);
	//ȡ��MDLӳ��
	MmMdlUnMapped(mappedAddr,mdl);
	return status;
}

// ============================= ����MDL, ӳ���ڴ� =====================================
PVOID MmMdlMapped(PVOID VirAddr, ULONG Length, OUT PMDL* pMdl)
{
	PVOID MappedAddr = NULL;
	PMDL Mdl = NULL;

	//1������MDL
	Mdl = IoAllocateMdl(VirAddr, Length, FALSE, FALSE, NULL);
	if (Mdl == NULL)
	{
		return NULL;
	}

	//������Ҫӳ�������ҳ  LockPagesǰ��Ҫ����mdl.flag��ֵ
	/*__try
	{
		MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
	}
	__except (1)
	{
		DbgPrint("MmProbeAndLockPages �������쳣");
		IoFreeMdl(Mdl);
		return NULL;
	}*/

	//2�����Ϊ�Ƿ�ҳ�ڴ�
	MmBuildMdlForNonPagedPool(Mdl);

	//3������
	Mdl->MdlFlags = MDL_WRITE_OPERATION | MDL_ALLOCATED_FIXED_SIZE | MDL_PAGES_LOCKED;
	MappedAddr = MmMapLockedPages(Mdl, KernelMode);
	if (!MappedAddr)
	{
		//ӳ��ʧ��
		IoFreeMdl(pMdl);
		return FALSE;
	}

	//MDL ����������ҳ��ӳ�䵽һ�������ַ
	/*__try
	{
		MappedAddr = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmNonCached, VirAddr, FALSE, MdlMappingNoWrite);
	}
	__except (1)
	{
		DbgPrint("MmMapLockedPagesSpecifyCache �������쳣");
		MmUnlockPages(Mdl);
		IoFreeMdl(Mdl);
		return NULL;
	}*/

	*pMdl = Mdl;
	return MappedAddr;
}

// ============================= �ͷ�MDL, ȡ��ӳ�� =====================================
void MmMdlUnMapped(PVOID VirAddr, IN PMDL pMdl)
{
	//ȡ��ӳ��
	MmUnmapLockedPages(VirAddr, pMdl);
	//�������
	//MmUnlockPages(pMdl);
	//�ͷ�Mdl
	IoFreeMdl(pMdl);
}