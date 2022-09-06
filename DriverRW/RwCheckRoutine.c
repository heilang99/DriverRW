#include "RwCheckRoutine.h"
void PcreateProcessNotifyRoutine(HANDLE ParentId,HANDLE ProcessId,BOOLEAN Create)
{
	if (Create == TRUE)
	{
		PEPROCESS eprocess = NULL;
		NTSTATUS status = PsLookupProcessByProcessId(ProcessId, &eprocess);
		if (!NT_SUCCESS(status))
		{
			return;
		}
		ULONG TableBase = *(PULONG)((ULONG64)eprocess + 0x28);
		DbgPrint("Pid = %d  Cr3 = %x", (ULONG)ProcessId, TableBase);
		ObDereferenceObject(eprocess);
	}
	else
	{
		DbgPrint("关闭了进程");
	}
}
void PcreateThreadNotifyRoutine(HANDLE ProcessId, HANDLE ThreadId, BOOLEAN Create)
{
	if (Create == TRUE)
	{
		DbgPrint("创建了线程  %d", (ULONG)ThreadId);
	}
	else
	{
		DbgPrint("关闭了线程");
	}
}
void PloadImageNotifyRoutine(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	if (ProcessId != 0)
	{
		DbgPrint("文件名:%ws,iamgebase:%llx", FullImageName->Buffer, (ULONG64)ImageInfo->ImageBase);
	}
}
//============== 监控进程 ============== 
NTSTATUS RwInstallCheckProcess()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsSetCreateProcessNotifyRoutine(PcreateProcessNotifyRoutine,FALSE);
	if (NT_SUCCESS(status))
	{
		DbgPrint("安装系统回调成功 监控进程");
	}
	return status;
}
NTSTATUS RwUnstallCheckProcess()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsSetCreateProcessNotifyRoutine(PcreateProcessNotifyRoutine, TRUE);
	if (NT_SUCCESS(status))
	{
		DbgPrint("卸载系统回调成功 监控进程");
	}
	return status;
}

//============== 监控线程 ============== 
NTSTATUS RwInstallCheckThread()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsSetCreateThreadNotifyRoutine(PcreateThreadNotifyRoutine);
	if (NT_SUCCESS(status))
	{
		DbgPrint("安装系统回调成功 监控线程");
	}
	return status;
}
NTSTATUS RwUnstallCheckThread()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsRemoveCreateThreadNotifyRoutine(PcreateThreadNotifyRoutine);
	if (NT_SUCCESS(status))
	{
		DbgPrint("卸载系统回调成功 监控线程");
	}
	return status;
}

//============== 监控模块 ============== 
NTSTATUS RwInstallCheckModule()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsSetLoadImageNotifyRoutine(PloadImageNotifyRoutine);
	if (NT_SUCCESS(status))
	{
		DbgPrint("安装系统回调成功 监控模块");
	}
	return status;
}
NTSTATUS RwUnstallCheckModule()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsRemoveLoadImageNotifyRoutine(PloadImageNotifyRoutine);
	if (NT_SUCCESS(status))
	{
		DbgPrint("卸载系统回调成功 监控模块");
	}
	return status;
}