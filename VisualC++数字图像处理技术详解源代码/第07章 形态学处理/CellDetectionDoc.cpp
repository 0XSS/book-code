// CellDetectionDoc.cpp : CCellDetectionDoc ���ʵ��
//

#include "stdafx.h"
#include "CellDetection.h"

#include "CellDetectionDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCellDetectionDoc

IMPLEMENT_DYNCREATE(CCellDetectionDoc, CDocument)

BEGIN_MESSAGE_MAP(CCellDetectionDoc, CDocument)
END_MESSAGE_MAP()


// CCellDetectionDoc ����/����

CCellDetectionDoc::CCellDetectionDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CCellDetectionDoc::~CCellDetectionDoc()
{
}

BOOL CCellDetectionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CCellDetectionDoc ���л�

void CCellDetectionDoc::Serialize(CArchive& ar)
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


// CCellDetectionDoc ���

#ifdef _DEBUG
void CCellDetectionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCellDetectionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCellDetectionDoc ����
