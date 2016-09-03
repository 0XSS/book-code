// SegmentView.cpp : CSegmentView ���ʵ��
//

#include "stdafx.h"
#include "Segment.h"

#include "SegmentDoc.h"
#include "SegmentView.h"

#include "EdgeDetection.h"
#include "Threshold.h"
#include "ContourSegment.h"
#include "RegionSegment.h"
#include "ColorSeg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSegmentView

IMPLEMENT_DYNCREATE(CSegmentView, CView)

BEGIN_MESSAGE_MAP(CSegmentView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_COMMAND(ID_ADAPTHRESH, &CSegmentView::OnAdapthresh)
    ON_COMMAND(ID_CONTOUREXTRACT, &CSegmentView::OnContourextract)
    ON_COMMAND(ID_CONTOURTRACK, &CSegmentView::OnContourtrack)
    ON_COMMAND(ID_REGIONGROW, &CSegmentView::OnRegiongrow)
    ON_WM_LBUTTONDOWN()
    ON_COMMAND(ID_OTUSTHRESHOLD, &CSegmentView::OnOtusthreshold)
    ON_COMMAND(ID_COLORPREWITT, &CSegmentView::OnColorprewitt)
    ON_COMMAND(ID_ROBERTS, &CSegmentView::OnRoberts)
    ON_COMMAND(ID_SOBEL, &CSegmentView::OnSobel)
    ON_COMMAND(ID_PREWITT, &CSegmentView::OnPrewitt)
    ON_COMMAND(ID_KRISCH, &CSegmentView::OnKrisch)
    ON_COMMAND(ID_LAPLACIAN, &CSegmentView::OnLaplacian)
    ON_COMMAND(ID_GUASSLAPLACIAN, &CSegmentView::OnGuasslaplacian)
END_MESSAGE_MAP()

// CSegmentView ����/����

CSegmentView::CSegmentView()
{
	// TODO: �ڴ˴���ӹ������
    flag = FALSE;

}

CSegmentView::~CSegmentView()
{
}

BOOL CSegmentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CSegmentView ����

void CSegmentView::OnDraw(CDC* pDC)
{
	CSegmentDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        CSize size = pDib->GetDimension();
        pDib->Draw(pDC, CPoint(0,0), size);
    }
}


// CSegmentView ��ӡ

BOOL CSegmentView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSegmentView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CSegmentView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CSegmentView ���

#ifdef _DEBUG
void CSegmentView::AssertValid() const
{
	CView::AssertValid();
}

void CSegmentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSegmentDoc* CSegmentView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSegmentDoc)));
	return (CSegmentDoc*)m_pDocument;
}
#endif //_DEBUG


void CSegmentView::OnAdapthresh()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
            return;
        }
        else
        {
            CThreshold threshold(pDib);
        threshold.AdaptiveThreshold();
        Invalidate();
        }        
    }    
}

void CSegmentView::OnContourextract()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
            return;
        }
        else
        {
            CContourSegment contourSeg(pDib);
            contourSeg.ContourExtract();
            Invalidate();
        }        
    }
}

void CSegmentView::OnContourtrack()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
            return;
        }
        else
        {
            CContourSegment contourSeg(pDib);
            contourSeg.ContourTrack();
            Invalidate();
        }        
    }
}

void CSegmentView::OnRegiongrow()
{
    // TODO: �ڴ���������������
    AfxMessageBox("����ͼ�ϵ���������ѡȡ���ӵ㣡");
    flag = TRUE;
}

void CSegmentView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if(flag)
    {
        flag = FALSE;
        CSegmentDoc *pDoc = GetDocument();
        CDib *pDib = pDoc->GetDib();
        if(pDib->IsValid())
        {
            if(!pDib->IsGrade())
            {
                AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
                return;
            }
            else
            { 
                CRegionSegment regionSeg(pDib);
                regionSeg.RegionGrow(point);
                Invalidate();
            }       
        }
    }

    CView::OnLButtonDown(nFlags, point);
}

void CSegmentView::OnOtusthreshold()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
            return;
        }
        else
        {
            CThreshold threshold(pDib);
            threshold.OtusThreshold();
            Invalidate();
        }       
    }
}

void CSegmentView::OnColorprewitt()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧��24λ���λͼ��");
            return;
        }
        else
        {
            CColorSeg colorSeg(pDib);
            colorSeg.Prewitt();
            Invalidate();
        }
    }
}

void CSegmentView::OnRoberts()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.Roberts();
            Invalidate();
        }       
    }
}

void CSegmentView::OnSobel()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.Sobel();
            Invalidate();
        }       
    }
}

void CSegmentView::OnPrewitt()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.Prewitt();
            Invalidate();
        }       
    }
}

void CSegmentView::OnKrisch()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.Krisch();
            Invalidate();
        }       
    }
}

void CSegmentView::OnLaplacian()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.Laplacian();
            Invalidate();
        }       
    }
}

void CSegmentView::OnGuasslaplacian()
{
    // TODO: �ڴ���������������
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("������ֻ֧�ֻҶ�λͼ��");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.GuassLaplacian();
            Invalidate();
        }       
    }
}