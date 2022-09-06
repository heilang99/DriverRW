#pragma once
#include <ntifs.h>

//函数声明
NTSTATUS WriteR3Memory(HANDLE processId, PVOID writeAddress, ULONG writeSize, PVOID targetWriteAddr);

//MDL  函数再Mmread.c里面
extern PVOID MmMdlMapped(PVOID VirAddr, ULONG Length, OUT PMDL* pMdl);
extern void MmMdlUnMapped(PVOID VirAddr, IN PMDL pMdl);