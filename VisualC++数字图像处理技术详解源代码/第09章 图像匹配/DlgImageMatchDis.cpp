// DlgImageMatchDis.cpp : implementation file
//

#include "stdafx.h"
#include "imagematching.h"
#include "DlgImageMatchDis.h"

#include "CDib.h"
#include "GlobalApi.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchDis dialog


CDlgImageMatchDis::CDlgImageMatchDis(CWnd* pParent /*=NULL*/, CImageMatchingDoc* pDoc)
	: CDialog(CDlgImageMatchDis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImageMatchDis)
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


void CDlgImageMatchDis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImageMatchDis)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImageMatchDis, CDialog)
	//{{AFX_MSG_MAP(CDlgImageMatchDis)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_OPENTEMPLATE, OnOpentemplate)
	ON_BN_CLICKED(IDC_IMAGEMATCH, OnImagematch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchDis message handlers

void CDlgImageMatchDis::CalImageLocation()
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

void CDlgImageMatchDis::OnPaint() 
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

void CDlgImageMatchDis::OnCancel() 
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

void CDlgImageMatchDis::OnOK() 
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

void CDlgImageMatchDis::OnOpentemplate() 
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

void CDlgImageMatchDis::OnImagematch() 
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
	if (ImageMatchDis(m_pDibResult, m_pDibModel))
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

BOOL CDlgImageMatchDis::ImageMatchDis(CDib *pDibSrc, CDib *pDibTemplate)
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

	//���ƥ���λ��
	int nMaxWidth;
	int nMaxHeight;

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

	//����1�ĸ���
	long Na, Nb;
	Na=0;
	Nb=0;

	//����
	double dbDis;
	double dbDist[300][300];
	double dbSigmaGT;

	//���
	double dbPMatch;
	double dbMinPMatch=1;

	//ģ����������
	unsigned char unchTemplatePixelm10=0;
	unsigned char unchTemplatePixelm01=0;
	unsigned char unchTemplatePixeln01=0;
	unsigned char unchTemplatePixeln10=0;
	unsigned char unchTemplatePixelm1n1=0;
	unsigned char unchTemplatePixelm1n0=0;
	unsigned char unchTemplatePixelm0n1=0;
	unsigned char unchTemplatePixelm0n0=0;

	//ģ��������
	for (n = 0;n < nTemplateHeight ;n++)
	{
		for(m = 0;m < nTemplateWidth ;m++)
		{
			// ָ��ģ��ͼ������n�У���m�����ص�ָ��			
			lpTemplateSrc = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;
			unchTemplatePixel = (unsigned char)*lpTemplateSrc;
			
			if(unchTemplatePixel==0)
			{
				dbDis=0;
				Nb++;
			}
			else
			{
				//�ж�m,nλ�á��־���
				if(m==0 && n==0)
				{
					unchTemplatePixelm10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m+1);
					unchTemplatePixeln10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m);
					unchTemplatePixelm1n1=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m+1);
					if(unchTemplatePixelm10==0 || unchTemplatePixeln10==0)
						dbDis=0.3;
					else if(unchTemplatePixelm1n1==0)
						dbDis=0.7;
					else
						dbDis=1;
				}
				else if(m>0 && m<(nTemplateWidth-1) && n==0)
				{
					unchTemplatePixelm10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m+1);
					unchTemplatePixelm01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m-1);
					unchTemplatePixeln10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m);
					unchTemplatePixelm1n1=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m+1);
					unchTemplatePixelm0n1=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m-1);
					if(unchTemplatePixelm10==0 || unchTemplatePixelm01==0 || unchTemplatePixeln10==0)
						dbDis=0.3;
					else if(unchTemplatePixelm1n1==0 || unchTemplatePixelm0n1==0)
						dbDis=0.7;
					else
						dbDis=1;
				}
				else if(m==(nTemplateWidth-1) && n==0)
				{
					unchTemplatePixelm01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m-1);
					unchTemplatePixeln10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m);
					unchTemplatePixelm0n1=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m-1);
					if(unchTemplatePixelm01==0 || unchTemplatePixeln10==0)
						dbDis=0.3;
					else if(unchTemplatePixelm0n1==0)
						dbDis=0.7;
					else
						dbDis=1;
				}
				else if(m==0 && n>0 && n<(nTemplateHeight-1))
				{
					unchTemplatePixelm10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m+1);
					unchTemplatePixeln01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m);
					unchTemplatePixeln10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m);
					unchTemplatePixelm1n1=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m+1);
					unchTemplatePixelm1n0=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m+1);
					if(unchTemplatePixelm10==0 || unchTemplatePixeln01==0 || unchTemplatePixeln10==0)
						dbDis=0.3;
					else if(unchTemplatePixelm1n1==0 || unchTemplatePixelm1n0==0)
						dbDis=0.7;
					else
						dbDis=1;
				}
				else if(m==(nTemplateWidth-1) && n>0 && n<(nTemplateHeight-1))
				{
					unchTemplatePixelm01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m-1);
					unchTemplatePixeln01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m);
					unchTemplatePixeln10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m);
					unchTemplatePixelm0n0=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m-1);
					unchTemplatePixelm0n1=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m-1);
					if(unchTemplatePixelm01==0 || unchTemplatePixeln01==0 || unchTemplatePixeln10==0)
						dbDis=0.3;
					else if(unchTemplatePixelm0n0==0 || unchTemplatePixelm0n1==0)
						dbDis=0.7;
					else
						dbDis=1;
				}
				else if(m==0 && n==(nTemplateHeight-1))
				{
					unchTemplatePixelm10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m+1);
					unchTemplatePixeln01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m);
					unchTemplatePixelm1n0=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m+1);
					if(unchTemplatePixelm10==0 || unchTemplatePixeln01==0)
						dbDis=0.3;
					else if(unchTemplatePixelm1n0==0)
						dbDis=0.7;
					else
						dbDis=1;
				}
				else if(m>0 && m<(nTemplateWidth-1) && n==(nTemplateHeight-1))
				{
					unchTemplatePixelm10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m+1);
					unchTemplatePixelm01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m-1);
					unchTemplatePixeln01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m);
					unchTemplatePixelm1n0=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m+1);
					unchTemplatePixelm0n0=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m-1);
					if(unchTemplatePixelm10==0 || unchTemplatePixelm01==0 || unchTemplatePixeln01==0)
						dbDis=0.3;
					else if(unchTemplatePixelm1n0==0 || unchTemplatePixelm0n0==0)
						dbDis=0.7;
					else
						dbDis=1;
				}
				else if(m==(nTemplateWidth-1) && n==(nTemplateHeight-1))
				{
					unchTemplatePixelm01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m-1);
					unchTemplatePixeln01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m);
					unchTemplatePixelm0n0=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m-1);
					if(unchTemplatePixelm01==0 || unchTemplatePixeln01==0)
						dbDis=0.3;
					else if(unchTemplatePixelm0n0==0)
						dbDis=0.7;
					else
						dbDis=1;
				}
				else
				{
					unchTemplatePixelm10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m+1);
					unchTemplatePixelm01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m-1);
					unchTemplatePixeln01=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m);
					unchTemplatePixeln10=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m);
					unchTemplatePixelm1n1=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m+1);
					unchTemplatePixelm1n0=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m+1);
					unchTemplatePixelm0n1=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n+1) + m-1);
					unchTemplatePixelm0n0=*((LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * (n-1) + m-1);
					if(unchTemplatePixelm10==0 || unchTemplatePixelm01==0 || unchTemplatePixeln01==0 || unchTemplatePixeln10==0)
						dbDis=0.3;
					else if(unchTemplatePixelm1n1==0 || unchTemplatePixelm1n0==0 || unchTemplatePixelm0n1==0 || unchTemplatePixelm0n0==0)
						dbDis=0.7;
					else
						dbDis=1;
				}
			}
			dbDist[m][n]=dbDis;
		}
	}
	//ƥ��
	for (j = 0;j < nImageHeight - nTemplateHeight +1 ;j++)
	{
		for(i = 0;i < nImageWidth - nTemplateWidth + 1;i++)
		{
			dbSigmaGT=0;
			Na=0;
			
			for (n = 0;n < nTemplateHeight ;n++)
			{
				for(m = 0;m < nTemplateWidth ;m++)
				{
					// ָ��Դͼ������j+n�У���i+m�����ص�ָ��			
					lpSrc  = (LPBYTE)pDibSrc->m_lpImage + sizeSaveImage.cx * (j+n) + (i+m);		
					unchPixel = (unsigned char)*lpSrc;

					if(unchPixel==0)
					{
						dbSigmaGT+=dbDist[m][n];
						Na++;
					}
				}
			}
			//����ƥ�����
			if(Na>=Nb)
				dbPMatch=(dbSigmaGT+Na-Nb)/(Na+Nb);
			else
				dbPMatch=(dbSigmaGT+Nb-Na)/(Na+Nb);
			//����Сƥ�����Ƚ�
			if (dbPMatch <  dbMinPMatch)
			{
				 dbMinPMatch = dbPMatch;
				nMaxWidth = i;
				nMaxHeight = j;
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
		}
	}
	
	// ����ͼ��
	memcpy(pDibSrc->m_lpImage, pDibNew->m_lpImage, nImageWidth * nImageHeight);

	// �ͷ��ڴ�
	pDibNew->Empty();
	
	// ����
	return TRUE;
}
