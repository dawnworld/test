
// ProgramCalcView.h : CProgramCalcView 类的接口
//


#pragma once
#include "afxcmn.h"
#include <list>


class CProgramCalcView : public CFormView
{
protected: // 仅从序列化创建
	CProgramCalcView();
	DECLARE_DYNCREATE(CProgramCalcView)

public:
	enum{ IDD = IDD_PROGRAMCALC_FORM };

// 属性
public:
	CProgramCalcDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用
	// 业务逻辑函数
	int modfiyListItems(int fixValue);

// 实现
public:
	virtual ~CProgramCalcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // ProgramCalcView.cpp 中的调试版本
inline CProgramCalcDoc* CProgramCalcView::GetDocument() const
   { return reinterpret_cast<CProgramCalcDoc*>(m_pDocument); }
#endif

