#include <ntifs.h>
#include "utilFun.h"
#include "RwProcessProtect.h"
void Unload(PDRIVER_OBJECT pDriver)
{
	NTSTATUS status = STATUS_SUCCESS;
	UnloadCallBack();
	//注意，这是回调，得卸载
	RwUnstallProcessProtect();
	DbgPrint("驱动卸载");

}
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver,PUNICODE_STRING pRegPath)
{
	NTSTATUS status = STATUS_SUCCESS;
	pDriver->DriverUnload = Unload;
	status = RegisterCallBack();
	if (!NT_SUCCESS(status))
	{
		DbgPrint("注册通信回调函数失败");
	}
	return STATUS_SUCCESS;
}