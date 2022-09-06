#pragma once
#include <ntifs.h>

//ע��ص�ʵ�ֽ��̱���
NTSTATUS RwInstallProcessProtect(HANDLE pid);
NTSTATUS RwUnstallProcessProtect();

OB_PREOP_CALLBACK_STATUS PobPreOperationCallback(
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION OperationInformation
);

//�޸Ľ��̶���ͷʵ�ֽ��̱���.
NTSTATUS RwInProtectByEditHeader(HANDLE protectPid);
NTSTATUS RwUnProtectByEditHeader(HANDLE protectPid);