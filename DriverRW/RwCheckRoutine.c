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
		DbgPrint("�ر��˽���");
	}
}
void PcreateThreadNotifyRoutine(HANDLE ProcessId, HANDLE ThreadId, BOOLEAN Create)
{
	if (Create == TRUE)
	{
		DbgPrint("�������߳�  %d", (ULONG)ThreadId);
	}
	else
	{
		DbgPrint("�ر����߳�");
	}
}
void PloadImageNotifyRoutine(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	if (ProcessId != 0)
	{
		DbgPrint("�ļ���:%ws,iamgebase:%llx", FullImageName->Buffer, (ULONG64)ImageInfo->ImageBase);
	}
}
//============== ��ؽ��� ============== 
NTSTATUS RwInstallCheckProcess()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsSetCreateProcessNotifyRoutine(PcreateProcessNotifyRoutine,FALSE);
	if (NT_SUCCESS(status))
	{
		DbgPrint("��װϵͳ�ص��ɹ� ��ؽ���");
	}
	return status;
}
NTSTATUS RwUnstallCheckProcess()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsSetCreateProcessNotifyRoutine(PcreateProcessNotifyRoutine, TRUE);
	if (NT_SUCCESS(status))
	{
		DbgPrint("ж��ϵͳ�ص��ɹ� ��ؽ���");
	}
	return status;
}

//============== ����߳� ============== 
NTSTATUS RwInstallCheckThread()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsSetCreateThreadNotifyRoutine(PcreateThreadNotifyRoutine);
	if (NT_SUCCESS(status))
	{
		DbgPrint("��װϵͳ�ص��ɹ� ����߳�");
	}
	return status;
}
NTSTATUS RwUnstallCheckThread()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsRemoveCreateThreadNotifyRoutine(PcreateThreadNotifyRoutine);
	if (NT_SUCCESS(status))
	{
		DbgPrint("ж��ϵͳ�ص��ɹ� ����߳�");
	}
	return status;
}

//============== ���ģ�� ============== 
NTSTATUS RwInstallCheckModule()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsSetLoadImageNotifyRoutine(PloadImageNotifyRoutine);
	if (NT_SUCCESS(status))
	{
		DbgPrint("��װϵͳ�ص��ɹ� ���ģ��");
	}
	return status;
}
NTSTATUS RwUnstallCheckModule()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = PsRemoveLoadImageNotifyRoutine(PloadImageNotifyRoutine);
	if (NT_SUCCESS(status))
	{
		DbgPrint("ж��ϵͳ�ص��ɹ� ���ģ��");
	}
	return status;
}