#pragma once
#include <ntifs.h>

typedef NTSTATUS(*_PsSuspendThread)(PETHREAD Thread, OUT PULONG PreviousSuspendCount);
typedef NTSTATUS(*_PsResumeThread)(PETHREAD Thread, OUT PULONG PreviousSuspendCount);

NTSTATUS RwThreadHijackWin10(HANDLE Tid,PVOID Routine);
NTSTATUS RwThreadHijack(HANDLE Pid, ULONG Argc, ULONG* Argv);