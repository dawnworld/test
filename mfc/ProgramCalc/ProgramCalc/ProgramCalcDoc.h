
// ProgramCalcDoc.h : CProgramCalcDoc ��Ľӿ�
//


#pragma once


class CProgramCalcDoc : public CDocument
{
protected: // �������л�����
	CProgramCalcDoc();
	DECLARE_DYNCREATE(CProgramCalcDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CProgramCalcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	// parse file
	int parseFile(CString filePath);
protected:
	int saveFile(CArchive& ar);
};


