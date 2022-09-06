#pragma once
#include <ntifs.h>
//进程
NTSTATUS RwInstallCheckProcess();
NTSTATUS RwUnstallCheckProcess();

//线程
NTSTATUS RwInstallCheckThread();
NTSTATUS RwUnstallCheckThread();

//模块
NTSTATUS RwInstallCheckModule();
NTSTATUS RwUnstallCheckModule();