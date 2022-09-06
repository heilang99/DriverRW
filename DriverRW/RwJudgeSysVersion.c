#include "RwJudgeSysVersion.h"
BOOLEAN JudgeSysIsWin7()
{
	RTL_OSVERSIONINFOW versionInfo = { 0 };
	versionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	RtlGetVersion(&versionInfo);
	if (versionInfo.dwBuildNumber == 7601 || versionInfo.dwBuildNumber == 7600)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}