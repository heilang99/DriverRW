#include "InstallResource.h"
//��װ��Դ
BOOL InstallResource(LPCSTR lpFileName,LPCSTR lpName, LPCSTR lpType)
{
	//1���õ���ǰ���̵�����
	HRSRC hRsc = FindResourceA(NULL, lpName, lpType);
	if (hRsc == NULL)
	{
		return FALSE;
	}
	//2�����ص��ڴ�
	HGLOBAL hGlobal = LoadResource(NULL, hRsc);
	if (hGlobal == NULL)
	{
		return FALSE;
	}
	//3���õ��ڴ��е�ָ��
	LPVOID resourceAddr = NULL;
	resourceAddr = LockResource(hGlobal);
	if (resourceAddr == NULL)
	{
		return FALSE;
	}
	//4���õ���Դ��С
	DWORD resSize = SizeofResource(NULL, hRsc);

	//��.sysд���ļ���
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

	//�رվ��
	CloseHandle(hFile);
	CloseHandle(hRsc);
}

//ж����Դ
BOOL UnstallResource(LPCSTR lpFileName)
{
	if (DeleteFileA(lpFileName))
	{
		return TRUE;
	}
	return FALSE;
}