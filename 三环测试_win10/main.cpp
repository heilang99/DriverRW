#include <Windows.h>
#include <iostream>
#include "resource.h"
//================ Լ��ͨ�Žṹ�� ======================
typedef enum _COMMUNICATE_NUMBER
{
	IsCr3ToCr0 = 0x1234,
	DriverRead,
	DriverWrite,
	GetMMType,
	InstallProtect,
	UnstallProtect
}COMMUNICATE_NUMBER, * PCOMMUNICATE_NUMBER;

typedef struct _PACKAGE
{
	COMMUNICATE_NUMBER communicateID;
	COMMUNICATE_NUMBER communicateFun;
	ULONG64 content;
	ULONG64 length;
	ULONG64 result;
}PACKAGE, * PPACKAGE;

//������ͨ�� �ṹ��
typedef struct _MMREAD
{
	ULONG ProcessId;
	PVOID readAddress;
	ULONG readSize;
	PVOID readBuff;
}MMREAD, * PMMREAD;

//д����ͨ�� �ṹ��
typedef struct _MMWRITE
{
	ULONG64 ProcessId;
	PVOID writeAddress;
	ULONG64 writeSize;
	PVOID targetWriteAddr;
}MMWRITE, * PMMWRITE;

//�ڴ���Ϣͨ�� �ṹ��
typedef struct _MMTYPE {
	ULONG64 ProcessId;
	PVOID checkAddress;
	PVOID MMInfoBuff;
} MMTYPE, * PMMTYPE;

//���̱���ͨ�� �ṹ��
typedef struct _INPROTECT {
	HANDLE pid;
} INPROTECT, * PINPROTECT;

//�Զ����ڴ���Ϣ�ṹ
typedef struct _MYMEMORY_BASIC_INFORMATION {
ULONG64 BaseAddress;
ULONG64 AllocationBase;
ULONG64 AllocationProtect;
ULONG64 RegionSize;
ULONG64 State;
ULONG64 Protect;
ULONG64 Type;
} MYMEMORY_BASIC_INFORMATION, * PMYMEMORY_BASIC_INFORMATION;

//Win10����ͨ��
typedef ULONG64(_fastcall* _FuncPos)(char, PVOID, PVOID, PVOID);

//��װ����
BOOL InstallDriver(const char* lpszDriverName, const char* serviceName)
{
	//�򿪷������
	SC_HANDLE hSc = NULL;
	hSc = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSc == NULL)
	{
		printf("�򿪷�����ƹ�����ʧ��\n");
		return FALSE;
	}

	//��������
	SC_HANDLE hService = NULL;
	hService = CreateServiceA(hSc, serviceName, serviceName, SC_MANAGER_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, lpszDriverName, NULL, NULL, NULL, NULL, NULL);
	if (hService == NULL)
	{
		printf("������������ʧ��\n");
		CloseServiceHandle(hSc);
		return FALSE;
	}

	//��������
	if (!StartServiceA(hService, NULL, NULL))
	{
		printf("������������ʧ��.%d\n", GetLastError());
		return FALSE;
	}

	printf("������������ɹ�.\n");
	CloseServiceHandle(hSc);
	CloseServiceHandle(hService);
	return TRUE;
}

//ж������
BOOL UnstallDriver(const char* lpServiceName)
{
	SC_HANDLE hServiceMgr = OpenSCManagerA(0, 0, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenServiceA(hServiceMgr, lpServiceName, SERVICE_ALL_ACCESS);
	if (hServiceMgr == NULL || hService == NULL)
	{
		return FALSE;
	}

	//ֹͣ����
	SERVICE_STATUS serviceStatus = { 0 };
	ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);

	//ж������
	DeleteService(hService);

	//�رվ��
	printf("ж����������ɹ�.\n");
	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceMgr);
	return TRUE;
}

//�������
void RandDirectroyName(const char* fileType, char* targetName, int nameLen)
{
	memset(targetName, 0, nameLen);
	//���������ʱ������
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

//��װ��Դ
BOOL InstallResource(LPCSTR lpFileName)
{
	//1���õ���ǰ���̵�����
	HRSRC hRsc = FindResourceA(NULL, MAKEINTRESOURCEA(IDR_MYSYS31), "mysys3");
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
		printf("�ɹ���������������\n");
	}
	else
	{
		printf("�������������� Error\n");
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
		printf("��ж����Դ\n");
		return TRUE;
	}
	return FALSE;
}

// ================ ������ ======================
int main()
{
	//// ================ ��װ������������Ƕ�뵽exe�� ================ 
	//char driverName[11] = { 0 };
	//char serviceName[6] = { 0 };
	//char szDriverFullPath[MAX_PATH] = { 0 };
	//RandDirectroyName(".sys", driverName, 11);
	//RandDirectroyName("", serviceName, 6);
	//GetFullPathNameA(driverName, MAX_PATH, szDriverFullPath, NULL);
	//
	////��װ��Դ
	//InstallResource(szDriverFullPath);

	////��װ����
	//InstallDriver(szDriverFullPath, serviceName);

	// ================================ ͨ�� ================================ 
	HMODULE hMod = NULL;
	hMod = LoadLibraryA("ntdll.dll");
	_FuncPos  NtConvertBetween = (_FuncPos)GetProcAddress(hMod, "NtConvertBetweenAuxiliaryCounterAndPerformanceCounter");

	//Ŀ�������Ϣ
	HWND hwnd = FindWindowA(NULL, "PETool v1.0.0.5");
	DWORD Pid = 0;
	printf("Pid: ");
	scanf("%d", &Pid);
	DWORD read = 0;
	//GetWindowThreadProcessId(hwnd, &Pid);
	if (!Pid)
	{
		printf("û���ҵ���Ӧ����\n");
		/*UnstallDriver(serviceName);
		if (UnstallResource(szDriverFullPath))
		{
			system("pause");
			return 0;
		}*/
		system("pause");
		return 0;
	}

	//��ʼ����ȡ�ṹ��
	PINPROTECT pProcetect = (PINPROTECT)malloc(sizeof(INPROTECT));
	pProcetect->pid = (HANDLE)Pid;

	PPACKAGE package = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(package, 0, sizeof(PACKAGE));
	package->communicateID = IsCr3ToCr0;
	package->communicateFun = InstallProtect;
	package->content = (ULONG64)pProcetect;

	ULONG64 arg = 0;
	ULONG64 ret = NtConvertBetween(1,&package,&arg,&arg);
	if (ret == 0)
	{
		printf("���̱����ɹ�\n");
	}

	system("pause");

	//��ʼ��д��ṹ��
	LPVOID writeAddr = 0;
	printf("\nPid������writeAddr:");
	scanf("%x", &writeAddr);

	float writeBuff = 5000;
	PMMWRITE pMmWrite = (PMMWRITE)malloc(sizeof(MMWRITE));
	pMmWrite->ProcessId = Pid;
	pMmWrite->writeAddress = &writeBuff;
	pMmWrite->writeSize = 4;
	pMmWrite->targetWriteAddr = (PVOID)writeAddr;

	//��ʼ��ͨ�Ű��ṹ��
	PPACKAGE package2 = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(package2, 0, sizeof(PACKAGE));
	package2->communicateID = IsCr3ToCr0;
	package2->communicateFun = DriverWrite;
	package2->content = (ULONG64)pMmWrite;

	//�ڶ����������� ָ���ָ��
	ULONG64 arg2 = 0;
	ULONG64 ret2 = NtConvertBetween(1,&package2,&arg2,&arg2);

	//�������
	if (ret2 == 0)
	{
		printf("д��ɹ� %llx\n", ret);
	}
	free(pMmWrite);
	free(package);

	system("pause");

	PPACKAGE package1 = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(package1, 0, sizeof(PACKAGE));
	package1->communicateID = IsCr3ToCr0;
	package1->communicateFun = UnstallProtect;
	package1->content = (ULONG64)pProcetect;

	ULONG64 arg1 = 0;
	ULONG64 ret1 = NtConvertBetween(1, &package1, &arg1, &arg1);
	if (ret1 == 0)
	{
		printf("������̱����ɹ�\n");
	}

	// ================================ ����ͨ�� ================================
	//system("pause");
	//PMYMEMORY_BASIC_INFORMATION pMemInfo = (PMYMEMORY_BASIC_INFORMATION)malloc(sizeof(MYMEMORY_BASIC_INFORMATION));
	//memset(pMemInfo, 0, sizeof(MYMEMORY_BASIC_INFORMATION));

	////��ʼ���ڴ���Ϣ�ṹ��
	//PMMTYPE pMmType = (PMMTYPE)malloc(sizeof(MMTYPE));
	//pMmType->ProcessId = Pid;
	//pMmType->checkAddress = (PVOID)0x400000;
	//pMmType->MMInfoBuff = pMemInfo;

	////��ʼ��ͨ�Ű��ṹ��
	//PPACKAGE package2 = (PPACKAGE)malloc(sizeof(PACKAGE));
	//memset(package2, 0, sizeof(PACKAGE));
	//package2->communicateID = IsCr3ToCr0;
	//package2->communicateFun = GetMMType;
	//package2->content = (ULONG64)pMmType;

	////�ڶ����������� ָ���ָ��
	//ULONG64 ret2 = NtConvertBetween(1, &package2, &arg, &arg);

	////�����ȡ����
	//printf("AllocationBase: %llx\n", pMemInfo->AllocationBase);
	//printf("AllocationProtect: %llx\n", pMemInfo->AllocationProtect);
	//printf("BaseAddress: %llx\n", pMemInfo->BaseAddress);
	//printf("Protect: %llx\n", pMemInfo->Protect);
	//printf("RegionSize: %llx\n", pMemInfo->RegionSize);
	//printf("State: %llx\n", pMemInfo->State);
	//printf("Type: %llx\n", pMemInfo->Type);

	//free(pMemInfo);
	//free(pMmType);
	//free(package2);

	////ж������
	//UnstallDriver(serviceName);

	////ɾ�������ļ�
	//UnstallResource(szDriverFullPath);

	system("pause");
	return 0;
}