#include "InstallDriver.h"

//��װ����
BOOL InstallDriver(const char* lpszDriverName, const char* serviceName)
{
	//�򿪷������
	SC_HANDLE hSc = NULL;
	hSc = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSc == NULL)
	{
		MessageBoxA(NULL, "�򿪷�����ƹ�����ʧ��", "������ʾ", MB_OK);
		return FALSE;
	}

	//��������
	SC_HANDLE hService = NULL;
	hService = CreateServiceA(hSc, serviceName, serviceName, SC_MANAGER_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, lpszDriverName, NULL, NULL, NULL, NULL, NULL);
	if (hService == NULL)
	{
		MessageBoxA(NULL, "������������ʧ��", "������ʾ", MB_OK);
		CloseServiceHandle(hSc);
		return FALSE;
	}

	//��������
	if (!StartServiceA(hService, NULL, NULL))
	{
		MessageBoxA(NULL, "������������ʧ��", "������ʾ", MB_OK);
		return FALSE;
	}
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
	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceMgr);
	return TRUE;
}