// FngprntView.h : CFngprntView ��Ľӿ�
//


#pragma once


class CFngprntView : public CView
{
protected: // �������л�����
	CFngprntView();
	DECLARE_DYNCREATE(CFngprntView)

// ����
public:
	CFngprntDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CFngprntView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnFngprnt();
};

#ifndef _DEBUG  // FngprntView.cpp �еĵ��԰汾
inline CFngprntDoc* CFngprntView::GetDocument() const
   { return reinterpret_cast<CFngprntDoc*>(m_pDocument); }
#endif

