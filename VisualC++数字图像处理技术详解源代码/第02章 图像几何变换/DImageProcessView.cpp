// DImageProcessView.cpp : implementation of the CDImageProcessView class
//

#include "stdafx.h"
#include "DImageProcess.h"
#include "DlgTran.h"
#include "DlgRot.h"
#include "DlgZoom.h"
#include "Dlggeo.h"

#include "matlib.h"
///#include "DlgHistogram.h"
//#include "DlgLiner.h"
//#include "DlgStretch.h"
//#include "DlgThreshold.h"
//#include "DlgWindow.h"

//#include "GrayEnhance.h"
//#include "MedianFiltDlg.h"
//#include "LaplacSharp.h"
//#include "lowpassPra.h"
//#include "higpassPara.h"
//#include "ErosionPara.h"
//#include "DilationPara.h"
//#include "OpenclosePara.h"
//#include "EdgeDetection.h"
//#include "ThresholdDivide.h"
//#include "RegionGrow.h"
//#include "Projection.h"
//#include "HuffmanCoding.h"
//#include "ShannonCoding.h"

#include "function.h"

#include "DImageProcessView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDImageProcessView* pView;
/////////////////////////////////////////////////////////////////////////////
// CDImageProcessView

IMPLEMENT_DYNCREATE(CDImageProcessView, CView)

BEGIN_MESSAGE_MAP(CDImageProcessView, CView)
	//{{AFX_MSG_MAP(CDImageProcessView)
	ON_COMMAND(ID_TRANSPOSE, OnTranspose)
	ON_COMMAND(ID_MIRROR, OnMirror)
	//ON_COMMAND(ID_HISTOGRAM, OnHistogram)
	//ON_COMMAND(ID_gray_equal, OnGrayEqual)
	ON_COMMAND(ID_geo_rotation, OnRotation)
	ON_COMMAND(ID_geo_translation, OnTranslation)
	ON_COMMAND(ID_geo_zoom, OnZoom)
	//ON_COMMAND(ID_pixel_liner, OnLiner)
	//ON_COMMAND(ID_pixel_stretch, OnStretch)
	//ON_COMMAND(ID_pixel_threshold, OnThreshold)
	//ON_COMMAND(ID_pixel_window, OnWindow)
	ON_COMMAND(ID_dct, OnDct)
	ON_COMMAND(ID_walh_har, OnWalhHar)
	//ON_COMMAND(ID_pixel_log, OnLog)
	ON_COMMAND(ID_fourier, OnFourier)
	ON_COMMAND(ID_kl, OnKL)
	//**ON_COMMAND(ID_inverRestore, OnInverRestore)
	//ON_COMMAND(ID_winnerRestore, OnWinnerRestore)
	//ON_COMMAND(ID_GRAY_ENHANCE, OnGrayEnhance)
	//ON_COMMAND(ID_HIST_ENHANCE, OnHistEnhance)
	//ON_COMMAND(ID_SMOOTH_ENHANCE, OnSmoothEnhance)
	//ON_COMMAND(ID_MEDIAN_FILTER, OnMedianFilter)
	//ON_COMMAND(ID_LAPLAS_SHARP, OnLaplasSharp)
	//ON_COMMAND(ID_LOWPASS_FILER, OnLowpassFiler)
	//ON_COMMAND(ID_HIGHPASS_FILER, OnHighpassFiler)
	//ON_COMMAND(ID_EROSION_CALCULATION, OnErosionCalculation)
	//ON_COMMAND(ID_DILATION_CALCULATION, OnDilationCalculation)
	//ON_COMMAND(ID_OPENCLOSE_CALCULATION, OnOpencloseCalculation)
	//ON_COMMAND(ID_THINING_CALCULATION, OnThiningCalculation)
	//ON_COMMAND(_EDGE_DETECTION, OnEdgeDetection)
	//ON_COMMAND(_HOUGH_TRANS, OnHoughTrans)
	//ON_COMMAND(ID_OUTLINE, OnOutline)
	//ON_COMMAND(ID_CONTOURTRACE, OnContourtrace)
	//ON_COMMAND(ID_THRESHOLD_DIVIDE, OnThresholdDivide)
	//ON_COMMAND(ID_REGION_GROW, OnRegionGrow)
	//ON_COMMAND(ID_PROJECTION, OnProjection)
	//ON_COMMAND(ID_SUBTRACT, OnSubtract)
	//ON_COMMAND(ID_TEMPLETEMATCH, OnTempletematch)
	//ON_COMMAND(ID_HUFFMANCODING, OnHuffmancoding)
	//ON_COMMAND(ID_SHANNONCODING, OnShannoncoding)
	//ON_COMMAND(ID_RLECODING, OnRlecoding)
	//ON_COMMAND(ID_GIFLZW, OnGiflzw)
	//ON_COMMAND(ID_GRAY_ENHANCE, OnGrayEnhance)
	ON_COMMAND(ID_WL, OnWl)
	//}}AFX_MSG_MAP
	//ON_COMMAND_RANGE(ID_SCAN_UPDOWN, ID_INSERT_LEFT, OnImageStunt)
	ON_COMMAND(ID_MIRROR2, &CDImageProcessView::OnMirror2)
		ON_COMMAND(ID_GEOM_ROTA, &CDImageProcessView::OnGeomRota)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessView construction/destruction

CDImageProcessView::CDImageProcessView()
{
	// TODO: add construction code here
	m_pDbImage=NULL;
	m_nDWTCurDepth=0;

}

CDImageProcessView::~CDImageProcessView()
{
}

BOOL CDImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessView drawing

void CDImageProcessView::OnDraw(CDC* pDC)
{
	// ��ȡ�ĵ�
	CDImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	HGLOBAL hDIB = pDoc->GetHObject();

	// �ж�DIB�Ƿ�Ϊ��
	if (hDIB != NULL)
	{
		LPSTR lpDibSection = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

		// ��ȡDIB���
		int cxDIB = (int) pDoc->m_dib.GetWidth(lpDibSection);

		// ��ȡDIB�߶�
		int cyDIB = (int) pDoc->m_dib.GetHeight(lpDibSection);

		::GlobalUnlock((HGLOBAL) hDIB);

		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;

		CRect rcDest= rcDIB;	
		// ���DIB
		pDoc->m_dib.DrawDib(pDC->m_hDC, &rcDest, pDoc->GetHObject(),
			&rcDIB, pDoc->GetDocPal());
	}

	/*	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��
	LPSTR	lpSrcStartBits;		//ָ��Դͼ����ʼ��ָ��
	LPBITMAPINFO lpbmi;// ָ��BITMAPINFO�ṹ��ָ��
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB		
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);// ��ȡͼ��ĸ߶�
	lpbmi = (LPBITMAPINFO)lpSrcDib;// ��ȡָ��
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);// �ҵ�DIBͼ��������ʼλ��
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CPalette * pPal=new CPalette;// �����µ�ɫ��
	pDoc->m_dib.ConstructPalette(pDoc->m_dib.m_hDib,pPal);

	CPalette * pOldPal=pDC->SelectPalette(pPal, TRUE);
	pDC->RealizePalette();
	::StretchDIBits(pDC->GetSafeHdc(),0,0, lSrcWidth, lSrcHeight,
	0, 0, lSrcWidth , lSrcHeight ,
	lpSrcStartBits, lpbmi,
	DIB_RGB_COLORS, SRCCOPY);
	pDC->SelectPalette(pOldPal, TRUE);
	::DeleteObject(pPal);*/
}

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessView diagnostics

#ifdef _DEBUG
void CDImageProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CDImageProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDImageProcessDoc* CDImageProcessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDImageProcessDoc)));
	return (CDImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG



void CDImageProcessView::OnTranspose() 
{
	// TODO: Add your command handler code here

	CDImageProcessDoc* pDoc = GetDocument();

	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	long lDstLineBytes;		//��ͼ��ÿ�е��ֽ���
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}											//�ж��Ƿ���8-bppλͼ,�����򷵻�	
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���

	/////////////////////////////////////////////////////////////////////////////////////////////////

	lDstLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcHeight * 8);	// ������ͼ��ÿ�е��ֽ���
	if (Transpose(lpSrcDib,lpSrcStartBits,lSrcWidth,
		lSrcHeight,lSrcLineBytes,lDstLineBytes))	// ����Transpose()����ת��DIB
	{		
		pDoc->SetDib();									// ����DIB��С�͵�ɫ��		
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// �������
	}
	else		
	{
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�")); // ����
	}
}

void CDImageProcessView::OnMirror() 	
{	
	CDImageProcessDoc* pDoc = GetDocument();						// ��ȡ�ĵ�											
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}											//�ж��Ƿ���8-bppλͼ,�����򷵻�	
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////
	if (Mirror(lpSrcStartBits,lSrcWidth, lSrcHeight,lSrcLineBytes))// ����Mirror()����ˮƽ����DIB
	{
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// �������
	}
	else
	{	
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));
	}// ����	
}



void CDImageProcessView::OnTranslation() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();		
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	long lDstLineBytes;		//��ͼ��ÿ�е��ֽ���
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}											//�ж��Ƿ���8-bppλͼ,�����򷵻�	
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	lDstLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcHeight * 8);	// ������ͼ��ÿ�е��ֽ���	


	CDlgTran TranPara;// �����Ի���	
	if (TranPara.DoModal() != IDOK)// ��ʾ�Ի�����ʾ�û��趨��
		return;	
	int temver=TranPara.m_verOff;
	int temhor=TranPara.m_horOff;

	if (Translation(lpSrcStartBits, lSrcWidth,lSrcHeight,// ����Translation()����ƽ��DIB
		temver,temhor,lSrcLineBytes,lDstLineBytes))		
	{
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// �������
	}
	else
	{
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));
	}//����	
}
void CDImageProcessView::OnRotation() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	long	lDstWidth;      //��ʱͼ��Ŀ�Ⱥ͸߶�
	long	lDstHeight;
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}										//�ж��Ƿ���8-bppλͼ,�����򷵻�	
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	long lDstLineBytes;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgRot RotPara;// �����Ի���	
	if(RotPara.DoModal() != IDOK)// ��ʾ�Ի����趨��ת�Ƕ�
	{
		return;	
	}
	DWORD palSize=pDoc->m_dib.GetPalSize(lpSrcDib);	

	float fRotateAngle = (float) AngleToRadian(RotPara.m_rotAngle);// ����ת�ǶȴӶ�ת��������		
	float fSina = (float) sin((double)fRotateAngle);// ������ת�Ƕȵ�������	
	float fCosa = (float) cos((double)fRotateAngle);	
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;// ��תǰ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩		
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩

	fSrcX1 = (float) (- (lSrcWidth  - 1) / 2);// ����ԭͼ���ĸ��ǵ�����
	fSrcY1 = (float) (  (lSrcHeight - 1) / 2);
	fSrcX2 = (float) (  (lSrcWidth  - 1) / 2);
	fSrcY2 = (float) (  (lSrcHeight - 1) / 2);
	fSrcX3 = (float) (- (lSrcWidth  - 1) / 2);
	fSrcY3 = (float) (- (lSrcHeight - 1) / 2);
	fSrcX4 = (float) (  (lSrcWidth  - 1) / 2);
	fSrcY4 = (float) (- (lSrcHeight - 1) / 2);

	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;// ������ͼ�ĸ��ǵ�����
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	lDstWidth= (long) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);// ������ת���ͼ��ʵ�ʿ��
	lDstLineBytes=pDoc->m_dib.GetReqByteWidth(lDstWidth * 8);// ������ͼ��ÿ�е��ֽ���
	lDstHeight= (long) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);// ������ת���ͼ��߶�

	HGLOBAL hDstDIB = NULL;// ������DIB		
	hDstDIB = (HGLOBAL) Rotate(lpSrcDib,lpSrcStartBits,lSrcWidth,lSrcHeight,lSrcLineBytes,
		palSize,lDstWidth,lDstHeight,lDstLineBytes,fSina,fCosa);// ����Rotate()������תDIB		
	if(hDstDIB != NULL)// �ж���ת�Ƿ�ɹ�
	{				
		pDoc->UpdateObject(hDstDIB);// �滻DIB��ͬʱ�ͷž�DIB����		
		pDoc->SetDib();// ����DIB��С�͵�ɫ��				
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// �������
	}
	else
	{		
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));
	}// ����		
}
void CDImageProcessView::OnZoom() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();		
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	long	lDstWidth;      //��ʱͼ��Ŀ�Ⱥ͸߶�
	long	lDstHeight;
	long lDstLineBytes;
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}						
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD palSize=pDoc->m_dib.GetPalSize(lpSrcDib);	

	CDlgZoom ZoomPara;// �����Ի���,�趨ƽ����
	if (ZoomPara.DoModal() != IDOK)
	{
		return;	
	}
	float fX = ZoomPara.m_horZoom;// ��ȡ�趨��ƽ����,���ű���
	float fY = ZoomPara.m_verZoom;

	lDstWidth= (long) (lSrcWidth*fX + 0.5);// �������ź��ͼ��ʵ�ʿ��,��0.5������ǿ������ת��ʱ���������룬����ֱ�ӽ�ȥС������
	lDstLineBytes=pDoc->m_dib.GetReqByteWidth(lDstWidth * 8);	//ת����ͼ��Ӧ�е����ֽ�����Ϊ4�ı���
	lDstHeight= (long) (lSrcHeight * fY + 0.5);// �������ź��ͼ��߶�

	HGLOBAL hDstDIB = NULL;// ������DIB	
	hDstDIB = (HGLOBAL) Zoom(lpSrcDib,lpSrcStartBits,lSrcWidth,lSrcHeight,
		lSrcLineBytes,palSize,lDstWidth,lDstLineBytes,lDstHeight,fX, fY);// ����Zoom()����ת��DIB		

	if(hDstDIB != NULL)// �ж���ת�Ƿ�ɹ�
	{				
		pDoc->UpdateObject(hDstDIB);// �滻DIB��ͬʱ�ͷž�DIB����		
		pDoc->SetDib();// ����DIB��С�͵�ɫ��				
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// �������
	}
	else
	{		
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));
	}
	// ����	
}




void CDImageProcessView::OnDct() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();	
	SetCursor(LoadCursor(NULL, IDC_WAIT));//�����ʾΪ����״̬
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}		
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////		
	// ����Dct()����������ɢ���ұ任
	if (CosTran(lpSrcStartBits, lSrcWidth, lSrcHeight,lSrcLineBytes))
	{
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// �������
	}
	else
	{		
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));
	}// ����
	SetCursor(LoadCursor(NULL, IDC_ARROW));//�����ʾΪ����״̬
}

void CDImageProcessView::OnWalhHar() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}		
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////			
	// ����Walsh()���б任
	if (Walsh_HarTran(lpSrcStartBits, lSrcWidth, lSrcHeight,lSrcLineBytes))
	{
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// �������
	}
	else
	{		
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));
	}// ����
}

void CDImageProcessView::OnLog() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();// ��ȡ�ĵ�				
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}		
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	LogTranslation(lpSrcStartBits, lSrcWidth, lSrcHeight,lSrcLineBytes);	
	pDoc->SetModifiedFlag(TRUE);								// ��������		
	pDoc->UpdateAllViews(NULL);									// ������ͼ		
	::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// �������
}

void CDImageProcessView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	pView=this;
}


void CDImageProcessView::OnFourier() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();
	SetCursor(LoadCursor(NULL, IDC_WAIT));//�����ʾΪ����״̬
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}		
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////		
	double dTmp;// ��ʱ����
	long	wid=1,hei=1;// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���ʼ��Ϊ1		
	int		widpor=0,heiPor=0;//2������	
	unsigned char*	lpSrcUnChr;	//ָ�����ص�ָ��
	long i;                 //��ѭ������
	long j;                 //��ѭ������	
	while(wid * 2 <= lSrcWidth)// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	{
		wid *= 2;
		widpor++;
	}	
	while(hei * 2 <= lSrcHeight)
	{
		hei *= 2;
		heiPor++;
	}
	CplexNum *pTd = new CplexNum[sizeof(CplexNum)*wid * hei];// �����ڴ�
	CplexNum *pFd = new CplexNum[sizeof(CplexNum)*wid * hei];
	// ��ʼ��
	// ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData
	// ��һ����������Ҫ��0
	for(i=0; i<hei; i++)
	{
		for(j=0; j<wid; j++)
		{
			pTd[i*wid + j].re=0;
			pTd[i*wid + j].im=0;
		}
	}
	// ��ͼ�����ݴ���pCTData
	for(i=0; i<lSrcHeight; i++)
	{
		for(j=0; j<lSrcWidth; j++)
		{
			lpSrcUnChr=(unsigned char*)lpSrcStartBits+lSrcLineBytes*(lSrcHeight-1-i)+j;
			pTd[i*wid + j].re=*lpSrcUnChr;//complex<double>(unchValue,0);
			pTd[i*wid + j].im=0;
		}
	}
	Fourier(pTd, lSrcWidth, lSrcHeight, pFd) ;

	for(i=0; i<lSrcHeight; i++)
	{
		for(j=0; j<lSrcWidth; j++)
		{
			dTmp = pFd[i * wid + j].re* pFd[i * wid + j].re
				+ pFd[i * wid + j].im* pFd[i * wid + j].im;			
			dTmp = sqrt(dTmp) ;

			// Ϊ����ʾ����Ҫ�Է��ȵĴ�С��������
			dTmp /= 100  ;
			// ����ͼ�����ݵĴ�С
//			dTmp = min(dTmp, 255) ;
			lpSrcStartBits[i*lSrcLineBytes +j] = (unsigned char)(int)dTmp;
		}
	}
	// Ϊ������Ļ����ʾ�����ǰѷ���ֵ��Ĳ����ú�ɫ��ʾ
	for(i=0; i<lSrcHeight; i++)
	{
		for(j=0; j<lSrcWidth; j++)
		{
			lpSrcStartBits[i*lSrcLineBytes +j] = 255 - lpSrcStartBits[i*lSrcLineBytes +j];
		}
	}			
	delete pTd;// �ͷ��ڴ�
	delete pFd;	
	pDoc->SetModifiedFlag(TRUE);// ��������			
	pDoc->UpdateAllViews(NULL);// ������ͼ
	SetCursor(LoadCursor(NULL, IDC_ARROW));//�����ʾΪ����״̬
}

void CDImageProcessView::OnKL() 
{
	// TODO: Add your command handler code here
	// ��ȡ�ĵ�
	CDImageProcessDoc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}		
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//����DisK_L�任
	if (DisK_L(lpSrcStartBits,lSrcWidth,lSrcHeight,lSrcLineBytes))
	{
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// �������
	}
	else
	{		
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));// ����
	}
}







void CDImageProcessView::OnWl() 
{
	CDImageProcessDoc* pDoc = GetDocument();					//��ȡ�ĵ�
	LPSTR lpDIB;											//ָ��Դͼ���ָ��
	LPSTR    lpDIBBits;									//ָ��DIB����ָ��
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());		//����DIB
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);							//�ҵ�DIBͼ��������ʼλ��
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)							//�ж��Ƿ���256ɫλͼ
	{
		//��ʾ�û�
		//MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);	
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());				//�������
		return;											//����
	}
	int nWidth;												//ͼ����
	int nHeight;												//ͼ��߶�
	nWidth=(int)pDoc->m_dib.GetWidth(lpDIB);									//��ȡͼ����
	nHeight=(int)pDoc->m_dib.GetHeight(lpDIB);									//��ȡͼ��߶�

	DIBDWTStep(lpDIBBits,m_pDbImage, nWidth,nHeight, 0, m_nDWTCurDepth,1);
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHObject());

}

void CDImageProcessView::OnMirror2()
{
	// TODO: �ڴ���������������
	CDImageProcessDoc* pDoc = GetDocument();						// ��ȡ�ĵ�											
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{		
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// �������		
		return;									//����
	}											//�ж��Ƿ���8-bppλͼ,�����򷵻�	
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////
	if (Mirror2(lpSrcStartBits,lSrcWidth, lSrcHeight,lSrcLineBytes))// ����Mirror2()����ˮƽ����DIB
	{
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// �������
	}
	else
	{	
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));
	}// ����
} 



void CDImageProcessView::OnGeomRota()
{
	// TODO: �ڴ���������������
	// ͼ����ת

	CDImageProcessDoc* pDoc = GetDocument();
	long lSrcLineBytes;		                       //ͼ��ÿ�е��ֽ���
	long	lSrcWidth;                                                     //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		                                      //ָ��Դͼ���ָ��
	LPSTR	lpSrcStartBits;	                                     //ָ��Դ���ص�ָ��
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());//lpSrcDib=lpDIB GetHDIB=GetHObject
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ת�������Ŀ������ƣ�
	if (pDoc->m_dib.GetColorNum(lpSrcDib)  != 256)
	{
		 AfxMessageBox (_T("�Բ��𣬲���ɫλͼ��"));                           // ����
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());                     // �������
		return;						             //����
	}

	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);		// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);		// ��ȡͼ��Ŀ��		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);		// ��ȡͼ��ĸ߶�		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);	   // ����ͼ��ÿ�е��ֽ���

	CDlggeo RotPara;//CGeoRotaDlg dlgPara;
	//dlgPara.m_iRotateAngle = 90;	
	// ��ʾ�Ի�����ʾ�û��趨��ת�Ƕ�
	if (RotPara.DoModal() != IDOK)
	{
		return;
	}
	float fRotateAngle = (float) AngleToRadian(RotPara.m_rotAngle);
	
	delete RotPara;	
	DWORD palSize=pDoc->m_dib.GetPalSize(lpSrcDib);

	HGLOBAL hNewDIB = NULL;//HDIB
	BeginWaitCursor();

	hNewDIB = (HGLOBAL) RotateDIB2(lpSrcDib, fRotateAngle,lpSrcStartBits,lSrcWidth,lSrcHeight,palSize);
	if (hNewDIB != NULL)
	{		
		pDoc->UpdateObject(hNewDIB);			// �滻DIB��ͬʱ�ͷž�DIB����		
		pDoc->SetDib();				           // ����DIB��С�͵�ɫ��		
		pDoc->SetModifiedFlag(TRUE);		   // ��������		
		//SetScrollSizes(MM_TEXT, pDoc->GetDocSize());// �������ù�����ͼ��С	
		pDoc->UpdateAllViews(NULL);			// ������ͼ
		
	}
	else
	{
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));//MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	::GlobalUnlock((HGLOBAL) pDoc->GetHObject());  // �������//::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	EndWaitCursor();
}
