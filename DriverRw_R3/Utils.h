#pragma once
#include <Windows.h>
#include <iostream>

//ȫ�ֱ���
extern ULONG Pid;

//���������
void RandDirectroyName(const char* fileType, char* targetName, int nameLen);

//�ֽ�ת�ַ�
BOOLEAN ByteToChar(const PUCHAR source, PUCHAR dest, ULONG sourceLen);

//�ַ�ת�ֽ�
BOOLEAN CharToByte(const PUCHAR source, PUCHAR dest, ULONG sourceLen);

//Win10ͨ��
ULONG CommunicateWin10(PVOID package);

//Win7ͨ��
ULONG CommunicateWin7(PVOID package);

//ȫ�ֱ�������
DWORD EnumProcessThreadProc(LPVOID lpParameter);
