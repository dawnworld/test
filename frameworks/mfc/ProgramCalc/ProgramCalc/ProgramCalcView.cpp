
// ProgramCalcView.cpp : CProgramCalcView 类的实现
//

#include "stdafx.h"
#include "ProgramCalc.h"

#include "ProgramCalcDoc.h"
#include "ProgramCalcView.h"

#include "PopDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProgramCalcView

IMPLEMENT_DYNCREATE(CProgramCalcView, CFormView)

BEGIN_MESSAGE_MAP(CProgramCalcView, CFormView)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CProgramCalcView::OnNMRClickList1)
	ON_COMMAND(ID_32771, &CProgramCalcView::OnEditX)
	ON_COMMAND(ID_SELECT_ALL, &CProgramCalcView::OnSelectAll)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CProgramCalcView::OnCustomdrawList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CProgramCalcView::OnNMDblclkList1)
END_MESSAGE_MAP()

// CProgramCalcView 构造/析构

CProgramCalcView::CProgramCalcView()
	: CFormView(CProgramCalcView::IDD)
	, isInitCtrl(false)
{
	// TODO: 在此处添加构造代码

}

CProgramCalcView::~CProgramCalcView()
{
}

void CProgramCalcView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BOOL CProgramCalcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CProgramCalcView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

// 	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
// 	CRect rect;
// 	m_list.GetClientRect(rect);
// 	m_list.InsertColumn(0, _T("LINE"), LVCFMT_LEFT, 40);
// 	m_list.InsertColumn(1, _T("CODE"));
// 	m_list.SetColumnWidth(1, rect.Width() - 40);	

}

void CProgramCalcView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CProgramCalcView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CProgramCalcView 诊断

#ifdef _DEBUG
void CProgramCalcView::AssertValid() const
{
	CFormView::AssertValid();
}

void CProgramCalcView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CProgramCalcDoc* CProgramCalcView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProgramCalcDoc)));
	return (CProgramCalcDoc*)m_pDocument;
}
#endif //_DEBUG


// CProgramCalcView 消息处理程序

int CProgramCalcView::addLineToList(int num, CString strLine)
{
	CString lineNum;
	lineNum.Format(_T("%d"), num);
	int nRow = m_list.InsertItem(0,  lineNum);
	m_list.SetItemText(nRow, 1, strLine);

	return 0;
}

// set vertical scroll and others
int CProgramCalcView::updateListCtrl(void)
{
	int lineOfPage = 17;
	int count = m_list.GetItemCount();
	int pages = count / lineOfPage;
	if(count > lineOfPage)
	{
		SCROLLINFO si;
		si.cbSize = sizeof( SCROLLINFO );
		si.fMask = SIF_PAGE | SIF_RANGE;	
		si.nMin = 0;
		si.nMax =count;
		si.nPage = pages;
		m_list.SetScrollInfo( SB_VERT, &si );
	}

	return 0;
}

int CProgramCalcView::initListCtrl(void)
{
	m_list.DeleteAllItems();
	if(isInitCtrl) return 0;
	
 	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	CRect rect;
	m_list.GetClientRect(rect);
	m_list.InsertColumn(0, _T("行号"), LVCFMT_LEFT, 40);
	m_list.InsertColumn(1, _T("内容"));
	m_list.SetColumnWidth(1, rect.Width() - 40);
	isInitCtrl = true;

// 	int nRow = m_list.InsertItem(0,  _T("1"));
// 	m_list.SetItemText(nRow, 1, _T("hello"));
// 
// 	nRow = m_list.InsertItem(0,  _T("2"));
// 	m_list.SetItemText(nRow, 1,  _T("world"));
	return 0;
}

void CProgramCalcView::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码

	CPoint point;
	::GetCursorPos(&point);
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU1));
	CMenu * popup = menu.GetSubMenu(0);
	ASSERT(popup != NULL);
	popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);

	*pResult = 0;
}

void CProgramCalcView::OnEditX()
{
	// TODO: 在此添加命令处理程序代码
	
	CPopDialog popDialog;

	popDialog.DoModal();

	int val = popDialog.getFixValue();
	if(!val) return;

	this->modfiyListItems(val);
	//popDialog.ShowWindow(SW_SHOWNA);
}

// 业务逻辑函数
int CProgramCalcView::modfiyListItems(int fixValue)
{
	int result = 0;
	CString content;
	CString fixContent;

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(NULL == pos) return -1;

	while (pos)
	{
		int item = m_list.GetNextSelectedItem(pos);
		content = m_list.GetItemText(item, 1);
		result = handleEachLine(content, fixContent, fixValue);
		if(-1 == result) continue;

		m_list.SetItemText(item, 1, fixContent);
		this->addFixLine(item);
	}
	return 0;
}

int CProgramCalcView::handleEachLine(CString inString, CString& outString, int fixValue)
{
	int result = 0;

	float param = 0.0;
	wchar_t head[4];

	result = swscanf_s(inString, _T("%3*sX%fY%*s"), &param);
	if(result != 1) return -1;

	result = swscanf_s(inString, L"%3s", head, _countof(head));
	if(result != 1) return -1;

	int yPos = inString.Find(_T("Y"));
	if(yPos == -1) return -1;

	result = inString.Delete(0, yPos + 1);

	param += fixValue;
	outString.Format(_T("%3sX%.3fY%s"), head, param, inString);
	return result;
}

void CProgramCalcView::OnSelectAll()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		m_list.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CProgramCalcView::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
    //////////////////////////////////////////////////////
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    // Take the default processing unless we 

// set this to something else below.

    *pResult = CDRF_DODEFAULT;

    // First thing - check the draw stage. If it's the control's prepaint

// stage, then tell Windows we want messages for every item.


    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
    {
        *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
    {
        COLORREF crText,crBk;
		// check odd
        if ( (pLVCD->nmcd.dwItemSpec % 2) == 0 ){
            crText = RGB(0,0,0);//RGB(32,32,255);
            crBk =RGB(229,232,239);
        }
        else if ( (pLVCD->nmcd.dwItemSpec % 2) == 1 ){
            crText = RGB(0,0,0);
            crBk = RGB(240,247,249);
        }
        else{
            crText = RGB(0,0,0);
            crBk = RGB(0,0,126);
        }
        
		std::list<int>::iterator iter;
		for (iter = fixLines.begin(); iter != fixLines.end(); iter++)
		{
			int item = *iter;
			if (item == pLVCD->nmcd.dwItemSpec)
			{
				crText = RGB(255,0,0);
			}
		}

        // Store the color back in the NMLVCUSTOMDRAW struct.

        pLVCD->clrText = crText;
        pLVCD->clrTextBk = crBk;
        //设置选择项的颜色
//         if( this->m_list.GetItemState(pLVCD->nmcd.dwItemSpec, CDIS_SELECTED) ){
//             crBk =RGB(75, 149, 229);//itunes//RGB(10, 36, 106);//RGB(0, 0, 64);
//             crText = RGB(255,255,255);
// 
// 			if(pLVCD->nmcd.dwItemSpec == 3)
// 				crText = RGB(255,255,0);
// 
//             pLVCD->clrText = crText;
//             pLVCD->clrTextBk = crBk;
//             *pResult = CDRF_NEWFONT;
//         }
// 	
//         if(LVIS_SELECTED == m_list.GetItemState(pLVCD->nmcd.dwItemSpec,LVIS_SELECTED))
//         {
//             //清除选择状态，如果不清除的话，还是会显示出蓝色的高亮条
//             BOOL b = m_list.SetItemState(pLVCD->nmcd.dwItemSpec,0,LVIS_SELECTED); 
//             pLVCD->clrText = crText;
//             pLVCD->clrTextBk = crBk;
// 
//             *pResult = CDRF_NEWFONT;
//             return;
//         }

        *pResult = CDRF_NEWFONT;
        //*pResult = CDRF_DODEFAULT;
    }
}

int CProgramCalcView::getStrItemList(std::list<CString>& strList)
{
	if(!strList.empty())
		strList.clear();

	for (int i = 0; i < this->m_list.GetItemCount(); i++)
	{
		CString content = m_list.GetItemText(i, 1);
		strList.push_back(content);
	}
	return 0;
}

void CProgramCalcView::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int result = 0;

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(NULL == pos) return;

	int item = m_list.GetNextSelectedItem(pos);
	CString content = m_list.GetItemText(item, 1);

	float param = 0.0;

	result = swscanf_s(content, _T("%3*sX%fY%*s"), &param);
	if(result != 1) return;

	this->OnEditX();

	*pResult = 0;
}

int CProgramCalcView::addFixLine(int item)
{
	std::list<int>::iterator iter;
	for (iter = fixLines.begin(); iter != fixLines.end(); iter++)
	{
		int value = (int)*iter;
		if(value == item) return 1;
	}
	fixLines.push_back(item);
	return 0;
}
