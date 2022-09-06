#include "MmRead.h"

// ============================= 进程挂靠，直接读写 =====================================
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
	//挂靠
	KeStackAttachProcess(eProcess, &ApcState);
	if (!(MmIsAddressValid(readAddress) || !MmIsAddressValid((PVOID)((ULONG64)readAddress + readSize - 1))))
	{
		KeUnstackDetachProcess(&ApcState);
		ExFreePool(allocBuff);
		return STATUS_ACCESS_VIOLATION;
	}
	//复制到0环
	memcpy(allocBuff, readAddress, readSize);
	KeUnstackDetachProcess(&ApcState);
	//复制到3环
	memcpy(readBuff, allocBuff, readSize);
	ExFreePool(allocBuff);
	return status;
}

// ============================= 通过API读取内存 =====================================
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

// ============================= 通过MDL读取内存 =====================================
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
	//挂靠
	KeStackAttachProcess(eProcess, &ApcState);
	if (!(MmIsAddressValid(readAddress) && MmIsAddressValid((PVOID)((ULONG64)readAddress + readSize - 1))))
	{
		KeUnstackDetachProcess(&ApcState);
		return STATUS_ACCESS_VIOLATION;
	}
	//MDL映射
	PMDL mdl = NULL;
	PVOID mappedAddr = NULL;
	mappedAddr = MmMdlMapped(readAddress, readSize, &mdl);
	if (mappedAddr == NULL)
	{
		KeUnstackDetachProcess(&ApcState);
		return STATUS_INVALID_PARAMETER_2;
	}
	KeUnstackDetachProcess(&ApcState);
	//复制到3环
	memcpy(readBuff, mappedAddr, readSize);
	//取消MDL映射
	MmMdlUnMapped(mappedAddr,mdl);
	return status;
}

// ============================= 创建MDL, 映射内存 =====================================
PVOID MmMdlMapped(PVOID VirAddr, ULONG Length, OUT PMDL* pMdl)
{
	PVOID MappedAddr = NULL;
	PMDL Mdl = NULL;

	//1、创建MDL
	Mdl = IoAllocateMdl(VirAddr, Length, FALSE, FALSE, NULL);
	if (Mdl == NULL)
	{
		return NULL;
	}

	//锁定需要映射的物理页  LockPages前不要随便给mdl.flag赋值
	/*__try
	{
		MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
	}
	__except (1)
	{
		DbgPrint("MmProbeAndLockPages 触发了异常");
		IoFreeMdl(Mdl);
		return NULL;
	}*/

	//2、标记为非分页内存
	MmBuildMdlForNonPagedPool(Mdl);

	//3、锁定
	Mdl->MdlFlags = MDL_WRITE_OPERATION | MDL_ALLOCATED_FIXED_SIZE | MDL_PAGES_LOCKED;
	MappedAddr = MmMapLockedPages(Mdl, KernelMode);
	if (!MappedAddr)
	{
		//映射失败
		IoFreeMdl(pMdl);
		return FALSE;
	}

	//MDL 描述的物理页面映射到一个虚拟地址
	/*__try
	{
		MappedAddr = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmNonCached, VirAddr, FALSE, MdlMappingNoWrite);
	}
	__except (1)
	{
		DbgPrint("MmMapLockedPagesSpecifyCache 触发了异常");
		MmUnlockPages(Mdl);
		IoFreeMdl(Mdl);
		return NULL;
	}*/

	*pMdl = Mdl;
	return MappedAddr;
}

// ============================= 释放MDL, 取消映射 =====================================
void MmMdlUnMapped(PVOID VirAddr, IN PMDL pMdl)
{
	//取消映射
	MmUnmapLockedPages(VirAddr, pMdl);
	//解除锁定
	//MmUnlockPages(pMdl);
	//释放Mdl
	IoFreeMdl(pMdl);
}