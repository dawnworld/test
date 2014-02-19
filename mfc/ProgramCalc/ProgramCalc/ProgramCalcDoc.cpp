
// ProgramCalcDoc.cpp : CProgramCalcDoc 类的实现
//

#include "stdafx.h"
#include "ProgramCalc.h"

#include "ProgramCalcDoc.h"
#include "MainFrm.h"
#include "ProgramCalcView.h"

#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProgramCalcDoc

IMPLEMENT_DYNCREATE(CProgramCalcDoc, CDocument)

BEGIN_MESSAGE_MAP(CProgramCalcDoc, CDocument)
END_MESSAGE_MAP()


// CProgramCalcDoc 构造/析构

CProgramCalcDoc::CProgramCalcDoc()
{
	// TODO: 在此添加一次性构造代码

}

CProgramCalcDoc::~CProgramCalcDoc()
{
}

BOOL CProgramCalcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CProgramCalcDoc 序列化

void CProgramCalcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
		this->saveFile(ar);
	}
	else
	{
		// TODO: 在此添加加载代码
		this->parseFile(ar.m_strFileName);
	}
}


// CProgramCalcDoc 诊断

#ifdef _DEBUG
void CProgramCalcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CProgramCalcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// parse file

int CProgramCalcDoc::parseFile(CString filePath)
{
	CStdioFile mFile(filePath, CFile::modeRead | CFile::shareDenyNone);
	if(mFile.GetLength() < 2) return -1;
	
	CMainFrame * pFram = (CMainFrame *)AfxGetMainWnd();
	CProgramCalcView *pView = (CProgramCalcView *)pFram->GetActiveView();
	pView->initListCtrl();

	CString readLine;
	int lineNum = 0;
	std::list<CString> linesList;
	while(mFile.ReadString(readLine))
	{
		lineNum++;
		linesList.push_front(readLine);
	}

	std::list<CString>::iterator iter;
	int num = 0;
	for (iter = linesList.begin(); iter != linesList.end(); iter++)
	{
		CString curLine = (CString)*iter;
		pView->addLineToList( linesList.size() - num++, curLine);
	}
	pView->updateListCtrl();

	mFile.Close();
	return 0;
}

int CProgramCalcDoc::saveFile(CArchive& ar)
{
	std::list<CString> strList;

	CMainFrame * pFram = (CMainFrame *)AfxGetMainWnd();
	CProgramCalcView *pView = (CProgramCalcView *)pFram->GetActiveView();

	pView->getStrItemList(strList);
	if (strList.empty()) return -1;

// 	CStdioFile mFile;
// 	mFile.Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);

// 	if(NULL == mFile.m_pStream) return -1;

	std::list<CString>::iterator iter;
	for (iter = strList.begin(); iter != strList.end(); iter++)
	{
		CString str;
		CString oneLine = (CString)*iter;
// 		str.Format(_T("%s\r\n"), oneLine);
		for (int i = 0; i < oneLine.GetLength(); i++)
		{
			char c[1];
			c[0] = oneLine.GetAt(i);
			ar.Write(c,1);
		}
		char newLine[2];
		newLine[0] = '\r';
		newLine[1] = '\n';
		ar.Write(newLine, sizeof(newLine));
// 		ar.WriteString(str);
// 		ar << oneLine << _T("\r\n");
// 		mFile.WriteString(oneLine);
	}
// 	mFile.Close();
	return 0;
}
