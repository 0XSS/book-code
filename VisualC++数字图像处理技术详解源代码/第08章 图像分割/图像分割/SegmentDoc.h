// SegmentDoc.h : CSegmentDoc ��Ľӿ�
//


#pragma once
#include "Dib.h"


class CSegmentDoc : public CDocument
{
protected: // �������л�����
	CSegmentDoc();
	DECLARE_DYNCREATE(CSegmentDoc)

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
	virtual ~CSegmentDoc();
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
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    
public:
    CDib* GetDib(void);

};


