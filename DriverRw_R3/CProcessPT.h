#pragma once
#include <afxwin.h>
#include "afxdialogex.h"
#include "resource.h"

// CProcessPT 对话框
class CProcessPT : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessPT)

public:
	CProcessPT(CWnd* pParent = nullptr); 
	virtual ~CProcessPT();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRaisehandle();
	afx_msg void OnBnClickedButtonRemoveport();
	afx_msg void OnBnClickedButtonObjectheader();
	afx_msg void OnBnClickedButtonDeclinehandle();
	afx_msg void OnBnClickedButtonObjectheader2();
	afx_msg void OnBnClickedButton1();
public:
	CEdit Edit_FuncAddr;
	CEdit Edit_Arg1;
	CEdit Edit_Arg2;
	CEdit Edit_Arg3;
	CEdit Edit_Arg4;
	CEdit Edit_Argc;
};

//全局变量
extern ULONG Pid;
extern BOOL IsDriver;