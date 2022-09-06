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

// ============================= 0��ע��ص����� =====================================
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
// ============================= 0��ע��ص����� WIN7=====================================
NTSTATUS RegisterCallBackWin7()
{
	NTSTATUS status = STATUS_SUCCESS;
	_ExRegisterAttributeInformationCallback registerCallBack = NULL;
	ULONG offset = 0;
	//��ȡExRegisterAttributeInformationCallback������ַ
	UNICODE_STRING funName = RTL_CONSTANT_STRING(L"ExRegisterAttributeInformationCallback");
	registerCallBack = MmGetSystemRoutineAddress(&funName);

	if (registerCallBack != NULL)
	{
		//��ʽ����һ�е�ַ(�Ͱ�λ) + ƫ�� = ������ַ(�Ͱ�λ)
		offset = *(PULONG)((ULONG64)registerCallBack + 16);
		ULONG nextLowAddr = nextLowAddr = ((ULONG64)registerCallBack + 20) & 0xFFFFFFFF;
		ULONG tagetLowerAddr = nextLowAddr + offset;
		PULONG64 pAttrInfoFunAddr = (PULONG64)(((ULONG64)registerCallBack & 0xFFFFFFFF00000000) + tagetLowerAddr);

		g_AttrInfoFunAddr = pAttrInfoFunAddr;

		//�������
		OldDisQueryAttr = pAttrInfoFunAddr[0];
		OldDisSetAttr = pAttrInfoFunAddr[1];
		pAttrInfoFunAddr[0] = 0;
		pAttrInfoFunAddr[1] = 0;

		//����ExRegisterAttributeInformationCallbackΪ����ȫ�ֱ�����ֵ,��ʵ�Լ���ֵҲ��һ����
		RWCALL_BACK_FUNC rwCallBack = { 0 };
		rwCallBack.ExpDisQueryAttributeInformation = MyQueryAttributeInformation;
		rwCallBack.ExpDisSetAttributeInformation = MySetAttributeInformation;
		status = registerCallBack(&rwCallBack);
		DbgPrint("Win7ע��ص��ɹ�");
		return status;
	}
	else
	{
		DbgPrint("Win7ע��ص�ʧ��");
		return STATUS_UNSUCCESSFUL;
	}
}
// ============================= 0��ע��ص����� WIN10=====================================
NTSTATUS RegisterCallBackWin10()
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG64 offsetAddr = NULL;
	ULONG nextLowAddr = 0;
	offsetAddr = RwSearchOpCode(FConvertBetween, FConvertBetweenOffset);
	if (offsetAddr == 0)
	{
		DbgPrint("Win10ע��ص�ʧ��");
		return STATUS_UNSUCCESSFUL;
	}

	//��ʽ����һ�е�ַ(�Ͱ�λ) + ƫ�� = ������ַ(�Ͱ�λ)
	nextLowAddr = (offsetAddr + 4) & 0xFFFFFFFF;
	ULONG offset = *(PULONG)offsetAddr;
	ULONG BetweenLowerAddr = nextLowAddr+ offset;
	g_EnumerateAddr = (offsetAddr & 0xFFFFFFFF00000000) + BetweenLowerAddr;
	if (!MmIsAddressValid(g_EnumerateAddr))
	{
		DbgPrint("Win10ע��ص�ʧ��");
		return STATUS_UNSUCCESSFUL;
	}
	OldCallBackWin10 = g_EnumerateAddr[0];
	g_EnumerateAddr[0] = CallbackFuncWin10;
	DbgPrint("Win10ע��ص��ɹ�");
	return status;
}

// ============================= ͨ�ź���Query Win7 =====================================
NTSTATUS MyQueryAttributeInformation(HANDLE handle, PVOID arg1)
{
	PPACKAGE package = (PPACKAGE)arg1;
	NTSTATUS status = STATUS_SUCCESS;
	if (package->communicateID = IsCr3ToCr0)
	{
		DbgPrint("QueryAttr���ܵ�����ͨ��");
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
// ============================= ͨ�ź���Set Win7 =====================================
NTSTATUS MySetAttributeInformation(HANDLE handle, PVOID arg1)
{
	PPACKAGE package = (PPACKAGE)arg1;
	NTSTATUS status = STATUS_SUCCESS;
	if (package->communicateID = IsCr3ToCr0)
	{
		DbgPrint("SetAttr���ܵ�����ͨ��");
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

// ============================= ͨ�ź��� Win10 =====================================
NTSTATUS CallbackFuncWin10(PVOID arg1, PVOID arg2, PVOID arg3)
{ 
	PPACKAGE package = (PPACKAGE)arg1;
	NTSTATUS status = STATUS_SUCCESS;
	if (package->communicateID = IsCr3ToCr0)
	{
		DbgPrint("Win10�ص� ���ܵ�����ͨ��");
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

// ============================= ���ַܷ����� =====================================
NTSTATUS DisPatchCallEntry(PPACKAGE package)
{
	ULONG64 functionId = package->communicateFun;
	NTSTATUS status = STATUS_SUCCESS;
	switch (functionId)
	{
		case DriverRead:
		{
			DbgPrint("������....");
			PMMREAD MmRead = (PMMREAD)(package->content);
			status = ReadR3MemoryByApi(MmRead->ProcessId, MmRead->readAddress, MmRead->readSize, MmRead->readBuff);
			break;
		}
		case DriverWrite:
		{
			DbgPrint("����д....");
			PMMWRITE MmWrite = (PMMWRITE)(package->content);
			status = WriteR3Memory(MmWrite->ProcessId, MmWrite->writeAddress, MmWrite->writeSize, MmWrite->targetWriteAddr);
			break;
		}
		case GetMMType:
		{
			DbgPrint("��ȡ�ڴ�����....");
			PMMTYPE MmType = (PMMTYPE)(package->content);
			status = RwCheckMemory(MmType->ProcessId, MmType->checkAddress, MmType->MMInfoBuff);
			break;
		}
		case InstallProtect:
		{
			DbgPrint("���ؽ��̱���....");
			PINPROTECT pProtect = (PINPROTECT)(package->content);
			status = RwInstallProcessProtect(pProtect->pid);
			break;
		}
		case UnstallProtect:
		{
			DbgPrint("ȥ�����̱���....");
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
			DbgPrint("��������...");
			break;
		}
	}
	return status;
}
// ============================= ж�ػص����� =====================================
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
		DbgPrint("ж��CallBack���� Win7");
		g_AttrInfoFunAddr[0] = OldDisQueryAttr;
		g_AttrInfoFunAddr[1] = OldDisSetAttr;
		g_AttrInfoFunAddr = NULL;
	}
}
void UnloadCallBackWin10()
{
	if(g_EnumerateAddr !=NULL)
	{
		DbgPrint("ж��CallBack���� Win10");
		g_EnumerateAddr[0] = OldCallBackWin10;
		g_EnumerateAddr = NULL;
	}
}