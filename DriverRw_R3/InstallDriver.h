#pragma once
#include <Windows.h>
BOOL InstallDriver(const char* lpszDriverName, const char* serviceName);
BOOL UnstallDriver(const char* lpServiceName);