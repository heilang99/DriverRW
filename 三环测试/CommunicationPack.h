#pragma once
#include <Windows.h>
#include <iostream>
//================ 约定通信结构体 ======================
typedef enum _COMMUNICATE_NUMBER
{
    IsCr3ToCr0 = 0x1234,
    DriverRead
}COMMUNICATE_NUMBER, * PCOMMUNICATE_NUMBER;

typedef struct _PACKAGE
{
    COMMUNICATE_NUMBER communicateID;
    COMMUNICATE_NUMBER communicateFun;
    ULONG64 content;
    ULONG64 length;
    ULONG64 result;
}PACKAGE, * PPACKAGE;

//读数据结构体
typedef struct _MMREAD
{
    ULONG64 ProcessId;
    PVOID readAddress;
    ULONG64 readSize;
    PVOID readBuff;
}MMREAD, * PMMREAD;