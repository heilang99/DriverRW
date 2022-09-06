// CMmRW.cpp: 实现文件
//
#include "CMmRW.h"
#include "CMainDlg.h"
#include "StructInfo.h"
#include "Utils.h"
#include <VersionHelpers.h>

// CMmRW 对话框
IMPLEMENT_DYNAMIC(CMmRW, CDialogEx)
CMmRW::CMmRW(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_Mm, pParent)
{
}
CMmRW::~CMmRW()
{
}
void CMmRW::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PID, Edit_Pid);
	DDX_Control(pDX, IDC_EDIT_READADDR, Edit_ReadAddr);
	DDX_Control(pDX, IDC_EDIT_WRITEADDR, Edit_WriteAddr);
	DDX_Control(pDX, IDC_EDIT_READSIZE, Edit_ReadSize);
	DDX_Control(pDX, IDC_EDIT_WRITECONTENT, Edit_WriteContent);
	DDX_Control(pDX, IDC_EDIT_INFO, Edit_Info);
	DDX_Control(pDX, IDC_EDIT_WRITEADDR2, Edit_WriteAddr2);
	DDX_Control(pDX, IDC_EDIT_WRITECONTENT2, Edit_WriteContent2);
}
BEGIN_MESSAGE_MAP(CMmRW, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GetPid, &CMmRW::OnBnClickedButtonGetpid)
	ON_EN_CHANGE(IDC_EDIT_READADDR, &CMmRW::OnEnChangeEditReadaddr)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CMmRW::OnBnClickedButtonRead)
	ON_EN_CHANGE(IDC_EDIT_WRITEADDR, &CMmRW::OnEnChangeEditWriteaddr)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CMmRW::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_WRITE2, &CMmRW::OnBnClickedButtonWrite2)
	ON_EN_CHANGE(IDC_EDIT_WRITEADDR2, &CMmRW::OnEnChangeEditWriteaddr2)
	ON_EN_CHANGE(IDC_EDIT_WRITECONTENT2, &CMmRW::OnEnChangeEditWritecontent2)
	ON_EN_CHANGE(IDC_EDIT_WRITECONTENT, &CMmRW::OnEnChangeEditWritecontent)
END_MESSAGE_MAP()

// ================================  消息处理程序  ================================
//初始化
BOOL CMmRW::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Edit_Pid.SetLimitText(5);
	Edit_ReadAddr.SetLimitText(16);
	Edit_WriteAddr.SetLimitText(16);
	Edit_ReadSize.SetLimitText(4);
	Edit_WriteContent.SetLimitText(8);
	Edit_WriteAddr2.SetLimitText(16);

	return TRUE;
}
//读内存地址 编辑框事件
void CMmRW::OnEnChangeEditReadaddr()
{
	CString cstrEdit;
	Edit_ReadAddr.GetWindowTextW(cstrEdit);
	int nStringLength = cstrEdit.GetLength();
	int nDotCount = 0;
	for(int nIndexm = 0; nIndexm < nStringLength; nIndexm++)
	{
		char cmpChar = cstrEdit[nIndexm];
		if (!(cmpChar >= '0' && cmpChar <= '9' || cmpChar >= 'a' && cmpChar <= 'f' || cmpChar >= 'A' && cmpChar <= 'F'))
		{
			cstrEdit = cstrEdit.Left(nIndexm) + cstrEdit.Right(nStringLength - nIndexm - 1);
			Edit_ReadAddr.SetWindowTextW(cstrEdit);
		}
	}
}
//编辑框事件 限制输入仅输入16进制数字
void CMmRW::OnEnChangeEditWriteaddr()
{
	CString cstrEdit;
	Edit_WriteAddr.GetWindowTextW(cstrEdit);
	int nStringLength = cstrEdit.GetLength();
	int nDotCount = 0;
	for (int nIndexm = 0; nIndexm < nStringLength; nIndexm++)
	{
		char cmpChar = cstrEdit[nIndexm];
		if (!(cmpChar >= '0' && cmpChar <= '9' || cmpChar >= 'a' && cmpChar <= 'f' || cmpChar >= 'A' && cmpChar <= 'F'))
		{
			cstrEdit = cstrEdit.Left(nIndexm) + cstrEdit.Right(nStringLength - nIndexm - 1);
			Edit_WriteAddr.SetWindowTextW(cstrEdit);
		}
	}
}
//编辑框事件 限制输入仅输入16进制数字
void CMmRW::OnEnChangeEditWriteaddr2()
{
	CString cstrEdit;
	Edit_WriteAddr2.GetWindowTextW(cstrEdit);
	int nStringLength = cstrEdit.GetLength();
	int nDotCount = 0;
	for (int nIndexm = 0; nIndexm < nStringLength; nIndexm++)
	{
		char cmpChar = cstrEdit[nIndexm];
		if (!(cmpChar >= '0' && cmpChar <= '9' || cmpChar >= 'a' && cmpChar <= 'f' || cmpChar >= 'A' && cmpChar <= 'F'))
		{
			cstrEdit = cstrEdit.Left(nIndexm) + cstrEdit.Right(nStringLength - nIndexm - 1);
			Edit_WriteAddr2.SetWindowTextW(cstrEdit);
		}
	}
}

//编辑框事件 限制输入仅输入16进制数字
void CMmRW::OnEnChangeEditWritecontent()
{
	CString cstrEdit;
	Edit_WriteContent.GetWindowTextW(cstrEdit);
	int nStringLength = cstrEdit.GetLength();
	int nDotCount = 0;
	for (int nIndexm = 0; nIndexm < nStringLength; nIndexm++)
	{
		char cmpChar = cstrEdit[nIndexm];
		if (!(cmpChar >= '0' && cmpChar <= '9' || cmpChar >= 'a' && cmpChar <= 'f' || cmpChar >= 'A' && cmpChar <= 'F'))
		{
			cstrEdit = cstrEdit.Left(nIndexm) + cstrEdit.Right(nStringLength - nIndexm - 1);
			Edit_WriteContent.SetWindowTextW(cstrEdit);
		}
	}
}

//编辑框事件 限制输入仅输入16进制数字
void CMmRW::OnEnChangeEditWritecontent2()
{
	CString cstrEdit;
	Edit_WriteContent2.GetWindowTextW(cstrEdit);
	int nStringLength = cstrEdit.GetLength();
	int nDotCount = 0;
	for (int nIndexm = 0; nIndexm < nStringLength; nIndexm++)
	{
		char cmpChar = cstrEdit[nIndexm];
		if (!(cmpChar >= '0' && cmpChar <= '9' || cmpChar >= 'a' && cmpChar <= 'f' || cmpChar >= 'A' && cmpChar <= 'F'))
		{
			cstrEdit = cstrEdit.Left(nIndexm) + cstrEdit.Right(nStringLength - nIndexm - 1);
			Edit_WriteContent2.SetWindowTextW(cstrEdit);
		}
	}
}
// ================
// ======================================  功能处理  ======================================
//锁定PID
ULONG Pid = 0;
void CMmRW::OnBnClickedButtonGetpid()
{
	CString info;
	if (!IsDriver)
	{
		MessageBox(L"请先安装驱动");
		return;
	}

	CString str_pid;
	HANDLE hProcess = NULL;
	//PID
	Edit_Pid.GetWindowTextW(str_pid);
	swscanf_s(str_pid, L"%d", &Pid);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, Pid);
	if (hProcess == NULL)
	{
		info = L"进程 PID 错误";
		Edit_Info.SetWindowTextW(info);
		return;
	}
	else
	{
		info = L"已锁定PID";
		Edit_Info.SetWindowTextW(info);

		//创建线程全局遍历 看当前进程是否被关闭
		static int flag = 0;
		if (flag == 0)
		{
			HANDLE hThread = CreateThread(NULL, NULL, EnumProcessThreadProc, NULL, NULL, NULL);
			flag = 1;
			CloseHandle(hThread);
		}

		return;
	}
}

//======================================  功能处理  读内存======================================
void CMmRW::OnBnClickedButtonRead()
{
	CString infoError;
	if (IsDriver == FALSE)
	{
		infoError = L"驱动尚未加载";
		Edit_Info.SetWindowTextW(infoError);
		return;
	}
	if (Pid == 0)
	{
		infoError = L"指定的PID无效";
		Edit_Info.SetWindowTextW(infoError);
		return;
	}

	CString ReadAddrStr;
	CString ReadSizeStr;
	ULONG64 readAddr = 0;
	ULONG64 readSize = 0;
	char* readBuffer = NULL;
	
	//获取通信参数
	Edit_ReadAddr.GetWindowTextW(ReadAddrStr);
	Edit_ReadSize.GetWindowTextW(ReadSizeStr);
	swscanf_s(ReadAddrStr, L"%llx", &readAddr);
	swscanf_s(ReadSizeStr, L"%lld", &readSize);
	readBuffer = (char*)malloc(readSize);
	memset(readBuffer, 0, readSize);
	if (readAddr == 0 || readSize == 0)
	{
		infoError = L"参数错误，请指定\"读取地址\" 和 \"读取大小\"";
		Edit_Info.SetWindowTextW(infoError);
		return;
	}

	//读取参数包
	PMMREAD pMmRead = (PMMREAD)malloc(sizeof(MMREAD));
	pMmRead->ProcessId = Pid;
	pMmRead->readAddress = (PVOID)readAddr;
	pMmRead->readBuff = readBuffer;
	pMmRead->readSize = readSize;

	//通信包
	PPACKAGE pPackage = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(pPackage, 0, sizeof(PACKAGE));
	pPackage->communicateID = IsCr3ToCr0;
	pPackage->communicateFun = DriverRead;
	pPackage->content = (ULONG64)pMmRead;
	
	//驱动通信
	ULONG ret = 0;
	if (IsWindows8OrGreater())
	{
		//		Win10
		ret = CommunicateWin10(pPackage);
		if (ret == 0)
		{
			ULONG realSize = (readSize + 1) * 2;
			char* ansiBuffer = (char*)malloc(realSize);
			memset(ansiBuffer, 0, realSize);
			ByteToChar((PUCHAR)readBuffer, (PUCHAR)ansiBuffer, readSize);	//字节转换成字符

			ULONG WideSize = realSize * 2;
			LPWSTR wideBuffer = (LPWSTR)malloc(WideSize);
			memset(wideBuffer, 0, WideSize);
			MultiByteToWideChar(CP_ACP, MB_COMPOSITE, (LPCCH)ansiBuffer, realSize, wideBuffer, WideSize);

			CString Info = L"读取成功：\n\r";
			Info.Append(wideBuffer, WideSize);
			Edit_Info.SetWindowTextW(Info);
			free(wideBuffer);
			free(ansiBuffer);
		}
		else
		{
			CString infoError = L"读取失败 errorCode: ";
			infoError.Format(L"%ls %X", infoError, ret);
			Edit_Info.SetWindowTextW(infoError);
		}
	}
	else
	{	
		////		Win7
		ret = CommunicateWin7(pPackage);
		if (ret == 0)
		{
			ULONG realSize = (readSize + 1) * 2;
			char* ansiBuffer = (char*)malloc(realSize);
			memset(ansiBuffer, 0, realSize);
			ByteToChar((PUCHAR)readBuffer, (PUCHAR)ansiBuffer, readSize);	//字节转换成字符

			ULONG WideSize = realSize * 2;
			LPWSTR wideBuffer = (LPWSTR)malloc(WideSize);
			memset(wideBuffer, 0, WideSize);
			MultiByteToWideChar(CP_ACP, MB_COMPOSITE, (LPCCH)ansiBuffer, realSize, wideBuffer, WideSize);

			CString Info;
			Info.Append(wideBuffer, WideSize);
			Edit_Info.SetWindowTextW(Info);
			free(wideBuffer);
			free(ansiBuffer);
		}
		else
		{
			CString infoError = L"读取失败 errorCode: ";
			infoError.Format(L"%ls %X", infoError, ret);
			Edit_Info.SetWindowTextW(infoError);
		}

	}
	//释放内存
	free(readBuffer);
	free(pMmRead);
	free(pPackage);
}

//======================================  功能处理 写内存整数 ======================================
void CMmRW::OnBnClickedButtonWrite()
{
	CString infoError;
	if (IsDriver == FALSE)
	{
		infoError = L"驱动尚未加载";
		Edit_Info.SetWindowTextW(infoError);
		return;
	}
	if (Pid == 0)
	{
		infoError = L"指定的PID无效";
		Edit_Info.SetWindowTextW(infoError);
		return;
	}

	CString WriteAddrStr;
	ULONG64 writeAddr = 0;
	CString ContentStr;
	ULONG WriteData = 0;

	//获取通信参数
	Edit_WriteAddr.GetWindowTextW(WriteAddrStr);
	swscanf_s(WriteAddrStr, L"%llx", &writeAddr);
	Edit_WriteContent.GetWindowTextW(ContentStr);
	swscanf_s(ContentStr, L"%x", &WriteData);
	if (writeAddr == 0 || WriteData == 0)
	{
		infoError = L"参数错误，请指定\"写入地址\" 和 \"写入内容\"";
		Edit_Info.SetWindowTextW(infoError);
		return;
	}

	//构造通信包
	PMMWRITE pMmWrite = (PMMWRITE)malloc(sizeof(MMWRITE));
	pMmWrite->ProcessId = Pid;
	pMmWrite->writeAddress = (PVOID)&WriteData;
	pMmWrite->writeSize = sizeof(ULONG);
	pMmWrite->targetWriteAddr = (PVOID)writeAddr;

	PPACKAGE pPackage = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(pPackage, 0, sizeof(PACKAGE));
	pPackage->communicateID = IsCr3ToCr0;
	pPackage->communicateFun = DriverWrite;
	pPackage->content = (ULONG64)pMmWrite;

	//驱动通信
	ULONG ret = 0;
	if (IsWindows8OrGreater())
	{
		//    Win10
		ret = CommunicateWin10(pPackage);
		if (ret == 0)
		{
			infoError = L"写入成功";
			Edit_Info.SetWindowTextW(infoError);
		}
		else
		{
			infoError = L"写入失败 errorCode: ";
			infoError.Format(L"%ls %X", infoError, ret);
			Edit_Info.SetWindowTextW(infoError);
		}
	}
	else
	{
		//    Win7
		ret = CommunicateWin7(pPackage);
		if (ret == 0)
		{
			infoError = L"写入成功";
			Edit_Info.SetWindowTextW(infoError);
		}
		else
		{
			infoError = L"写入失败 errorCode: ";
			infoError.Format(L"%ls %X", infoError, ret);
			Edit_Info.SetWindowTextW(infoError);
		}
	}
}

//======================================  功能处理 写内存字节集 ======================================
void CMmRW::OnBnClickedButtonWrite2()
{
	CString infoError;
	if (IsDriver == FALSE)
	{
		infoError = L"驱动尚未加载";
		Edit_Info.SetWindowTextW(infoError);
		return;
	}
	if (Pid == 0)
	{
		infoError = L"指定的PID无效";
		Edit_Info.SetWindowTextW(infoError);
		return;
	}

	CString WriteAddrStr;
	CString wideContentStr;
	ULONG64 writeAddr = 0;
	ULONG ContentLen = 0;
	ULONG writelen = 0;
	char* ansiContent = NULL;
	char* shellCode = NULL;

	//获取通信参数
	Edit_WriteAddr2.GetWindowTextW(WriteAddrStr);
	swscanf_s(WriteAddrStr, L"%llx", &writeAddr);
	Edit_WriteContent2.GetWindowTextW(wideContentStr);
	if (writeAddr == 0 || wideContentStr.GetLength() == 0)
	{
		infoError = L"参数错误，请指定\"写入地址\" 和 \"写入内容\"";
		Edit_Info.SetWindowTextW(infoError);
		return;
	}
	ContentLen = wideContentStr.GetLength() + 1;
	if (ContentLen % 2 == 0)
	{
		infoError = L"shellcode错误  长度应该是2的整数倍";
		Edit_Info.SetWindowTextW(infoError);
		return;
	}
	
	//转换成asci
	ansiContent = (char*)malloc(ContentLen);
	if (ansiContent == NULL)return;
	memset(ansiContent, 0, ContentLen);
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wideContentStr, ContentLen, ansiContent, ContentLen, NULL, NULL);

	//字符转字节
	shellCode = (char*)malloc(ContentLen);
	if (shellCode == NULL)return;
	memset(shellCode, 0, ContentLen);
	CharToByte((PUCHAR)ansiContent, (PUCHAR)shellCode, ContentLen);
	writelen = strlen(shellCode);

	//构造通信包
	PMMWRITE pMmWrite = (PMMWRITE)malloc(sizeof(MMWRITE));
	pMmWrite->ProcessId = Pid;
	pMmWrite->writeAddress = (PVOID)shellCode;
	pMmWrite->writeSize = writelen;
	pMmWrite->targetWriteAddr = (PVOID)writeAddr;

	PPACKAGE pPackage = (PPACKAGE)malloc(sizeof(PACKAGE));
	memset(pPackage, 0, sizeof(PACKAGE));
	pPackage->communicateID = IsCr3ToCr0;
	pPackage->communicateFun = DriverWrite;
	pPackage->content = (ULONG64)pMmWrite;

	//驱动通信
	ULONG ret = 0;
	if (IsWindows8OrGreater())
	{
		//    Win10
		ret = CommunicateWin10(pPackage);
		if (ret == 0)
		{
			infoError = L"写入成功";
			Edit_Info.SetWindowTextW(infoError);
		}
		else
		{
			infoError = L"写入失败 errorCode: ";
			infoError.Format(L"%ls %X", infoError, ret);
			Edit_Info.SetWindowTextW(infoError);
		}
	}
	else
	{
		//    Win7
		ret = CommunicateWin7(pPackage);
		if (ret == 0)
		{
			infoError = L"写入成功";
			Edit_Info.SetWindowTextW(infoError);
		}
		else
		{
			infoError = L"写入失败 errorCode: ";
			infoError.Format(L"%ls %X", infoError, ret);
			Edit_Info.SetWindowTextW(infoError);
		}
	}
	free(ansiContent);
	free(shellCode);
}
