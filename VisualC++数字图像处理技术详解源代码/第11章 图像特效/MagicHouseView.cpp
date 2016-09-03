// MagicHouseView.cpp : CMagicHouseView ���ʵ��
//

#include "stdafx.h"
#include "MagicHouse.h"

#include "MagicHouseDoc.h"
#include "MagicHouseView.h"

#include "MagicHouseDoc.h"
#include "mainfrm.h"

#include "EffectDisplay.h"
#include "myDib.h"
#include "HistogramDlg.h"

#include "JpegDecoder.h"
#include "PreviewDlg.h"
#include "LineTranDlg.h"
#include "StretchDlg.h"
#include "ThresholdDlg.h"
#include "GrayOperator.h"
#include "LogTranDlg.h"
#include "PowerTranDlg.h"
#include "ExpTranDlg.h"
#include "MoveTranDlg.h"
#include "MirTranDlg.h"
#include "ZoomDlg.h"
#include "RotateDlg.h"
#include "FogDlg.h"

#include "GeoOperator.h"
#include "improve.h"
#include "Filter.h"

#include <iostream>
#include <string>
using namespace std;

#pragma warning ( disable : 4996)

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

typedef struct s_BM_header {
	WORD BMP_id ; // 'B''M'
	DWORD size; // size in bytes of the BMP file
	DWORD zero_res; // 0
	DWORD offbits; // 54
	DWORD biSize; // 0x28
	DWORD Width;  // X
	DWORD Height;  // Y
	WORD  biPlanes; // 1
	WORD  biBitCount ; // 24
	DWORD biCompression; // 0 = BI_RGB
	DWORD biSizeImage; // 0
	DWORD biXPelsPerMeter; // 0xB40
	DWORD biYPelsPerMeter; // 0xB40
	DWORD biClrUsed; //0
	DWORD biClrImportant; //0
} BM_header;

typedef struct s_RGB {
	BYTE B;
	BYTE G;
	BYTE R;
} RGB;

// CMagicHouseView

IMPLEMENT_DYNCREATE(CMagicHouseView, CView)

BEGIN_MESSAGE_MAP(CMagicHouseView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MODE_NORMAL, &CMagicHouseView::OnModeNormal)
	ON_COMMAND(ID_MODE_WIDTH, &CMagicHouseView::OnModeWidth)
	ON_COMMAND(ID_MODE_HEIGHT, &CMagicHouseView::OnModeHeight)
	ON_COMMAND(ID_MODE_SCREEN, &CMagicHouseView::OnModeScreen)
	ON_UPDATE_COMMAND_UI(ID_MODE_NORMAL, &CMagicHouseView::OnUpdateModeNormal)
	ON_UPDATE_COMMAND_UI(ID_MODE_WIDTH, &CMagicHouseView::OnUpdateModeWidth)
	ON_UPDATE_COMMAND_UI(ID_MODE_HEIGHT, &CMagicHouseView::OnUpdateModeHeight)
	ON_UPDATE_COMMAND_UI(ID_MODE_SCREEN, &CMagicHouseView::OnUpdateModeScreen)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_IMAGE_PREV, &CMagicHouseView::OnImagePrev)
	ON_COMMAND(ID_IMAGE_NEXT, &CMagicHouseView::OnImageNext)
	ON_COMMAND(ID_IMAGE_FIRST, &CMagicHouseView::OnImageFirst)
	ON_COMMAND(ID_IMAGE_LAST, &CMagicHouseView::OnImageLast)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_PREV, &CMagicHouseView::OnUpdateImagePrev)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_NEXT, &CMagicHouseView::OnUpdateImageNext)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_FIRST, &CMagicHouseView::OnUpdateImageFirst)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_LAST, &CMagicHouseView::OnUpdateImageLast)
	ON_COMMAND(ID_EFFECT_NONE, &CMagicHouseView::OnEffectNone)
	ON_COMMAND(ID_EFFECT_RAND, &CMagicHouseView::OnEffectRand)
	ON_COMMAND(ID_EFFECT_SCANDOWN, &CMagicHouseView::OnEffectScan)
	ON_COMMAND(ID_EFFECT_VSCAN, &CMagicHouseView::OnEffectDscan)
	ON_COMMAND(ID_EFFECT_MOVERIGHT, &CMagicHouseView::OnEffectMove)
	ON_COMMAND(ID_EFFECT_VBLIND, &CMagicHouseView::OnEffectVBlind)
	ON_COMMAND(ID_EFFECT_HRASTER, &CMagicHouseView::OnEffectHRaster)
	ON_COMMAND(ID_EFFECT_MOSAIC, &CMagicHouseView::OnEffectMosaic)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_NONE, &CMagicHouseView::OnUpdateEffectNone)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_RAND, &CMagicHouseView::OnUpdateEffectRand)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_SCANDOWN, &CMagicHouseView::OnUpdateEffectScan)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_VSCAN, &CMagicHouseView::OnUpdateEffectDscan)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_MOVERIGHT, &CMagicHouseView::OnUpdateEffectMove)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_VBLIND, &CMagicHouseView::OnUpdateEffectBlind)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_HRASTER, &CMagicHouseView::OnUpdateEffectRaster)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_MOSAIC, &CMagicHouseView::OnUpdateEffectMosaic)
	ON_COMMAND(ID_EFFECT_HSMOVE, &CMagicHouseView::OnEffectHsmove)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_HSMOVE, &CMagicHouseView::OnUpdateEffectHsmove)
	ON_COMMAND(ID_EFFECT_SBLIND, &CMagicHouseView::OnEffectSblind)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_SBLIND, &CMagicHouseView::OnUpdateEffectSblind)
	ON_COMMAND(ID_EFFECT_VRASTER, &CMagicHouseView::OnEffectVraster)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_VRASTER, &CMagicHouseView::OnUpdateEffectVraster)
	ON_COMMAND(ID_EFFECT_RAINDROP, &CMagicHouseView::OnEffectRaindrop)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_RAINDROP, &CMagicHouseView::OnUpdateEffectRaindrop)
	ON_COMMAND(ID_FILE_OPEN, &CMagicHouseView::OnFileOpen)
	ON_COMMAND(ID_MODE_BROWSE, &CMagicHouseView::OnModeBrowse)
	ON_COMMAND(ID_MODE_EDIT, &CMagicHouseView::OnModeEdit)
	ON_COMMAND(ID_FILE_SAVE, &CMagicHouseView::OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_MODE_BROWSE, &CMagicHouseView::OnUpdateModeBrowse)
	ON_UPDATE_COMMAND_UI(ID_MODE_EDIT, &CMagicHouseView::OnUpdateModeEdit)
	ON_COMMAND(ID_FILTER_NEGATIVE, &CMagicHouseView::OnFilterNegative)
	ON_COMMAND(ID_FILTER_EMBOSS, &CMagicHouseView::OnFilterEmboss)
	ON_COMMAND(ID_FILTER_BW, &CMagicHouseView::OnFilterBw)
	ON_COMMAND(ID_FILTER_FOG, &CMagicHouseView::OnFilterFog)
	ON_COMMAND(ID_FILTER_MOSAIC, &CMagicHouseView::OnFilterMosaic)
	ON_COMMAND(ID_FILTER_SKETCH, &CMagicHouseView::OnFilterSketch)
END_MESSAGE_MAP()

// CMagicHouseView ����/����

CMagicHouseView::CMagicHouseView()
{
	m_bIsEditMode = false;
	m_nShowType = ST_NORMAL;
	m_bHeightOut = false;
	m_bWidhtOut = false;
	m_nXX = 0;
	m_nYY = 0;
	m_nXXMax = 0;
	m_nYYMax = 0;
	m_nPos = -1;

	m_pBitmap = NULL;
	m_bEffectDraw = false;
	m_nEffectDisplayType = EDT_RAND;

	m_pImageBuffer = NULL;
	m_pImageTempBuffer = NULL;
	m_nPicWidth = m_nPicHeight = 0;
}

CMagicHouseView::~CMagicHouseView()
{
	::delete m_pBitmap;

	FreeImageBuffer();
}

//--------------------------------------------------------------
//	����:		��ͼ�ػ溯��
//--------------------------------------------------------------
void CMagicHouseView::OnDraw(CDC* pDC)
{
	CMagicHouseDoc* pDoc = GetDocument();
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

	if (m_bEffectDraw && m_nEffectDisplayType != EDT_NONE)
	{
		int nHeight = rect.Height() - m_nShowPicHeight;
		int nWidth = rect.Width() - m_nShowPicWidth;

		if (nHeight < 0)	
			nHeight = 0;
		if (nWidth < 0)	
			nWidth = 0;

		pDC->FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));

		EffectDisplay::s_nOffsetX = nWidth / 2;
		EffectDisplay::s_nOffsetY = nHeight / 2;
		EffectDisplay::s_nPicWidth = m_nShowPicWidth;
		EffectDisplay::s_nPicHeight = m_nShowPicHeight;
		EffectDisplay::s_nCDCWidth = rect.Width();
		EffectDisplay::s_nCDCHeight = rect.Height();

		EffectDisplayImage(pDC, &memDC);

		m_bEffectDraw = false;
	}
	else
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	MemBitmap.DeleteObject();
	memDC.DeleteDC();
}

//--------------------------------------------------------------
//	����:		��ָ��·�����ļ�
//--------------------------------------------------------------
void CMagicHouseView::OpenFile(const CString& strPath)
{	
	FreeImageBuffer();
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
	m_bEffectDraw = true;
	Invalidate();

	if (m_bIsEditMode)
	{
		CString strPath = GetFilePath(m_nPos);
		GetImageBuffer(strPath);
	}
}

void CMagicHouseView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

// CMagicHouseView ��ӡ

BOOL CMagicHouseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMagicHouseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMagicHouseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CMagicHouseView ���

#ifdef _DEBUG
void CMagicHouseView::AssertValid() const
{
	CView::AssertValid();
}

void CMagicHouseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMagicHouseDoc* CMagicHouseView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMagicHouseDoc)));
	return (CMagicHouseDoc*)m_pDocument;
}
#endif //_DEBUG

//----------------------------------
// ����:		"��"��������Ӧ����
//----------------------------------
void CMagicHouseView::OnFileOpen()
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

//----------------------------------
//	����:		��ֹ�ػ汳��
//----------------------------------
BOOL CMagicHouseView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//----------------------------------
//	����:			��ָ��ģʽ��ʾͼ��
//	����:		
//  	pDC			Ŀ���豸������ָ��
//		image		��ʾͼ��
//		nShowType	��ʾģʽ
//----------------------------------
void CMagicHouseView::ShowPicture(CDC* pDC, Image& image, int nShowType)
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

//----------------------------------
//	����:		������ʾģʽ
//	����:		
//		nType	�µ���ʾģʽ
//----------------------------------
void CMagicHouseView::SetShowType(int nType)
{
	m_nShowType = nType;
}

//----------------------------------
//	����:			���ָ��ģʽ��ͼ��Ĵ�С
//	����:		
//		image		ͼ�����
//		nShowType	��ʾģʽ
//	����ֵ:			��ָ��ģʽ��ͼ�����ʾ��С
//----------------------------------
CSize CMagicHouseView::GetShowPicSize(Image& image, int nShowType)
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


//----------------------------------
//	����:		��갴�µ���Ӧ����
//----------------------------------
void CMagicHouseView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_psMove = point;

	CView::OnLButtonDown(nFlags, point);
}

//----------------------------------
//	����:		����ͼ���ƫ���������Ϣ
//	��ע:		����ʾģʽΪ����
//----------------------------------
void CMagicHouseView::FixOffset()
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

//----------------------------------
//	����:		����ƶ�����Ӧ����
//	��ע:		���ͼ��Ϊ�϶�״̬,���Զ�����ͼ���ƫ������
//----------------------------------
void CMagicHouseView::OnMouseMove(UINT nFlags, CPoint point)
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

//------------------------------------------------------------
//	����:		��������ı������
//------------------------------------------------------------
BOOL CMagicHouseView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bHeightOut || m_bWidhtOut)
	{
		SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		return TRUE;
	}
	else
		return CView::OnSetCursor(pWnd, nHitTest, message);
}

//--------------------------------------------------------------
//	����:		���ڴ���Ԥ�Ȼ���ͼ��
//--------------------------------------------------------------
void CMagicHouseView::PreDrawImage(void)
{
	Image image(GetFilePath(m_nPos));

	::delete m_pBitmap;

	m_pBitmap = ::new Bitmap(image.GetWidth(), image.GetHeight(), PixelFormat32bppARGB);
	Graphics* graph = Graphics::FromImage(m_pBitmap);

	graph->DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
	SAFE_DELETE (graph);
}

//--------------------------------------------------------------
//	����:		����״̬����ָʾ����Ϣ
//--------------------------------------------------------------
void CMagicHouseView::RefreshIndicator()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	CString strTemp;
	strTemp.Format(L"%d/%d", m_nPos + 1, m_nPicNum);
	pMain->UpdateIndicator(ID_INDICATOR_PICNUM, strTemp);
	pMain->UpdateIndicator(ID_INDICATOR_PICNAME, m_FilesNameAry[m_nPos]);
}

//--------------------------------------------------------------
//	����:		ʹ����Ч��ʾͼ��
//	����:
//		pDC		Ŀ���豸������
//		pMemDC	�ڴ��豸������
//	��ע:	���ڴ��豸���������Ѿ����Ƶ�ͼ������Ч��ʽ��ʾ��Ŀ���豸��������
//--------------------------------------------------------------
void CMagicHouseView::EffectDisplayImage(CDC* pDC, CDC* pMemDC)
{
	int nType = m_nEffectDisplayType;

	if (nType == EDT_RAND)
	{
		LARGE_INTEGER seed;
		QueryPerformanceFrequency(&seed);
		QueryPerformanceCounter(&seed);

		//��ʼ��һ����΢��Ϊ��λ��ʱ������
		srand((int)seed.QuadPart);

		nType = rand() % 10;
	}

	switch (nType)
	{
	case EDT_SCANDOWN:
		EffectDisplay::ScanDownDisplay(pDC, pMemDC);
		break;

	case EDT_VSCAN:
		EffectDisplay::VSScanDisplay(pDC, pMemDC);
		break;

	case EDT_MOVERIGHT:
		EffectDisplay::MoveRightDisplay(pDC, pMemDC);
		break;

	case EDT_HSMOVE:
		EffectDisplay::HSMoveDisplay(pDC, pMemDC);
		break;

	case EDT_VBLIND:
		EffectDisplay::VBlindDisplay(pDC, pMemDC);
		break;

	case EDT_HBLIND:
		EffectDisplay::HBlindDisplay(pDC, pMemDC);
		break;

	case EDT_VRASTER:
		EffectDisplay::VRasterDisplay(pDC, pMemDC);
		break;

	case EDT_HRASTER:
		EffectDisplay::HRasterDisplay(pDC, pMemDC);
		break;

	case EDT_MOSAIC:
		EffectDisplay::MosaicDisplay(pDC, pMemDC);
		break;

	default:
		EffectDisplay::RaindropDisplay(pDC, pMemDC);
	}
}

void CMagicHouseView::OnModeNormal()
{
	if (m_nShowType != ST_NORMAL)
	{
		m_nShowType = ST_NORMAL;
		FixOffset();
		Invalidate();
	}
}

void CMagicHouseView::OnModeWidth()
{
	if (m_nShowType != ST_FIXWIDTH)
	{
		m_nShowType = ST_FIXWIDTH;
		FixOffset();
		Invalidate();
	}
}

void CMagicHouseView::OnModeHeight()
{
	if (m_nShowType != ST_FIXHEIGHT)
	{
		m_nShowType = ST_FIXHEIGHT;
		FixOffset();
		Invalidate();
	}
}

void CMagicHouseView::OnModeScreen()
{
	if (m_nShowType != ST_FIXSCREEN)
	{
		m_nShowType = ST_FIXSCREEN;
		FixOffset();
		Invalidate();
	}
}

void CMagicHouseView::OnUpdateModeNormal(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_NORMAL);
}

void CMagicHouseView::OnUpdateModeWidth(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXWIDTH);
}

void CMagicHouseView::OnUpdateModeHeight(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXHEIGHT);
}

void CMagicHouseView::OnUpdateModeScreen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXSCREEN);
}

void CMagicHouseView::OnImagePrev()
{
	if (m_nPos <= 0)
		return;

	m_nPos--;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CMagicHouseView::OnImageNext()
{
	if (m_nPos >= m_nPicNum - 1)
		return;

	m_nPos++;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CMagicHouseView::OnImageFirst()
{
	m_nPos = 0;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CMagicHouseView::OnImageLast()
{
	m_nPos = m_nPicNum - 1;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CMagicHouseView::OnUpdateImagePrev(CCmdUI *pCmdUI)
{
	if (m_nPos <= 0 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMagicHouseView::OnUpdateImageNext(CCmdUI *pCmdUI)
{
	if ((m_nPos < 0) || m_nPos >= m_nPicNum - 1 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMagicHouseView::OnUpdateImageFirst(CCmdUI *pCmdUI)
{
	if (m_nPos < 0 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMagicHouseView::OnUpdateImageLast(CCmdUI *pCmdUI)
{
	if (m_nPos < 0 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMagicHouseView::OnEffectNone()
{
	m_nEffectDisplayType = EDT_NONE;
}

void CMagicHouseView::OnEffectRand()
{
	m_nEffectDisplayType = EDT_RAND;
}

void CMagicHouseView::OnEffectScan()
{
	m_nEffectDisplayType = EDT_SCANDOWN;
}

void CMagicHouseView::OnEffectDscan()
{
	m_nEffectDisplayType = EDT_VSCAN;
}

void CMagicHouseView::OnEffectMove()
{
	m_nEffectDisplayType = EDT_MOVERIGHT;
}

void CMagicHouseView::OnEffectHsmove()
{
	m_nEffectDisplayType = EDT_HSMOVE;
}

void CMagicHouseView::OnEffectVBlind()
{
	m_nEffectDisplayType = EDT_VBLIND;
}

void CMagicHouseView::OnEffectSblind()
{
	m_nEffectDisplayType = EDT_HBLIND;
}

void CMagicHouseView::OnEffectVraster()
{
	m_nEffectDisplayType = EDT_VRASTER;
}

void CMagicHouseView::OnEffectHRaster()
{
	m_nEffectDisplayType = EDT_HRASTER;
}

void CMagicHouseView::OnEffectMosaic()
{
	m_nEffectDisplayType = EDT_MOSAIC;
}

void CMagicHouseView::OnEffectRaindrop()
{
	m_nEffectDisplayType = EDT_RAINDROP;
}

void CMagicHouseView::OnUpdateEffectNone(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_NONE);
}

void CMagicHouseView::OnUpdateEffectRand(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_RAND);
}

void CMagicHouseView::OnUpdateEffectScan(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_SCANDOWN);
}

void CMagicHouseView::OnUpdateEffectDscan(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_VSCAN);
}

void CMagicHouseView::OnUpdateEffectMove(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_MOVERIGHT);
}

void CMagicHouseView::OnUpdateEffectBlind(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_VBLIND);
}

void CMagicHouseView::OnUpdateEffectRaster(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_HRASTER);
}

void CMagicHouseView::OnUpdateEffectMosaic(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_MOSAIC);
}

void CMagicHouseView::OnUpdateEffectHsmove(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_HSMOVE);
}

void CMagicHouseView::OnUpdateEffectSblind(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_HBLIND);
}

void CMagicHouseView::OnUpdateEffectVraster(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_VRASTER);
}

void CMagicHouseView::OnUpdateEffectRaindrop(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_RAINDROP);
}

void CMagicHouseView::OnModeBrowse()
{
	if (!m_bIsEditMode)
		return;

	FreeImageBuffer();
	m_bIsEditMode = false;
}

void CMagicHouseView::OnModeEdit()
{
	if (m_bIsEditMode)
		return;

	m_bIsEditMode = true;

	if (m_nPos != -1)
	{
		CString strPath = GetFilePath(m_nPos);
		GetImageBuffer(strPath);
	}
}

//--------------------------------------------------------------
//	����:			��ָ��·����ͼ����Ϣ��ȡ��������
//	����:			
//		strPath		ͼ���ļ���ַ
//--------------------------------------------------------------
bool CMagicHouseView::GetImageBuffer(CString& strPath)
{
	CString fileNameExt = strPath.Right(3);
	string filePath = (LPCSTR)CStringA(strPath);

	if (fileNameExt == "jpg" || fileNameExt == "JPG")
	{
		FILE *fp;

		fp = fopen(filePath.c_str(), "rb");

		JpegDecoder dec(fp);
		fclose(fp);

		m_nPicWidth = m_nTempWidth = dec.GetWidth();
		m_nPicHeight = m_nTempHeight = dec.GetHeight();
		dec.GetJPEGBuffer(m_nPicWidth, m_nPicHeight,&m_pImageBuffer);
		int sizeb = m_nPicWidth * m_nPicHeight * 4;

		m_pImageTempBuffer = (BYTE*)malloc(sizeb);
		memcpy(m_pImageTempBuffer, m_pImageBuffer, sizeb);
	}

	if (fileNameExt == "bmp" || fileNameExt == "BMP")
	{
		MyDib* dib = new MyDib(strPath);

		int b = dib->GetBitsPerPixel();

		m_nPicWidth = m_nTempWidth = dib->GetWidth();
		m_nPicHeight = m_nTempHeight = dib->GetHeight();
		int sizeofbuffer = m_nPicWidth * m_nPicHeight * 4;
		m_pImageTempBuffer = (BYTE *)malloc(sizeofbuffer);

		m_pImageBuffer = (BYTE *)malloc(dib->GetBodySize());
		memcpy(m_pImageBuffer, dib->GetBits(), dib->GetBodySize());	

		SAFE_DELETE (dib);

		if ( b != 24 && b != 32 && b != 8 ) 
		{
			free(m_pImageTempBuffer);
			free(m_pImageBuffer);
			MessageBox(L"��������ܴ���24λ��32λ��ɫͼ��8λ�Ҷ�ͼ!");
			return false;
		}

		if (b == 32) 
		{
			for(unsigned int i = 0; i < m_nPicWidth * 4; i += 4) 
			{
				for (unsigned int j = 0; j < m_nPicHeight / 2; j++) 
				{
					swap(m_pImageBuffer[j*m_nPicWidth*4+i], m_pImageBuffer[(m_nPicHeight-j-1)*m_nPicWidth*4+i]);
					swap(m_pImageBuffer[j*m_nPicWidth*4+i+1], m_pImageBuffer[(m_nPicHeight-j-1)*m_nPicWidth*4+i+1]);
					swap(m_pImageBuffer[j*m_nPicWidth*4+i+2], m_pImageBuffer[(m_nPicHeight-j-1)*m_nPicWidth*4+i+2]);
					swap(m_pImageBuffer[j*m_nPicWidth*4+i+3], m_pImageBuffer[(m_nPicHeight-j-1)*m_nPicWidth*4+i+3]);
				}
			}
			memcpy(m_pImageTempBuffer, m_pImageBuffer, sizeofbuffer);
		}

		if (b == 24) 
		{
			int externWidth;
			externWidth = m_nPicWidth * 3;
			if(externWidth % 4 != 0)
				externWidth = 4 - externWidth % 4;
			else
				externWidth = 0;

			int k = 0;

			for (int n = m_nPicHeight - 1; n >= 0; n--) 
			{
				for (UINT m = 0; m < m_nPicWidth * 3; m += 3) 
				{
					m_pImageTempBuffer[k]   = m_pImageBuffer[n*(m_nPicWidth*3+externWidth)+m];	//blue
					m_pImageTempBuffer[k+1] = m_pImageBuffer[n*(m_nPicWidth*3+externWidth)+m+1];//green
					m_pImageTempBuffer[k+2] = m_pImageBuffer[n*(m_nPicWidth*3+externWidth)+m+2];//red
					m_pImageTempBuffer[k+3] = 255;
					k += 4;
				}
			}

			free(m_pImageBuffer);
			m_pImageBuffer = (BYTE *)malloc(sizeofbuffer);
			memcpy(m_pImageBuffer, m_pImageTempBuffer, sizeofbuffer);
		}

		if (b==8) {

			int externWidth;
			externWidth = m_nPicWidth;
			if(externWidth%4!=0)
				externWidth=4-externWidth%4;
			else
				externWidth=0;

			int k = 0;

			for (int n=m_nPicHeight-1; n>=0; n--) {
				for (UINT m=0; m<m_nPicWidth; m+=1) {

					m_pImageTempBuffer[k]  =m_pImageBuffer[n*(m_nPicWidth+externWidth)+m];//blue
					m_pImageTempBuffer[k+1]=m_pImageBuffer[n*(m_nPicWidth+externWidth)+m];//green
					m_pImageTempBuffer[k+2]=m_pImageBuffer[n*(m_nPicWidth+externWidth)+m];//red
					m_pImageTempBuffer[k+3]=0;
					k+=4;
				}
			}

			free(m_pImageBuffer);
			m_pImageBuffer = (BYTE*)malloc(sizeofbuffer);
			memcpy(m_pImageBuffer, m_pImageTempBuffer, sizeofbuffer);
		}
	}

	return true;
}

//--------------------------------------------------------------
//	����:		�ͷ�ͼ����Ϣռ�õ��ڴ�
//--------------------------------------------------------------
void CMagicHouseView::FreeImageBuffer(void)
{
	if (m_pImageBuffer != NULL)
	{
		free(m_pImageBuffer);
		m_pImageBuffer = NULL;
	}

	if (m_pImageTempBuffer != NULL)
	{
		free(m_pImageTempBuffer);
		m_pImageTempBuffer = NULL;
	}
}

//--------------------------------------------------------------
//	����:		"����"��������Ӧ����
//--------------------------------------------------------------
void CMagicHouseView::OnFileSave()
{
	if (m_nPos == -1) 
	{
		MessageBox(L"��������δ����ͼƬ�ļ�!", L"Magic House", MB_ICONWARNING);
		return;
	}
	if (m_pImageTempBuffer == NULL) 
	{
		MessageBox(L"û�п��Ա�����ļ�!", L"Magic House", MB_ICONWARNING);
		return;
	}
	CFileDialog SaveDlg( FALSE, L"bmp", L"δ����", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, L"ͼ���ļ���ʽ(*.bmp) |*.bmp||", NULL);
	if(SaveDlg.DoModal() == IDOK)
	{
		CString tempname = SaveDlg.GetPathName();
		string savePath = (LPCSTR)CStringA(tempname);
		WriteBufferToBMP(m_pImageTempBuffer, m_nTempWidth, m_nTempHeight, savePath.c_str());
		tempname.ReleaseBuffer();
		MessageBox(L"�ļ��ѳɹ�����!");
	}
}

//--------------------------------------------------------------
//	����:			��ͼ����Ϣд��bmp�ļ���
//	����:		
//		im_buffer	ͼ����Ϣ
//	    X_bitmap	���
//		Y_bitmap	�߶�
//		BMPname		����bmp�ļ�·��
//--------------------------------------------------------------
void CMagicHouseView::WriteBufferToBMP(BYTE *im_buffer, WORD X_bitmap, WORD Y_bitmap, const char* BMPname)
{
	SWORD x,y;
	RGB *pixel;
	BM_header BH;
	FILE *fp_bitmap;
	DWORD im_loc_bytes;
	BYTE nr_fillingbytes, i;
	BYTE zero_byte=0;

	fp_bitmap=fopen(BMPname,"wb");
	if (fp_bitmap==NULL) 
	{
			AfxMessageBox(L"����! �ļ��޷�����.", MB_ICONWARNING);
			return;
	}

	if (X_bitmap%4!=0) nr_fillingbytes=4-((X_bitmap*3L)%4);
	else nr_fillingbytes=0;

	BH.BMP_id = 'M'*256+'B';     fwrite(&BH.BMP_id,2,1,fp_bitmap);
	BH.size=54+Y_bitmap*(X_bitmap*3+nr_fillingbytes);fwrite(&BH.size,4,1,fp_bitmap);
	BH.zero_res = 0;             fwrite(&BH.zero_res,4,1,fp_bitmap);
	BH.offbits = 54;             fwrite(&BH.offbits,4,1,fp_bitmap);
	BH.biSize = 0x28;            fwrite(&BH.biSize,4,1,fp_bitmap);
	BH.Width = X_bitmap;	      fwrite(&BH.Width,4,1,fp_bitmap);
	BH.Height = Y_bitmap;	      fwrite(&BH.Height,4,1,fp_bitmap);
	BH.biPlanes = 1;             fwrite(&BH.biPlanes,2,1,fp_bitmap);
	BH.biBitCount = 24;          fwrite(&BH.biBitCount,2,1,fp_bitmap);
	BH.biCompression = 0;        fwrite(&BH.biCompression,4,1,fp_bitmap);
	BH.biSizeImage = 0;          fwrite(&BH.biSizeImage,4,1,fp_bitmap);
	BH.biXPelsPerMeter = 0xB40;  fwrite(&BH.biXPelsPerMeter,4,1,fp_bitmap);
	BH.biYPelsPerMeter = 0xB40;  fwrite(&BH.biYPelsPerMeter,4,1,fp_bitmap);
	BH.biClrUsed = 0;	          fwrite(&BH.biClrUsed,4,1,fp_bitmap);
	BH.biClrImportant = 0;	      fwrite(&BH.biClrImportant,4,1,fp_bitmap);

	im_loc_bytes=(DWORD)im_buffer+((DWORD)Y_bitmap-1)*X_bitmap*4;

	for (y=0;y<Y_bitmap;y++)
	{
		for (x=0;x<X_bitmap;x++)
		{
			pixel=(RGB *)im_loc_bytes;
			fwrite(pixel, 3, 1, fp_bitmap);
			im_loc_bytes+=4;
		}
		for (i=0;i<nr_fillingbytes;i++)
			fwrite(&zero_byte,1,1,fp_bitmap);
		im_loc_bytes-=2L*X_bitmap*4;
	}
	fclose(fp_bitmap);
}
void CMagicHouseView::OnUpdateModeBrowse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bIsEditMode);
}

void CMagicHouseView::OnUpdateModeEdit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bIsEditMode);
}

void CMagicHouseView::ResetImage()
{
	m_nTempWidth = m_nPicWidth;
	m_nTempHeight = m_nPicHeight;

	int sizeofbuffer = m_nTempWidth * m_nTempHeight * 4;
	SAFE_DELETE (m_pImageTempBuffer);

	m_pImageTempBuffer = new BYTE[sizeofbuffer];
	memcpy(m_pImageTempBuffer, m_pImageBuffer, sizeofbuffer);
}



void CMagicHouseView::OnFilterNegative()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"���ȴ�ͼ���ļ���Ȼ��ѡ��༭ģʽ��");
		return;
	}

	ResetImage();
	Negative(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// ����Ԥ���Ի���
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// �Ի����ڹر�ʱ��ʾ�Ƿ񱣴�
	dlg.DoModal();
}
void CMagicHouseView::OnFilterEmboss()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"���ȴ�ͼ���ļ���Ȼ��ѡ��༭ģʽ��");
		return;
	}

	ResetImage();
	Emboss(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// ����Ԥ���Ի���
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// �Ի����ڹر�ʱ��ʾ�Ƿ񱣴�
	dlg.DoModal();
}
void CMagicHouseView::OnFilterBw()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"���ȴ�ͼ���ļ���Ȼ��ѡ��༭ģʽ��");
		return;
	}

	ResetImage();
	ColorToBW(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// ����Ԥ���Ի���
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// �Ի����ڹر�ʱ��ʾ�Ƿ񱣴�
	dlg.DoModal();
}
void CMagicHouseView::OnFilterFog()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"���ȴ�ͼ���ļ���Ȼ��ѡ��༭ģʽ��");
		return;
	}
	CFogDlg dlg;
	ResetImage();
	dlg.DoModal();
}

void CMagicHouseView::OnFilterMosaic()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"���ȴ�ͼ���ļ���Ȼ��ѡ��༭ģʽ��");
		return;
	}

	//������������ĳ���
	int sum = m_nPicWidth * m_nPicHeight * 4;
	memcpy(m_pImageTempBuffer, m_pImageBuffer, sum);

	//ͨ��degree�����������˻��ĳ̶�
	unsigned int degree = 3;
	//���������˴�����������������д���
	Mosaic(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight, degree);

	// ����Ԥ���Ի���
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// �Ի����ڹر�ʱ��ʾ�Ƿ񱣴�
	dlg.DoModal();
}

void CMagicHouseView::OnFilterSketch()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"���ȴ�ͼ���ļ���Ȼ��ѡ��༭ģʽ��");
		return;
	}

	ResetImage();
	LaplacianB(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// ����Ԥ���Ի���
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// �Ի����ڹر�ʱ��ʾ�Ƿ񱣴�
	dlg.DoModal();
}

