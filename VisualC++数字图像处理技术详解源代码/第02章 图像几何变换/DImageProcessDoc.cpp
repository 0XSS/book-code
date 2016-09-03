// DImageProcessDoc.cpp : implementation of the CDImageProcessDoc class
//

#include "stdafx.h"
#include "DImageProcess.h"
#include "DImageProcessDoc.h"
//#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessDoc

IMPLEMENT_DYNCREATE(CDImageProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CDImageProcessDoc, CDocument)
	//{{AFX_MSG_MAP(CDImageProcessDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessDoc construction/destruction

CDImageProcessDoc::CDImageProcessDoc()
{
	// TODO: add one-time construction code here	

	m_hDIB = NULL;// ��ʼ������
	m_palDIB = NULL;
	m_sizeDoc = CSize(1,1);

}

CDImageProcessDoc::~CDImageProcessDoc()
{

	if (m_hDIB != NULL)// �ж��Ƿ���DIB����
	{		
		::GlobalFree((HGLOBAL) m_hDIB);
	}


	if (m_palDIB != NULL)// �жϵ�ɫ���Ƿ����
	{
		delete m_palDIB;
	}
}

BOOL CDImageProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDImageProcessDoc serialization

void CDImageProcessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessDoc diagnostics

#ifdef _DEBUG
void CDImageProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDImageProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessDoc commands

BOOL CDImageProcessDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	if (!file.Open(lpszPathName, CFile::modeRead | 
		CFile::shareDenyWrite))// ���ļ�
	{					
		return FALSE;// ����FALSE
	}	
	DeleteContents();
	m_hDIB=m_dib.LoadFile(file);
	if(m_hDIB==NULL)// ����ReadDIBFile()��ȡͼ��
	{			
		return FALSE;
	}		
	SetDib();// ��ʼ��DIB
	if (m_hDIB == NULL)	// �ж϶�ȡ�ļ��Ƿ�ɹ�
	{		
		AfxMessageBox(_T("��ȡͼ��ʱ����"));// ��ʾ����				
		return FALSE;// ����FALSE
	}		
	SetPathName(lpszPathName);// �����ļ�����		
	SetModifiedFlag(FALSE);// ��ʼ���ͱ��ΪFALSE		
	return TRUE;// ����TRUE
}

void CDImageProcessDoc::SetDib()
{		
	LPSTR lpdib = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);	

	if (m_dib.GetWidth(lpdib) > INT_MAX ||m_dib.GetHeight(lpdib) > INT_MAX)// �ж�ͼ���Ƿ����
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);				
		::GlobalFree((HGLOBAL) m_hDIB);	// �ͷ�DIB����			
		m_hDIB = NULL;// ����DIBΪ��		
		AfxMessageBox(_T("��ʼ��ʧ��")); 		
		return;
	}	
	m_sizeDoc = CSize((int)m_dib.GetWidth(lpdib), (int)m_dib.GetHeight(lpdib));// �����ĵ���С	
	::GlobalUnlock((HGLOBAL) m_hDIB);	
	m_palDIB = new CPalette;// �����µ�ɫ��		
	if (m_palDIB == NULL)// �ж��Ƿ񴴽��ɹ�
	{		
		::GlobalFree((HGLOBAL) m_hDIB);	// ʧ��		
		m_hDIB = NULL;// ����DIB����Ϊ��
		return;
	}	
	// ����CreateDIBPalette��������ɫ��
	if (m_dib.ConstructPalette(m_hDIB, m_palDIB) == NULL)
	{				
		delete m_palDIB;// ɾ��				
		m_palDIB = NULL;// ����Ϊ��	
		return;// ���ؿ�
	}
}

BOOL CDImageProcessDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	CFile file;
	if(!file.Open(lpszPathName, CFile::modeCreate |// ���ļ�
		CFile::modeReadWrite | CFile::shareExclusive))
	{
		return FALSE;// ����FALSE
	}
	BOOL bSuccess = FALSE;	
	bSuccess = m_dib.SaveFile(m_hDIB, file);	// ����ͼ��	
	file.Close();// �ر��ļ�	
	SetModifiedFlag(FALSE);// �����ͱ��ΪFALSE
	if (!bSuccess)
	{			
		AfxMessageBox(_T("����BMPͼ��ʱ����"));// ��ʾ����
	}	
	return bSuccess;
}

void CDImageProcessDoc::UpdateObject(HGLOBAL hDIB)
{		
	if (m_hDIB != NULL)               // �ж�DIB�Ƿ�Ϊ��
	{		
		::GlobalFree((HGLOBAL) m_hDIB);// �ǿգ������
	}	
	m_hDIB = hDIB;                     // �滻���µ�DIB����	
}
