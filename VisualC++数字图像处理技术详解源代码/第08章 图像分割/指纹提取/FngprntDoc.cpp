// FngprntDoc.cpp : CFngprntDoc ���ʵ��
//

#include "stdafx.h"
#include "Fngprnt.h"

#include "FngprntDoc.h"
#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFngprntDoc

IMPLEMENT_DYNCREATE(CFngprntDoc, CDocument)

BEGIN_MESSAGE_MAP(CFngprntDoc, CDocument)
END_MESSAGE_MAP()


// CFngprntDoc ����/����

CFngprntDoc::CFngprntDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CFngprntDoc::~CFngprntDoc()
{
}

BOOL CFngprntDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CFngprntDoc ���л�

void CFngprntDoc::Serialize(CArchive& ar)
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


// CFngprntDoc ���

#ifdef _DEBUG
void CFngprntDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFngprntDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFngprntDoc ����


CDib* CFngprntDoc::GetDib(void)
{
    return &dib;
}


BOOL CFngprntDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // TODO:  �ڴ������ר�õĴ�������

    // �������λͼʧ��
    if(!dib.LoadFromFile(lpszPathName))
    {
        // ��ʾ�Ի�����ʾ�û�λͼ����ʧ��
        AfxMessageBox("����λͼ����ʧ�ܣ�");
        return FALSE;
    }

    return TRUE;
}
