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

	//�ҿ�
	KeStackAttachProcess(targetProcess, &ApcState);
	if (!MmIsAddressValid(targetWriteAddr))
	{
		KeUnstackDetachProcess(&ApcState);
		ObDereferenceObject(targetProcess);
		return STATUS_ACCESS_VIOLATION;
	}


	//============================= ������Ȼ����д �����е�С����   ����д���ر������´�exeȴ�������޸�״̬ ====================
	//============================= �����޸�Cr0��WPλҲ������������exe�ļ���ʵ��û���޸ĵģ������������exe����û���޸ĵ�״̬ ====================
	////д��Ŀ���������
	//PHYSICAL_ADDRESS physicAddr = MmGetPhysicalAddress(targetWriteAddr);	//�õ������ַ
	//PVOID mapAddr = MmMapIoSpace(physicAddr, writeSize, MmNonCached);		//ӳ��һ�����Ե�ַ
	//memcpy(mapAddr, allocBuff, writeSize);
	//MmUnmapIoSpace(mapAddr, writeSize);									//ȡ��ӳ��
	

	//============================= ��һ��MDL��ʽʵ�� ==================== ����һ�� ���������ַ���
	PMDL mdl = NULL;
	PVOID mappedAddr = NULL;
	mappedAddr = MmMdlMapped(targetWriteAddr, writeSize, &mdl);
	if (mappedAddr == NULL)
	{
		KeUnstackDetachProcess(&ApcState);
		return STATUS_INVALID_PARAMETER_2;
	}

	//ȡ���ҿ�
	KeUnstackDetachProcess(&ApcState);

	//���Ƶ�3��
	memcpy(mappedAddr, writeAddress, writeSize);

	//ȡ��MDLӳ��
	MmMdlUnMapped(mappedAddr, mdl);

	ObDereferenceObject(targetProcess);
	return status;
}