#pragma once
#include "afxwin.h"


// CPopDialog �Ի���

class CPopDialog : public CDialog
{
	DECLARE_DYNAMIC(CPopDialog)

public:
	CPopDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPopDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int validInput(CString str);

protected:
	// �޸�ֵ
	CEdit changeEdit;

private:
	int fixValue;
public:
	int getFixValue(void);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
