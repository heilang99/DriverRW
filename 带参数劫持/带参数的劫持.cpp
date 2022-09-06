#include <Windows.h>
#include <iostream>

//线程劫持通信 结构体
typedef struct _INJECTTHREAD {
	HANDLE pid;
	ULONG funcAddr;
	ULONG Argc;
	ULONG Arg[4];
} INJECTTHREAD, * PINJECTTHREAD;

void func1()
{
	MessageBox(0, L"线程劫持了", L"提示", MB_OK);
	char readBuff[0x100]{ 0 };
	HANDLE hFile = 0;
	DWORD realWrite{ 0 };
	DWORD realRead{ 0 };
	PINJECTTHREAD pInject = NULL;
	DWORD funcAddr = 0;
 	hFile = CreateFileW(L"\\\\.\\Nioce", GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	else
	{
		DWORD ret = ReadFile(hFile, readBuff, 0x100, &realRead, NULL);
		if (ret)
		{
			pInject = (PINJECTTHREAD)readBuff;
		}
		CloseHandle(hFile);
	}
	funcAddr = pInject->funcAddr;
	DWORD arg = 0;
	for (size_t i = 0; i < pInject->Argc; i++)
	{
		arg = pInject->Arg[i];
		__asm push arg;
	}
	__asm call funcAddr;
}