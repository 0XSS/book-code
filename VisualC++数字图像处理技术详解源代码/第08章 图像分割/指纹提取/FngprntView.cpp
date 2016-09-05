// FngprntView.cpp : CFngprntView ���ʵ��
//

#include "stdafx.h"
#include "Fngprnt.h"

#include "FngprntDoc.h"
#include "FngprntView.h"

#include "Threshold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFngprntView

IMPLEMENT_DYNCREATE(CFngprntView, CView)

BEGIN_MESSAGE_MAP(CFngprntView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_COMMAND(ID_FNGPRNT, &CFngprntView::OnFngprnt)
END_MESSAGE_MAP()

// CFngprntView ����/����

CFngprntView::CFngprntView()
{
	// TODO: �ڴ˴���ӹ������

}

CFngprntView::~CFngprntView()
{
}

BOOL CFngprntView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CFngprntView ����

void CFngprntView::OnDraw(CDC* pDC)
{
	CFngprntDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���

    // ��ȡλͼָ��
    CDib *pDib = pDoc->GetDib();

    // ���λͼ��Ч
    if(pDib->IsValid())
    {
        // ��ȡλͼ��С
        CSize size = pDib->GetDimension();

        // �ڿͻ�������ʾλͼ
        pDib->Draw(pDC, CPoint(0,0), size);
    }
}


// CFngprntView ��ӡ

BOOL CFngprntView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CFngprntView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CFngprntView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CFngprntView ���

#ifdef _DEBUG
void CFngprntView::AssertValid() const
{
	CView::AssertValid();
}

void CFngprntView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFngprntDoc* CFngprntView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFngprntDoc)));
	return (CFngprntDoc*)m_pDocument;
}
#endif //_DEBUG


// CFngprntView ��Ϣ�������

void CFngprntView::OnFngprnt()
{
    // TODO: �ڴ���������������

    // ��ȡ�ĵ����ָ��
    CFngprntDoc *pDoc = GetDocument();

    // ��ȡλͼָ��
    CDib *pDib = pDoc->GetDib();

    // ���λͼ��Ч
    if(pDib->IsValid())
    {
        // ����CThreshold���е���󷽲���ֵ�ָ��
        CThreshold fngprnt(pDib);
        fngprnt.OtusThreshold();

        // �ͻ����ػ�
        Invalidate();
    }
}
