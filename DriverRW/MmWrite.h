#pragma once
#include <ntifs.h>

//��������
NTSTATUS WriteR3Memory(HANDLE processId, PVOID writeAddress, ULONG writeSize, PVOID targetWriteAddr);

//MDL  ������Mmread.c����
extern PVOID MmMdlMapped(PVOID VirAddr, ULONG Length, OUT PMDL* pMdl);
extern void MmMdlUnMapped(PVOID VirAddr, IN PMDL pMdl);