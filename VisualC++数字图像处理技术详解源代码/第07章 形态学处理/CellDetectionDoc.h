// CellDetectionDoc.h : CCellDetectionDoc ��Ľӿ�
//


#pragma once


class CCellDetectionDoc : public CDocument
{
protected: // �������л�����
	CCellDetectionDoc();
	DECLARE_DYNCREATE(CCellDetectionDoc)

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
	virtual ~CCellDetectionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


