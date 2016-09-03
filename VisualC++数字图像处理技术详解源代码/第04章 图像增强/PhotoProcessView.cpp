// PhotoProcessView.cpp : CPhotoProcessView ���ʵ��
//

#include "stdafx.h"
#include "PhotoProcess.h"

#include "PhotoProcessDoc.h"
#include "PhotoProcessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPhotoProcessView

IMPLEMENT_DYNCREATE(CPhotoProcessView, CView)

BEGIN_MESSAGE_MAP(CPhotoProcessView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_COMMAND(ID_OPEN_PHOTO, &CPhotoProcessView::OnOpenPhoto)
    ON_COMMAND(ID_LINEAR_TRANSFORM, &CPhotoProcessView::OnLinearTransform)
    ON_COMMAND(ID_SEG_LINE_TRANSFORM, &CPhotoProcessView::OnSegLineTransform)
    ON_COMMAND(ID_LOG_TRANSFORM, &CPhotoProcessView::OnLogTransform)
    ON_COMMAND(ID_HISTOGRAM_EQUAL, &CPhotoProcessView::OnHistogramEqual)
    ON_COMMAND(ID_HISTOGRAM_MATCH, &CPhotoProcessView::OnHistogramMatch)
    ON_COMMAND(ID_GATE_GRAD, &CPhotoProcessView::OnGateGrad)
    ON_COMMAND(ID_LAPLACIAN, &CPhotoProcessView::OnLaplacian)
    
    ON_COMMAND(ID_AVERAGE_SMOOTH, &CPhotoProcessView::OnAverageSmooth)
    ON_COMMAND(ID_VALUE_AVRG_SMOOTH, &CPhotoProcessView::OnValueAvrgSmooth)
    ON_COMMAND(ID_SELECT_SMOOTH, &CPhotoProcessView::OnSelectSmooth)
    ON_COMMAND(ID_MIDDLE_SMOOTH, &CPhotoProcessView::OnMiddleSmooth)
    ON_COMMAND(ID_PREFECT_LOW_FILTER, &CPhotoProcessView::OnPrefectLowFilter)
    ON_COMMAND(ID_PREFECT_HIGH_FILTER, &CPhotoProcessView::OnPrefectHighFilter)
    ON_COMMAND(ID_PSEUDO_COLOR_ENHANCE, &CPhotoProcessView::OnPseudoColorEnhance)
END_MESSAGE_MAP()

// CPhotoProcessView ����/����

CPhotoProcessView::CPhotoProcessView()
{
	// TODO: �ڴ˴���ӹ������
    flag=FALSE;

}

CPhotoProcessView::~CPhotoProcessView()
{
}

BOOL CPhotoProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPhotoProcessView ����

void CPhotoProcessView::OnDraw(CDC* /*pDC*/)
{
	CPhotoProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPhotoProcessView ��ӡ

BOOL CPhotoProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPhotoProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPhotoProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CPhotoProcessView ���

#ifdef _DEBUG
void CPhotoProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CPhotoProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPhotoProcessDoc* CPhotoProcessView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhotoProcessDoc)));
	return (CPhotoProcessDoc*)m_pDocument;
}
#endif //_DEBUG


// CPhotoProcessView ��Ϣ�������

void CPhotoProcessView::OnOpenPhoto()
{
    // �����������Ǵ򿪲���ʾ��Ƭͼ��
    CString  filename;
    CFileDialog dlg(TRUE,_T("BMP"),_T("*.BMP"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("λͼ�ļ�(*.BMP)|*.BMP|"));	
    if(IDOK==dlg.DoModal())	
    filename.Format ("%s",dlg.GetPathName() ); 
   
    m_PhotoImage.LoadFromFile(filename);//����Ƭͼ��
    flag=TRUE;//��Ƭͼ��򿪺󣬽���Ǳ�����Ϊ��

	//��ʾͼ��
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);

}

void CPhotoProcessView::OnLinearTransform()
{
    // �˺���ʵ�ֻҶ�������ǿ����
    
    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }
    
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);//���ƴ���ǰ��ͼ��

    CGrayTransformDib GrayTransform(&m_PhotoImage);//����һ��CGrayTransformDib����
    BYTE min,max;  //���ñ任��ĻҶ�����
    min=100;
    max=200;
	 
	GrayTransform.Linear_Transform( min,max);  //���ûҶ����Ա任��ǿ����

	CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}

void CPhotoProcessView::OnSegLineTransform()
{
    // �˺���ʵ�ֶַ�������ǿ����

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }
    
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);//���ƴ���ǰ��ͼ��

    CGrayTransformDib GrayTransform(&m_PhotoImage); //����һ��CGrayTransformDib����
    BYTE gSrc1,gSrc2, gDst1, gDst2;  //���÷ֶε�
    gSrc1=100;
    gSrc2=150;
    gDst1=50;
    gDst2=200;
	GrayTransform.Seg_Linear_Transform( gSrc1,  gSrc2, gDst1, gDst2);//���÷ֶ����Ա任��ǿ����

	CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}

void CPhotoProcessView::OnLogTransform()
{
    // �˺���ʵ�ֶ������������Ա任��ǿ����

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }
    
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);//���ƴ���ǰ��ͼ��

    CGrayTransformDib GrayTransform(&m_PhotoImage);//����һ��CGrayTransformDib����

    double a,b,c;  //���������任��������
    a=50.0;
    b=0.8;
    c=1.05;       
	GrayTransform.Log_Transform( a, b, c);  //���ö������������Ա任��ǿ����

	CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}

void CPhotoProcessView::OnHistogramEqual()
{
   
    // �˺���ʵ��ֱ��ͼ���⻯��ǿ����

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }
    
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);//���ƴ���ǰ��ͼ��

    CHistogramDib Histgram(&m_PhotoImage);//����һ��CHistogramDib����
         
    Histgram.Histogram_Equalization( );   //����ֱ��ͼ���⻯������

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}

void CPhotoProcessView::OnHistogramMatch()
{
    // �˺���ʵ��ֱ��ͼ�涨����ǿ����

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }
    
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);//���ƴ���ǰ��ͼ��

    CHistogramDib Histgram(&m_PhotoImage);//����һ��CHistogramDib����
         
    int nu[64];  //�涨ֱ��ͼӳ���ϵ,����涨ֱ��ͼ�ĻҶȼ�Ϊ64
    float pu[64]; //�涨�Ҷȷֲ�����
    float a=1.0f/(32.0f*63.0f); 
    for(int i=0;i<64;i++)
    {
        nu[i]=i*4;
        pu[i]=a*i;
    }      
     
	Histgram.Histogram_Match( 64,nu,pu); //����ֱ��ͼ�涨������

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ
}


void CPhotoProcessView::OnAverageSmooth()
{
    // �˺���ʵ�������ֵƽ������

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }
    
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);//���ƴ���ǰ��ͼ��

    CSmoothProcessDib Smooth(&m_PhotoImage);//����һ��CSmoothProcessDib����         
    Smooth.Average_Smooth( );  //��������ƽ��ƽ������

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}

void CPhotoProcessView::OnValueAvrgSmooth()
{
    // �˺���ʵ�������Ȩ��ֵƽ������

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }
    
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);//���ƴ���ǰ��ͼ��

    CSmoothProcessDib Smooth(&m_PhotoImage);//����һ��CSmoothProcessDib����         
    
    int Structure[3][3]={1,2,1,2,4,2,1,2,1};//�����Ȩģ��
     
	Smooth.Value_Average_Smooth(Structure); //���������Ȩƽ��ƽ������

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}

void CPhotoProcessView::OnSelectSmooth()
{
    // �˺���ʵ��ѡ��ʽ��Ĥƽ������

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }
    
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);//���ƴ���ǰ��ͼ��

    CSmoothProcessDib Smooth(&m_PhotoImage);//����һ��CSmoothProcessDib����         
    Smooth.Select_Smooth( );  //����ѡ��ʽ��Ĥƽ������

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}

void CPhotoProcessView::OnMiddleSmooth()
{
     // �˺���ʵ����ֵ�˲�ƽ������

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }
    
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);//���ƴ���ǰ��ͼ��

    CSmoothProcessDib Smooth(&m_PhotoImage);//����һ��CSmoothProcessDib����         
    Smooth.Middle_Smooth( ); //������ֵ�˲�ƽ������

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}



void CPhotoProcessView::OnGateGrad( )
{
    // �˺���ʵ�������ݶ��񻯴���

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }

	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);//���ƴ���ǰ��ͼ��

    CSharpenProcessDib Sharpen(&m_PhotoImage);//����һ��CSharpenProcessDib����
         
    BYTE t=30;
	Sharpen.GateGrad(t);   //���������ݶ��񻯴�����

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��     
}

void CPhotoProcessView::OnLaplacian( )
{
    // �˺���ʵ��������˹�񻯴���

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }

	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);    //���ƴ���ǰ��ͼ��

    CSharpenProcessDib Sharpen(&m_PhotoImage);//����һ��CSharpenProcessDib����         
    
	Sharpen.Laplacian( );   //����������˹�񻯴�����

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
    
}

void CPhotoProcessView::OnPrefectLowFilter()
{
    // �˺���ʵ�������ͨ�˲�����

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }

	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);    //���ƴ���ǰ��ͼ��

    CFrequencyFilterDib FrequencyFilter(&m_PhotoImage);// ����һ��CFrequencyFilterDib����
    //���ý�ֹƵ���������
    int u=100;
    int v=100;     
	FrequencyFilter.Perfect_Low_Filter( u,v); //���������ͨ�˲�����

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}

void CPhotoProcessView::OnPrefectHighFilter()
{
    // �˺���ʵ�������ͨ�˲�����

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }

	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);    //���ƴ���ǰ��ͼ��

    CFrequencyFilterDib FrequencyFilter(&m_PhotoImage);// ����һ��CFrequencyFilterDib����
    //���ý�ֹƵ���������
    int u=100;
    int v=100;     
	FrequencyFilter.Perfect_High_Filter( u,v); //���������ͨ�˲�����

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}

void CPhotoProcessView::OnPseudoColorEnhance()
{
    // �˺���ʵ��������˹�񻯴���

    //���֮ǰû����ͼ�񣬽���������ʾ
    if(flag==FALSE)
    {
        AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
        return;
    }

	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_PhotoImage.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_PhotoImage.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_PhotoImage.Draw(pDC,point,size);    //���ƴ���ǰ��ͼ��

    CColorEnhanceDib ColorEnhance(&m_PhotoImage);// ����һ��CColorEnhanceDib����

    BYTE *bpColorsTable;
    // α��ɫ�����
    BYTE ColorsTable[256*4]={      
      0,  0,  0,0  ,     0,  0,  7,0  ,     0,  0, 15,0  ,     0,  0, 23,0  , //4
      0,  0, 31,0  ,     0,  0, 39,0  ,     0,  0, 47,0  ,     0,  0, 55,0  , //8
      0,  0, 63,0  ,     0,  0, 71,0  ,     0,  0, 79,0  ,     0,  0, 87,0  , //12
      0,  0, 85,0  ,     0,  0,103,0  ,     0,  0,111,0  ,     0,  0,119,0  , //16
      0,  0,127,0  ,     0,  0,135,0  ,     0,  0,143,0  ,     0,  0,151,0  , //20
      0,  0,159,0  ,     0,  0,167,0  ,     0,  0,175,0  ,     0,  0,183,0  , //24
      0,  0,191,0  ,     0,  0,199,0  ,     0,  0,207,0  ,     0,  0,215,0  , //28
      0,  0,223,0  ,     0,  0,231,0  ,     0,  0,239,0  ,     0,  0,247,0  , //32
      0,  0,255,0  ,     0,  8,255,0  ,     0, 16,255,0  ,     0, 24,255,0  , //36
      0, 32,255,0  ,     0, 40,255,0  ,     0, 48,255,0  ,     0, 56,255,0  , //40
      0, 64,255,0  ,     0, 72,255,0  ,     0, 80,255,0  ,     0, 88,255,0  , //44
      0, 96,255,0  ,     0,104,255,0  ,     0,112,255,0  ,     0,120,255,0  , //48
      0,128,255,0  ,     0,136,255,0  ,     0,144,255,0  ,     0,152,255,0  , //52
      0,160,255,0  ,     0,168,255,0  ,     0,176,255,0  ,     0,184,255,0  , //56
      0,192,255,0  ,     0,200,255,0  ,     0,208,255,0  ,     0,216,255,0  , //60
      0,224,255,0  ,     6,232,255,0  ,     0,240,255,0  ,     0,248,255,0  , //64
      0,255,255,0  ,     0,255,247,0  ,     0,255,239,0  ,     0,255,231,0  , //68
      0,255,223,0  ,     0,255,215,0  ,     0,255,207,0  ,     0,255,199,0  , //72
      0,255,191,0  ,     0,255,183,0  ,     0,255,175,0  ,     0,255,167,0  , //76
      0,255,159,0  ,     0,255,151,0  ,     0,255,143,0  ,     0,255,135,0  , //80
      0,255,127,0  ,     0,255,119,0  ,     0,255,111,0  ,     0,255,103,0  , //84
      0,255, 95,0  ,     0,255, 87,0  ,     0,255, 79,0  ,     0,255, 71,0  , //88
      0,255, 63,0  ,     0,255, 55,0  ,     0,255, 47,0  ,     0,255, 39,0  , //92
      0,255, 31,0  ,     0,255, 23,0  ,     0,255, 15,0  ,     0,255,  7,0  , //96
      0,255,  0,0  ,     8,255,  0,0  ,    16,255,  0,0  ,    24,255,  0,0  , //100
     32,255,  0,0  ,    40,255,  0,0  ,    48,255,  0,0  ,    56,255,  0,0  , //104
     64,255,  0,0  ,    72,255,  0,0  ,    80,255,  0,0  ,    88,255,  0,0  , //108
     96,255,  0,0  ,   104,255,  0,0  ,   112,255,  0,0  ,   120,255,  0,0  , //112
    128,255,  0,0  ,   136,255,  0,0  ,   144,255,  0,0  ,   152,255,  0,0  , //116
    160,255,  0,0  ,   168,255,  0,0  ,   176,255,  0,0  ,   184,255,  0,0  , //120
    192,255,  0,0  ,   200,255,  0,0  ,   208,255,  0,0  ,   216,255,  0,0  , //124
    224,255,  0,0  ,   232,255,  0,0  ,   240,255,  0,0  ,   248,255,  0,0  , //128
    255,255,  0,0  ,   255,251,  0,0  ,   255,247,  0,0  ,   255,243,  0,0  , //132
    255,239,  0,0  ,   255,235,  0,0  ,   255,231,  0,0  ,   255,227,  0,0  , //136
    255,223,  0,0  ,   255,219,  0,0  ,   255,215,  0,0  ,   255,211,  0,0  , //140
    255,207,  0,0  ,   255,203,  0,0  ,   255,199,  0,0  ,   255,195,  0,0  , //144
    255,191,  0,0  ,   255,187,  0,0  ,   255,183,  0,0  ,   255,179,  0,0  , //148
    255,175,  0,0  ,   255,171,  0,0  ,   255,167,  0,0  ,   255,163,  0,0  , //152
    255,159,  0,0  ,   255,155,  0,0  ,   255,151,  0,0  ,   255,147,  0,0  , //156
    255,143,  0,0  ,   255,139,  0,0  ,   255,135,  0,0  ,   255,131,  0,0  , //160
    255,127,  0,0  ,   255,123,  0,0  ,   255,119,  0,0  ,   255,115,  0,0  , //164
    255,111,  0,0  ,   255,107,  0,0  ,   255,103,  0,0  ,   255, 99,  0,0  , //168
    255, 95,  0,0  ,   255, 91,  0,0  ,   255, 87,  0,0  ,   255, 83,  0,0  , //172
    255, 79,  0,0  ,   255, 75,  0,0  ,   255, 71,  0,0  ,   255, 67,  0,0  , //176
    255, 63,  0,0  ,   255, 59,  0,0  ,   255, 55,  0,0  ,   255, 51,  0,0  , //180
    255, 47,  0,0  ,   255, 43,  0,0  ,   255, 39,  0,0  ,   255, 35,  0,0  , //184
    255, 31,  0,0  ,   255, 27,  0,0  ,   255, 23,  0,0  ,   255, 19,  0,0  , //188
    255, 15,  0,0  ,   255, 11,  0,0  ,   255,  7,  0,0  ,   255,  3,  0,0  , //192
    255,  0,  0,0  ,   255,  4,  4,0  ,   255,  8,  8,0  ,   255, 12, 12,0  , //196
    255, 16, 16,0  ,   255, 20, 20,0  ,   255, 24, 24,0  ,   255, 28, 28,0  , //200
    255, 32, 32,0  ,   255, 36, 36,0  ,   255, 40, 40,0  ,   255, 44, 44,0  , //204
    255, 48, 48,0  ,   255, 52, 52,0  ,   255, 56, 56,0  ,   255, 60, 60,0  , //208
    255, 64, 64,0  ,   255, 68, 68,0  ,   255, 72, 72,0  ,   255, 76, 76,0  , //212
    255, 80, 80,0  ,   255, 84, 84,0  ,   255, 88, 88,0  ,   255, 92, 92,0  , //216
    255, 96, 96,0  ,   255,100,100,0  ,   255,104,104,0  ,   255,108,108,0  , //220
    255,112,112,0  ,   255,116,116,0  ,   255,120,120,0  ,   255,124,124,0  , //224
    255,128,128,0  ,   255,132,132,0  ,   255,136,136,0  ,   255,140,140,0  , //228
    255,144,144,0  ,   255,148,148,0  ,   255,152,152,0  ,   255,156,156,0  , //232
    255,160,160,0  ,   255,164,164,0  ,   255,168,168,0  ,   255,172,172,0  , //236
    255,176,176,0  ,   255,180,180,0  ,   255,184,184,0  ,   255,188,188,0  , //240
    255,192,192,0  ,   255,196,196,0  ,   255,200,200,0  ,   255,204,204,0  , //244
    255,208,208,0  ,   255,212,212,0  ,   255,216,216,0  ,   255,220,220,0  , //248
    255,224,224,0  ,   255,228,228,0  ,   255,232,232,0  ,   255,236,236,0  , //252
    255,240,240,0  ,   255,244,244,0  ,   255,248,248,0  ,   255,252,252,0   //256
    };

    bpColorsTable=ColorsTable ;
	ColorEnhance.Pseudo_Color_Enhance( bpColorsTable );

    CPoint point1;
	point1.x=lWidth+20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y=0;
	m_PhotoImage.Draw(pDC,point1,size);//���ƴ�����ͼ��
}
