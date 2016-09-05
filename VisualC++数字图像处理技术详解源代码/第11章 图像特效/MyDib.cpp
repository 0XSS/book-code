// MyDib.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MagicHouse.h"
#include "MyDib.h"


MyDib::MyDib ( LPCTSTR dibName )
: DibBits ( NULL ), DibInfo ( NULL )
{
	//����CFile����
	CFile fp ( dibName, CFile::modeRead | CFile::typeBinary );

	BITMAPFILEHEADER bmfileheader;
	BITMAPINFOHEADER bmheader;

	ULONGLONG headpos;
	int PaletteSize = 0;
	int ret, cbHeaderSize;
	//��ȡ�ļ�ָ���λ��
	headpos = fp.GetPosition();
	//��ȡBMP�ļ�ͷ
	ret = fp.Read ( &bmfileheader, sizeof(BITMAPFILEHEADER) );
	//����ļ����ͱ�ͷ���ǡ�0x4d42������ʾ���ļ�����BMP�����ļ�
	//����ʾ�����˳���ע�⡰0x4d42�����ַ�������ǡ�BM��
	if ( bmfileheader.bfType != 0x4d42) 
	{
		AfxMessageBox(L"Error!");
		return;
	}

	//��ȡBMP�ļ���Ϣͷ	
	ret = fp.Read ( &bmheader, sizeof(BITMAPINFOHEADER) );
	//����RGBQUAD�Ĵ�С
	switch ( bmheader.biBitCount )
	{
	case 1:
		PaletteSize = 2;
		break;
	case 4:
		PaletteSize = 16;
		break;
	case 8:
		PaletteSize = 256;
		break;
	}

	//ΪBITMAPINFO�ṹ�����ڴ�
	cbHeaderSize = sizeof(BITMAPINFOHEADER) + PaletteSize*sizeof ( RGBQUAD );
	DibInfo = (BITMAPINFO*) new char [ cbHeaderSize ];
	DibInfo->bmiHeader = bmheader;

	if ( PaletteSize )
	{	
		ret = fp.Read ( &(DibInfo->bmiColors[0]), PaletteSize*sizeof ( RGBQUAD ) );
		if ( ret != int( PaletteSize*sizeof ( RGBQUAD ) ) )
		{
			delete[] DibInfo;
			DibInfo = NULL;
			return;
		}
	}

	//Ϊ�����������ռ䣬��С��GetBodySize()����
	DibBits = (void*) new char[GetBodySize()];
	//���ļ�ָ���ƶ���DIBͼ������
	fp.Seek ( headpos + bmfileheader.bfOffBits, CFile::begin );

	ret = fp.Read ( DibBits, GetBodySize() );
	if ( ret != int ( GetBodySize() ) )
	{
		delete[] DibInfo;
		delete[] DibBits;
		DibInfo = NULL;
		DibBits = NULL;
	}
	//�ر��ļ�
	fp.Close();
}

MyDib::MyDib ( CBitmap& ddb, CPalette* palette )
: DibBits ( NULL ), DibInfo ( NULL )
{
	BITMAP ddbinfo;
	//���DDB�������Ϣ
	ddb.GetBitmap ( &ddbinfo );	
	/*�������ͼ��ĸ߶ȿ�ȵ���Ϣ����дDIB�������BITMAPINFO*/
	int w = ddbinfo.bmWidth;
	int h = ddbinfo.bmHeight;
	int BitsPerPixel = ddbinfo.bmPlanes*ddbinfo.bmBitsPixel;

	int i;
	int PaletteSize = 0, cbHeaderSize;

	//����ÿ������Ҫ����bit������RGBQUAD����Ĵ�С
	switch ( BitsPerPixel )	
	{
	case 1:
		PaletteSize = 2;
		break;
	case 4:
		PaletteSize = 16;
		break;
	case 8:
		PaletteSize = 256;
		break;
		//����8λ��DIB����Ҫ��ɫ��
	case 15:
	case 16:
		BitsPerPixel = 16;
		break;
	}

	cbHeaderSize = sizeof(BITMAPINFOHEADER) + PaletteSize * sizeof ( RGBQUAD );
	if ( DibInfo ) delete[] DibInfo;
	//ΪBITMAPINFO�Ķ���DibInfo����һ���ڴ�ռ�
	DibInfo = (BITMAPINFO*) new char [ cbHeaderSize ]; 
	//��дBITMAPINFO
	DibInfo->bmiHeader.biSize = sizeof ( BITMAPINFOHEADER );
	DibInfo->bmiHeader.biWidth = w;
	DibInfo->bmiHeader.biHeight = h;
	DibInfo->bmiHeader.biPlanes = 1;
	DibInfo->bmiHeader.biBitCount = short(BitsPerPixel);
	DibInfo->bmiHeader.biCompression = BI_RGB;
	DibInfo->bmiHeader.biSizeImage = BytesPerLine() * DibInfo->bmiHeader.biHeight;
	DibInfo->bmiHeader.biXPelsPerMeter =
		DibInfo->bmiHeader.biYPelsPerMeter = 3780;
	DibInfo->bmiHeader.biClrUsed = PaletteSize;
	DibInfo->bmiHeader.biClrImportant = PaletteSize;

	if ( PaletteSize )
	{
		//��RGBQUAD�����е�ÿ��Ԫ��ȫ����0
		for ( i = 0 ; i < PaletteSize ; i ++ )
		{
			DibInfo->bmiColors[i].rgbRed = 0;
			DibInfo->bmiColors[i].rgbGreen = 0;
			DibInfo->bmiColors[i].rgbBlue = 0;
			DibInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	//ΪDIB�����������ռ䣬��С��GetBodySize()����
	DibBits = (void*) new char[GetBodySize()];
	//�ж��Ƿ��ɵ�ɫ�壬�������palette�е�entry
	//���Ƶ�BITMAPINFO��RGBQUAD������
	if ( palette ) CopyPalette ( *palette );

	// ��DDB�е�ͼ����Ϣ���Ƶ�DIB��
	// GetDesktopWindow()�������洰�ڵľ��
	CWnd DesktopWnd;	
	DesktopWnd.Attach ( ::GetDesktopWindow() );
	CWindowDC dtpDC ( &DesktopWnd );
	// ��ɴ�DDB��DIB��ת������
	::GetDIBits( dtpDC.m_hDC,		
		HBITMAP ( ddb ),	 
		0,		  
		ddbinfo.bmHeight,	
		LPVOID ( DibBits ),	
		DibInfo,
		DIB_RGB_COLORS  
		);
	DesktopWnd.Detach();
}

MyDib::~MyDib()
{
	delete[] DibInfo;
	delete[] DibBits;
}

void* MyDib::GetBits() 
{
	return DibBits; 
}

int MyDib::GetBitsPerPixel () 
{ 
	if( !DibInfo )return 0;
	return DibInfo->bmiHeader.biBitCount; 
}

long MyDib::GetWidth() 
{ 
	return DibInfo->bmiHeader.biWidth; 
}

long MyDib::GetHeight() 
{ 
	return DibInfo->bmiHeader.biHeight; 
}

long MyDib::GetBodySize() 
{ 
	return BytesPerLine() * DibInfo->bmiHeader.biHeight; 
}

int MyDib::GetPaletteCount()
{
	int PaletteSize = 0;
	if (!DibInfo) return 0;
	switch ( DibInfo->bmiHeader.biBitCount )
	{
	case 1:
		PaletteSize = 2;
		break;
	case 4:
		PaletteSize = 16;
		break;
	case 8:
		PaletteSize = 256;
		break;
	}
	return PaletteSize;
}

long MyDib::BytesPerLine() 
{ 
	return ((((DibInfo->bmiHeader.biWidth * GetBitsPerPixel())+31)/32)*4);
}

int MyDib::SetToDC ( CDC& dc, CRect& src, CPoint& dst )
{
	return ::SetDIBitsToDevice(
		dc.m_hDC,	
		dst.x,	
		dst.y,	 
		src.Width(),	 
		src.Height(),	 
		src.left,	 
		src.top,	 
		0,	 
		DibInfo->bmiHeader.biHeight,	 
		DibBits,	 
		DibInfo,	 
		DIB_RGB_COLORS	 
		);	
}

int MyDib::StretchToDC ( CDC& dc, CRect& src, CRect& dst, DWORD rop )
{
	return ::StretchDIBits (
		dc.m_hDC,	
		dst.left,	 
		dst.top,	 
		dst.Width(),	 
		dst.Height(),	 
		src.left,	 
		src.top,	 
		src.Width(),	 
		src.Height(),	 
		DibBits,	 
		DibInfo,	
		DIB_RGB_COLORS,	 
		rop		
		);	
}

void MyDib::SaveDIB ( LPCTSTR dibName )
{
	//��DIBΪ������ʾ���󲢷���
	if(!DibInfo)
	{
		AfxMessageBox(L"DIB��Ϣ�����ڣ�");
		return;
	}

	//ͨ���ļ�������CFile����
	//���ļ���ʧ������ʾ���󲢷���
	CFile file;
	if(!file.Open(dibName, CFile::modeCreate | CFile::modeReadWrite))
	{
		AfxMessageBox(L"�����ļ�ʧ�ܣ�");
		return;
	}

	//Bitmap�ļ�ͷ
	BITMAPFILEHEADER bmfileheader;

	long size = BytesPerLine() * DibInfo->bmiHeader.biHeight;
	int PaletteSize = GetPaletteCount();

	//��дBMP�ļ�������
	bmfileheader.bfType = 0x4d42;//���ͱ�ʾ"BM"
	bmfileheader.bfReserved1 = bmfileheader.bfReserved2 = 0;//������0
	bmfileheader.bfOffBits = 
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+PaletteSize*sizeof( RGBQUAD );
	bmfileheader.bfSize = bmfileheader.bfOffBits + size;

	//���ļ�ͷ��λͼ��Ϣ����������д������ļ�
	file.Write ( &bmfileheader, sizeof(BITMAPFILEHEADER) );
	file.Write ( DibInfo, sizeof(BITMAPINFOHEADER)+PaletteSize*sizeof( RGBQUAD ) );
	file.Write ( DibBits, size );
	//�ر��ļ�
	file.Close();	
}

void MyDib::CopyPalette( CPalette& palette )
{
	int i;
	PALETTEENTRY *pe;

	if ( !DibInfo->bmiHeader.biBitCount ) return;
	if ( palette.GetEntryCount() != DibInfo->bmiHeader.biBitCount ) return;

	pe = new PALETTEENTRY[palette.GetEntryCount()];
	palette.GetPaletteEntries( 0, palette.GetEntryCount(), pe );

	for ( i = 0 ; i < palette.GetEntryCount() ; i ++ )
	{
		DibInfo->bmiColors[i].rgbRed = pe[i].peRed;
		DibInfo->bmiColors[i].rgbGreen = pe[i].peGreen;
		DibInfo->bmiColors[i].rgbBlue = pe[i].peBlue;
		DibInfo->bmiColors[i].rgbReserved = 0;
	}

	delete[] pe;
}