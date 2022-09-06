#include "InstallDriver.h"

//安装驱动
BOOL InstallDriver(const char* lpszDriverName, const char* serviceName)
{
	//打开服务管理
	SC_HANDLE hSc = NULL;
	hSc = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSc == NULL)
	{
		MessageBoxA(NULL, "打开服务控制管理器失败", "错误提示", MB_OK);
		return FALSE;
	}

	//创建服务
	SC_HANDLE hService = NULL;
	hService = CreateServiceA(hSc, serviceName, serviceName, SC_MANAGER_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, lpszDriverName, NULL, NULL, NULL, NULL, NULL);
	if (hService == NULL)
	{
		MessageBoxA(NULL, "创建驱动服务失败", "错误提示", MB_OK);
		CloseServiceHandle(hSc);
		return FALSE;
	}

	//启动服务
	if (!StartServiceA(hService, NULL, NULL))
	{
		MessageBoxA(NULL, "运行驱动服务失败", "错误提示", MB_OK);
		return FALSE;
	}
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
	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceMgr);
	return TRUE;
}