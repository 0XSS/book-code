// DlgImageMatchMoment.cpp : implementation file
//

#include "stdafx.h"
#include "imagematching.h"
#include "DlgImageMatchMoment.h"

#include "CDib.h"
#include "GlobalApi.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchMoment dialog


CDlgImageMatchMoment::CDlgImageMatchMoment(CWnd* pParent /*=NULL*/, CImageMatchingDoc* pDoc)
	: CDialog(CDlgImageMatchMoment::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImageMatchMoment)
	m_a1_inimage = 0.0;
	m_a1_template = 0.0;
	m_a2_inimage = 0.0;
	m_a2_template = 0.0;
	m_a3_inimage = 0.0;
	m_a3_template = 0.0;
	m_a4_inimage = 0.0;
	m_a4_template = 0.0;
	m_a5_inimage = 0.0;
	m_a5_template = 0.0;
	m_a6_inimage = 0.0;
	m_a6_template = 0.0;
	m_a7_inimage = 0.0;
	m_a7_template = 0.0;
	m_R = 0.0;
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
//	m_pDibResult = new CDib;
}


void CDlgImageMatchMoment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImageMatchMoment)
	DDX_Text(pDX, IDC_A1_INIIMAGE, m_a1_inimage);
	DDX_Text(pDX, IDC_A1_TEMPLATE, m_a1_template);
	DDX_Text(pDX, IDC_A2_INIIMAGE, m_a2_inimage);
	DDX_Text(pDX, IDC_A2_TEMPLATE, m_a2_template);
	DDX_Text(pDX, IDC_A3_INIIMAGE, m_a3_inimage);
	DDX_Text(pDX, IDC_A3_TEMPLATE, m_a3_template);
	DDX_Text(pDX, IDC_A4_INIIMAGE, m_a4_inimage);
	DDX_Text(pDX, IDC_A4_TEMPLATE, m_a4_template);
	DDX_Text(pDX, IDC_A5_INIIMAGE, m_a5_inimage);
	DDX_Text(pDX, IDC_A5_TEMPLATE, m_a5_template);
	DDX_Text(pDX, IDC_A6_INIIMAGE, m_a6_inimage);
	DDX_Text(pDX, IDC_A6_TEMPLATE, m_a6_template);
	DDX_Text(pDX, IDC_A7_INIIMAGE, m_a7_inimage);
	DDX_Text(pDX, IDC_A7_TEMPLATE, m_a7_template);
	DDX_Text(pDX, IDC_SIMULATE, m_R);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImageMatchMoment, CDialog)
	//{{AFX_MSG_MAP(CDlgImageMatchMoment)
	ON_BN_CLICKED(IDC_CALMOMENT, OnCalmoment)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_OPENTEMPLATE, OnOpentemplate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchMoment message handlers

void CDlgImageMatchMoment::CalImageLocation()
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

	/* ------------------------------------------------------
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
	*/
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
	
	// ���ÿؼ�IDC_CALMOMENT��λ�ô�С
	pWnd=GetDlgItem(IDC_CALMOMENT);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
		
	// �����˶Ի���Ĵ�С	
	this->GetWindowPlacement(winPlacement);
	//winPlacement->rcNormalPosition.top = nIniImgtop -50;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 172;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 350;
	this->SetWindowPlacement(winPlacement);

	// �ͷ��ѷ����ڴ�
	delete winPlacement;

	// ���ü���ͼ��ؼ�λ�ñ�־λΪTRUE
	m_bCalImgLoc = TRUE;

}

void CDlgImageMatchMoment::OnCalmoment() 
{
	// TODO: Add your control notification handler code here
	// ���Ĺ����״
	BeginWaitCursor();

		// ������ͼ���ڴ�
/*	if(!m_pDibInit->IsEmpty()){
		// ��������ڴ�ʧ�ܣ����Ƴ�
		if(!CopyDIB(m_pDibInit,m_pDibResult)){
			
			// �ͷ��ѷ����ڴ�
			m_pDibResult->Empty();

			// ����
			return ;
		}	
	}*/

	// ���ü���ͼ��ؼ�λ�ñ�־λΪFALSE����������ͼ��ؼ�λ��
	m_bCalImgLoc = FALSE;

	// ����CalMoment()��������ģ��ƥ��
	if (CalMoment(m_pDibInit, m_pDibModel))
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

void CDlgImageMatchMoment::OnOK() 
{
	// TODO: Add extra validation here
	if(!m_pDibModel->IsEmpty()){
		m_pDibModel->Empty();
		m_pDibModel = NULL;
	}
/*	if(!m_pDibResult->IsEmpty()){
		m_pDibResult->Empty();
		m_pDibResult = NULL;
	}*/	

	CDialog::OnOK();
}

void CDlgImageMatchMoment::OnCancel() 
{
    if(!m_pDibModel->IsEmpty()){
		m_pDibModel->Empty();
		m_pDibModel = NULL;
	}
/*	if(!m_pDibResult->IsEmpty()){
		m_pDibResult->Empty();
		m_pDibResult = NULL;
	}*/		
	CDialog::OnCancel();
}

void CDlgImageMatchMoment::OnPaint() 
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
/*	if(!m_pDibResult->IsEmpty()){
		sizeDisplay.cx=m_pDibResult->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibResult->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectResltImage.left;
		pointDisplay.y = m_rectResltImage.top;
		m_pDibResult->Draw(&dc,pointDisplay,sizeDisplay);		
	}*/
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgImageMatchMoment::OnOpentemplate() 
{
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


BOOL CDlgImageMatchMoment::CalMoment(CDib *pDibSrc, CDib *pDibTemplate)
{
	// ԭͼ��ģ������x��y����ֵ
	int nBarycenterX,nBarycenterY;
	int nTBarycenterX, nTBarycenterY;
	
	// ѭ������
	int i,j;

	// ��ʱ����
	double temp;

	//ԭͼ��ģ����߸�����أ����ף��������ľ�
	double Sa[8], Ta[8];
	double Su00, Su02, Su20, Su11, Su30, Su12, Su21, Su03;
	double Tu00, Tu02, Tu20, Tu11, Tu30, Tu12, Tu21, Tu03;

	//ԭͼ��ģ������ƶ�
	double dbR;
	//�м���
	double dSigmaST=0;
	double dSigmaS=0;
	double dSigmaT=0;

	//��������x,y����
	CalBarycenter(pDibSrc, &nBarycenterX, &nBarycenterY);
	CalBarycenter(pDibTemplate, &nTBarycenterX, &nTBarycenterY);

	//������ף��������ľ�
/*	Su02 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 0, 2);
	Su20 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 2, 0);
	Su11 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 1, 1);
	Su30 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 3, 0);
	Su12 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 1, 2);
	Su21 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 2, 1);
	Su03 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 0, 3);

	Tu02 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 0, 2);
	Tu20 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 2, 0);
	Tu11 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 1, 1);
	Tu30 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 3, 0);
	Tu12 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 1, 2);
	Tu21 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 2, 1);
	Tu03 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 0, 3);
*/
	Su00 = BarycenterMoment(pDibSrc, 0, 0, 0, 0);
	Su02 = BarycenterMoment(pDibSrc, 0, 0, 0, 2)/pow(Su00,2);
	Su20 = BarycenterMoment(pDibSrc, 0, 0, 2, 0)/pow(Su00,2);
	Su11 = BarycenterMoment(pDibSrc, 0, 0, 1, 1)/pow(Su00,2);
	Su30 = BarycenterMoment(pDibSrc, 0, 0, 3, 0)/pow(Su00,2.5);
	Su12 = BarycenterMoment(pDibSrc, 0, 0, 1, 2)/pow(Su00,2.5);
	Su21 = BarycenterMoment(pDibSrc, 0, 0, 2, 1)/pow(Su00,2.5);
	Su03 = BarycenterMoment(pDibSrc, 0, 0, 0, 3)/pow(Su00,2.5);

	Tu00 = BarycenterMoment(pDibTemplate, 0, 0, 0, 0);
	Tu02 = BarycenterMoment(pDibTemplate, 0, 0, 0, 2)/pow(Tu00,2);
	Tu20 = BarycenterMoment(pDibTemplate, 0, 0, 2, 0)/pow(Tu00,2);
	Tu11 = BarycenterMoment(pDibTemplate, 0, 0, 1, 1)/pow(Tu00,2);
	Tu30 = BarycenterMoment(pDibTemplate, 0, 0, 3, 0)/pow(Tu00,2.5);
	Tu12 = BarycenterMoment(pDibTemplate, 0, 0, 1, 2)/pow(Tu00,2.5);
	Tu21 = BarycenterMoment(pDibTemplate, 0, 0, 2, 1)/pow(Tu00,2.5);
	Tu03 = BarycenterMoment(pDibTemplate, 0, 0, 0, 3)/pow(Tu00,2.5);

	Sa[1] = Su20 + Su02;
	Sa[2] = (Su20 - Su02)*(Su20 - Su02)+4*Su11*Su11;
	Sa[3] = pow((Su30 - 3*Su12), 2) + pow((3*Su21 - Su03), 2);
	Sa[4] = pow((Su30 + Su12), 2) + pow((Su21 + Su03), 2);
	Sa[5] = (Su30 - 3*Su12)*(Su30 + Su12)*(pow((Su30 + Su12), 2)-3*pow((Su21+Su03),2))
		+3*(Su21-Su03)*(Su21+Su03)*(3*pow((Su30+Su12),2)-pow((Su21+Su03),2));
	Sa[6] = (Su20-Su02)*(pow((Su30+Su12),2)-pow((Su21+Su03),2))+
		4*Su11*(Su30+Su12)*(Su21+Su03);
	Sa[7] = (3*Su21-Su03)*(Su30+Su12)*(pow((Su30+Su12),2)-3*pow((Su21+Su03),2))+
		(Su30-3*Su12)*(Su21+Su03)*(3*pow((Su30+Su12),2)-pow((Su21+Su03),2));

	Ta[1] = Tu20 + Tu02;
	Ta[2] = (Tu20 - Tu02)*(Tu20 - Tu02)+4*Tu11*Tu11;
	Ta[3] = pow((Tu30 - 3*Tu12), 2) + pow((3*Tu21 - Tu03), 2);
	Ta[4] = pow((Tu30 + Tu12), 2) + pow((Tu21 + Tu03), 2);
	Ta[5] = (Tu30 - 3*Tu12)*(Tu30 + Tu12)*(pow((Tu30 + Tu12), 2)-3*pow((Tu21+Tu03),2))
		+3*(Tu21-Tu03)*(Tu21+Tu03)*(3*pow((Tu30+Tu12),2)-pow((Tu21+Tu03),2));
	Ta[6] = (Tu20-Tu02)*(pow((Tu30+Tu12),2)-pow((Tu21+Tu03),2))+
		4*Tu11*(Tu30+Tu12)*(Tu21+Tu03);
	Ta[7] = (3*Tu21-Tu03)*(Tu30+Tu12)*(pow((Tu30+Tu12),2)-3*pow((Tu21+Tu03),2))+
		(Tu30-3*Tu12)*(Tu21+Tu03)*(3*pow((Tu30+Tu12),2)-pow((Tu21+Tu03),2));

	for(i=1;i<8;i++)
	{
		temp=Sa[i]*Ta[i];
		dSigmaST+=temp;
		
		dSigmaS+=pow(Sa[i],2);

		dSigmaT+=pow(Ta[i],2);
	}
	dbR=dSigmaST/( sqrt(dSigmaS)*sqrt(dSigmaT));

	m_a1_inimage=Sa[1];
	m_a1_template=Ta[1];
	m_a2_inimage=Sa[2];
	m_a2_template=Ta[2];
	m_a3_inimage=Sa[3];
	m_a3_template=Ta[3];
	m_a4_inimage=Sa[4];
	m_a4_template=Ta[4];
	m_a5_inimage=Sa[5];
	m_a5_template=Ta[5];
	m_a6_inimage=Sa[6];
	m_a6_template=Ta[6];
	m_a7_inimage=Sa[7];
	m_a7_template=Ta[7];
	m_R=dbR;
	UpdateData(FALSE);

	return true;

}

void CDlgImageMatchMoment::CalBarycenter(CDib *pDib, int *nBarycenterX, int *nBarycenterY)
{
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ�Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ�Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ͼ������ֵ
	int  nPixelValue;

	// 0�ξ�m00,x�����һ�ξ�m01��y�����һ�ξ�m10
	double m00, m10, m01;
	
	// ѭ������
	int i,j;

	// ��ʱ����
	double temp;

	// ����ֵΪ��
	m00 = 0;
	m01 = 0;
	m10 = 0;

	// ��0�ξ�m00,x�����һ�ξ�m01��y�����һ�ξ�m10
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			nPixelValue = *lpSrc;

			m00 = m00 + nPixelValue;

			temp = i * nPixelValue;
			m10  = m10 + temp;

			temp = j * nPixelValue;
			m01  = m01 + temp;
		}
	}

	// ����x��y����ֵ
	*nBarycenterX = (int)(m10 / m00 + 0.5);
	*nBarycenterY = (int)(m01 / m00 + 0.5);
}

double CDlgImageMatchMoment::BarycenterMoment(CDib *pDib, int nBarycenterX, int nBarycenterY, int ip, int jq)
{
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ�Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ�Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ͼ��ľ�
	double dbImageMoment;

	// ѭ������
	int i,j;

	// ��ʱ����
	double temp;

	dbImageMoment = 0;

	// ���صļ���
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			temp =pow((double)(i-nBarycenterX),ip)*pow((double)(j-nBarycenterY),jq);
			temp = temp * (*lpSrc);

			dbImageMoment = dbImageMoment + temp;
		}
	}
	
	return dbImageMoment;
}
