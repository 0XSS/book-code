// FngprntDoc.h : CFngprntDoc ��Ľӿ�
//


#pragma once
#include "Dib.h"

class CFngprntDoc : public CDocument
{
protected: // �������л�����
	CFngprntDoc();
	DECLARE_DYNCREATE(CFngprntDoc)

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
	virtual ~CFngprntDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
private:
    CDib dib;
public:
    CDib* GetDib(void);

public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};



