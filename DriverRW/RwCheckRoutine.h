#pragma once
#include <ntifs.h>
//����
NTSTATUS RwInstallCheckProcess();
NTSTATUS RwUnstallCheckProcess();

//�߳�
NTSTATUS RwInstallCheckThread();
NTSTATUS RwUnstallCheckThread();

//ģ��
NTSTATUS RwInstallCheckModule();
NTSTATUS RwUnstallCheckModule();