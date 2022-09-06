#include <ntifs.h>
#include "utilFun.h"
#include "RwProcessProtect.h"
void Unload(PDRIVER_OBJECT pDriver)
{
	NTSTATUS status = STATUS_SUCCESS;
	UnloadCallBack();
	//ע�⣬���ǻص�����ж��
	RwUnstallProcessProtect();
	DbgPrint("����ж��");

}
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver,PUNICODE_STRING pRegPath)
{
	NTSTATUS status = STATUS_SUCCESS;
	pDriver->DriverUnload = Unload;
	status = RegisterCallBack();
	if (!NT_SUCCESS(status))
	{
		DbgPrint("ע��ͨ�Żص�����ʧ��");
	}
	return STATUS_SUCCESS;
}