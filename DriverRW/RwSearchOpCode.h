#pragma once
#include "RwGetModuleSuper.h"
//˽�з���
//BOOLEAN ByteToChar(const PUCHAR source, PUCHAR dest, ULONG sourceLen);
//ULONG64 SearchOpCode(ULONG64 BeginAddr, ULONG64 EndAddr, PCHAR opCode, ULONG codeSize);

//���з���
ULONG64 RwSearchOpCode(PCHAR opCode, int offset);
