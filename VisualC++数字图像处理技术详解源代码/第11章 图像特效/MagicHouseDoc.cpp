// MagicHouseDoc.cpp : CMagicHouseDoc ���ʵ��
//

#include "stdafx.h"
#include "MagicHouse.h"

#include "MagicHouseDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMagicHouseDoc

IMPLEMENT_DYNCREATE(CMagicHouseDoc, CDocument)

BEGIN_MESSAGE_MAP(CMagicHouseDoc, CDocument)
END_MESSAGE_MAP()


// CMagicHouseDoc ����/����

CMagicHouseDoc::CMagicHouseDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CMagicHouseDoc::~CMagicHouseDoc()
{
}

BOOL CMagicHouseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CMagicHouseDoc ���л�

void CMagicHouseDoc::Serialize(CArchive& ar)
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


// CMagicHouseDoc ���

#ifdef _DEBUG
void CMagicHouseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMagicHouseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMagicHouseDoc ����
