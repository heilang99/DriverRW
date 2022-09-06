#include "RwGetModule.h"
#include "MmRead.h"

// ============================= 得到3环的模块地址 =====================================
ULONG64 RwGetModuleBase(HANDLE pid, char* moduleName)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG64 ModuleBase = 0;
	if (moduleName == NULL)
	{
		return 0;
	}
	//得到进程
	PEPROCESS targetProcess = NULL;
	status = PsLookupProcessByProcessId(pid, &targetProcess);
	if (!NT_SUCCESS(status))
	{
		return 0;
	}
	ObDereferenceObject(targetProcess);

	//根据进程环境得到模块基址
	PPEB32 peb32 = PsGetProcessWow64Process(targetProcess);
	if (peb32 != NULL)
	{
		return GetModuleBase32(targetProcess, peb32, moduleName);
	}
	else
	{
		PPEB64 peb64 = PsGetProcessPeb(targetProcess);
		if (peb64 == NULL)return 0;
		return GetModuleBase64(targetProcess, peb64, moduleName);
	}
	return ModuleBase;
}

// ============================= 32位进程的模块地址 =====================================
ULONG64 GetModuleBase32(PEPROCESS targetProcess,PPEB32 peb32, char* moduleName)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG64 ModuleBase = 0;
	//使用MmCopyVirtualMemory函数一下，锁定物理页，防止物理页发生交换
	CHAR readBuff = 0;
	SIZE_T size = 0;
	status = MmCopyVirtualMemory(targetProcess, peb32, IoGetCurrentProcess(), &readBuff, 1, KernelMode, &size);
	if (!NT_SUCCESS(status))
	{
		return 0;
	}

	//进程挂靠
	KAPC_STATE apcState = { 0 };
	KeStackAttachProcess(targetProcess, &apcState);
	if (!MmIsAddressValid(peb32))
	{
		KeUnstackDetachProcess(&apcState);
		return 0;
	}

	PPEB_LDR_DATA32 ldr32 = (PPEB_LDR_DATA32)peb32->Ldr;
	//当前LIST_ENTRY结构地址
	PLDR_DATA_TABLE_ENTRY32 ldrTable32 = (PLDR_DATA_TABLE_ENTRY32)(&ldr32->InLoadOrderModuleList);
	//下个LIST_ENTRY结构地址
	PLDR_DATA_TABLE_ENTRY32 ldrTableNext32 = (PLDR_DATA_TABLE_ENTRY32)ldr32->InLoadOrderModuleList.Flink;
	ANSI_STRING ModuleAnsiStr = { 0 };
	UNICODE_STRING ModuleUniStr = { 0 };
	RtlInitAnsiString(&ModuleAnsiStr, moduleName);
	RtlAnsiStringToUnicodeString(&ModuleUniStr, &ModuleAnsiStr, TRUE);

	//循环
	while (ldrTable32 != ldrTableNext32)
	{
		//因为是32位进程，3环的UNICODE_STRING是UNICODE_STRING32；得转换成64位
		WCHAR* temp = ldrTableNext32->BaseDllName.Buffer;
		UNICODE_STRING CurrentStr = { 0 };
		RtlInitUnicodeString(&CurrentStr, temp);
		//比较
		if (RtlCompareUnicodeString(&CurrentStr, &ModuleUniStr, TRUE) == 0)
		{
			ModuleBase = ldrTableNext32->DllBase;
			break;
		}
		ldrTableNext32 = (PLDR_DATA_TABLE_ENTRY32)ldrTableNext32->InLoadOrderLinks.Flink;
	}
	RtlFreeUnicodeString(&ModuleUniStr);
	KeUnstackDetachProcess(&apcState);
	return ModuleBase;
}

// ============================= 64位进程的模块地址 =====================================
ULONG64 GetModuleBase64(PEPROCESS targetProcess, PPEB64 peb64, char* moduleName)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG64 ModuleBase = 0;
	CHAR readBuff = 0;
	SIZE_T size = 0;
	status = MmCopyVirtualMemory(targetProcess, peb64, IoGetCurrentProcess(), &readBuff, 1, KernelMode, &size);
	if (!NT_SUCCESS(status))
	{
		return 0;
	}
	//进程挂靠
	KAPC_STATE apcState = { 0 };
	KeStackAttachProcess(targetProcess, &apcState);

	PPEB_LDR_DATA64 ldr64 = (PPEB_LDR_DATA64)peb64->Ldr;
	PLDR_DATA_TABLE_ENTRY64 ldrTable64 = (PLDR_DATA_TABLE_ENTRY64)(&ldr64->InLoadOrderModuleList);
	PLDR_DATA_TABLE_ENTRY64 ldrTableNext64 = (PLDR_DATA_TABLE_ENTRY64)ldr64->InLoadOrderModuleList.Flink;
	ANSI_STRING ModuleAnsiStr = { 0 };
	UNICODE_STRING ModuleUniStr = { 0 };
	RtlInitAnsiString(&ModuleAnsiStr, moduleName);
	RtlAnsiStringToUnicodeString(&ModuleUniStr, &ModuleAnsiStr, TRUE);

	//循环
	while (ldrTable64 != ldrTableNext64)
	{
		//比较
		if (RtlCompareUnicodeString(&(ldrTableNext64->BaseDllName), &ModuleUniStr, TRUE) == 0)
		{
			ModuleBase = ldrTableNext64->DllBase;
			break;
		}
		ldrTableNext64 = (PLDR_DATA_TABLE_ENTRY64)ldrTableNext64->InLoadOrderLinks.Flink;
	}
	RtlFreeUnicodeString(&ModuleUniStr);
	KeUnstackDetachProcess(&apcState);
	return ModuleBase;
}