// MagicHouseDoc.h : CMagicHouseDoc ��Ľӿ�
//


#pragma once


class CMagicHouseDoc : public CDocument
{
protected: // �������л�����
	CMagicHouseDoc();
	DECLARE_DYNCREATE(CMagicHouseDoc)

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
	virtual ~CMagicHouseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


