#include "RwProcessProtect.h"
#include "RwSearchOpCode.h"

#define JMPECX "FFE1"
#define MMVERIFYWIN10 "BA20000000E8????????85C00F84????????498B4E184885C9"
#define VerrifyOffset 6

//ȫ�ֱ���
PVOID RegistrationHandle = NULL;
HANDLE protectPid;

//�ص�����
OB_PREOP_CALLBACK_STATUS PobPreOperationCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{
	HANDLE targetPid = PsGetProcessId(OperationInformation->Object);
	if (targetPid == protectPid)
	{
		OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = PROCESS_ALL_ACCESS;
		OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess = PROCESS_ALL_ACCESS;
		OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess = PROCESS_ALL_ACCESS;
		OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess = PROCESS_ALL_ACCESS;
	}
	else
	{
		return OB_PREOP_SUCCESS;
	}
	return OB_PREOP_SUCCESS;
}

//��װ���̱���
NTSTATUS RwInstallProcessProtect(HANDLE pid)
{
	NTSTATUS status = STATUS_SUCCESS;

	//pid
	PEPROCESS eprocess = NULL;
	status = PsLookupProcessByProcessId(pid, &eprocess);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("pidû�ж�Ӧ����");
		return status;
	}
	ObDereferenceObject(eprocess);
	protectPid = pid;

	//jmp ecx��ַ ���ﲻ��ʹ��jmp [ecx]
	static ULONG64 jmpAddr = 0;
	if (jmpAddr == 0)
	{
		//���̱������ܳ��ã�������������ƥ�书��ִ����һЩ���Ż�һ��
		jmpAddr = RwSearchOpCode(JMPECX, 0);
		if (jmpAddr == 0)
		{
			jmpAddr = 0;
			return STATUS_UNSUCCESSFUL;
		}
	}

	OB_OPERATION_REGISTRATION operatReg = { 0 };
	operatReg.ObjectType = PsProcessType;
	operatReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
	operatReg.PostOperation = NULL;
	operatReg.PreOperation = jmpAddr;

	OB_CALLBACK_REGISTRATION callbackReg = { 0 };
	callbackReg.Version = ObGetFilterVersion();
	callbackReg.OperationRegistrationCount = 1;
	callbackReg.OperationRegistration = &operatReg;
	callbackReg.RegistrationContext = PobPreOperationCallback;
	RtlInitUnicodeString(&callbackReg.Altitude, L"99999");

	// **** ����ص������е��жϣ�PG����ʱʱ�̶̿��ڼ�أ� ****
	PVOID verifyFunc = NULL;
	ULONG offset;
	static ULONG64 offsetAddr = NULL;
	if (offsetAddr == NULL)
	{
		offsetAddr = RwSearchOpCode(MMVERIFYWIN10, VerrifyOffset);
		if (offsetAddr == 0)
		{
			return STATUS_UNSUCCESSFUL;
		}
	}
	
	//ƫ�Ƽ���
	ULONG64 verifyAddrHigh = offsetAddr & 0xFFFFFFFF00000000;
	ULONG verifyAddrLower = (*(PULONG)offsetAddr) + ((offsetAddr + 4) & 0xFFFFFFFF);
	verifyFunc = verifyAddrHigh + verifyAddrLower;

	//�޸� MmVerifyCallbackFunctionCheckFlags ֱ�ӷ���1
	UCHAR shellcode[10] = { 0xb8,0x01,0x00,0x00,0x00,0xc3,0x90,0x90,0x90,0x90 };
	UCHAR oldCode[10] = { 0 };
	memcpy(oldCode, verifyFunc, 10);
	PHYSICAL_ADDRESS physicAddr = MmGetPhysicalAddress(verifyFunc);
	PVOID verifyWriteAddr = MmMapIoSpace(physicAddr, 10, MmNonCached);
	memcpy(verifyWriteAddr, shellcode, 10);

	//ע��ص�
	status = ObRegisterCallbacks(&callbackReg, &RegistrationHandle);

	//��ԭ
	memcpy(verifyWriteAddr, shellcode, 10);
	MmUnmapIoSpace(verifyWriteAddr, 10);

	if (NT_SUCCESS(status))
	{
		DbgPrint("�ѽ��н��̱���");
	}
	return status;
}

//ж�ؽ��̱���
NTSTATUS RwUnstallProcessProtect()
{
	NTSTATUS status = STATUS_SUCCESS;
	if (RegistrationHandle)
	{
		DbgPrint("�ѽ�����̱���");
		ObUnRegisterCallbacks(RegistrationHandle);
		RegistrationHandle = NULL;
		protectPid = 0;
	}
	/*else
	{
		DbgPrint("���������̱���");
		status = STATUS_UNSUCCESSFUL;
	}*/
	return status;
}

//�޸Ľ��̶���ͷ
NTSTATUS RwInProtectByEditHeader(HANDLE protectPid)
{
	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS eprocess = NULL;
	status = PsLookupProcessByProcessId(protectPid, &eprocess);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("pidû�ж�Ӧ����");
		return status;
	}
	ObDereferenceObject(eprocess);
	//�޸ĵ�Flags  ��_OBJECT_HEADER+1b��
	ULONG64  objectHeader = (ULONG64)eprocess - 0x30;
	*(PUCHAR)(objectHeader + 0x1b) |= 4;
	return status;
}

//�޸Ľ��̶���ͷ  ʵ�ֲ���������.
NTSTATUS RwUnProtectByEditHeader(HANDLE protectPid)
{
	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS eprocess = NULL;
	status = PsLookupProcessByProcessId(protectPid, &eprocess);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("pidû�ж�Ӧ����");
		return status;
	}
	ObDereferenceObject(eprocess);
	//�޸ĵ�Flags  ��_OBJECT_HEADER+1b��
	ULONG64  objectHeader = (ULONG64)eprocess - 0x30;
	*(PUCHAR)(objectHeader + 0x1b) &= 0xFb;
	return status;
}