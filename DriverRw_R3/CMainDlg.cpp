#include "CMainDlg.h"
#include "CMmRW.h"
#include "CProcessPT.h"
#include "InstallDriver.h"
#include "InstallResource.h"
#include "Utils.h"

//全局变量
BOOL IsDriver = FALSE;

//MFC
IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)
CMainDlg::CMainDlg(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_Main, pParent)
{	}
CMainDlg::~CMainDlg()
{	}
void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_table);
	DDX_Control(pDX, IDC_BUTTON_InDriver, m_InstallDriver);
	DDX_Control(pDX, IDC_BUTTON_UnDriver, m_UnstallDriver);
}
BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMainDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_InDriver, &CMainDlg::OnBnClickedButtonIndriver)
	ON_BN_CLICKED(IDC_BUTTON_UnDriver, &CMainDlg::OnBnClickedButtonUndriver)
END_MESSAGE_MAP()

// ================================  消息处理程序  ================================

//初始化对话框
BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//设置图标
	HICON hIcon = LoadIconW(AfxGetInstanceHandle(), (LPCWSTR)IDI_ICON1);
	SetIcon(hIcon, 1);
	SetIcon(hIcon, 0);

	//插入标题
	m_table.InsertItem(0, L"驱动读写");
	m_table.InsertItem(1, L"进程保护");
	SubDlg[0] = new CMmRW;
	SubDlg[1] = new CProcessPT;

	//新窗口的父窗口是table控件
	SubDlg[0]->Create(IDD_DIALOG_Mm, &m_table);
	SubDlg[1]->Create(IDD_DIALOG_PT, &m_table);

	//显示
	CRect rect;
	for (int i = 0; i < 2; i++)
	{
		SubDlg[i]->GetClientRect(rect);
		rect.DeflateRect(0, 25, 0, 0);
		SubDlg[i]->MoveWindow(rect);
	}
	SubDlg[0]->ShowWindow(SW_SHOWNORMAL);

	//设置安装卸载驱动按钮
	m_InstallDriver.EnableWindow(TRUE);
	m_UnstallDriver.EnableWindow(FALSE);

	return TRUE;  
}

//窗口切换
void CMainDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	//只显示当前切换到的窗口，其它的都隐藏
	int index = m_table.GetCurSel();
	for (int i = 0; i < 2; i++)
	{
		if (index == i)
		{
			SubDlg[i]->ShowWindow(SW_SHOWNORMAL);
		}
		else
		{
			SubDlg[i]->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}

//窗口摧毁
void CMainDlg::OnDestroy()
{
	if (IsDriver)
	{
		if (::MessageBoxW(NULL, L"检查到驱动未卸载，是否卸载", L"提示", MB_YESNO) == IDYES)
		{
			//卸载
			IsDriver = FALSE;
			UnstallDriver(serviceName);
		}
	}
	CDialogEx::OnDestroy();
}

//安装驱动按钮
void CMainDlg::OnBnClickedButtonIndriver()
{
	memset(driverName, 0, 15);
	memcpy(driverName, "DriverRW.sys", 13);

	char szDriverFullPath[MAX_PATH] = { 0 };
	GetFullPathNameA(driverName, MAX_PATH, szDriverFullPath, NULL);
	RandDirectroyName("", serviceName, 6);
	if (InstallDriver(szDriverFullPath, serviceName))
	{
		IsDriver = TRUE;
		MessageBox(L"加载驱动成功");
		m_InstallDriver.EnableWindow(FALSE);
		m_UnstallDriver.EnableWindow(TRUE);
	}
}

//卸载驱动按钮
void CMainDlg::OnBnClickedButtonUndriver()
{
	if (UnstallDriver(serviceName))
	{
		IsDriver = FALSE;
		MessageBox(L"卸载驱动成功");
		m_InstallDriver.EnableWindow(TRUE);
		m_UnstallDriver.EnableWindow(FALSE);
	}
}
