#pragma once
#include <afxwin.h>
#include "afxdialogex.h"
#include "resource.h"
// CMmRW 对话框
class CMmRW : public CDialogEx
{
	DECLARE_DYNAMIC(CMmRW)

public:
	CMmRW(CWnd* pParent = nullptr); 
	virtual ~CMmRW();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Mm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	CEdit Edit_Pid;
	CEdit Edit_Info;
	CEdit Edit_ReadAddr;
	CEdit Edit_ReadSize;
	CEdit Edit_WriteAddr;
	CEdit Edit_WriteContent;
	CEdit Edit_WriteAddr2;
	CEdit Edit_WriteContent2;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonGetpid();
	afx_msg void OnEnChangeEditReadaddr();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnEnChangeEditWriteaddr();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonWrite2();
	afx_msg void OnEnChangeEditWriteaddr2();
	afx_msg void OnEnChangeEditWritecontent2();

	afx_msg void OnEnChangeEditWritecontent();
};
//全局变量 PID
extern ULONG Pid;