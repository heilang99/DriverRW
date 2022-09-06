#include "utilFun.h"
#include "MmRead.h"
#include "MmWrite.h"
#include "RwJudgeSysVersion.h"
#include "RwSearchOpCode.h"
#include "RwCheckMemory.h"
#include "RwProcessProtect.h"
#include "RwThreadHijack.h"

#define FConvertBetween "488B05????????7507488B05????????E8????????8BC885C07840"
#define FConvertBetweenOffset 3

ULONG64 OldDisQueryAttr = 0;
ULONG64 OldDisSetAttr = 0;
PULONG64 g_AttrInfoFunAddr = NULL;

ULONG64 OldCallBackWin10 = 0;
PULONG64 g_EnumerateAddr = NULL;

// ============================= 0环注册回调函数 =====================================
NTSTATUS RegisterCallBack()
{
	NTSTATUS status = STATUS_SUCCESS;
	if (JudgeSysIsWin7())
	{
		status = RegisterCallBackWin7();
	}
	else
	{
		status = RegisterCallBackWin10();
	}
	return status;
}
// ============================= 0环注册回调函数 WIN7=====================================
NTSTATUS RegisterCallBackWin7()
{
	NTSTATUS status = STATUS_SUCCESS;
	_ExRegisterAttributeInformationCallback registerCallBack = NULL;
	ULONG offset = 0;
	//获取ExRegisterAttributeInformationCallback函数地址
	UNICODE_STRING funName = RTL_CONSTANT_STRING(L"ExRegisterAttributeInformationCallback");
	registerCallBack = MmGetSystemRoutineAddress(&funName);

	if (registerCallBack != NULL)
	{
		//公式：下一行地址(低八位) + 偏移 = 真正地址(低八位)
		offset = *(PULONG)((ULONG64)registerCallBack + 16);
		ULONG nextLowAddr = nextLowAddr = ((ULONG64)registerCallBack + 20) & 0xFFFFFFFF;
		ULONG tagetLowerAddr = nextLowAddr + offset;
		PULONG64 pAttrInfoFunAddr = (PULONG64)(((ULONG64)registerCallBack & 0xFFFFFFFF00000000) + tagetLowerAddr);

		g_AttrInfoFunAddr = pAttrInfoFunAddr;

		//保存清空
		OldDisQueryAttr = pAttrInfoFunAddr[0];
		OldDisSetAttr = pAttrInfoFunAddr[1];
		pAttrInfoFunAddr[0] = 0;
		pAttrInfoFunAddr[1] = 0;

		//调用ExRegisterAttributeInformationCallback为两个全局变量赋值,其实自己赋值也是一样的
		RWCALL_BACK_FUNC rwCallBack = { 0 };
		rwCallBack.ExpDisQueryAttributeInformation = MyQueryAttributeInformation;
		rwCallBack.ExpDisSetAttributeInformation = MySetAttributeInformation;
		status = registerCallBack(&rwCallBack);
		DbgPrint("Win7注册回调成功");
		return status;
	}
	else
	{
		DbgPrint("Win7注册回调失败");
		return STATUS_UNSUCCESSFUL;
	}
}
// ============================= 0环注册回调函数 WIN10=====================================
NTSTATUS RegisterCallBackWin10()
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG64 offsetAddr = NULL;
	ULONG nextLowAddr = 0;
	offsetAddr = RwSearchOpCode(FConvertBetween, FConvertBetweenOffset);
	if (offsetAddr == 0)
	{
		DbgPrint("Win10注册回调失败");
		return STATUS_UNSUCCESSFUL;
	}

	//公式：下一行地址(低八位) + 偏移 = 真正地址(低八位)
	nextLowAddr = (offsetAddr + 4) & 0xFFFFFFFF;
	ULONG offset = *(PULONG)offsetAddr;
	ULONG BetweenLowerAddr = nextLowAddr+ offset;
	g_EnumerateAddr = (offsetAddr & 0xFFFFFFFF00000000) + BetweenLowerAddr;
	if (!MmIsAddressValid(g_EnumerateAddr))
	{
		DbgPrint("Win10注册回调失败");
		return STATUS_UNSUCCESSFUL;
	}
	OldCallBackWin10 = g_EnumerateAddr[0];
	g_EnumerateAddr[0] = CallbackFuncWin10;
	DbgPrint("Win10注册回调成功");
	return status;
}

// ============================= 通信函数Query Win7 =====================================
NTSTATUS MyQueryAttributeInformation(HANDLE handle, PVOID arg1)
{
	PPACKAGE package = (PPACKAGE)arg1;
	NTSTATUS status = STATUS_SUCCESS;
	if (package->communicateID = IsCr3ToCr0)
	{
		DbgPrint("QueryAttr接受到三环通信");
		status = DisPatchCallEntry(package);
	}
	else
	{
		if (OldDisQueryAttr != 0)
		{
			return ((_AttributeInformationCallback)OldDisQueryAttr)(handle, arg1);
		}
	}
	return status;
}
// ============================= 通信函数Set Win7 =====================================
NTSTATUS MySetAttributeInformation(HANDLE handle, PVOID arg1)
{
	PPACKAGE package = (PPACKAGE)arg1;
	NTSTATUS status = STATUS_SUCCESS;
	if (package->communicateID = IsCr3ToCr0)
	{
		DbgPrint("SetAttr接受到三环通信");
		status = DisPatchCallEntry(package);
	}
	else
	{
		if (OldDisSetAttr != 0)
		{
			return ((_AttributeInformationCallback)OldDisSetAttr)(handle, arg1);
		}
	}
	return status;
}

// ============================= 通信函数 Win10 =====================================
NTSTATUS CallbackFuncWin10(PVOID arg1, PVOID arg2, PVOID arg3)
{ 
	PPACKAGE package = (PPACKAGE)arg1;
	NTSTATUS status = STATUS_SUCCESS;
	if (package->communicateID = IsCr3ToCr0)
	{
		DbgPrint("Win10回调 接受到三环通信");
		status = DisPatchCallEntry(package);
	}
	else
	{
		if (OldCallBackWin10 != 0)
		{
			return ((ULONG64(__fastcall*)(PVOID, PVOID, PVOID))OldCallBackWin10)(arg1, arg2, arg3);
		}
	}
	return status;
}

// ============================= 功能分发函数 =====================================
NTSTATUS DisPatchCallEntry(PPACKAGE package)
{
	ULONG64 functionId = package->communicateFun;
	NTSTATUS status = STATUS_SUCCESS;
	switch (functionId)
	{
		case DriverRead:
		{
			DbgPrint("驱动读....");
			PMMREAD MmRead = (PMMREAD)(package->content);
			status = ReadR3MemoryByApi(MmRead->ProcessId, MmRead->readAddress, MmRead->readSize, MmRead->readBuff);
			break;
		}
		case DriverWrite:
		{
			DbgPrint("驱动写....");
			PMMWRITE MmWrite = (PMMWRITE)(package->content);
			status = WriteR3Memory(MmWrite->ProcessId, MmWrite->writeAddress, MmWrite->writeSize, MmWrite->targetWriteAddr);
			break;
		}
		case GetMMType:
		{
			DbgPrint("获取内存属性....");
			PMMTYPE MmType = (PMMTYPE)(package->content);
			status = RwCheckMemory(MmType->ProcessId, MmType->checkAddress, MmType->MMInfoBuff);
			break;
		}
		case InstallProtect:
		{
			DbgPrint("加载进程保护....");
			PINPROTECT pProtect = (PINPROTECT)(package->content);
			status = RwInstallProcessProtect(pProtect->pid);
			break;
		}
		case UnstallProtect:
		{
			DbgPrint("去掉进程保护....");
			status = RwUnstallProcessProtect();
			break;
		}
		case ModifyHeaderFlag:
		{
			PINPROTECT pProtect = (PINPROTECT)(package->content);
			status = RwInProtectByEditHeader(pProtect->pid);
			break;
		}
		case RestoreHeaderFlag:
		{
			PINPROTECT pProtect = (PINPROTECT)(package->content);
			status = RwUnProtectByEditHeader(pProtect->pid);
			break;
		}
		case InJectThread:
		{
			PINJECTTHREAD pProtect = (PINJECTTHREAD)(package->content);
			status = RwThreadHijack(pProtect->pid, pProtect->Argc, pProtect->Arg);
			break;
		}
		default:
		{
			DbgPrint("其他功能...");
			break;
		}
	}
	return status;
}
// ============================= 卸载回调函数 =====================================
void UnloadCallBack()
{
	if (JudgeSysIsWin7())
	{
		UnloadCallBackWin7();
	}
	else
	{
		UnloadCallBackWin10();
	}
}
void UnloadCallBackWin7()
{
	if (g_AttrInfoFunAddr != NULL)
	{
		DbgPrint("卸载CallBack函数 Win7");
		g_AttrInfoFunAddr[0] = OldDisQueryAttr;
		g_AttrInfoFunAddr[1] = OldDisSetAttr;
		g_AttrInfoFunAddr = NULL;
	}
}
void UnloadCallBackWin10()
{
	if(g_EnumerateAddr !=NULL)
	{
		DbgPrint("卸载CallBack函数 Win10");
		g_EnumerateAddr[0] = OldCallBackWin10;
		g_EnumerateAddr = NULL;
	}
}