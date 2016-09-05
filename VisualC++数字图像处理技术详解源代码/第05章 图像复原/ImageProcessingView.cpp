// ImageProcessingView.cpp : implementation of the CImageProcessingView class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"
#include  "GlobalApi.h"

#include <complex>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessingView)
	ON_COMMAND(ID_DEGENERATION_INVERSE, OnDegenerationInverse)
	ON_COMMAND(ID_DEGENERATION_MOTION, OnDegenerationMotion)
	ON_COMMAND(ID_RESTORE_INVERSE, OnRestoreInverse)
	ON_COMMAND(ID_RESTORE_MOTION, OnRestoreMotion)
	ON_COMMAND(ID_RESTORE_WINNER, OnRestoreWinner)
	ON_COMMAND(ID_RESTORE_MINDBL, OnRestoreMindbl)
	ON_COMMAND(ID_RESTORE_MAXPRO, OnRestoreMaxpro)
	ON_COMMAND(ID_RESTORE_PROJECT, OnRestoreProject)
	ON_COMMAND(ID_RESTORE_BLIND, OnRestoreBlind)
	ON_COMMAND(ID_RESTORE_GEO, OnRestoreGeo)
	ON_COMMAND(ID_RESTORE_MAXENTR, OnRestoreMaxentr)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView construction/destruction

CImageProcessingView::CImageProcessingView()
{
	// ΪС���任���õĲ���
	// ��ʱ���С���任ϵ���ڴ�
	m_pDbImage = NULL;	
	
	// ���õ�ǰ����
	m_nDWTCurDepth = 0;

	// ����С������֧������
	m_nSupp = 1;
}

CImageProcessingView::~CImageProcessingView()
{
	// �ͷ��ѷ����ڴ�
	if(m_pDbImage){
		delete[]m_pDbImage;
		m_pDbImage = NULL;
	}
}

BOOL CImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView drawing

void CImageProcessingView::OnDraw(CDC* pDC)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeDibDisplay;		
	

	
	if(!pDoc->m_pDibInit->IsEmpty()){	
		sizeDibDisplay = pDoc->m_pDibInit->GetDimensions();
		pDoc->m_pDibInit->Draw(pDC,CPoint(0,0),sizeDibDisplay);	
	}	

}

void CImageProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeTotal = pDoc->m_pDibInit->GetDimensions();
	SetScrollSizes(MM_TEXT, sizeTotal);

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView printing

BOOL CImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView diagnostics

#ifdef _DEBUG
void CImageProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessingDoc* CImageProcessingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingDoc)));
	return (CImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView message handlers




void CImageProcessingView::OnDegenerationInverse() 
{
	// ͼ���ģ��
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ͼ��ģ����
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ģ����", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBNoRestriction(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreInverse() 
{
	// ͼ������˲�
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�����˲���
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBInverseFilter(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnDegenerationMotion() 
{
	// ͼ����˶�ģ��
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ���˶�ģ����
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ���˶�ģ����", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBMotionDegeneration(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreMotion() 
{
	// �˶�ģ��ͼ��ԭ
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ���˶�ģ����ԭ��
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ���˶�ģ����ԭ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBMotionRestore(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreWinner() 
{
	// ͼ���ά���˲�
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ά���˲���
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ά���˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBWinnerFilter(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreMindbl() 
{
	// ͼ�����Сƽ���˲�
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ά���˲���
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ά���˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBMinDblFilter(pDib);
	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreMaxpro() 
{
	// ͼ����������˲�
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ά���˲���
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ά���˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBMaxProFilter(pDib);
	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreProject() 
{
	// ͼ���ͶӰ�˲�
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ά���˲���
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ά���˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBProjectFilter(pDib);
	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreBlind() 
{
	// ͼ��ĵ���äĿ�˲�
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ά���˲���
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ά���˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBBlindFilter(pDib);
	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();	
}



void CImageProcessingView::OnRestoreGeo() //���θ�ԭ
{
	// ���Ĺ����״
	BeginWaitCursor();
	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();
	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;
	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ά���˲���
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ά���˲���", "ϵͳ��ʾ" ,MB_ICONINFORMATION | MB_OK);		
		// ����
		return;
	}	
	::DIBGeoFilter(pDib);
	// ��������
	pDoc->SetModifiedFlag(TRUE);		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
    // �ָ����
	EndWaitCursor();	
}

void CImageProcessingView::OnRestoreMaxentr() 
{
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ά���˲���
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ά���˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBMaxEntrFilter(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}
