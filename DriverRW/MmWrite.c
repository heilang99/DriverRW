#include "MmWrite.h"

NTSTATUS WriteR3Memory(HANDLE processId, PVOID writeAddress, ULONG writeSize, PVOID targetWriteAddr)
{
	NTSTATUS status = STATUS_SUCCESS;
	if (writeAddress == NULL || writeSize == 0 || targetWriteAddr == NULL)
	{
		return STATUS_INVALID_PARAMETER;
	}

	if (((ULONG64)writeAddress + writeSize) >= MM_HIGHEST_USER_ADDRESS || targetWriteAddr >= MM_HIGHEST_USER_ADDRESS)
	{
		return STATUS_ACCESS_VIOLATION;
	}
	PEPROCESS targetProcess = NULL;
	KAPC_STATE ApcState = { 0 };
	status = PsLookupProcessByProcessId(processId, &targetProcess);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	//挂靠
	KeStackAttachProcess(targetProcess, &ApcState);
	if (!MmIsAddressValid(targetWriteAddr))
	{
		KeUnstackDetachProcess(&ApcState);
		ObDereferenceObject(targetProcess);
		return STATUS_ACCESS_VIOLATION;
	}


	//============================= 这里虽然可以写 但是有点小问题   就是写完后关闭再重新打开exe却还是已修改状态 ====================
	//============================= 利用修改Cr0的WP位也是这样，但是exe文件其实是没有修改的，电脑重启后打开exe就是没有修改的状态 ====================
	////写到目标进程三环
	//PHYSICAL_ADDRESS physicAddr = MmGetPhysicalAddress(targetWriteAddr);	//得到物理地址
	//PVOID mapAddr = MmMapIoSpace(physicAddr, writeSize, MmNonCached);		//映射一份线性地址
	//memcpy(mapAddr, allocBuff, writeSize);
	//MmUnmapIoSpace(mapAddr, writeSize);									//取消映射
	

	//============================= 用一下MDL方式实现 ==================== 还是一样 和上面两种方法
	PMDL mdl = NULL;
	PVOID mappedAddr = NULL;
	mappedAddr = MmMdlMapped(targetWriteAddr, writeSize, &mdl);
	if (mappedAddr == NULL)
	{
		KeUnstackDetachProcess(&ApcState);
		return STATUS_INVALID_PARAMETER_2;
	}

	//取消挂靠
	KeUnstackDetachProcess(&ApcState);

	//复制到3环
	memcpy(mappedAddr, writeAddress, writeSize);

	//取消MDL映射
	MmMdlUnMapped(mappedAddr, mdl);

	ObDereferenceObject(targetProcess);
	return status;
}