// DlgImageMatchLSC.cpp : implementation file
//

#include "stdafx.h"
#include "imagematching.h"
#include "DlgImageMatchLSC.h"

#include "CDib.h"
#include "GlobalApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchLSC dialog


CDlgImageMatchLSC::CDlgImageMatchLSC(CWnd* pParent /*=NULL*/, CImageMatchingDoc* pDoc)
	: CDialog(CDlgImageMatchLSC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImageMatchLSC)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// ���ü���ͼ��ؼ�λ�ñ�־λΪFALSE
	m_bCalImgLoc = FALSE;

	// ���ó�ʼͼ��
	m_pDibInit = pDoc->m_pDibInit;

	// ����ĵ�ָ��
	m_pDoc = pDoc;

	// ����ģ��ͼ���ڴ�
	m_pDibModel = new CDib;
		
	// ������ͼ��
	m_pDibResult = new CDib;
}


void CDlgImageMatchLSC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImageMatchLSC)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImageMatchLSC, CDialog)
	//{{AFX_MSG_MAP(CDlgImageMatchLSC)
	ON_BN_CLICKED(IDC_OPENTEMPLATE, OnOpentemplate)
	ON_BN_CLICKED(IDC_IMAGEMATCH, OnImagematch)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchLSC message handlers
void CDlgImageMatchLSC::CalImageLocation()
{
	// ��ÿؼ�IDC_INIIMAGE�ľ��������ÿؼ��ĳ�ʼλ����Ϣ
	CWnd* pWnd=GetDlgItem(IDC_INIIMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);
	
	// ͼ����
	int nImageWidth;
	nImageWidth = m_pDibInit->m_lpBMIH->biWidth;

	// ͼ��߶�
	int nImageHeight;
	nImageHeight = m_pDibInit->m_lpBMIH->biHeight;

	// -----------------------------------------------------------------
	// �����ؼ�IDC_INIIMAGE�Ĵ�Сλ�ã���ͬʱ������ʾ��׼ͼ���λ��
	if(nImageHeight > 256){
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;	
		m_rectInitImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectInitImage.top    = winPlacement->rcNormalPosition.top;		
	}
	else{
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 256;
		m_rectInitImage.bottom = winPlacement->rcNormalPosition.top + 128 + nImageHeight/2;
		m_rectInitImage.top    = winPlacement->rcNormalPosition.top + 128 - nImageHeight/2;
	}
	if(nImageWidth > 256){
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectInitImage.right = winPlacement->rcNormalPosition.right;
		m_rectInitImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 256;
		m_rectInitImage.right = winPlacement->rcNormalPosition.left + 128 + nImageWidth/2;
		m_rectInitImage.left  = winPlacement->rcNormalPosition.left + 128 - nImageWidth/2;
	}

	// ����IDC_INIIMAGE�ؼ��Ĵ�Сλ��
	pWnd->SetWindowPlacement(winPlacement);	
			
	// �����ʾģ��ͼ��ؼ����ұ�λ�ã��Ա�ȷ����ʾģ��ͼ��ؼ���λ��
	int nIniImgRight;
	nIniImgRight = winPlacement->rcNormalPosition.right;
	
	int  nIniImgLeft;
	nIniImgLeft   = winPlacement->rcNormalPosition.left;

	// ���IDC_INIIMAGEE�ؼ����±�λ�ã��Ա���������ؼ���λ��
	int nIniImgBottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;

	// ���IDC_INIIMAGE�ؼ����±�λ�ã��Ա���������ؼ���λ��
	int nIniImgtop = winPlacement->rcNormalPosition.top;

	// ------------------------------------------------------
	// ��ÿؼ�IDC_TEMPLATE�ľ��������ó�ʼλ����Ϣ
	pWnd=GetDlgItem(IDC_TEMPLATE);
	pWnd->GetWindowPlacement(winPlacement);
	
	// �����δ��ģ��ͼ�������ý��ͼ���С�ͳ�ʼͼ���С���
	if(!m_pDibModel->IsEmpty()){
		nImageWidth  = m_pDibModel->m_lpBMIH->biWidth;
		nImageHeight = m_pDibModel->m_lpBMIH->biHeight;
	}
	
	// �����ؼ�IDC_TEMPLATE�Ĵ�Сλ�ã���ͬʱ������ʾ���ͼ���λ��

	// �ȵ����ؼ������λ�ã���IDC_INIIMAGE�ؼ����15������
	winPlacement->rcNormalPosition.left = nIniImgRight + 15;

	if(nImageHeight > 256){		
		winPlacement->rcNormalPosition.top = nIniImgtop;
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;
		m_rectModelImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectModelImage.top    = nIniImgtop;		
	}
	else{
		winPlacement->rcNormalPosition.top = nIniImgtop;
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 256;
		m_rectModelImage.bottom = winPlacement->rcNormalPosition.top + 128 + nImageHeight/2;
		m_rectModelImage.top    = winPlacement->rcNormalPosition.top + 128 - nImageHeight/2;
	}
	if(nImageWidth > 256){		
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectModelImage.right = winPlacement->rcNormalPosition.right;
		m_rectModelImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{		
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 256;
		m_rectModelImage.right = winPlacement->rcNormalPosition.left + 128 + nImageWidth/2;
		m_rectModelImage.left  = winPlacement->rcNormalPosition.left + 128 - nImageWidth/2;
	}

	// ����IDC_TEMPLATE�ؼ��Ĵ�Сλ��
	pWnd->SetWindowPlacement(winPlacement);
		
	// ���IDC_TEMPLATE�ؼ����ұ�λ�ã��Ա���������ؼ���λ��
	nIniImgRight  = winPlacement->rcNormalPosition.right;

	// ------------------------------------------------------
	// ��ÿؼ�IDC_RESULTIMAGE�ľ��������ó�ʼλ����Ϣ
	pWnd=GetDlgItem(IDC_RESULTIMAGE);
	pWnd->GetWindowPlacement(winPlacement);
	
	// �����δ���ɽ��ͼ�������ý��ͼ���С�ͳ�ʼͼ���С���
	if(!m_pDibResult->IsEmpty()){
		nImageWidth  = m_pDibResult->m_lpBMIH->biWidth;
		nImageHeight = m_pDibResult->m_lpBMIH->biHeight;
	}
	
	// �����ؼ�IDC_RESULTIMAGE�Ĵ�Сλ�ã���ͬʱ������ʾ���ͼ���λ��

	// �ȵ����ؼ������λ�ã���IDC_TEMPLATE�ؼ����15������
	winPlacement->rcNormalPosition.left = nIniImgRight + 15;

	if(nImageHeight > 256){		
		winPlacement->rcNormalPosition.top = nIniImgtop;
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;	
		m_rectResltImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectResltImage.top    = winPlacement->rcNormalPosition.top;		
	}
	else{
		winPlacement->rcNormalPosition.top = nIniImgtop;
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 256;
		m_rectResltImage.bottom = winPlacement->rcNormalPosition.top + 128 + nImageHeight/2;
		m_rectResltImage.top    = winPlacement->rcNormalPosition.top + 128 - nImageHeight/2;
	}
	if(nImageWidth > 256){
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectResltImage.right = winPlacement->rcNormalPosition.right;
		m_rectResltImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 256;
		m_rectResltImage.right = winPlacement->rcNormalPosition.left + 128 + nImageWidth/2;
		m_rectResltImage.left  = winPlacement->rcNormalPosition.left + 128 - nImageWidth/2;
	}

	// ����IDC_RESULTIMAGE�ؼ��Ĵ�Сλ��
	pWnd->SetWindowPlacement(winPlacement);
	
	// ------------------------------------------------------
	if(nIniImgBottom > winPlacement->rcNormalPosition.bottom)
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	
	nIniImgRight  = winPlacement->rcNormalPosition.right;
	
	// ���ÿؼ�IDOK��λ�ô�С
	pWnd=GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	
	// ���ÿؼ�IDCANCEL��λ�ô�С
	pWnd=GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);

	// ���ÿؼ�IDC_OPENTEMPLATE��λ�ô�С
	pWnd=GetDlgItem(IDC_OPENTEMPLATE);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	
	// ���ÿؼ�IDC_IMAGEMATCH��λ�ô�С
	pWnd=GetDlgItem(IDC_IMAGEMATCH);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
		
	// �����˶Ի���Ĵ�С	
	this->GetWindowPlacement(winPlacement);
	//winPlacement->rcNormalPosition.top = nIniImgtop -50;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 272;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 20;
	this->SetWindowPlacement(winPlacement);

	// �ͷ��ѷ����ڴ�
	delete winPlacement;

	// ���ü���ͼ��ؼ�λ�ñ�־λΪTRUE
	m_bCalImgLoc = TRUE;
}

void CDlgImageMatchLSC::OnCancel() 
{
	// TODO: Add extra cleanup here
	// �ͷ��ѷ����ڴ�
	if(!m_pDibModel->IsEmpty()){
		m_pDibModel->Empty();
		m_pDibModel = NULL;
	}
	if(!m_pDibResult->IsEmpty()){
		m_pDibResult->Empty();
		m_pDibResult = NULL;
	}	
	CDialog::OnCancel();
}

void CDlgImageMatchLSC::OnOK() 
{
	// TODO: Add extra validation here
	// �ͷ��ѷ����ڴ�
	if(!m_pDibModel->IsEmpty()){
		m_pDibModel->Empty();
		m_pDibModel = NULL;
	}
	if(!m_pDibResult->IsEmpty()){
		m_pDibResult->Empty();
		m_pDibResult = NULL;
	}	
	CDialog::OnOK();
}

void CDlgImageMatchLSC::OnOpentemplate() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	if(dlg.DoModal() == IDOK)
	{
 
	 	CFile file;
	 
	 	CString strPathName;
 
		strPathName = dlg.GetPathName();
 
		// ���ļ�
		if( !file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite))
		{
			// ����	
			return ;
		}

		// ����ģ��ͼ��
		if(!m_pDibModel->Read(&file)){
			// �ָ������״
			EndWaitCursor();

			// ����ѷ����ڴ�
			m_pDibModel->Empty();

			// ����
 			return;
		}
	}
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ģ����׼�������Ŀ������ƣ�
	if(m_pDibModel->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// ����ѷ����ڴ�
		m_pDibModel->Empty();

		// ����
		return;
	}

	// ��ʼͼ��ĳ����С
	CSize sizeImage			= m_pDibInit->GetDimensions();
	int nImageWidth			= sizeImage.cx		;
	int nImageHeight			= sizeImage.cy		;
	
	// ģ��ͼ��ĳ����С
	CSize sizeModelImage	= m_pDibModel->GetDimensions()	;
	int nModelWidth		= sizeImage.cx		;
	int nModelHeight	= sizeImage.cy		;

	// �ж�ģ��ߴ�ͳ�ʼͼ��Ĵ�С�����ģ����ڳ�ʼͼ�����Ƴ�
	if(nModelHeight > nImageHeight || nModelWidth > nImageWidth )
	{
		// ��ʾ�û�
		MessageBox("ģ��ߴ����Դͼ��ߴ磡", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// �ͷ��ѷ����ڴ�
		m_pDibModel->Empty();

		// ����
		return;

	}

	// ������µĴ���׼�ļ�����ͼ��λ�����ñ�־λ��ΪFALSE���Ա��ٴε���λ��
	m_bCalImgLoc = FALSE;
	
	// ������ʾ	
	this->Invalidate();	
}

void CDlgImageMatchLSC::OnImagematch() 
{
	// TODO: Add your control notification handler code here
	// ���Ĺ����״
	BeginWaitCursor();

		// ������ͼ���ڴ�
	if(!m_pDibInit->IsEmpty()){
		// ��������ڴ�ʧ�ܣ����Ƴ�
		if(!CopyDIB(m_pDibInit,m_pDibResult)){
			
			// �ͷ��ѷ����ڴ�
			m_pDibResult->Empty();

			// ����
			return ;
		}	
	}

	// ���ü���ͼ��ؼ�λ�ñ�־λΪFALSE����������ͼ��ؼ�λ��
	m_bCalImgLoc = FALSE;

	// ����ImageMatchSSDA()��������ģ��ƥ��
	if (ImageMatchLSC(m_pDibResult, m_pDibModel))
	{
		// ������ʾ
		Invalidate();
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	// �ָ����
	EndWaitCursor();

	// ������ͼ
	Invalidate();
}

BOOL CDlgImageMatchLSC::ImageMatchLSC(CDib *pDibSrc, CDib *pDibTemplate)
{
	// ָ��Դͼ���ָ��
	LPBYTE	lpSrc,lpTemplateSrc;
	
	// ָ�򻺴�ͼ���ָ��
	LPBYTE	lpDst;

	//ѭ������
	long i;
	long j;
	long m;
	long n;
	int k;
	int u;
	int v;

	//�ۼ�������
	long lR = 0;

	//����ۼ���Ӵ���
	long  dbMaxR = 0;

	//���ƥ���λ��
	int nMaxWidth;
	int nMaxHeight;
	
	//�Ƿ���dbr<lTk���֣���br=true.
	bool br=false;

	//����ֵ
	unsigned char unchPixel;
	unsigned char unchTemplatePixel;

	// ���ͼ�����ݴ洢�ĸ߶ȺͿ��
	CSize sizeSaveImage;
	sizeSaveImage = pDibSrc->GetDibSaveDim();

	// ���ģ��ͼ�����ݴ洢�ĸ߶ȺͿ��
	CSize sizeSaveTemplate;
	sizeSaveTemplate = pDibTemplate->GetDibSaveDim();


	// ��ʱ�����ڴ棬�Ա�����ͼ��
	CDib* pDibNew;
	pDibNew = new CDib;

	// ��������ڴ�ʧ�ܣ����Ƴ�
	if(!CopyDIB(pDibSrc,pDibNew)){
		// �ͷ��ѷ����ڴ�
		pDibNew->Empty();

		// ����
		return FALSE;
	}
	

	// ��ʼ���·�����ڴ�
	lpDst = (LPBYTE)pDibNew->m_lpImage; 
	
	// ͼ��ĸ߶�
	int nImageHeight ;
	nImageHeight = pDibSrc->m_lpBMIH->biHeight;

	// ͼ��Ŀ��
	int nImageWidth;
	nImageWidth = pDibSrc->m_lpBMIH->biWidth;

	// ģ��ͼ��ĸ߶�
	int nTemplateHeight;
	nTemplateHeight = pDibTemplate->m_lpBMIH->biHeight;

	// ģ��ͼ��Ŀ��
	int nTemplateWidth;
	nTemplateWidth = pDibTemplate->m_lpBMIH->biWidth;
	//////////////////////////////////////////////////////////////
	int mt[16][2];//��ʱ�洢ģ����������
	double E2; //�������
	double dbMinE2=10000;
	int dim;//ģ������������
	//������Ϊ255������
	k=0;
	for (n = 0;n < nTemplateHeight ;n++)
	{
		for(m = 0;m < nTemplateWidth ;m++)
		{
			// ָ��ģ��ͼ������j�У���i�����ص�ָ��			
			lpTemplateSrc = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;
			unchTemplatePixel = (unsigned char)*lpTemplateSrc;
			if (unchTemplatePixel==255)
			{
				if(k>8)
				{
					// ��ʾ�û�
					MessageBox("ƥ������8����", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
					return false;
				}
				mt[k][0]=m;
				mt[k][1]=n;
				k++;
				}
		}
	}
	dim=k;
	//ά�������ľ�����Ҫ��̬����
	int *const MatrixY = new int[2*dim];
	int *const MatrixX = new int[2*dim*4];
	int *const MatrixXt = new int[4*2*dim];
	double *const  MatrixXtX = new double[4*4]; //����
	double *const _MatrixXtX = new double[4*4]; //�����
	double *const MatrixXtXXt = new double[4*2*dim];
	double *const Matrixb = new double[4];
	double *const MatrixXb = new double[2*dim];
	double *const MatrixY_Xb = new double[2*dim];
	double value;//��ʱ����
    //���ɾ���X,��Ӧģ��������
	for(u=0;u<(dim*2);u++)
	{
		for(v=0;v<4;v++)
		{
			if(v==0)
			{
				if(u%2==0)//������
				{
					MatrixX[u*4+v]=mt[u/2][0];
				}
				if(u%2==1)//ż����
				{
					MatrixX[u*4+v]=mt[u/2][1];
				}
			}
			if(v==1)
			{
				if(u%2==0)//������
				{
					MatrixX[u*4+v]=mt[u/2][1];
				}
				if(u%2==1)//ż����
				{
					MatrixX[u*4+v]=-mt[u/2][0];
				}
			}
			if(v==2)
			{
				if(u%2==0)//������
				{
					MatrixX[u*4+v]=1;
				}
				if(u%2==1)//ż����
				{
					MatrixX[u*4+v]=0;
				}
			}
			if(v==3)
			{
				if(u%2==0)//������
				{
					MatrixX[u*4+v]=0;
				}
				if(u%2==1)//ż����
				{
					MatrixX[u*4+v]=1;
				}
			}
		}
	}
	//MatrixXt
	for(u=0;u<4;u++)
	{
		for(v=0;v<(dim*2);v++)
		{
			MatrixXt[u*dim*2+v]=MatrixX[v*4+u];
		}
	}
	//MatrixXtX
	for(u=0;u<4;u++)
	{
		for(v=0;v<4;v++)
		{
			value=0;
			for(k=0;k<(dim*2);k++)
			{
				value+=MatrixXt[u*dim*2+k]*MatrixX[k*4+v];
			}
			MatrixXtX[u*4+v]=value;
		}
	}
    //����
	if(!ContraryMatrix(MatrixXtX, _MatrixXtX, 4))
	{
		return false;
	}
	//MatrixXtXXt
	for(u=0;u<4;u++)
	{
		for(v=0;v<(dim*2);v++)
		{
			value=0;
			for(k=0;k<4;k++)
			{
				value+=_MatrixXtX[u*4+k]*MatrixXt[k*2*dim+v];
			}
			MatrixXtXXt[u*dim*2+v]=value;
		}
	}
	//����ԭͼ�ж�Ӧλ������Ϊ255������
	int num; //��ͼ�����������
	for (j = 0;j < nImageHeight - nTemplateHeight +1 ;j++)
	{
		for(i = 0;i < nImageWidth - nTemplateWidth + 1;i++)
		{
			num=0;
			for (n = 0;n < nTemplateHeight ;n++)
			{
				for(m = 0;m < nTemplateWidth ;m++)
				{
					// ָ��Դͼ������j+n�У���i+m�����ص�ָ��			
					lpSrc  = (LPBYTE)pDibSrc->m_lpImage + sizeSaveImage.cx * (j+n) + (i+m);
					unchPixel = (unsigned char)*lpSrc;
					
					if(unchPixel==255)
					{
						//���ԭͼ�е������������ģ�岻һ�£�������
						if(num==dim*2)
						{
							num++;
							break;
						}
						MatrixY[num]=m;
						num++;
						MatrixY[num]=n;
						num++;						
					}
				}
				if(num>dim*2) break;
			}
			//���������һ�½���ƥ��
			if(num==dim*2)
			{
				//Matrixb
				for(u=0;u<4;u++)
				{
					value=0;
					for(k=0;k<(dim*2);k++)
					{
						value+=MatrixXtXXt[u*dim*2+k]*MatrixY[k];
					}
					Matrixb[u]=value;
				}
				//MatrixXb
				for(u=0;u<(dim*2);u++)
				{
					value=0;
					for(k=0;k<4;k++)
					{
						value+=MatrixX[u*4+k]*Matrixb[k];
					}
					MatrixXb[u]=value;
				}
				//MatrixY_Xb				
				for(u=0;u<(dim*2);u++)
				{
					MatrixY_Xb[u]=MatrixY[u]-MatrixXb[u];
				}
				//E2				
				value=0;
				for(u=0;u<(dim*2);u++)
				{
					value+=MatrixY_Xb[u]*MatrixY_Xb[u];
				}
				E2=value;
				if (E2 <= dbMinE2)
				{
					dbMinE2 = E2;
					nMaxWidth = i+(int)Matrixb[2];
					nMaxHeight = j+(int)Matrixb[3];
				}


			}
		}
	}
	// ��Ŀ��ͼ������ؽ��и�ֵ
	for(i=0; i<nImageHeight; i++)
		for( j=0; j<nImageWidth; j++){
			lpDst[i*sizeSaveImage.cx +j] /=2;			
		}

	//����������Գ������򲿷ָ��Ƶ�Ŀ��ͼ��
	for (n = 0;n < nTemplateHeight ;n++)
	{
		for(m = 0;m < nTemplateWidth ;m++)
		{
			lpTemplateSrc = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;
			lpDst = (LPBYTE)pDibNew->m_lpImage + sizeSaveImage.cx * (n+nMaxHeight) + (m+nMaxWidth);
			*lpDst = *lpTemplateSrc;
			if(*lpTemplateSrc==0)
			{
				*lpDst=6;
			}
		}
	}
	
	// ����ͼ��
	memcpy(pDibSrc->m_lpImage, pDibNew->m_lpImage, nImageWidth * nImageHeight);

	// �ͷ��ڴ�
	pDibNew->Empty();
	
	delete[] MatrixY;
	delete[] MatrixX;
	delete[] MatrixXt;
	delete[]  MatrixXtX; //����
	delete[] _MatrixXtX; //�����
	delete[] MatrixXtXXt;
	delete[] Matrixb;
	delete[] MatrixXb;
	delete[] MatrixY_Xb;
	
	// ����
	return TRUE;
}

BOOL CDlgImageMatchLSC::ContraryMatrix(double *const pMatrix, double *const _pMatrix, const int dim)
{
	double *tMatrix = new double[2*dim*dim];
	int i;
    for (i=0; i<dim; i++){
        for (int j=0; j<dim; j++)
            tMatrix[i*dim*2+j] = pMatrix[i*dim+j];        
    }
    for (i=0; i<dim; i++){
        for (int j=dim; j<dim*2; j++)
            tMatrix[i*dim*2+j] = 0.0;
        tMatrix[i*dim*2+dim+i]  = 1.0;        
    }
    //Initialization over!
    for (i=0; i<dim; i++)//Process Cols
    {
        double base = tMatrix[i*dim*2+i];
        if (fabs(base) < 1E-300){
            // ��ʾ�û�
			MessageBox("�����������б����", "�޷����" , MB_ICONINFORMATION | MB_OK);
			return false;
        }
        for (int j=0; j<dim; j++)//row
        {
            if (j == i) continue;
            double times = tMatrix[j*dim*2+i]/base;
            for (int k=0; k<dim*2; k++)//col
            {        
                tMatrix[j*dim*2+k] = tMatrix[j*dim*2+k] - times*tMatrix[i*dim*2+k];
            }
        }        
        for (int k=0; k<dim*2; k++){
            tMatrix[i*dim*2+k] /= base;
        }
    }
    for (i=0; i<dim; i++)
    {
        for (int j=0; j<dim; j++)
            _pMatrix[i*dim+j] = tMatrix[i*dim*2+j+dim];        
    }    
    delete[] tMatrix;
	return true;
}

void CDlgImageMatchLSC::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// �����û�м���ͼ���λ�ã�����м���
	if(!m_bCalImgLoc){
		CalImageLocation();
	}

	// ��ʾ��С
	CSize sizeDisplay;

	// ��ʾλ��
	CPoint pointDisplay;
	
	// ��ʾ��ʼͼ��
	if(!m_pDibInit->IsEmpty()){
		sizeDisplay.cx=m_pDibInit->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibInit->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectInitImage.left;
		pointDisplay.y = m_rectInitImage.top;
		m_pDibInit->Draw(&dc,pointDisplay,sizeDisplay);		
	}
	
	// ��ʾģ��ͼ��
	if(!m_pDibModel->IsEmpty()){
		sizeDisplay.cx=m_pDibModel->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibModel->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectModelImage.left;
		pointDisplay.y = m_rectModelImage.top;
		m_pDibModel->Draw(&dc,pointDisplay,sizeDisplay);		
	}

	// ��ʾ���ͼ��
	if(!m_pDibResult->IsEmpty()){
		sizeDisplay.cx=m_pDibResult->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibResult->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectResltImage.left;
		pointDisplay.y = m_rectResltImage.top;
		m_pDibResult->Draw(&dc,pointDisplay,sizeDisplay);		
	}
	// Do not call CDialog::OnPaint() for painting messages
}
