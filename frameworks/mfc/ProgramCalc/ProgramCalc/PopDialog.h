#pragma once
#include "afxwin.h"


// CPopDialog 对话框

class CPopDialog : public CDialog
{
	DECLARE_DYNAMIC(CPopDialog)

public:
	CPopDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPopDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int validInput(CString str);

protected:
	// 修改值
	CEdit changeEdit;

private:
	int fixValue;
public:
	int getFixValue(void);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
