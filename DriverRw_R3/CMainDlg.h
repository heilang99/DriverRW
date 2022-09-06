#pragma once
#include <afxwin.h>
#include <afxdialogex.h>
#include "resource.h"

// CMainDlg 对话框
class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = nullptr);  
	virtual ~CMainDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Main };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	DECLARE_MESSAGE_MAP()
public:
	char driverName[15];
	char serviceName[6];
	CTabCtrl m_table;
	CDialogEx* SubDlg[2];
	CButton m_InstallDriver;
	CButton m_UnstallDriver;
public:
	virtual BOOL OnInitDialog();										//初始化
	afx_msg void OnDestroy();											//窗口摧毁
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);	//切换窗口
	afx_msg void OnBnClickedButtonIndriver();
	afx_msg void OnBnClickedButtonUndriver();
};

//全局变量，标记驱动是否加载
extern BOOL IsDriver;