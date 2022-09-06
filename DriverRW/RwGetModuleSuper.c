#include "RwGetModuleSuper.h"
//获取内核模块地址
ULONG64 RwGetModuleSuper(PCHAR szModuleName,OUT PULONG64 ModuleSize)
{
	if (szModuleName == NULL)
	{
		return 0;
	}
	NTSTATUS status = STATUS_SUCCESS;
	ULONG64 SuperModuleBase = 0;
	ULONG64 realSize = 0;
	UCHAR temmpBuff = 0;
	PVOID infoBuff = NULL;
	//得到实际大小
	status = ZwQuerySystemInformation(SystemModuleInformation, &temmpBuff, 1, &realSize);
	if (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		infoBuff = ExAllocatePool(NonPagedPool, realSize);
		if (infoBuff == NULL)
		{
			return 0;
		}
		RtlZeroMemory(infoBuff, realSize);
		status = ZwQuerySystemInformation(SystemModuleInformation, infoBuff, realSize, &realSize);
		if (!NT_SUCCESS(status))
		{
			ExFreePool(infoBuff);
			return 0;
		}
	}
	PRTL_PROCESS_MODULES processModules = (PRTL_PROCESS_MODULES)infoBuff;
	PRTL_PROCESS_MODULE_INFORMATION ModuleInfo = processModules->Modules;
	for (size_t i = 0; i < processModules->NumberofSections; i++)
	{
		if (strstr(ModuleInfo[i].FullPathName, szModuleName) != NULL)
		{
			SuperModuleBase = ModuleInfo[i].ImageBase;
			*ModuleSize = ModuleInfo[i].ImageSize;
			break;
		}
	}
	if (infoBuff != NULL)ExFreePool(infoBuff);
	return SuperModuleBase;
}