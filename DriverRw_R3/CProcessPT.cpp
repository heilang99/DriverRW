// CProcessPT.cpp: 实现文件
//
#include "CProcessPT.h"
#include "StructInfo.h"
#include "Utils.h"
#include <VersionHelpers.h>

// CProcessPT 对话框
IMPLEMENT_DYNAMIC(CProcessPT, CDialogEx)
CProcessPT::CProcessPT(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_PT, pParent)
{

}
CProcessPT::~CProcessPT()
{
}
void CProcessPT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, Edit_Arg1);
	DDX_Control(pDX, IDC_EDIT4, Edit_Arg2);
	DDX_Control(pDX, IDC_EDIT5, Edit_Arg3);
	DDX_Control(pDX, IDC_EDIT6, Edit_Arg4);
	DDX_Control(pDX, IDC_EDIT1, Edit_FuncAddr);
	DDX_Control(pDX, IDC_EDIT7, Edit_Argc);
}
BEGIN_MESSAGE_MAP(CProcessPT, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RAISEHANDLE, &CProcessPT::OnBnClickedButtonRaisehandle)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEPORT, &CProcessPT::OnBnClickedButtonRemoveport)
	ON_BN_CLICKED(IDC_BUTTON_OBJECTHEADER, &CProcessPT::OnBnClickedButtonObjectheader)
	ON_BN_CLICKED(IDC_BUTTON_DECLINEHANDLE, &CProcessPT::OnBnClickedButtonDeclinehandle)
	ON_BN_CLICKED(IDC_BUTTON_OBJECTHEADER2, &CProcessPT::OnBnClickedButtonObjectheader2)
	ON_BN_CLICKED(IDC_BUTTON1, &CProcessPT::OnBnClickedButton1)
END_MESSAGE_MAP()

// ================================  消息处理程序  ================================
// 
//====================== 提升句柄权限回调函数 ====================== 
void CProcessPT::OnBnClickedButtonRaisehandle()
{
	CString infoError;
	if (IsDriver == FALSE)
	{
		infoError = L"驱动尚未加载";
		MessageBox(infoError);
		return;
	}
	if (Pid == 0)
	{
		infoError = L"指定的PID无效";
		MessageBox(infoError);
		return;
	}
	//构造通信包
	PINPROTECT pProtect = (PINPROTECT)malloc(sizeof(INPROTECT));
	pProtect->pid = (HANDLE)Pid;
	
	PPACKAGE pPackage = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(pPackage, 0, sizeof(PACKAGE));
	pPackage->communicateID = IsCr3ToCr0;
	pPackage->communicateFun = InstallProtect;
	pPackage->content = (ULONG64)pProtect;
	ULONG ret = 0;
	if (IsWindows8OrGreater())
	{
		ret = CommunicateWin10(pPackage);
	}
	else
	{
		ret = CommunicateWin7(pPackage);
	}
	if (ret == 0)
	{
		MessageBox(L"开启保护(其实这里是提升了句柄权限)");
	}
	else
	{
		infoError = L"开启错误 errorCode ";
		infoError.Format(L"%ls %X", infoError, ret);
		MessageBox(infoError);
	}
	free(pProtect);
	free(pPackage);
}

// ====================== 还原,卸载回调函数====================== 
//
void CProcessPT::OnBnClickedButtonDeclinehandle()
{
	CString infoError;
	if (IsDriver == FALSE)
	{
		infoError = L"驱动尚未加载";
		MessageBox(infoError);
		return;
	}
	if (Pid == 0)
	{
		infoError = L"指定的PID无效";
		MessageBox(infoError);
		return;
	}
	//构造通信包
	PPACKAGE pPackage = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(pPackage, 0, sizeof(PACKAGE));
	pPackage->communicateID = IsCr3ToCr0;
	pPackage->communicateFun = UnstallProtect;

	ULONG ret = 0;
	if (IsWindows8OrGreater())
	{
		ret = CommunicateWin10(pPackage);
	}
	else
	{
		ret = CommunicateWin7(pPackage);
	}
	if (ret == 0)
	{
		MessageBox(L"关闭保护");
	}
	else
	{
		infoError = L"关闭错误 errorCode ";
		infoError.Format(L"%ls %X", infoError, ret);
		MessageBox(infoError);
	}
	free(pPackage);
}

// ====================== 修改对象头反调试 ====================== 
void CProcessPT::OnBnClickedButtonObjectheader()
{
	CString infoError;
	if (IsDriver == FALSE)
	{
		infoError = L"驱动尚未加载";
		MessageBox(infoError);
		return;
	}
	if (Pid == 0)
	{
		infoError = L"指定的PID无效";
		MessageBox(infoError);
		return;
	}

	//构造通信包
	PINPROTECT pProtect = (PINPROTECT)malloc(sizeof(INPROTECT));
	pProtect->pid = (HANDLE)Pid;

	PPACKAGE pPackage = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(pPackage, 0, sizeof(PACKAGE));
	pPackage->communicateID = IsCr3ToCr0;
	pPackage->communicateFun = ModifyHeaderFlag;
	pPackage->content = (ULONG64)pProtect;
	ULONG ret = 0;
	if (IsWindows8OrGreater())
	{
		ret = CommunicateWin10(pPackage);
	}
	else
	{
		ret = CommunicateWin7(pPackage);
	}
	if (ret == 0)
	{
		MessageBox(L"已修改");
	}
	else
	{
		infoError = L"修改错误 errorCode ";
		infoError.Format(L"%ls %X", infoError, ret);
		MessageBox(infoError);
	}
	free(pProtect);
	free(pPackage);
}

// ====================== 还原对象头 ====================== 
void CProcessPT::OnBnClickedButtonObjectheader2()
{
	CString infoError;
	if (IsDriver == FALSE)
	{
		infoError = L"驱动尚未加载";
		MessageBox(infoError);
		return;
	}
	if (Pid == 0)
	{
		infoError = L"指定的PID无效";
		MessageBox(infoError);
		return;
	}
	//构造通信包
	PINPROTECT pProtect = (PINPROTECT)malloc(sizeof(INPROTECT));
	pProtect->pid = (HANDLE)Pid;

	PPACKAGE pPackage = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(pPackage, 0, sizeof(PACKAGE));
	pPackage->communicateID = IsCr3ToCr0;
	pPackage->communicateFun = RestoreHeaderFlag;
	pPackage->content = (ULONG64)pProtect;
	ULONG ret = 0;
	if (IsWindows8OrGreater())
	{
		ret = CommunicateWin10(pPackage);
	}
	else
	{
		ret = CommunicateWin7(pPackage);
	}
	if (ret == 0)
	{
		MessageBox(L"已还原");
	}
	else
	{
		infoError = L"还原错误 errorCode ";
		infoError.Format(L"%ls %X", infoError, ret);
		MessageBox(infoError);
	}
	free(pProtect);
	free(pPackage);
}

// ====================== 抹掉DebugPort ====================== 
void CProcessPT::OnBnClickedButtonRemoveport()
{
	MessageBox(L"预留功能");
}


// ====================== 线程劫持函数调用 ====================== 

void CProcessPT::OnBnClickedButton1()
{
	CString infoError;
	/*if (IsDriver == FALSE)
	{
		infoError = L"驱动尚未加载";
		MessageBox(infoError);
		return;
	}
	if (Pid == 0)
	{
		infoError = L"指定的PID无效";
		MessageBox(infoError);
		return;
	}*/
	CString str_funcAddr;
	CString str_Argc;
	CString str_Arg[4] = {  };
	ULONG64 funcAddr = 0;
	DWORD Argc = 0;
	DWORD Arg[4] = { 0 };
	Edit_FuncAddr.GetWindowTextW(str_funcAddr);
	Edit_Arg4.GetWindowTextW(str_Argc);
	Edit_Arg1.GetWindowTextW(str_Arg[0]);
	Edit_Arg2.GetWindowTextW(str_Arg[1]);
	Edit_Arg3.GetWindowTextW(str_Arg[2]);
	Edit_Arg4.GetWindowTextW(str_Arg[3]);
	swscanf_s(str_funcAddr, L"%llx", &funcAddr);
	swscanf_s(str_Argc, L"%lld", &Argc);
	for (size_t i = 0; i < Argc; i++)
	{
		swscanf_s(str_Arg[i], L"%lld", &Arg[i]);
	}
	
	//通信包
	PINJECTTHREAD injectThread = (PINJECTTHREAD)malloc(sizeof(INJECTTHREAD));
	injectThread->pid = (HANDLE)Pid;
	injectThread->Argc = Argc;
	for (size_t i = 0; i < Argc; i++)
	{
		injectThread->Arg[i] = Arg[i];
	}
	PPACKAGE pPackage = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(pPackage, 0, sizeof(PACKAGE));
	pPackage->communicateID = IsCr3ToCr0;
	pPackage->communicateFun = InJectThread;
	pPackage->content = (ULONG64)injectThread;
	ULONG ret = 0;
	if (IsWindows8OrGreater())
	{
		ret = CommunicateWin10(pPackage);
	}
	else
	{
		ret = CommunicateWin7(pPackage);
	}
	if (ret != 0)
	{
		infoError = L"错误 errorCode ";
		infoError.Format(L"%ls %X", infoError, ret);
		MessageBox(infoError);
	}
	free(injectThread);
	free(pPackage);
}
