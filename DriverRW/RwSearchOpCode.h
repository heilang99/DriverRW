#pragma once
#include "RwGetModuleSuper.h"
//私有方法
//BOOLEAN ByteToChar(const PUCHAR source, PUCHAR dest, ULONG sourceLen);
//ULONG64 SearchOpCode(ULONG64 BeginAddr, ULONG64 EndAddr, PCHAR opCode, ULONG codeSize);

//公有方法
ULONG64 RwSearchOpCode(PCHAR opCode, int offset);
