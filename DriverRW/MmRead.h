#pragma once
#include <ntifs.h>
//º¯Êý MmCopyVirtualMemory
extern NTSTATUS MmCopyVirtualMemory(
    IN PEPROCESS FromProcess,
    IN CONST VOID* FromAddress,
    IN PEPROCESS ToProcess,
    OUT PVOID ToAddress,
    IN SIZE_T BufferSize,
    IN KPROCESSOR_MODE PreviousMode,
    OUT PSIZE_T NumberOfBytesCopied
);

//º¯ÊýÉùÃ÷
NTSTATUS ReadR3Memory(HANDLE processId, PVOID readAddress, ULONG64 readSize, PVOID readBuff);
NTSTATUS ReadR3MemoryByApi(HANDLE processId, PVOID readAddress, ULONG64 readSize, PVOID readBuff);
NTSTATUS ReadR3MemoryByMDL(HANDLE processId, PVOID readAddress, ULONG64 readSize, PVOID readBuff);
PVOID MmMdlMapped(PVOID VirAddr, ULONG Length, OUT PMDL* pMdl);
void MmMdlUnMapped(PVOID VirAddr, IN PMDL pMdl);