//======================================================================
// �ļ��� GrayMorphDib.cpp
// ���ݣ� �Ҷ���̬ѧ����������㺯��-ԭ�ļ�
// ���ܣ� ��1����ʴ���㣻
//        ��2���������㣻
//        ��3����������㣻
//        ��4���Ҷ���̬ѧ�ݶȣ�
//        ��5��Top-Hat�任��
// 
//======================================================================

#include "StdAfx.h"
#include "GrayMorphDib.h"

//=======================================================
// �������ܣ� ���캯������ʼ�����ݳ�Ա
// ��������� λͼָ��
// ����ֵ��   ��
//=======================================================
CGrayMorphDib::CGrayMorphDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// �������ܣ� ��������
// ��������� ��
// ����ֵ��   ��
//=======================================================

CGrayMorphDib::~CGrayMorphDib(void)
{
}


//=======================================================
// �������ܣ� �Ҷ�ͼ��ʴ����
// ��������� int Structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   ��
//=======================================================

void CGrayMorphDib::Erosion(int Structure[3][3])
{
	LPBYTE	lpSrc;			// ָ��ԭͼ���ָ��	
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpTempDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	
	long i,j,n,m;			//ѭ������
	unsigned char p,q;	    //����ֵ,�м����

	LPBYTE lpDIBBits=m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth=m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight=m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	lpTempDIBBits=new BYTE[lWidth * lHeight];

	//��ʼ��
	memset(lpTempDIBBits, (BYTE)0, lWidth * lHeight);

	//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			// ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
			// ���������غ����ϱߺ����±ߵ���������

			// ָ��ԭͼ���j�У���i�����ص�ָ��			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// ָ��Ŀ��ͼ���j�У���i�����ص�ָ��			
			lpDst = (LPBYTE)lpTempDIBBits + lWidth * j + i;

			p=q=255;    //��ΪҪ����Сֵ���ʽ��м��������Ϊ255
			
			//ԭͼ���и������Ӧ�ṹԪ���и��������
			//�ҵ����֮������Сֵ��Ϊ�㣨i��j���ĻҶ�ֵ
			//ע����DIBͼ�������������µ��õ�
			for (m = 0;m < 3;m++ )
			{
				for (n = 0;n < 3;n++)
				{
					p=(unsigned char) (*(lpSrc + (1-m)*lWidth + (n-1))-Structure[m][n]);

					if (p<q )
					{	
						q = p;
					}
				}
			}
			* lpDst= q;
		}

	}

	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpTempDIBBits, lWidth * lHeight);
	//�ͷ��ڴ�
	delete lpTempDIBBits;

}

//=======================================================
// �������ܣ� �Ҷ�ͼ����������
// ��������� int Structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   ��
//=======================================================

void CGrayMorphDib::Dilation(int Structure[3][3])
{
	LPBYTE	lpSrc;			// ָ��ԭͼ���ָ��	
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpTempDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	
	long i,j,n,m;			//ѭ������
	unsigned char p,q;	    //����ֵ,�м����

	LPBYTE lpDIBBits=m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth=m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight=m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	lpTempDIBBits=new BYTE[lWidth * lHeight];

	//��ʼ��
	memset(lpTempDIBBits, (BYTE)0, lWidth * lHeight);

	//ʹ���Զ���ĽṹԪ�ؽ�������
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			// ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
			// ���������غ����ϱߺ����±ߵ���������

			// ָ��ԭͼ���j�У���i�����ص�ָ��			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// ָ��Ŀ��ͼ���j�У���i�����ص�ָ��			
			lpDst = (LPBYTE)lpTempDIBBits + lWidth * j + i;

			p=q=0;    //��ΪҪ�����ֵ���ʽ��м��������Ϊ0
			
			//ԭͼ���и������Ӧ�ṹԪ���и�����ӣ�
			//�ҵ����֮�͵����ֵ��Ϊ�㣨i��j���ĻҶ�ֵ
			//ע����DIBͼ�������������µ��õ�
			for (m = 0;m < 3;m++ )
			{
				for (n = 0;n < 3;n++)
				{
					p=(unsigned char) (*(lpSrc + (1-m)*lWidth + (n-1))+Structure[m][n]);

					if (p>q )
					{	
						q = p;
					}
				}
			}
			* lpDst= q;
		}

	}

	// �������ͺ��ͼ��
	memcpy(lpDIBBits, lpTempDIBBits, lWidth * lHeight);
	//�ͷ��ڴ�
	delete lpTempDIBBits;

}

//=======================================================
// �������ܣ� �Ҷ�ͼ������
// ��������� int Structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   ��
//=======================================================

void CGrayMorphDib::Opening(int Structure[3][3])
{
	//�ȸ�ʴ������
	Erosion(Structure);
	Dilation(Structure);

}

//=======================================================
// �������ܣ� �Ҷ�ͼ�������
// ��������� int Structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   ��
//=======================================================

void CGrayMorphDib::Closing(int Structure[3][3])
{
	//�����ͺ�ʴ	
	Dilation(Structure);
	Erosion(Structure);

}

//=======================================================
// �������ܣ� �Ҷ���̬ѧ�ݶ�
// ��������� int Structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   ��
//=======================================================

void CGrayMorphDib::Grads(int Structure[3][3])
{
	LPBYTE	lpSave_of_SrcDIBBits;	// ָ�����ڱ���ԭͼ�񻺴��ָ��
	LPBYTE	lpRst_of_DilDIBBits;	// ָ�����ͽ�������ָ��
	long i,j;//ѭ������
	
		
	LPBYTE lpDIBBits=m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth=m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight=m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

	//�����ڴ��Ա���ͼ��
	lpSave_of_SrcDIBBits=new BYTE[lWidth * lHeight];
	lpRst_of_DilDIBBits=new BYTE[lWidth * lHeight];	

	memcpy(lpSave_of_SrcDIBBits,lpDIBBits, lWidth * lHeight);//����ԭͼ��

	//��������
	Dilation(Structure);

	memcpy(lpRst_of_DilDIBBits,lpDIBBits, lWidth * lHeight);//�������ͽ��

	memcpy(lpDIBBits, lpSave_of_SrcDIBBits, lWidth * lHeight);//�ָ�ԭͼ��

	Erosion(Structure);//��ʴ����

	//���ͽ���븯ʴ����ĲΪ��̬ѧ�ݶ�
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			*(lpDIBBits+lWidth*j+i)=(*(lpRst_of_DilDIBBits+lWidth*j+i))-(*(lpDIBBits+lWidth*j+i));
		}
	}

	//�ͷ��ڴ�
	delete	lpSave_of_SrcDIBBits;
	delete  lpRst_of_DilDIBBits;

}

//=======================================================
// �������ܣ� �Ҷ�ͼ��Top-Hat�任
// ��������� int Structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   ��
//=======================================================

void CGrayMorphDib::Top_Hat(int Structure[3][3])
{
	LPBYTE	lpSave_of_SrcDIBBits;	// ָ�����ڱ���ԭͼ�񻺴��ָ��
	long i,j;//ѭ������
		
		
	LPBYTE lpDIBBits=m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth=m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight=m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

	//�����ڴ��Ա���ͼ��
	lpSave_of_SrcDIBBits=new BYTE[lWidth * lHeight];
	
	memcpy(lpSave_of_SrcDIBBits,lpDIBBits, lWidth * lHeight);//����ԭͼ��

	//������
	Opening(Structure);


	//ԭͼ���뿪�������ĲΪTop-Hat�任
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			*(lpDIBBits+lWidth*j+i)=(*(lpSave_of_SrcDIBBits+lWidth*j+i))-(*(lpDIBBits+lWidth*j+i));
		}
	}

	//�ͷ��ڴ�
	delete	lpSave_of_SrcDIBBits;

}