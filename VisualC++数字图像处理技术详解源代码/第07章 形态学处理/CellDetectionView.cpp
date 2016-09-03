// CellDetectionView.cpp : CCellDetectionView ���ʵ��
//

#include "stdafx.h"
#include "CellDetection.h"

#include "CellDetectionDoc.h"
#include "CellDetectionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCellDetectionView

IMPLEMENT_DYNCREATE(CCellDetectionView, CView)

BEGIN_MESSAGE_MAP(CCellDetectionView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_OPEN_IMAGE, &CCellDetectionView::OnOpenImage)
	ON_COMMAND(ID_CELL_DETECTION, &CCellDetectionView::OnCellDetection)
END_MESSAGE_MAP()

// CCellDetectionView ����/����

CCellDetectionView::CCellDetectionView()
{
	// TODO: �ڴ˴���ӹ������

}

CCellDetectionView::~CCellDetectionView()
{
}

BOOL CCellDetectionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CCellDetectionView ����

void CCellDetectionView::OnDraw(CDC* /*pDC*/)
{
	CCellDetectionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CCellDetectionView ��ӡ

BOOL CCellDetectionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CCellDetectionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CCellDetectionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CCellDetectionView ���

#ifdef _DEBUG
void CCellDetectionView::AssertValid() const
{
	CView::AssertValid();
}

void CCellDetectionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCellDetectionDoc* CCellDetectionView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCellDetectionDoc)));
	return (CCellDetectionDoc*)m_pDocument;
}
#endif //_DEBUG


// CCellDetectionView ��Ϣ�������

void CCellDetectionView::OnOpenImage( )
{
	// �˺���ʵ�ִ򿪲���ʾ��ϸ��ͼ��
	m_CellImage.LoadFromFile("CellImage.bmp");//�򿪰�ϸ��ͼ��

	//��ʾ��ϸ��ͼ��
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_CellImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_CellImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_CellImage.Draw(pDC,point,size);  
}

void CCellDetectionView::OnCellDetection()
{
	// �˺���ʵ�ְ�ϸ���˵ļ��

    long i,j;			//ѭ������
	unsigned char pixel;	//����ֵ

	CPoint point1,point2, point3,point4;	 //��ͼ���������
	CSize size;		 //��ͼ�ĳߴ�

    LPBYTE	lpSrc;			// ָ��ָ��Ҷ�ͼ���ָ��
    
	LONG lWidth=m_CellImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_CellImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�

	point1.x=0;
	point1.y=0;	
	point2.x=lWidth+20;	 //ʹ��ʾ�ĸ���ͼ�������������+20
	point2.y=0;
	point3.x=0;
	point3.y=lHeight+20;
	point4.x=lWidth+20;
	point4.y=lHeight+20; 

	size.cx= lWidth	;
	size.cy= lHeight;

	CDC *pDC=GetDC();//����豸DC

	m_CellImage.Draw(pDC,point1,size);//��ʾԭͼ��
	
	m_CellImage.RgbToGrade();//ת��Ϊ�Ҷ�ͼ��
	
	LPBYTE lpDIBBits=m_CellImage.GetData();//�ҵ��Ҷ�ͼ�����ʼλ��	
	
	//�Ҷ����촦��
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			
			// ָ��ԭͼ���j�У���i�����ص�ָ��			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			pixel = (unsigned char)*lpSrc;

			//�Ҷ����죬��СֵΪ75�����ֵΪ245
			if(pixel<75)
			{
				*lpSrc=0;
			}
			else if (pixel<245)
			{
				*lpSrc=(unsigned char)(255*(pixel-75)/(245-75));
			}
			else
			{
				*lpSrc=255;
			}
		}
	}

	m_CellImage.Draw(pDC,point2,size);//��ʾ�����ĻҶ�ͼ��
	
	//��ֵ��Ϊ��ֵͼ��
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			
			// ָ��ԭͼ���j�У���i�����ص�ָ��			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			pixel = (unsigned char)*lpSrc;

			//�趨��ֵ����ֵΪ50
			if(pixel<50)
			{
				*lpSrc=255;
			}
			
			else
			{
				*lpSrc=0;
			}
		}
	}

	m_CellImage.Draw(pDC,point3,size);   //��ʾ��ֵͼ��
	
    CBinaryMorphDib BinMorphProcess(&m_CellImage);//����һ����̬ѧ�������
	
	int Structure[3][3]={0,1,0,1,1,1,0,1,0};//����ṹԪ��

	//����������n1�Σ���������n1��3
	for(i= 0; i<3; i++) 
	{
		BinMorphProcess.Closing(Structure);
	}
	//������ʴn2�Σ���������n2��3
	for(i= 0; i<3; i++) 
	{
		BinMorphProcess.Erosion(Structure);
	}


	//���������븯ʴͬ���Ĵ���
	for(i= 0; i<3; i++) 
	{
		BinMorphProcess.Dilation(Structure);

	}
	 
	m_CellImage.Draw(pDC,point4,size);   //��ʾ�����	

	
}
