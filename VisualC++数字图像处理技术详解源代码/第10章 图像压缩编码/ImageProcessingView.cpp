// ImageProcessingView.cpp : implementation of the CImageProcessingView class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"
#include  "GlobalApi.h"
#include  "DlgCoding.h"
#include <complex>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessingView)
	ON_COMMAND(ID_CODING_SHANFINO, OnCodingShanfino)
	ON_COMMAND(ID_CODING_ARITH, OnCodingArith)
	ON_COMMAND(ID_CODING_BITPLANE, OnCodingBitplane)
	ON_COMMAND(ID_CODING_HUFFMAN, OnCodingHuffman)
	ON_COMMAND(ID_CODING_LOADIMG, OnCodingLoadimg)
	ON_COMMAND(ID_CODING_WRITEIMG, OnCodingWriteimg)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

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

// CImageProcessingView message handlers






void CImageProcessingView::OnCodingHuffman() 
{
	// �����������
	
	// ��ȡ�ĵ�
	CImageProcessingDoc * pDoc = GetDocument();
	
	// ָ��Դͼ�����ص�ָ��
	unsigned char *	lpSrc;
		
	// ͼ��ĸ߶ȺͿ��
	LONG	lHeight;
	LONG	lWidth;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ͼ����������
	LONG	lCountSum;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ����ָ��������������Ҷ�ֵ���ָ���
	double * dProba;
	
	// ��ǰͼ����ɫ��Ŀ
	int		nColorNum;

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;
			
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ��ֻ����8-bppλͼ�Ļ��������룩
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ�Ļ��������룡", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
//---------------------------------------
//	   ��ʼ��������Ҷȼ����ֵĸ���	
//	   �����Ҫ��ָ�������н��й���������,
//	   ֻҪ����һ���ĳɸ������Ҷȼ����ʸ�ֵ����
//---------------------------------------
	
	//  ��ͷ�ļ���Ϣ�õ�ͼ��ı��������Ӷ��õ���ɫ��Ϣ
	nColorNum = (int)pow(2.0f,lpBMIH->biBitCount);

	// �����ڴ�
	dProba = new double[nColorNum];
	
	//�õ�ͼ��Ŀ�Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;

	// ����ͼ����������
	lCountSum = lHeight * lWidth;
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	// ����ֵ
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = 0.0;
	}
	
	// ��������Ҷ�ֵ�ļ���
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// ָ��ͼ��ָ��
			lpSrc = lpDIBBits + lLineBytes * i + j;
			
			// ������1
			dProba[*(lpSrc)] = dProba[*(lpSrc)] + 1;
		}
	}
		
	
	// ��������Ҷ�ֵ���ֵĸ���
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = dProba[i] / (FLOAT)lCountSum;
	}
	
//---------------------------------------
//	 ������������������
//	 ���öԻ�����ʾ���������
//---------------------------------------
	
	// �����Ի���
	CDlgHuffman dlgCoding;
	
	// ��ʼ������ֵ
	dlgCoding.dProba = dProba;
	dlgCoding.nColorNum = nColorNum;
	
	// ��ʾ�Ի���
	dlgCoding.DoModal();
		
	// �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnCodingShanfino() 
{
	// ��ũ����ŵ�����
	
	// ��ȡ�ĵ�
	CImageProcessingDoc * pDoc = GetDocument();
	
	// ָ��Դͼ�����ص�ָ��
	unsigned char *	lpSrc;
		
	// ͼ��ĸ߶�
	LONG	lHeight;
	LONG	lWidth;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ��ȡ��ǰDIB��ɫ��Ŀ
	int		nColorNum;
	
	// ͼ����������
	LONG	lCountSum;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ��������Ҷ�ֵ���ָ��ʵ�����ָ��
	double * dProba;	

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;
			
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ũ����ŵ���룩
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ũ����ŵ���룡", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	//---------------------------------------
	// ��ʼ��������Ҷȼ����ֵĸ���	
	//
	// �����Ҫ��ָ�������н�����ũ����ŵ����,
	//ֻҪ����һ���ĳɸ������Ҷȼ����ʸ�ֵ����
	//---------------------------------------
	//  �Ҷ�ֵ�����ļ���
	nColorNum = (int)pow(2.0,lpBMIH->biBitCount);

	// �����ڴ�
	dProba = new double[nColorNum];
	
	//�õ�ͼ��Ŀ�Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;

	// ����ͼ����������
	lCountSum = lHeight * lWidth;
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;

	// ����ͼ����������
	lCountSum = lHeight * lWidth;
	
	// ���ü���Ϊ0
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = 0.0;
	}
	
	// ��������Ҷ�ֵ�ļ��������ڷ�256ɫλͼ���˴�������dProba��ֵ��������ͬ��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// ָ��ͼ��ָ��
			lpSrc = lpDIBBits + lLineBytes * i + j;
			
			// ������1
			dProba[*(lpSrc)] = dProba[*(lpSrc)]+ 1;
		}
	}
	
	
	// ��������Ҷ�ֵ���ֵĸ���
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] /= (double)lCountSum;
	}
	
	//---------------------------------------
	// ������ũ����ŵ��������
	// ���öԻ�����ʾ��ũ����ŵ���
	//---------------------------------------
	
	// �����Ի���
	CDlgShannon dlgPara;
	
	// ��ʼ������ֵ
	dlgPara.m_dProba = dProba;
	dlgPara.m_nColorNum = nColorNum;
	
	// ��ʾ�Ի���
	dlgPara.DoModal();

	//�ͷ��ڴ�
	delete dProba;
		
	// �ָ����
	EndWaitCursor();
}

void CImageProcessingView::OnCodingArith() 
{
	CDlgArith dlgCoding;
	
	// ��ʾ�Ի���
	dlgCoding.DoModal();
	
}

void CImageProcessingView::OnCodingBitplane() 
{
	// �����Ի���
	CDlgBitPlane dlgCoding;	
	
	// ��ʾ�Ի���
	dlgCoding.DoModal();	
	
	BYTE bBitNum = dlgCoding.m_BItNumber;

	// ��ȡ�ĵ�
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;
	
	// ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��λƽ��ֽ⣩
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��λƽ��ֽ⣡", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	DIBBITPLANE(pDib,bBitNum);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessingView::OnCodingWriteimg() 
{
	// �Ե�ǰͼ�����DPCM���루��ΪIMG��ʽ�ļ���
	
	// ��ȡ�ĵ�
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;
			
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������8-bppλͼ��DPCM���룩
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��DPCM���룡", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// �ļ�����·��
	CString strFilePath;
	
	// ��ȡԭʼ�ļ���
	strFilePath = pDoc->GetPathName();
	
	// ���ĺ�׺ΪIMG
	if (strFilePath.Right(4).CompareNoCase(".BMP") == 0)
	{	
		strFilePath = strFilePath.Left(strFilePath.GetLength()-3) + "IMG";
	}
	else
	{
		strFilePath += ".IMG";
	}

	// ����SaveAs�Ի���
	CFileDialog dlg(FALSE, "IMG", strFilePath, 
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		            "IMGͼ���ļ� (*.IMG) | *.IMG|�����ļ� (*.*) | *.*||", NULL);
	
	// ��ʾ�û�ѡ�񱣴��·��
	if (dlg.DoModal() != IDOK)
	{
		// �ָ����
		EndWaitCursor();
	
		return;
	}
	
	// ��ȡ�û�ָ�����ļ�·��
	strFilePath = dlg.GetPathName();
	
	// CFile��CFileException����
	CFile file;
	CFileException fe;
	
	// ���Դ���ָ����IMG�ļ�
	if (!file.Open(strFilePath, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		MessageBox("��ָ��IMG�ļ�ʱʧ�ܣ�", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
	
		return;
	}
	
	// ����WRITE2IMG()��������ǰ��DIB����ΪIMG�ļ�
	if (::WRITE2IMG(pDib, file))
	{
		MessageBox("�ɹ�����ΪIMG�ļ���", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		MessageBox("����ΪIMG�ļ�ʧ�ܣ�", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
	}	
		
	// �ָ����
	EndWaitCursor();
	
}


void CImageProcessingView::OnCodingLoadimg() 
{
	// ����IMG�ļ�
	
	// ��ȡ�ĵ�
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// �ļ�·��
	CString strFilePath;
	
	// ����Open�Ի���
	CFileDialog dlg(TRUE, "PCX", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					"IMGͼ���ļ� (*.PCX) | *.IMG|�����ļ� (*.*) | *.*||", NULL);
	
	// ��ʾ�û�ѡ�񱣴��·��
	if (dlg.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û�ָ�����ļ�·��
	strFilePath = dlg.GetPathName();
	
	// CFile��CFileException����
	CFile file;
	CFileException fe;
	
	// ���Դ�ָ����PCX�ļ�
	if (!file.Open(strFilePath, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// ��ʾ�û�
		MessageBox("��ָ��PCX�ļ�ʱʧ�ܣ�", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ����LOADIMG()������ȡָ����IMG�ļ�
	BOOL Succ = LOADIMG(pDib, file);

	if (Succ == TRUE)
	{
		// ��ʾ�û�
		MessageBox("�ɹ���ȡIMG�ļ���", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
	}
	else
	{
		// ��ʾ�û�
		MessageBox("��ȡIMG�ļ�ʧ�ܣ�", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
	}

	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
	
}

