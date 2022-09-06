#pragma once
#include <Windows.h>
#include <iostream>

//全局变量
extern ULONG Pid;

//随机化名称
void RandDirectroyName(const char* fileType, char* targetName, int nameLen);

//字节转字符
BOOLEAN ByteToChar(const PUCHAR source, PUCHAR dest, ULONG sourceLen);

//字符转字节
BOOLEAN CharToByte(const PUCHAR source, PUCHAR dest, ULONG sourceLen);

//Win10通信
ULONG CommunicateWin10(PVOID package);

//Win7通信
ULONG CommunicateWin7(PVOID package);

//全局遍历进程
DWORD EnumProcessThreadProc(LPVOID lpParameter);
