#pragma once
#include <ntifs.h>

//注册回调实现进程保护
NTSTATUS RwInstallProcessProtect(HANDLE pid);
NTSTATUS RwUnstallProcessProtect();

OB_PREOP_CALLBACK_STATUS PobPreOperationCallback(
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION OperationInformation
);

//修改进程对象头实现进程保护.
NTSTATUS RwInProtectByEditHeader(HANDLE protectPid);
NTSTATUS RwUnProtectByEditHeader(HANDLE protectPid);