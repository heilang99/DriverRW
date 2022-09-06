#include "InstallResource.h"
//安装资源
BOOL InstallResource(LPCSTR lpFileName,LPCSTR lpName, LPCSTR lpType)
{
	//1、得到当前进程的驱动
	HRSRC hRsc = FindResourceA(NULL, lpName, lpType);
	if (hRsc == NULL)
	{
		return FALSE;
	}
	//2、加载到内存
	HGLOBAL hGlobal = LoadResource(NULL, hRsc);
	if (hGlobal == NULL)
	{
		return FALSE;
	}
	//3、得到内存中的指针
	LPVOID resourceAddr = NULL;
	resourceAddr = LockResource(hGlobal);
	if (resourceAddr == NULL)
	{
		return FALSE;
	}
	//4、得到资源大小
	DWORD resSize = SizeofResource(NULL, hRsc);

	//将.sys写到文件中
	HANDLE hFile = NULL;
	DWORD realSize = 0;
	hFile = CreateFileA(lpFileName, GENERIC_ALL, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
	{
		return FALSE;
	}
	if (WriteFile(hFile, resourceAddr, resSize, &realSize, NULL))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	//关闭句柄
	CloseHandle(hFile);
	CloseHandle(hRsc);
}

//卸载资源
BOOL UnstallResource(LPCSTR lpFileName)
{
	if (DeleteFileA(lpFileName))
	{
		return TRUE;
	}
	return FALSE;
}