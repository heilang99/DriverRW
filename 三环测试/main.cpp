#include <Windows.h>
#include <iostream>
#include "WindowsApiDefine.h"

int main()
{
	HMODULE hMod = NULL;
	HANDLE hFile = NULL;
	_NtQueryInformationFile MyNtQueryInformationFile = NULL;

	hMod = LoadLibraryA("ntdll.dll");
	if (hMod == NULL)return 0;
	MyNtQueryInformationFile = (_NtQueryInformationFile)GetProcAddress(hMod,"NtQueryInformationFile");
	if (MyNtQueryInformationFile == NULL)return 0;
	
	//先随便打开个文件
	hFile = CreateFileA("C:\\temp.txt", GENERIC_ALL, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == NULL)return 0;

	//目标进程信息
	HWND hwnd = FindWindowA(NULL,"PETool v1.0.0.5");
	DWORD Pid = 0;
	DWORD read = 0;
	GetWindowThreadProcessId(hwnd, &Pid);
	if (!Pid)
	{
		printf("没有找到对应进程\n");
		system("pause");
	}
	PMMREAD pMmRead = (PMMREAD)malloc(sizeof(MMREAD));
	pMmRead->ProcessId = Pid;
	pMmRead->readAddress = (PVOID)0x415757;
	pMmRead->readBuff = &read;
	pMmRead->readSize = 4;

	IO_STATUS_BLOCK ioStatus = { 0 };
	char readBuffer[0x100] = { 0 };
	((PPACKAGE)readBuffer)->communicateID = IsCr3ToCr0;
	((PPACKAGE)readBuffer)->communicateFun = DriverRead;
	((PPACKAGE)readBuffer)->content = (ULONG64)pMmRead;
	ULONG ret = MyNtQueryInformationFile(hFile, &ioStatus, readBuffer, 0x100, 52);
	if (ret == 0)
	{
		printf("通信成功\n");
		printf("%x\n", read);
	}

	system("pause");
	return 0;
}