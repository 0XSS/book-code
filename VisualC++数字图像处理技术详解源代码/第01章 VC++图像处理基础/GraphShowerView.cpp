// GraphShowerView.cpp : CGraphShowerView ���ʵ��
//

#include "stdafx.h"
#include "GraphShower.h"

#include "GraphShowerDoc.h"
#include "GraphShowerView.h"

#include "GraphShowerDoc.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ͼ����ʾ����ö��
enum ShowType
{
	ST_NORMAL		= 0,	// ԭʼ��С
	ST_FIXWIDTH		= 1,	// �ʺϿ��
	ST_FIXHEIGHT	= 2,	// �ʺϸ߶�
	ST_FIXSCREEN	= 3		// �ʺ���Ļ
};

// CGraphShowerView

IMPLEMENT_DYNCREATE(CGraphShowerView, CView)

BEGIN_MESSAGE_MAP(CGraphShowerView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MODE_NORMAL, &CGraphShowerView::OnModeNormal)
	ON_COMMAND(ID_MODE_WIDTH, &CGraphShowerView::OnModeWidth)
	ON_COMMAND(ID_MODE_HEIGHT, &CGraphShowerView::OnModeHeight)
	ON_COMMAND(ID_MODE_SCREEN, &CGraphShowerView::OnModeScreen)
	ON_UPDATE_COMMAND_UI(ID_MODE_NORMAL, &CGraphShowerView::OnUpdateModeNormal)
	ON_UPDATE_COMMAND_UI(ID_MODE_WIDTH, &CGraphShowerView::OnUpdateModeWidth)
	ON_UPDATE_COMMAND_UI(ID_MODE_HEIGHT, &CGraphShowerView::OnUpdateModeHeight)
	ON_UPDATE_COMMAND_UI(ID_MODE_SCREEN, &CGraphShowerView::OnUpdateModeScreen)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_IMAGE_PREV, &CGraphShowerView::OnImagePrev)
	ON_COMMAND(ID_IMAGE_NEXT, &CGraphShowerView::OnImageNext)
	ON_COMMAND(ID_IMAGE_FIRST, &CGraphShowerView::OnImageFirst)
	ON_COMMAND(ID_IMAGE_LAST, &CGraphShowerView::OnImageLast)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_PREV, &CGraphShowerView::OnUpdateImagePrev)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_NEXT, &CGraphShowerView::OnUpdateImageNext)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_FIRST, &CGraphShowerView::OnUpdateImageFirst)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_LAST, &CGraphShowerView::OnUpdateImageLast)
	ON_COMMAND(ID_FILE_OPEN, &CGraphShowerView::OnFileOpen)
END_MESSAGE_MAP()

// CGraphShowerView ����/����

CGraphShowerView::CGraphShowerView()
{
	m_nShowType = ST_NORMAL;
	m_bHeightOut = false;
	m_bWidhtOut = false;
	m_nXX = 0;
	m_nYY = 0;
	m_nXXMax = 0;
	m_nYYMax = 0;

	m_pBitmap = NULL;
	m_nPos = -1;
}

CGraphShowerView::~CGraphShowerView()
{
	::delete m_pBitmap;
}

/******************************************************************************
*	����:		��ͼ�ػ溯��
******************************************************************************/
void CGraphShowerView::OnDraw(CDC* pDC)
{
	CGraphShowerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(rect);

	CDC memDC;
	CBitmap MemBitmap;

	// �豸�������ʼ��
	memDC.CreateCompatibleDC(NULL);

	// ��������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	// ѡȡ�հ�λͼ
	memDC.SelectObject(MemBitmap);
	memDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));

	if (m_nPos >= 0)
		ShowPicture(&memDC, *m_pBitmap, m_nShowType);


	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	MemBitmap.DeleteObject();
	memDC.DeleteDC();
}

/******************************************************************************
*	����:		��ָ��·�����ļ�
******************************************************************************/
void CGraphShowerView::OpenFile(const CString& strPath)
{
	m_FilesNameAry.RemoveAll();

	int nIndex = strPath.ReverseFind('\\');
	m_strPath = strPath.Left(nIndex);
	CString strFileName = strPath.Right(strPath.GetLength() - nIndex - 1);

	CMainFrame::GetAllFileNames(m_strPath, m_FilesNameAry);
	m_nPicNum = (int)m_FilesNameAry.GetCount();

	m_nPos = 0;

	while( m_nPos < m_nPicNum && m_FilesNameAry[m_nPos] != strFileName)
		m_nPos++;

	PreDrawImage();
	Invalidate();
}

void CGraphShowerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

// CGraphShowerView ��ӡ

BOOL CGraphShowerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CGraphShowerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CGraphShowerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CGraphShowerView ���

#ifdef _DEBUG
void CGraphShowerView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphShowerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphShowerDoc* CGraphShowerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphShowerDoc)));
	return (CGraphShowerDoc*)m_pDocument;
}
#endif //_DEBUG

/******************************************************************************
*	����:		"��"��������Ӧ����
******************************************************************************/
void CGraphShowerView::OnFileOpen()
{
	CFileDialog dlg(true, NULL, L"", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		L"����ͼ���ļ�(*.bmp; *.jpg)|*.bmp; *.jpg|�����ļ�(*.*)|*.*||");

	if (dlg.DoModal() == IDOK)
	{
		OpenFile(dlg.GetPathName());
		FixOffset();
		RefreshIndicator();
		Invalidate();
	}
}

/******************************************************************************
*	����:		��ֹ�ػ汳��
******************************************************************************/
BOOL CGraphShowerView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

/******************************************************************************
*	����:			��ָ��ģʽ��ʾͼ��
*	����:		
*		pDC			Ŀ���豸������ָ��
*		image		��ʾͼ��
*		nShowType	��ʾģʽ
******************************************************************************/
void CGraphShowerView::ShowPicture(CDC* pDC, Image& image, int nShowType)
{
	Graphics graph(pDC->GetSafeHdc());

	CRect winRect;
	GetClientRect(winRect);

	REAL x = 0.0;
	REAL y = 0.0;
	CSize showPicSize = GetShowPicSize(image, m_nShowType);
	REAL width  = (REAL)showPicSize.cx;
	REAL height = (REAL)showPicSize.cy;

	// ʹͼ�����
	if ((UINT)winRect.Width() > width)
		x = (winRect.Width() - width) / 2.0f;

	if ( (UINT)winRect.Height() > height )
		y = ((UINT)winRect.Height() - height) / 2.0f;

	if (m_bWidhtOut)
		x += m_nXX;

	if (m_bHeightOut)
		y += m_nYY;

	m_nShowPicHeight = (int)height;
	m_nShowPicWidth = (int)width;

	graph.DrawImage(&image, x, y, width, height);
}

/******************************************************************************
*	����:		������ʾģʽ
*	����:		
*		nType	�µ���ʾģʽ
******************************************************************************/
void CGraphShowerView::SetShowType(int nType)
{
	m_nShowType = nType;
}

/******************************************************************************
*	����:			���ָ��ģʽ��ͼ��Ĵ�С
*	����:		
*		image		ͼ�����
*		nShowType	��ʾģʽ
*	����ֵ:			��ָ��ģʽ��ͼ�����ʾ��С
******************************************************************************/
CSize CGraphShowerView::GetShowPicSize(Image& image, int nShowType)
{
	REAL width  = (REAL)image.GetWidth();
	REAL height = (REAL)image.GetHeight();
	REAL c		= 0.0f;

	CRect winRect;
	GetClientRect(winRect);

	switch (nShowType)
	{
	case ST_FIXWIDTH:	// �ʺϿ��ģʽ
		width = (REAL)winRect.Width();
		c = width / image.GetWidth();
		height = c * image.GetHeight();
		break;

	case ST_FIXHEIGHT:	// �ʺϸ߶�ģʽ
		height = (REAL)winRect.Height();
		c = height / image.GetHeight();
		width = c * image.GetWidth();

		break;

	case ST_FIXSCREEN:	// �ʺϴ�Сģʽ

		// ���ʺϿ��ģʽ����
		width = (REAL)winRect.Width();
		c = width / image.GetWidth();

		// ���û�г���
		if (c * image.GetHeight() < winRect.Height())
			height = c * image.GetHeight();
		// ����������ʺϸ߶�ģʽ
		else
		{
			height = (REAL)winRect.Height();
			c = height / image.GetHeight();
			width = c * image.GetWidth();
		}

		break;
	}

	return CSize((int)width, (int)height);
}


/******************************************************************************
*	����:		��갴�µ���Ӧ����
******************************************************************************/
void CGraphShowerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_psMove = point;

	CView::OnLButtonDown(nFlags, point);
}

/******************************************************************************
*	����:		����ͼ���ƫ���������Ϣ
*	��ע:		����ʾģʽΪ����
******************************************************************************/
void CGraphShowerView::FixOffset()
{
	if (m_nPos < 0 || m_nPos >= m_nPicNum)
		return;

	Image image(GetFilePath(m_nPos));

	if ( image.GetLastStatus() != Ok )
		return;

	m_bHeightOut = false;
	m_bWidhtOut = false;

	CSize imgSize = GetShowPicSize(image, m_nShowType);
	CRect rect;

	GetClientRect(rect);

	switch (m_nShowType)
	{
	case ST_NORMAL:
		if (imgSize.cx > rect.Width())
		{
			m_nXXMax = imgSize.cx - rect.Width();
			m_bWidhtOut = true;
		}

		if (imgSize.cy > rect.Height())
		{
			m_nYYMax = imgSize.cy - rect.Height();
			m_bHeightOut = true;
		}
		break;

	case ST_FIXWIDTH:
		if (imgSize.cy > rect.Height())
		{
			m_nYYMax = imgSize.cy - rect.Height();
			m_bHeightOut = true;
		}
		break;

	case ST_FIXHEIGHT:
		if (imgSize.cx > rect.Width())
		{
			m_nXXMax = imgSize.cx - rect.Width();
			m_bWidhtOut = true;
		}
		break;

	case ST_FIXSCREEN:
	default:;
	}
}

/******************************************************************************
*	����:		����ƶ�����Ӧ����
*	��ע:		���ͼ��Ϊ�϶�״̬,���Զ�����ͼ���ƫ������
******************************************************************************/
void CGraphShowerView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		if (m_bWidhtOut)
		{
			m_nXX += point.x - m_psMove.x;
			if (m_nXX < 0 && abs(m_nXX) > m_nXXMax)
				m_nXX = -m_nXXMax;
			if (m_nXX > 0)
				m_nXX = 0;
		}

		if (m_bHeightOut)
		{
			m_nYY += point.y - m_psMove.y;
			if (m_nYY < 0 && abs(m_nYY) > m_nYYMax)
				m_nYY = -m_nYYMax;
			if (m_nYY > 0)
				m_nYY = 0;
		}

		m_psMove = point;

		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}

/******************************************************************************
*	����:		��������ı������
******************************************************************************/
BOOL CGraphShowerView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bHeightOut || m_bWidhtOut)
	{
		SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		return TRUE;
	}
	else
		return CView::OnSetCursor(pWnd, nHitTest, message);
}

/******************************************************************************
*	����:		���ڴ���Ԥ�Ȼ���ͼ��
******************************************************************************/
void CGraphShowerView::PreDrawImage(void)
{
	Image image(GetFilePath(m_nPos));

	::delete m_pBitmap;

	m_pBitmap = ::new Bitmap(image.GetWidth(), image.GetHeight(), PixelFormat32bppARGB);
	Graphics* graph = Graphics::FromImage(m_pBitmap);

	graph->DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
	SAFE_DELETE (graph);
}

/******************************************************************************
*	����:		����״̬����ָʾ����Ϣ
******************************************************************************/
void CGraphShowerView::RefreshIndicator()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	CString strTemp;
	strTemp.Format(L"%d/%d", m_nPos + 1, m_nPicNum);
	pMain->UpdateIndicator(ID_INDICATOR_PICNUM, strTemp);
	pMain->UpdateIndicator(ID_INDICATOR_PICNAME, m_FilesNameAry[m_nPos]);
}

void CGraphShowerView::OnModeNormal()
{
	if (m_nShowType != ST_NORMAL)
	{
		m_nShowType = ST_NORMAL;
		ClearOffset();
		FixOffset();
		Invalidate();
	}
}

void CGraphShowerView::OnModeWidth()
{
	if (m_nShowType != ST_FIXWIDTH)
	{
		m_nShowType = ST_FIXWIDTH;
		ClearOffset();
		FixOffset();
		Invalidate();
	}
}

void CGraphShowerView::OnModeHeight()
{
	if (m_nShowType != ST_FIXHEIGHT)
	{
		m_nShowType = ST_FIXHEIGHT;
		ClearOffset();
		FixOffset();
		Invalidate();
	}
}

void CGraphShowerView::OnModeScreen()
{
	if (m_nShowType != ST_FIXSCREEN)
	{
		m_nShowType = ST_FIXSCREEN;
		ClearOffset();
		FixOffset();
		Invalidate();
	}
}

void CGraphShowerView::OnUpdateModeNormal(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_NORMAL);
}

void CGraphShowerView::OnUpdateModeWidth(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXWIDTH);
}

void CGraphShowerView::OnUpdateModeHeight(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXHEIGHT);
}

void CGraphShowerView::OnUpdateModeScreen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXSCREEN);
}

void CGraphShowerView::OnImagePrev()
{
	if (m_nPos <= 0)
		return;

	m_nPos--;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	Invalidate();
}

void CGraphShowerView::OnImageNext()
{
	if (m_nPos >= m_nPicNum - 1)
		return;

	m_nPos++;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	Invalidate();
}

void CGraphShowerView::OnImageFirst()
{
	m_nPos = 0;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	Invalidate();
}

void CGraphShowerView::OnImageLast()
{
	m_nPos = m_nPicNum - 1;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	Invalidate();
}

void CGraphShowerView::OnUpdateImagePrev(CCmdUI *pCmdUI)
{
	if (m_nPos <= 0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CGraphShowerView::OnUpdateImageNext(CCmdUI *pCmdUI)
{
	if ((m_nPos < 0) || m_nPos >= m_nPicNum - 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CGraphShowerView::OnUpdateImageFirst(CCmdUI *pCmdUI)
{
	if (m_nPos < 0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CGraphShowerView::OnUpdateImageLast(CCmdUI *pCmdUI)
{
	if (m_nPos < 0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

