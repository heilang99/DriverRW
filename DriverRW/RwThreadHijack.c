#include "RwThreadHijack.h"
#include "RwSearchOpCode.h"
#include "utilFun.h";
#define SUSPEND "84C00F84C12D12008B8710050000A8010F85A92D1200"
#define RESUME "B800800000418B887C08000085C874240FBAE1140F823FB615004885DB"
#define SuspendOffset -73
#define ResumeOffset -49

NTSTATUS RwThreadHijackWin10(HANDLE Tid, PVOID Routine)
{
	NTSTATUS status = STATUS_SUCCESS;
	if (Routine == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}
	PETHREAD eThread = NULL;
	status = PsLookupThreadByThreadId(Tid, &eThread);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("�߳�ID����");
		return STATUS_UNSUCCESSFUL;
	}
	//����������
	static _PsSuspendThread MyPsSuspendThread = NULL;
	static _PsResumeThread MyPsResumeThread = NULL;
	if (MyPsSuspendThread == NULL)
	{
		MyPsSuspendThread = RwSearchOpCode(SUSPEND, SuspendOffset);
		if (MyPsSuspendThread == 0)
		{
			DbgPrint("���������");
			return STATUS_UNSUCCESSFUL;
		}
	}
	if (MyPsResumeThread == NULL)
	{
		MyPsResumeThread = RwSearchOpCode(RESUME, ResumeOffset);
		if (MyPsResumeThread == 0)
		{
			DbgPrint("���������");
			return STATUS_UNSUCCESSFUL;
		}
	}
	ULONG PreviousCount = 0;
	//����
	status = MyPsSuspendThread(eThread, &PreviousCount);
	if (NT_SUCCESS(status))
	{
		//�޸�
		ULONG64 TrapFrame = *(PULONG64)((ULONG64)eThread + 0x90);
		PULONG64 RipAddr = (PULONG64)(TrapFrame + 0x168);
		*RipAddr = Routine;

		//�ָ�
		status = MyPsResumeThread(eThread, &PreviousCount);
	}
	ObDereferenceObject(eThread);
	return status;
}

//�ٳ����
NTSTATUS RwThreadHijack(HANDLE Pid,ULONG Argc,ULONG* Argv)
{
	NTSTATUS status = STATUS_SUCCESS;
	
	
	return status;
}
