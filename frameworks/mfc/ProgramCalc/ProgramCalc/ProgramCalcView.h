
// ProgramCalcView.h : CProgramCalcView ��Ľӿ�
//


#pragma once
#include "afxcmn.h"
#include <list>


class CProgramCalcView : public CFormView
{
protected: // �������л�����
	CProgramCalcView();
	DECLARE_DYNCREATE(CProgramCalcView)

public:
	enum{ IDD = IDD_PROGRAMCALC_FORM };

// ����
public:
	CProgramCalcDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���
	// ҵ���߼�����
	int modfiyListItems(int fixValue);

// ʵ��
public:
	virtual ~CProgramCalcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
protected:
	CListCtrl m_list;
public:
	int addLineToList(int num, CString strLine);
	// set vertical scroll and others
	int updateListCtrl(void);
	int initListCtrl(void);
	bool isInitCtrl;
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditX();

	int handleEachLine(CString inString, CString& outString,  int fixValue);
	afx_msg void OnSelectAll();
	afx_msg void OnCustomdrawList(NMHDR*, LRESULT*);
	int getStrItemList(std::list<CString>& strList);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	std::list<int> fixLines;
	int addFixLine(int item);
};

#ifndef _DEBUG  // ProgramCalcView.cpp �еĵ��԰汾
inline CProgramCalcDoc* CProgramCalcView::GetDocument() const
   { return reinterpret_cast<CProgramCalcDoc*>(m_pDocument); }
#endif

