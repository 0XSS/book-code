// SegmentDoc.cpp : CSegmentDoc ���ʵ��
//

#include "stdafx.h"
#include "Segment.h"

#include "SegmentDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSegmentDoc

IMPLEMENT_DYNCREATE(CSegmentDoc, CDocument)

BEGIN_MESSAGE_MAP(CSegmentDoc, CDocument)
END_MESSAGE_MAP()


// CSegmentDoc ����/����

CSegmentDoc::CSegmentDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CSegmentDoc::~CSegmentDoc()
{
}

BOOL CSegmentDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSegmentDoc ���л�

void CSegmentDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CSegmentDoc ���

#ifdef _DEBUG
void CSegmentDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSegmentDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSegmentDoc ����

CDib* CSegmentDoc::GetDib(void)
{
    return &dib;
}

BOOL CSegmentDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // TODO:  �ڴ������ר�õĴ�������
    if(!dib.LoadFromFile(lpszPathName))
    {
        AfxMessageBox("����λͼ����ʧ�ܣ�");
        return FALSE;
    }

    return TRUE;
}
