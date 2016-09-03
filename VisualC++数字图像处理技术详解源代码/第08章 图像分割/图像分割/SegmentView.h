// SegmentView.h : CSegmentView ��Ľӿ�
//


#pragma once


class CSegmentView : public CView
{
protected: // �������л�����
	CSegmentView();
	DECLARE_DYNCREATE(CSegmentView)

// ����
public:
	CSegmentDoc* GetDocument() const;

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

private:
    BOOL flag;

// ʵ��
public:
	virtual ~CSegmentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnAdapthresh();
public:
    afx_msg void OnContourextract();
public:
    afx_msg void OnContourtrack();
public:
    afx_msg void OnRegiongrow();
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
    afx_msg void OnOtusthreshold();
public:
    afx_msg void OnColorprewitt();
public:
    afx_msg void OnRoberts();
public:
    afx_msg void OnSobel();
public:
    afx_msg void OnPrewitt();
public:
    afx_msg void OnKrisch();
public:
    afx_msg void OnLaplacian();
public:
    afx_msg void OnGuasslaplacian();
};

#ifndef _DEBUG  // SegmentView.cpp �еĵ��԰汾
inline CSegmentDoc* CSegmentView::GetDocument() const
   { return reinterpret_cast<CSegmentDoc*>(m_pDocument); }
#endif

