#include "Utils.h"
#include "StructInfo.h"
#include <TLHelp32.h>

//名称随机
void RandDirectroyName(const char* fileType, char* targetName, int nameLen)
{
	memset(targetName, 0, nameLen);
	//定义随机的时间种子
	srand(time(NULL));
	int sysLen = strlen(fileType) + 1;
	char Directroy[12] = { 'a','2','w','W','R','T','E','U','m','O','P','G' };
	for (int i = 0; i < nameLen - sysLen; i++)
	{
		int index = rand() % 12;
		targetName[i] = Directroy[index];
	}
	strcat_s(targetName, nameLen, fileType);
}

//字节转换成字符串
BOOLEAN ByteToChar(const PUCHAR source, PUCHAR dest, ULONG sourceLen)
{
	UCHAR highByte = 0;
	UCHAR lowByte = 0;
	for (ULONG i = 0; i < sourceLen; i++)
	{
		//分割字节，高低位
		highByte = source[i] >> 4;
		lowByte = source[i] & 0x0f;
		//处理高位
		highByte += 0x30;
		if (highByte > 0x39)
			dest[i * 2] = highByte + 0x07;
		else
			dest[i * 2] = highByte;
		//处理低位
		lowByte += 0x30;
		if (lowByte > 0x39)
			dest[i * 2 + 1] = lowByte + 0x07;
		else
			dest[i * 2 + 1] = lowByte;
	}
	return TRUE;
}

//字符转字节
BOOLEAN CharToByte(const PUCHAR source, PUCHAR dest, ULONG sourceLen)
{
	UCHAR Overten = 0;
	UCHAR Lowerten = 0;
	for (ULONG i = 0; i < sourceLen; i++)
	{

		if (source[i] > 0x39)
		{
			Overten = source[i] - 0x37;
			if (i % 2 == 0)
			{
				dest[i / 2] = (Overten << 4);
			}
			else
			{
				dest[i / 2] = dest[i / 2] + (Overten & 0x0000FFFF);
			}
		}
		else
		{
			Lowerten = source[i] - 0x30;
			if (i % 2 == 0)
			{
				dest[i / 2] = (Lowerten << 4);
			}
			else
			{
				dest[i / 2] = dest[i / 2] + (Lowerten & 0x0000FFFF);
			}
		}
	}
	return TRUE;
}

//Win10通信
ULONG CommunicateWin10(PVOID package)
{
	ULONG ret = 0;
	ULONG64 arg = 0;
	HMODULE hMod = NULL;
	_FuncPos  NtConvertBetween = NULL;

	hMod = LoadLibraryA("ntdll.dll");
	if (hMod == NULL)
	{
		return STATUS_INVALID_HANDLE;
	}
	NtConvertBetween = (_FuncPos)GetProcAddress(hMod, "NtConvertBetweenAuxiliaryCounterAndPerformanceCounter");
	if (NtConvertBetween == NULL)
	{
		return STATUS_INVALID_HANDLE;
	}
	ret = NtConvertBetween(1, &package, &arg, &arg);
	return ret;
}

//Win7通信
ULONG CommunicateWin7(PVOID package)
{
	ULONG ret = 0;
	HMODULE hMod = NULL;
	HANDLE hFile = NULL;
	_NtQueryInformationFile MyNtQueryInformationFile = NULL;
	hMod = LoadLibraryW(L"ntdll.dll");
	if (hMod == NULL)
	{
		return STATUS_INVALID_HANDLE;
	}
	MyNtQueryInformationFile = (_NtQueryInformationFile)GetProcAddress(hMod, "NtQueryInformationFile");
	if (MyNtQueryInformationFile == NULL)
	{
		return STATUS_INVALID_HANDLE;
	}
	hFile = CreateFileA("C:\\temp.txt", GENERIC_ALL, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
	{
		return STATUS_INVALID_HANDLE;
	}
	IO_STATUS_BLOCK ioStatus = { 0 };
	char Buffer[0x100] = { 0 };
	memcpy(Buffer, package, sizeof(PACKAGE));
	//这个函数返回的Buffer要大一些，倒一手
	ret = MyNtQueryInformationFile(hFile, &ioStatus, Buffer, 0x100, 52);
	CloseHandle(hFile);
	return ret;
}

//遍历进程，如果关闭目标进程则PID赋0
DWORD EnumProcessThreadProc(LPVOID lpParameter)
{
	while (true)
	{
		BOOL Processflag = FALSE;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			return 0;
		}
		PROCESSENTRY32W ProcessEntry{ sizeof(PROCESSENTRY32W) };
		BOOL judge = Process32FirstW(hSnapshot, &ProcessEntry);
		if (judge)
		{
			do
			{
				if (ProcessEntry.th32ProcessID == Pid && Pid != 0)
				{
					Processflag = TRUE;
				}

			} while (Process32NextW(hSnapshot, &ProcessEntry));
		}
		if (Processflag == FALSE)
		{
			Pid = 0;
		}
		CloseHandle(hSnapshot);
		Sleep(2000);
	}
	return 0;
}