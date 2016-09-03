// ************************************************************************
//  �ļ�����restore.cpp
//
//  ͼ��ԭAPI�����⣺
//
//  DIBNoRestriction()			- ͼ��ģ��
//  DIBInverseFilter()			- ͼ�����˲���ԭ
//  DIBNoiseDegeneration()		- ͼ��ģ������
//  DIBWinnerFilter()			- ͼ��ά���˲�
//	DIBMotionDegeneration()		- ͼ���˶�ģ��
//	DIBMotionRestore()			- ͼ���˶�ģ����ԭ
//
// *************************************************************************


#include "stdafx.h"
#include "GlobalApi.h"
#include "Cdib.h"

#include <math.h>
#include <direct.h>
#include <complex>
using namespace std;

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

/*************************************************************************
 *
 * �������ƣ�
 *   DIBNoRestriction()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ�����ģ��������
 *
 ************************************************************************/

BOOL WINAPI DIBNoRestriction(CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//��ʱ����
	double temp;

	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ������
	complex<double>  *pCFSrc,*pCFH;
	
	//ͼ���һ������
	double MaxNum;
	
	//����ͼ��ĳ��Ϳ����Ϊ2��������
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// Ϊʱ���Ƶ����������ռ�
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];

	// �����ݴ���ʱ������
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lLineBytes; i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			if(i < 5 && j < 5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//��Դͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//Ƶ�����
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		pCFSrc[i] = pCFSrc[i]*pCFH[i];
	}

	//�Խ��ͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ȷ����һ������
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			temp = sqrt(pCTSrc[ lLineBytes*j + i ].real() * pCTSrc[ lLineBytes*j + i ].real()
						+pCTSrc[lLineBytes*j + i ].imag() * pCTSrc[ lLineBytes*j +i].imag());
			
			//ѡ���һ������
			if( MaxNum < temp)
				MaxNum = temp;
		}
	}
	
	//ת��Ϊͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum);
		}
	}
	
	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBInverseFilter()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE
 *
 * ˵��:
 *   �ú���������DIBInverseFilter()���ɵ�DIBͼ����и�ԭ������
 *
 ************************************************************************/

BOOL WINAPI DIBInverseFilter (CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//��ʱ����
	double tempre, tempim, a, b, c, d;

	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ������
	complex<double>  *pCFSrc,*pCFH;
	
	//ͼ���һ������
	double MaxNum;
	
	//�����˻�ͼ��ĳ��Ϳ����Ϊ2��������
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// Ϊʱ���Ƶ����������ռ�
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];

	// ���˻�ͼ�����ݴ���ʱ������
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lLineBytes; i++)
		{
			// ָ���˻�ͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			if(i < 5 && j < 5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//���˻�ͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);
	
	//Ƶ�����
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		a = pCFSrc[i].real();
		b = pCFSrc[i].imag();
		c = pCFH[i].real();
		d = pCFH[i].imag();
		
		//���Ƶ��ֵ̫С�����迼��
		if (c*c + d*d > 1e-3)
		{
			tempre = ( a*c + b*d ) / ( c*c + d*d );
			tempim = ( b*c - a*d ) / ( c*c + d*d );
		}

		pCFSrc[i]= complex<double>(tempre , tempim);
	}

	//�Ը�ԭͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ȷ����һ������
	MaxNum=300;

	//ת��Ϊ��ԭͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum);
		}
	}
	
	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�//�����˻��Ѿ�ɾ��
 *   DIBNoiseDegeneration()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - ģ����������ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ�����ģ�����������
 *
 ************************************************************************/

BOOL WINAPI DIBNoiseDegeneration (CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//ת��Ϊͼ�񣬼���
	unsigned char NoisePoint;
	
	//��ʱ����
	double temp;

	//ͼ���һ������
	double MaxNum;

	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ������
	complex<double>  *pCFSrc,*pCFH;
		
	// Ϊʱ���Ƶ����������ռ�
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];
	
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��				
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// ������ֵ�洢��ʱ��������
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// Ƶ����ֵ
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// ������ͼ�����˻���ϵͳ
			if(i < 5 && j <5 )
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			
			// Ƶ����ֵ
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//��Դͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//Ƶ�����
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		pCFSrc[i] = pCFSrc[i]*pCFH[i];
	}

	//�Խ��ͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ȷ����һ������
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			temp = sqrt(pCTSrc[ lLineBytes*j + i ].real() * pCTSrc[ lLineBytes*j + i ].real()
						+pCTSrc[lLineBytes*j + i ].imag() * pCTSrc[ lLineBytes*j +i].imag());
			
			//ѡ���һ������
			if( MaxNum < temp)
				MaxNum = temp;
		}
	}
	
	//����α���������
	srand((unsigned)time(NULL));

	//ת��Ϊͼ�񣬲�����α�������
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ����������
			NoisePoint = rand()/2048-8;
			
			// ָ��Դͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// ʱ����룬�洢����ֵ
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum + NoisePoint);
			
			//�������ֵ����ֱ�Ӹ�ֵ255
			if(*lpSrc > 255)
				*lpSrc = 255 ;
		}
	}	
	
	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBWinnerFilter()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - ά���˲���ԭ�����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ�����ά���˲���ԭ������
 *
 ************************************************************************/

BOOL WINAPI DIBWinnerFilter (CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//��ʱ����
	double temp, tempre, tempim, 
			a, b, c, d, norm2;
	
	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ������
	complex<double>  *pCFSrc,*pCFH;
	
	//�����˻�ͼ��ĳ��Ϳ����Ϊ2��������
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// Ϊʱ���Ƶ����������ռ�
	pCTSrc		= new complex<double> [lHeight*lLineBytes];
	pCTH		= new complex<double> [lHeight*lLineBytes];
	
	pCFSrc		= new complex<double> [lHeight*lLineBytes];
	pCFH		= new complex<double> [lHeight*lLineBytes];

	// �˲�����Ȩϵ��
	double *pCFFilter   = new double [lHeight*lLineBytes];

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ���˻�ͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// ������ֵ�洢��ʱ��������
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// Ƶ����ֵ
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// �˻�ϵͳʱ��ά���˲���Ȩϵ����ֵ
			if(i < 5 && j <5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
				pCFFilter[ lLineBytes*j + i ] = 0.5;
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);	
				pCFFilter[ lLineBytes*j + i ] = 0.05;
			}

			// Ƶ����ֵ
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}
	
	//���˻�ͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);
	
	// ����M
	for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// ��ֵ
			a = pCFSrc[i].real();
			b = pCFSrc[i].imag();
			c = pCFH[i].real();
			d = pCFH[i].imag();
			// |H(u,v)|*|H(u,v)|
			norm2 = c * c + d * d;
			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			temp  = (norm2 ) / (norm2 + pCFFilter[i]);
			{				
				tempre = ( a*c - b*d ) / ( c*c + d*d );
				tempim = ( b*c + a*d ) / ( c*c + d*d );
				// ���f(u,v)
				pCFSrc[i]= complex<double>(temp*tempre , temp*tempim);
				//pCFSrc[i]= complex<double>((a*c-b*d)/norm2 , (b*c+a*d)/norm2);
			}						
	}

	//�Ը�ԭͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ת��Ϊ��ԭͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			a = pCTSrc[(lLineBytes)*j + i].real();
			b = pCTSrc[(lLineBytes)*j + i].imag();

			norm2  = a*a + b*b;
			norm2  = sqrt(norm2) + 40;
			//norm2  = sqrt(norm2) ;
			if(norm2 > 255)
				norm2 = 255.0;
			if(norm2 < 0)
				norm2 = 0;	

			*lpSrc = (unsigned char) (norm2);			
		}
	}

	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;
	delete pCFFilter;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBMotionDegeneration()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ��ģ��������ֱ���˶���ɵ�ģ��
 *
 ***********************************************************************
 */

BOOL WINAPI DIBMotionDegeneration(CDib *pDib)
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
	
	//ѭ������
	long iColumn;
	long jRow;

	//��ʱ����
	int temp,m;

	// ��ʱ����
	double p,q;
	
	int nTotTime, nTotLen, nTime;

	//�ܵ��˶�ʱ��10s
	nTotTime = 10;

	// �ܵ��˶�����10�����ص�
	nTotLen = 10;
	
	// ������ı���ϵ��
	double B;

	B = 0.1;

	//�����洢Դͼ��ͱ任�˵�ʱ������
	int *nImageDegener;

	// Ϊʱ���Ƶ����������ռ�
	nImageDegener = new int [lHeight*lLineBytes];
	
	// �����ݴ���ʱ������
	for (jRow = 0; jRow < lHeight; jRow++)
	{
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{
			temp=0;
			
			// ָ��Դͼ������jRow�У���iColumn�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;

			// ���ص������ֵ����
			for ( nTime = 0; nTime < nTotTime; nTime++ )
			{
				p = (float)iColumn - (float)(nTotLen)*nTime/nTotTime;
				
				if (p > 0)
				{
					q = p - floor((double)p);
					
					if(q >= 0.5)
						m = (int)ceil((double)p);
					else
						m = (int)floor((double)p);

					// �ۼ�
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m;
					temp = temp + *lpSrc;
				}	
			}
			
			// ����������ı���ϵ��
			temp = B * temp;

			temp=(int)ceil((double)temp);
			
			// ʹ��temp��ȡֵ����ȡֵ��Χ
			if(temp<0)
				temp=0;

			if(temp>255)
				temp=255;

			nImageDegener[lLineBytes*jRow + iColumn] = temp;
		}
	}	
	
	//ת��Ϊͼ��
	for (jRow = 0;jRow < lHeight ;jRow++)
	{
		for(iColumn = 0;iColumn < lLineBytes ;iColumn++)
		{
			// ָ��Դͼ������jRow�У���iColumn�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;
	
			*lpSrc = nImageDegener[lLineBytes*jRow + iColumn];
		}
	}
	
	//�ͷŴ洢�ռ�
	delete nImageDegener;
	
	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBMotionRestore()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����������������ֱ���˶���ɵ�ģ��ͼ����и�ԭ
 *
 ***********************************************************************
 */

BOOL WINAPI DIBMotionRestore(CDib *pDib)
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
	
	//ѭ������
	long iColumn;
	long jRow;

	int i,n,m;

	//��ʱ����
	int temp1,temp2,
		totalq,q1,q2,z;

	double p,q;

	// ����A��ֵ
	int A = 80;
	
	//����B��ֵ
	int B = 10;
	
	//�ܵ��ƶ�����
	int nTotLen=10;

	// ͼ���Ȱ������ٸ�ntotlen
	int K=lLineBytes/nTotLen;
	
	int error[10];

	//�����洢Դͼ��ʱ������
	int *nImageDegener;

	// Ϊʱ���������ռ�
	nImageDegener = new int [lHeight*lLineBytes];

	// �����ش���������
	for (jRow = 0; jRow < lHeight; jRow++)
	{
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;	
			nImageDegener[lLineBytes*jRow + iColumn] = (*lpSrc);
		}
	}	
	
	for (jRow = 0; jRow < lHeight; jRow++)
	{		
		// ����error[i]
		for(i = 0; i < 10; i++)
		{			
			error[i] = 0;			
			for(n = 0; n < K; n++)
			{
				for(m = 0; m <= n; m++)
				{
					// �����Ƿ�Ϊһ�еĿ�ʼ��
					if(i == 0 && m == 0)
					{
						temp1=temp2=0;
					}					
					// ���в������
					else
					{
						lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m*10+i;
						temp1=*lpSrc;						
						lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m*10+i-1;
						temp2 = *lpSrc;
					}					
					error[i] = error[i] + temp1 - temp2;					
				}
			}
			error[i] = B * error[i] / K;
		}		
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{			
			// ����m���Լ�z
			m = iColumn / nTotLen;
			z = iColumn - m*nTotLen;				
			// ��ʼ��
			totalq = 0;	q = 0;			
			for(n = 0; n <= m; n++)
			{
				q1 = iColumn - nTotLen*n;				
				if(q1 == 0)
					q = 0;				
				// ���в������
				else
				{
					q2 = q1 - 1;					
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + q1;
					temp1 = *lpSrc;					
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + q2;
					temp2 = *lpSrc;					
					q = (temp1 - temp2) * B;			
				}				
				totalq = totalq + q;
			}			
			p = error[z];
			// �õ�f(x,y)��ֵ
			temp1 = totalq + A - p;			
			// ʹ�����ص�ȡֵ����ȡֵ��Χ
			if(temp1 < 0)
				temp1 = 0;			
			if(temp1 > 255)
				temp1 = 255;						
			nImageDegener[lLineBytes*jRow + iColumn] = temp1;
		}
	}
	//ת��Ϊͼ��
	for (jRow = 0;jRow < lHeight ;jRow++)
	{
		for(iColumn = 0;iColumn < lLineBytes ;iColumn++)
		{
			// ָ��Դͼ������jRow�У���iColumn�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;
	
			// �洢����ֵ
			*lpSrc = nImageDegener[lLineBytes*jRow + iColumn];
		}
	}	
	
	//�ͷŴ洢�ռ�
	delete nImageDegener;
	
	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBMinDblFilter()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - ά���˲���ԭ�����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ�������Сƽ���˲���ԭ������
 *
 ************************************************************************/

BOOL WINAPI DIBMinDblFilter (CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//��ʱ����
	double temp, tempre, tempim, 
			a, b, c, d, e, f, norm1,norm2;
	
	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ������
	complex<double>  *pCFSrc,*pCFH;
	
	//�����˻�ͼ��ĳ��Ϳ����Ϊ2��������
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// Ϊʱ���Ƶ����������ռ�
	pCTSrc		= new complex<double> [lHeight*lLineBytes];
	pCTH		= new complex<double> [lHeight*lLineBytes];
	
	pCFSrc		= new complex<double> [lHeight*lLineBytes];
	pCFH		= new complex<double> [lHeight*lLineBytes];

	// �˲�����Ȩϵ��
	complex<double> *pCFFilter   = new complex<double> [lHeight*lLineBytes];
	complex<double> *pCFFilterD   = new complex<double> [lHeight*lLineBytes];

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ���˻�ͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			pCFFilterD[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// ������ֵ�洢��ʱ��������
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// Ƶ����ֵ
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// �˻�ϵͳʱ��ά���˲���Ȩϵ����ֵ
			if(i < 5 && j <5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);			
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);					
			}
			pCFFilter[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);

			// Ƶ����ֵ
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}
	pCFFilter[ lLineBytes*0 + 1 ] = complex<double>(-1 , 0.0);
	pCFFilter[ lLineBytes*1 + 0 ] = complex<double>(-1 , 0.0);
	pCFFilter[ lLineBytes*1 + 1 ] = complex<double>( 4 , 0.0);
	pCFFilter[ lLineBytes*1 + 2 ] = complex<double>(-1 , 0.0);
	pCFFilter[ lLineBytes*2 + 1 ] = complex<double>(-1 , 0.0);
	
	//���˻�ͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//�Թ��������б任FFT
	::DIBFFT_2D(pCFFilter, lLineBytes, lHeight, pCFFilterD);
	
	// ����M
	double gama=0.05;
	for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// ��ֵ
			a = pCFSrc[i].real();
			b = pCFSrc[i].imag();
			c = pCFH[i].real();
			d = pCFH[i].imag();
			e = pCFFilter[i].real();
			f = pCFFilter[i].imag();
			// |H(u,v)|*|H(u,v)|
			norm1 = c * c + d * d;
			norm2 = e * e + f * f;
			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			temp  = norm1 + norm2 * gama ;
			{				
				tempre = ( a*c + b*d ) / temp;
				tempim = ( b*c - a*d ) / temp;
				// ���f(u,v)
				pCFSrc[i]= complex<double>(tempre , tempim);
			}						
	}

	//�Ը�ԭͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ת��Ϊ��ԭͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			a = pCTSrc[(lLineBytes)*j + i].real();
			b = pCTSrc[(lLineBytes)*j + i].imag();

			norm2  = a*a + b*b;
			norm2  = sqrt(norm2) + 40;
			//norm2  = sqrt(norm2) ;
			if(norm2 > 255)
				norm2 = 255.0;
			if(norm2 < 0)
				norm2 = 0;	

			*lpSrc = (unsigned char) (norm2);			
		}
	}

	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;
	delete pCFFilter;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBMaxProFilter()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �������˲���ԭ�����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ������������˲���ԭ������
 *
 ************************************************************************/

BOOL WINAPI DIBMaxProFilter (CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//��ʱ����
	double temp, tempre, tempim, 
			a, b, c, d, e, f, norm1,norm2;
	
	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ�����ݣ�*pCFHnew�µ��˲���
	complex<double>  *pCFSrc,*pCFH,*pGF,*pCFHnew,*f1,*f2;
	
	//�����˻�ͼ��ĳ��Ϳ����Ϊ2��������
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// Ϊʱ���Ƶ����������ռ�,
	pCTSrc		= new complex<double> [lHeight*lLineBytes];
	pCTH		= new complex<double> [lHeight*lLineBytes];
	
	pCFSrc		= new complex<double> [lHeight*lLineBytes];
	pCFH		= new complex<double> [lHeight*lLineBytes];
	pGF	    	= new complex<double> [lHeight*lLineBytes];
	pCFHnew     = new complex<double> [lHeight*lLineBytes];
	f1     = new complex<double> [lHeight*lLineBytes];
    f2     = new complex<double> [lHeight*lLineBytes];
	// �˲�����Ȩϵ��
	double *pCFFiletr   = new double [lHeight*lLineBytes];
	complex<double> *pCTNoise   = new complex<double> [lHeight*lLineBytes];
	complex<double> *pCFNoise   = new complex<double> [lHeight*lLineBytes];

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ���˻�ͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
				
			// ������ֵ�洢��ʱ��������
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// Ƶ����ֵ
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// �˻�ϵͳʱ��ά���˲���Ȩϵ����ֵ
			if(i < 5 && j <5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);	
				pCFFiletr[ lLineBytes*j + i ] = 0.05;
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);			
				pCFFiletr[ lLineBytes*j + i ] = 0.025;
			}			

			// Ƶ����ֵ
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			
			if(i==j)
				pCTNoise[ lLineBytes*j + i ] = complex<double>(10 , 0.0);
			else
				pCTNoise[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}
	
	//���˻�ͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	pGF=pCFSrc;
	f1=pCFSrc;
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//�Թ��������б任FFT
	::DIBFFT_2D(pCTNoise, lLineBytes, lHeight, pCFNoise);
	
	// ����M
	double gama=0.05;
	double fain=0,faif=0,lamta;
		for (i = 0; i < lHeight * lLineBytes; i++)
	{
		a = f1[i].real();
		b = f1[i].imag();		
		e = pCFNoise[i].real();
		f = pCFNoise[i].imag();
		fain += (e*e+f*f);
		faif += (a*a+b*b);		 
	}
	lamta = fain/faif;
	for (int k=1;k<6;k++)
	{
	
		for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// ��ֵ
			a = f1[i].real();
			b = f1[i].imag();
			c = pCFH[i].real();
			d = pCFH[i].imag();
			e = pCFNoise[i].real();
			f = pCFNoise[i].imag();
			// |H(u,v)|*|H(u,v)|
			norm1 = c * c + d * d;
			norm2 = a * a + b * b;
		//	norm3 = e * e + f * f;
		//	fain += norm2;
		//	faif += (a*a+b*b);
		 	//lamta =( e * e + f * f)/norm2;
			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			//temp  = norm1 + lamta ;
			temp  = norm1 + lamta ;			
			{				
				tempre = ( a*c + b*d ) / temp;
				tempim = ( b*c - a*d ) / temp;
				// ���f(u,v)
				pCFSrc[i]= complex<double>(tempre , tempim);
			}						
	}
     f2=pCFSrc;
	 for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// ��ֵ
			a = f1[i].real();
			b = f1[i].imag();
			c = f2[i].real();
			d = f2[i].imag();
			e = pGF[i].real();
			f = pGF[i].imag();
		
			// |H(u,v)|*|H(u,v)|
			norm1 = a * a + b * b;
			norm2 = c * c + d * d;
			
			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			//temp  = norm1 + lamta ;
			temp  = norm1 + norm2/lHeight/lLineBytes ;
			temp=lHeight * lLineBytes *temp;
			{				
				tempre = ( e*c - f*d ) / temp;
				tempim = ( f*c + e*d ) / temp;
				// ���f(u,v)
				pCFH[i]= complex<double>(tempre , tempim);
			}
			f1=f2;
			
	}
	}
	//�Ը�ԭͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ת��Ϊ��ԭͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			a = pCTSrc[(lLineBytes)*j + i].real();
			b = pCTSrc[(lLineBytes)*j + i].imag();

			norm2  = a*a + b*b;
			norm2  = sqrt(norm2)+10 ;
			//norm2  = sqrt(norm2) ;
			if(norm2 > 255)
				norm2 = 255.0;
			if(norm2 < 0)
				norm2 = 0;	

			*lpSrc = (unsigned char) (norm2);			
		}
	}

	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;
	delete pCTNoise;
	delete pCFNoise;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBProjectFilter()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - ͶӰ�˲���ԭ�����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ�����ͶӰ�˲���ԭ������
 *
 ************************************************************************/

BOOL WINAPI DIBProjectFilter (CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;
	int N=lHeight;//��ʼ��
	int K=15;//��������
	double **arr = new double*[N];
	//����ϵ����a
	for(j=0;j<N;j++)
	{
		arr[j] = new double[lLineBytes];
		for(i=0;i<lLineBytes;i++)
			arr[j][i] =1.15;
	}
	//aΪϵ����
	int **pDesFi = new int*[lHeight];
	int **tempFi = new int*[lHeight];
	int **gGFi = new int*[lHeight];
	int *Gj = new int[lHeight];
	int *Kj = new int[lHeight];
	for(j=0;j<lHeight;j++)
	{
		pDesFi[j] = new int[lLineBytes];
		gGFi[j] = new int[lLineBytes];
		tempFi[j] = new int[lLineBytes];
		for(i=0;i<lLineBytes;i++)
		{
			pDesFi[j][i]=*((unsigned char *)lpDIBBits + lLineBytes * j + i);//��ʼ��
			gGFi[j][i]=*((unsigned char *)lpDIBBits + lLineBytes * j + i);//��ʼ��
		}
	}
	for(j=1;j<=N*K;j++)
	{
		// ajTcaj�Ǽ���ϵ��ģ
		double ajTcaj = 0;
		for(i=0;i<lLineBytes;i++)
		{			
			ajTcaj = ajTcaj + arr[(j-1)%N][i] * arr[(j-1)%N][i];
		}
		// 
		for(i=0;i<lHeight;i++)
		{
			double t1=0,t2=0;
			for(int m=0;m<lLineBytes;m++)
			{				
				t1 = t1 + arr[(j-1)%N][m]*pDesFi[i][m];
				t2 = t2 + gGFi[m][i];
			}
			Kj[i] = t1;//��a(i)T*f(i-1) = Kj
			Gj[i] = t2;
			Kj[i] = Kj[i]-Gj[i];
		}
		for(int m=0;m<lHeight;m++)
		{
			for(i=0;i<lLineBytes;i++)
			{
				tempFi[m][i] = Kj[i]*arr[(j-1)%N][i]/ajTcaj;
				pDesFi[m][i] = pDesFi[m][i] - tempFi[m][i];
			}
		}		
	}	

	//ת��Ϊ��ԭͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			*lpSrc = (unsigned char) (pDesFi[j][i]);		
		}
	}
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBBlindFilter()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - ͶӰ�˲���ԭ�����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ����е���äĿ��ԭ������
 *
 ************************************************************************/

BOOL WINAPI DIBBlindFilter (CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//��ʱ����
	double temp, tempre, tempim, 
			a, b, c, d, e, f, norm1,norm2;
	
	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ�����ݣ�*pCFHnew�µ��˲���
	complex<double>  *pCFSrc,*pCFH,*pGF,*pCFHnew,*f1,*f2;
	
	//�����˻�ͼ��ĳ��Ϳ����Ϊ2��������
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// Ϊʱ���Ƶ����������ռ�,
	pCTSrc		= new complex<double> [lHeight*lLineBytes];
	pCTH		= new complex<double> [lHeight*lLineBytes];
	
	pCFSrc		= new complex<double> [lHeight*lLineBytes];
	pCFH		= new complex<double> [lHeight*lLineBytes];
	pGF	    	= new complex<double> [lHeight*lLineBytes];
	pCFHnew     = new complex<double> [lHeight*lLineBytes];
	f1     = new complex<double> [lHeight*lLineBytes];
    f2     = new complex<double> [lHeight*lLineBytes];
	// �˲�����Ȩϵ��
	double *pCFFiletr   = new double [lHeight*lLineBytes];
	complex<double> *pCTNoise   = new complex<double> [lHeight*lLineBytes];
	complex<double> *pCFNoise   = new complex<double> [lHeight*lLineBytes];

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ���˻�ͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
				
			// ������ֵ�洢��ʱ��������
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// Ƶ����ֵ
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// �˻�ϵͳʱ��ά���˲���Ȩϵ����ֵ
			if(i < 5 && j <5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);	
				pCFFiletr[ lLineBytes*j + i ] = 0.05;
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);			
				pCFFiletr[ lLineBytes*j + i ] = 0.025;
			}			

			// Ƶ����ֵ
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			
			if(i==j)
				pCTNoise[ lLineBytes*j + i ] = complex<double>(0.1 , 0.0);
			else
				pCTNoise[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}
	
	//���˻�ͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	pGF=pCFSrc;
	f1=pCFSrc;
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//�Թ��������б任FFT
	::DIBFFT_2D(pCTNoise, lLineBytes, lHeight, pCFNoise);
	//��������������
	double alf=0;
	
		for (i = 0; i < lHeight * lLineBytes; i++)
	{
		
		a = pCFNoise[i].real();
		b = pCFNoise[i].imag();
		alf=alf+a*a+b*b;
				 
	}
   alf=alf/lLineBytes/lHeight;
		
	for (int k=1;k<10;k++)
	{
	
		for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// ��ֵ
			a = f1[i].real();
			b = f1[i].imag();
			c = pCFH[i].real();
			d = pCFH[i].imag();
	     	e = pGF[i].real();
			f = pGF[i].imag();
			// |H(u,v)|*|H(u,v)|
				norm1 = a * a + b * b;
				norm2 = c * c + d * d;
			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			//temp  = norm1 + lamta ;
			temp  = norm1 + 1/ norm2;			
			{				
				tempre = ( a*e + b*f ) / temp;
				tempim = ( a*f - e*b ) / temp;
				// ���f(u,v)
				pCFHnew[i]= complex<double>(tempre , tempim);
			}						
	}
     
	 for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// ��ֵ
			a = f1[i].real();
			b = f1[i].imag();
			
	    	c = pCFH[i].real();
			d = pCFH[i].imag();
			e = pGF[i].real();
			f = pGF[i].imag();
			// |H(u,v)|*|H(u,v)|
			norm1 = a * a + b * b;
			norm2 = c * c + d * d;
			// a/(|f(u,v)|*|f(u,v)|)+(|H(u,v)|*|H(u,v)|)
			temp  = 1/norm1*alf + norm2 ;
			
			{				
				tempre = ( e*c + f*d ) / temp;
				tempim = ( c*f - d*e ) / temp;
				// ���f(u,v)
				pCFSrc[i]= complex<double>(tempre , tempim);
			}
				
	}
	 f1=pCFSrc;
	 pCFH=pCFHnew;
	}
	//�Ը�ԭͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ת��Ϊ��ԭͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			a = pCTSrc[(lLineBytes)*j + i].real();
			b = pCTSrc[(lLineBytes)*j + i].imag();

			norm2  = a*a + b*b;
			norm2  = sqrt(norm2)+10 ;
			//norm2  = sqrt(norm2) ;
			if(norm2 > 255)
				norm2 = 255.0;
			if(norm2 < 0)
				norm2 = 0;	

			*lpSrc = (unsigned char) (norm2);			
		}
	}

	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;
	delete pCTNoise;
	delete pCFNoise;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBMaxEntrFilter()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - ������˲���ԭ�����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ�����������˲���ԭ������
 *
 ************************************************************************/

BOOL WINAPI DIBMaxEntrFilter(CDib *pDib)
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
	
	//ѭ������
	long i,j,N=120;
	const int T = 5;
	double PSF[T][T];
	for(i=0;i<T;i++)
		for(j=0;j<T;j++)
			PSF[i][j]=1.0/(T*T);

	double **blurred_image = new double*[lHeight];	
	double **lambda = new double*[lHeight];	
	double **deblurred = new double*[lHeight];
	double **reblurred = new double*[lHeight];
	double **deblurred_image = new double*[lHeight];

	double mean2=0;
	double totd=0;
	double scale=0;
	double entropy=0; 
	
	for(j=0;j<lHeight;j++)
	{
		blurred_image[j] = new double[lLineBytes];
		lambda[j] = new double[lLineBytes];		
		deblurred[j] = new double[lLineBytes];
		reblurred[j] = new double[lLineBytes];
		deblurred_image[j] = new double[lLineBytes];

		for(i=0;i<lLineBytes;i++)
		{			
			blurred_image[j][i]=*((unsigned char *)lpDIBBits + lLineBytes * j + i);//��ʼ��
			mean2 += blurred_image[j][i];			
			lambda[j][i] = 0;//����ȫ0����		
		}
	}

	mean2 = mean2/( lHeight*lLineBytes );//blurred_image �ľ�ֵ
	for(j=0;j<lHeight;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			blurred_image[j][i]=blurred_image[j][i]
				+mean2*1/1000.0;
			totd += blurred_image[j][i];
		}
	}

	double sumT=0,sum=0;
	for(int n=1;n<=N;n++)
	{
		sumT=0;
		for(i=0;i<lHeight;i++)	
		{
			for(j=0;j<lLineBytes;j++)
			{
				int xx,yy;
				sum=0;
				for(xx=0;xx<T;xx++)
				{
					for(yy=0;yy<T;yy++)
					{
						if( (i+xx-T/2)<lHeight && (i+xx-T/2)>=0 && (j+yy-T/2)<lLineBytes && (j+yy-T/2)>=0 )
						{
							sum = sum + lambda[i+xx-T/2][j+yy-T/2]*PSF[xx][yy];
						}
					}
				}
				deblurred[i][j] = exp(-1+sum);
				sumT += deblurred[i][j];
			}
		}
		for(i=0;i<lHeight;i++)	
		{
			for(j=0;j<lLineBytes;j++)
			{
				deblurred[i][j] = deblurred[i][j]/sumT;
			}
		}
		sumT = 0;
		for(i=0;i<lHeight;i++)	
		{
			for(j=0;j<lLineBytes;j++)
			{
				int xx,yy;
				sum=0;
				for(xx=0;xx<T;xx++)
				{
					for(yy=0;yy<T;yy++)
					{
						if( (i+xx-T/2)<lHeight && (i+xx-T/2)>=0 && (j+yy-T/2)<lLineBytes && (j+yy-T/2)>=0 )
						{
							sum = sum + deblurred[i+xx-T/2][j+yy-T/2]*PSF[xx][yy];
						}
					}
				}
				reblurred[i][j] = sum;
				sumT += reblurred[i][j];
			}
		}
		scale = sumT/totd;
		//errû����������û���õ�		
		for(i=0;i<lHeight;i++)	
		{
			for(j=0;j<lLineBytes;j++)
			{
				entropy += deblurred[i][j]*log(deblurred[i][j]);
			}
		}
		entropy = -entropy;		
		if(n<30)
		{
			for(i=0;i<lHeight;i++)	
			{
				for(j=0;j<lLineBytes;j++)
				{
					lambda[i][j]=lambda[i][j]+(blurred_image[i][j]*scale-reblurred[i][j]);
				}
			}
		}
		else
		{
			for(i=0;i<lHeight;i++)	
			{
				for(j=0;j<lLineBytes;j++)
				{
					lambda[i][j]=lambda[i][j]+log(blurred_image[i][j]*scale/reblurred[i][j]);
				}
			}
		}
	}
	for(i=0;i<lHeight;i++)	
	{
		for(j=0;j<lLineBytes;j++)
		{
			deblurred_image[i][j]=deblurred[i][j]/scale-mean2/1000;
		}
	}
	
	//ת��Ϊ��ԭͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			*lpSrc = (unsigned char) ((deblurred_image[j][i]+0.5)>255?255:deblurred_image[j][i]+0.5);		
		}
	}
	return true;
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBGeoFilter()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - ���θ�ԭ�����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ����м��θ�ԭ������
 *
 ************************************************************************/
BOOL WINAPI DIBGeoFilter(CDib *pDib)
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
	
	int  i,j,g,l;
	double xu,yu,xd,yd,r,m,n,p,q;
	char *lpDIBBitsNew = new char[lHeight*lLineBytes];	

	for(i=0;i<lHeight;i++)
	{
		for(j=0;j<lLineBytes;j++)
		{			
			lpDIBBitsNew[i*lLineBytes+j]=*((unsigned char *)lpDIBBits + lLineBytes * i + j);//��ʼ��
		}
	}

	for(j=0;j<lHeight;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{          
			xu=(i-382.491)/599.359;
			yu=(j-268.013)/598.340;
			r=xu*xu+yu*yu;
			xd=xu+xu*r*(-0.304484-0.040202*r)+2*0.002783*xu*yu-0.000259*(r+2*xu);
			yd=yu+yu*r*(-0.304484-0.040202*r)+0.002783*(r+2*yu)-2*0.000259*xu*yu;
			m=xd*599.359+382.491;		  
			n=yd*598.340+268.013;
			g=int(m);       
			l=int(n);
			p=m-g;
			q=n-l;
			*(lpDIBBitsNew+j*lLineBytes+i)=
				(1-p)*(1-q)* (*(lpDIBBits+ l     *lLineBytes+  g))+
				(1-p)* q   * (*(lpDIBBits+ (l+1) *lLineBytes+  g))+
				  p  *(1-q)* (*(lpDIBBits+ (l)   *lLineBytes+  g+1))+
				  p  *  q  * (*(lpDIBBits+ (l+1) *lLineBytes+  g+1)); 
		}
	}

	//ת��Ϊ��ԭͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			*lpSrc = (unsigned char) lpDIBBitsNew[j*lLineBytes+i];	
		}
	}
	return true;
}
#undef SWAP

