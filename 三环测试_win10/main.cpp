#include <Windows.h>
#include <iostream>
#include "resource.h"
//================ 约定通信结构体 ======================
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

//读数据通信 结构体
typedef struct _MMREAD
{
	ULONG ProcessId;
	PVOID readAddress;
	ULONG readSize;
	PVOID readBuff;
}MMREAD, * PMMREAD;

//写数据通信 结构体
typedef struct _MMWRITE
{
	ULONG64 ProcessId;
	PVOID writeAddress;
	ULONG64 writeSize;
	PVOID targetWriteAddr;
}MMWRITE, * PMMWRITE;

//内存信息通信 结构体
typedef struct _MMTYPE {
	ULONG64 ProcessId;
	PVOID checkAddress;
	PVOID MMInfoBuff;
} MMTYPE, * PMMTYPE;

//进程保护通信 结构体
typedef struct _INPROTECT {
	HANDLE pid;
} INPROTECT, * PINPROTECT;

//自定义内存信息结构
typedef struct _MYMEMORY_BASIC_INFORMATION {
ULONG64 BaseAddress;
ULONG64 AllocationBase;
ULONG64 AllocationProtect;
ULONG64 RegionSize;
ULONG64 State;
ULONG64 Protect;
ULONG64 Type;
} MYMEMORY_BASIC_INFORMATION, * PMYMEMORY_BASIC_INFORMATION;

//Win10驱动通信
typedef ULONG64(_fastcall* _FuncPos)(char, PVOID, PVOID, PVOID);

//安装驱动
BOOL InstallDriver(const char* lpszDriverName, const char* serviceName)
{
	//打开服务管理
	SC_HANDLE hSc = NULL;
	hSc = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSc == NULL)
	{
		printf("打开服务控制管理器失败\n");
		return FALSE;
	}

	//创建服务
	SC_HANDLE hService = NULL;
	hService = CreateServiceA(hSc, serviceName, serviceName, SC_MANAGER_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, lpszDriverName, NULL, NULL, NULL, NULL, NULL);
	if (hService == NULL)
	{
		printf("创建驱动服务失败\n");
		CloseServiceHandle(hSc);
		return FALSE;
	}

	//启动服务
	if (!StartServiceA(hService, NULL, NULL))
	{
		printf("运行驱动服务失败.%d\n", GetLastError());
		return FALSE;
	}

	printf("运行驱动服务成功.\n");
	CloseServiceHandle(hSc);
	CloseServiceHandle(hService);
	return TRUE;
}

//卸载驱动
BOOL UnstallDriver(const char* lpServiceName)
{
	SC_HANDLE hServiceMgr = OpenSCManagerA(0, 0, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenServiceA(hServiceMgr, lpServiceName, SERVICE_ALL_ACCESS);
	if (hServiceMgr == NULL || hService == NULL)
	{
		return FALSE;
	}

	//停止驱动
	SERVICE_STATUS serviceStatus = { 0 };
	ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);

	//卸载驱动
	DeleteService(hService);

	//关闭句柄
	printf("卸载驱动服务成功.\n");
	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceMgr);
	return TRUE;
}

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

//安装资源
BOOL InstallResource(LPCSTR lpFileName)
{
	//1、得到当前进程的驱动
	HRSRC hRsc = FindResourceA(NULL, MAKEINTRESOURCEA(IDR_MYSYS31), "mysys3");
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
		printf("成功加载驱动到磁盘\n");
	}
	else
	{
		printf("加载驱动到磁盘 Error\n");
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
		printf("已卸载资源\n");
		return TRUE;
	}
	return FALSE;
}

// ================ 主函数 ======================
int main()
{
	//// ================ 安装驱动，将驱动嵌入到exe中 ================ 
	//char driverName[11] = { 0 };
	//char serviceName[6] = { 0 };
	//char szDriverFullPath[MAX_PATH] = { 0 };
	//RandDirectroyName(".sys", driverName, 11);
	//RandDirectroyName("", serviceName, 6);
	//GetFullPathNameA(driverName, MAX_PATH, szDriverFullPath, NULL);
	//
	////安装资源
	//InstallResource(szDriverFullPath);

	////安装驱动
	//InstallDriver(szDriverFullPath, serviceName);

	// ================================ 通信 ================================ 
	HMODULE hMod = NULL;
	hMod = LoadLibraryA("ntdll.dll");
	_FuncPos  NtConvertBetween = (_FuncPos)GetProcAddress(hMod, "NtConvertBetweenAuxiliaryCounterAndPerformanceCounter");

	//目标进程信息
	HWND hwnd = FindWindowA(NULL, "PETool v1.0.0.5");
	DWORD Pid = 0;
	printf("Pid: ");
	scanf("%d", &Pid);
	DWORD read = 0;
	//GetWindowThreadProcessId(hwnd, &Pid);
	if (!Pid)
	{
		printf("没有找到对应进程\n");
		/*UnstallDriver(serviceName);
		if (UnstallResource(szDriverFullPath))
		{
			system("pause");
			return 0;
		}*/
		system("pause");
		return 0;
	}

	//初始化读取结构体
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
		printf("进程保护成功\n");
	}

	system("pause");

	//初始化写入结构体
	LPVOID writeAddr = 0;
	printf("\nPid请输入writeAddr:");
	scanf("%x", &writeAddr);

	float writeBuff = 5000;
	PMMWRITE pMmWrite = (PMMWRITE)malloc(sizeof(MMWRITE));
	pMmWrite->ProcessId = Pid;
	pMmWrite->writeAddress = &writeBuff;
	pMmWrite->writeSize = 4;
	pMmWrite->targetWriteAddr = (PVOID)writeAddr;

	//初始化通信包结构体
	PPACKAGE package2 = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(package2, 0, sizeof(PACKAGE));
	package2->communicateID = IsCr3ToCr0;
	package2->communicateFun = DriverWrite;
	package2->content = (ULONG64)pMmWrite;

	//第二个参数传递 指针的指针
	ULONG64 arg2 = 0;
	ULONG64 ret2 = NtConvertBetween(1,&package2,&arg2,&arg2);

	//输出内容
	if (ret2 == 0)
	{
		printf("写入成功 %llx\n", ret);
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
		printf("解除进程保护成功\n");
	}

	// ================================ 二次通信 ================================
	//system("pause");
	//PMYMEMORY_BASIC_INFORMATION pMemInfo = (PMYMEMORY_BASIC_INFORMATION)malloc(sizeof(MYMEMORY_BASIC_INFORMATION));
	//memset(pMemInfo, 0, sizeof(MYMEMORY_BASIC_INFORMATION));

	////初始化内存信息结构体
	//PMMTYPE pMmType = (PMMTYPE)malloc(sizeof(MMTYPE));
	//pMmType->ProcessId = Pid;
	//pMmType->checkAddress = (PVOID)0x400000;
	//pMmType->MMInfoBuff = pMemInfo;

	////初始化通信包结构体
	//PPACKAGE package2 = (PPACKAGE)malloc(sizeof(PACKAGE));
	//memset(package2, 0, sizeof(PACKAGE));
	//package2->communicateID = IsCr3ToCr0;
	//package2->communicateFun = GetMMType;
	//package2->content = (ULONG64)pMmType;

	////第二个参数传递 指针的指针
	//ULONG64 ret2 = NtConvertBetween(1, &package2, &arg, &arg);

	////输出读取内容
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

	////卸载驱动
	//UnstallDriver(serviceName);

	////删除驱动文件
	//UnstallResource(szDriverFullPath);

	system("pause");
	return 0;
}