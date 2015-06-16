// PopDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ProgramCalc.h"
#include "PopDialog.h"


#include "MainFrm.h"
// #include "ProgramCalcView.h"
// 
// CPopDialog 对话框

IMPLEMENT_DYNAMIC(CPopDialog, CDialog)

CPopDialog::CPopDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPopDialog::IDD, pParent)
	, fixValue(0)
{
}

CPopDialog::~CPopDialog()
{
}

void CPopDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, changeEdit);
}


BEGIN_MESSAGE_MAP(CPopDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CPopDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPopDialog::OnBnClickedCancel)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CPopDialog 消息处理程序

void CPopDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_value;
	int res;
	changeEdit.GetWindowText(m_value);
	res = this->validInput(m_value);
	if(res == -1)
	{
		MessageBox(_T("输入有误，请重输!"));
		return;
	}	
	int val = _wtoi(m_value);
	printf("ready to change the number:%d\n", val);
	this->fixValue = val;
// 	CMainFrame * pFram = (CMainFrame *)AfxGetMainWnd();
// 	CProgramCalcView *pView = (CProgramCalcView *)pFram->GetActiveView();
// 	pView->modfiyListItems(val);
	OnOK();
}

void CPopDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

int CPopDialog::validInput(CString str)
{
	int len = str.GetLength();
	for (int i = 0; i < len - 1; i++)
	{
		char c = str.GetAt(i);
		if(i == 0 && c == '-')
			continue;
		if(c < '0'|| c > '9')
			return -1;
	}
	return 0;
}

int CPopDialog::getFixValue(void)
{
	return this->fixValue;
}

int CPopDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CPopDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
// 	HWND hWnd;
// 	GetDlgItem(IDC_EDIT1, &hWnd);
// 	::PostMessage(hWnd, WM_SETFOCUS, 0, 0);
	changeEdit.SetFocus();
}
