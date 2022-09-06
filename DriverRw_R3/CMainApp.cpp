#include "CMainDlg.h"
class MyApp :public CWinApp 
{
public:
	virtual BOOL InitInstance() override
	{
		CWinApp::InitInstance();
		CMainDlg MyDlg;
		m_pMainWnd = &MyDlg;
		MyDlg.DoModal();
		return TRUE;
	}
};
MyApp theApp;
