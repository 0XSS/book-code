// ImageMatchingView.cpp : implementation of the CImageMatchingView class
//

#include "stdafx.h"
#include "ImageMatching.h"

#include "ImageMatchingDoc.h"
#include "ImageMatchingView.h"

#include "CDib.h"
#include "GlobalApi.h"
#include "DlgImageMatchGYH.h"
#include "DlgImageMatchSSDA.h"
#include "DlgImageMatchMoment.h"
#include "DlgImageMatchDis.h"
#include "DlgImageMatchLSC.h"
#include "DlgImageMatchYG.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingView

IMPLEMENT_DYNCREATE(CImageMatchingView, CView)

BEGIN_MESSAGE_MAP(CImageMatchingView, CView)
	//{{AFX_MSG_MAP(CImageMatchingView)
	ON_COMMAND(ID_IMAGEMATCH_SSDA, OnImagematchSsda)
	ON_COMMAND(ID_IMAGEMATCH_MOMENT, OnImagematchMoment)
	ON_COMMAND(ID_IMAGEMATCH_DIS, OnImagematchDis)
	ON_COMMAND(ID_IMAGEMATCH_GYH, OnImagematchGyh)
	ON_COMMAND(ID_IMAGEMATCH_LSC, OnImagematchLsc)
	ON_COMMAND(ID_IMAGEMATCH, OnImagematch)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingView construction/destruction

CImageMatchingView::CImageMatchingView()
{
	// TODO: add construction code here

}

CImageMatchingView::~CImageMatchingView()
{
}

BOOL CImageMatchingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingView drawing

void CImageMatchingView::OnDraw(CDC* pDC)
{
	CImageMatchingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CSize sizeDibDisplay;		
	

	
	if(!pDoc->m_pDibInit->IsEmpty()){	
		sizeDibDisplay = pDoc->m_pDibInit->GetDimensions();
		pDoc->m_pDibInit->Draw(pDC,CPoint(0,0),sizeDibDisplay);	
	}	
}

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingView printing

BOOL CImageMatchingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageMatchingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageMatchingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingView diagnostics

#ifdef _DEBUG
void CImageMatchingView::AssertValid() const
{
	CView::AssertValid();
}

void CImageMatchingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageMatchingDoc* CImageMatchingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageMatchingDoc)));
	return (CImageMatchingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingView message handlers

void CImageMatchingView::OnImagematchSsda() 
{
	CImageMatchingDoc* pDoc = GetDocument();
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ��ʶ��Ի���
	CDlgImageMatchSSDA* pDlg = new CDlgImageMatchSSDA(NULL, pDoc);
	pDlg->DoModal();
	
	delete pDlg;	
}

void CImageMatchingView::OnImagematchMoment() 
{
	// TODO: Add your command handler code here
	CImageMatchingDoc* pDoc = GetDocument();
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ��ʶ��Ի���
	CDlgImageMatchMoment* pDlg = new CDlgImageMatchMoment(NULL, pDoc);
	pDlg->DoModal();
	
	delete pDlg;	
}

void CImageMatchingView::OnImagematchDis() 
{
	// TODO: Add your command handler code here
	CImageMatchingDoc* pDoc = GetDocument();
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ��ʶ��Ի���
	CDlgImageMatchDis* pDlg = new CDlgImageMatchDis(NULL, pDoc);
	pDlg->DoModal();
	
	delete pDlg;	
}

void CImageMatchingView::OnImagematchGyh() 
{
	// TODO: Add your command handler code here
	CImageMatchingDoc* pDoc = GetDocument();
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ��ʶ��Ի���
	CDlgImageMatchGYH* pDlg = new CDlgImageMatchGYH(NULL, pDoc);
	pDlg->DoModal();
	
	delete pDlg;
}

void CImageMatchingView::OnImagematchLsc() 
{
	// TODO: Add your command handler code here
	CImageMatchingDoc* pDoc = GetDocument();
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ��ʶ��Ի���
	CDlgImageMatchLSC* pDlg = new CDlgImageMatchLSC(NULL, pDoc);
	pDlg->DoModal();
	
	delete pDlg;
}

void CImageMatchingView::OnImagematch() 
{
	// TODO: Add your command handler code here
	CImageMatchingDoc* pDoc = GetDocument();
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ��ʶ��Ի���
	CDlgImageMatchYG* pDlg = new CDlgImageMatchYG(NULL, pDoc);
	pDlg->DoModal();
	
	delete pDlg;
}
