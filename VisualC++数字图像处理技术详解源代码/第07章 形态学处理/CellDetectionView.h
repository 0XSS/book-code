// CellDetectionView.h : CCellDetectionView ��Ľӿ�
//


#pragma once

#include "Dib.h"
#include "BinaryMorphDib.h"


class CCellDetectionView : public CView
{
protected: // �������л�����
	CCellDetectionView();
	DECLARE_DYNCREATE(CCellDetectionView)

// ����
public:
	CCellDetectionDoc* GetDocument() const;
	CDib m_CellImage; //����һ��CDib��Ķ���

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
	virtual ~CCellDetectionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenImage();
public:
	afx_msg void OnCellDetection();
};

#ifndef _DEBUG  // CellDetectionView.cpp �еĵ��԰汾
inline CCellDetectionDoc* CCellDetectionView::GetDocument() const
   { return reinterpret_cast<CCellDetectionDoc*>(m_pDocument); }
#endif

