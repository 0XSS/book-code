//======================================================================
// �ļ��� BinaryMorphDib.cpp
// ���ݣ� ��ֵ��̬ѧ����������㺯��-Դ�ļ�
// ���ܣ� ��1����ʴ���㣻
//        ��2���������㣻
//        ��3����������㣻
//        ��4����������������㣻
//        ��5���Ǽ���ȡ�㷨��
// 
//======================================================================

#include "StdAfx.h"
#include "BinaryMorphDib.h"


//=======================================================
// �������ܣ� ���캯������ʼ�����ݳ�Ա
// ��������� λͼָ��
// ����ֵ��   ��
//=======================================================
CBinaryMorphDib::CBinaryMorphDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// �������ܣ� ��������
// ��������� ��
// ����ֵ��   ��
//=======================================================

CBinaryMorphDib::~CBinaryMorphDib(void)
{
	
}


//=======================================================
// �������ܣ� ��ֵͼ��ʴ����
// ��������� int Structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   �ɹ�����TRUE�����򷵻�FALSE
//=======================================================

BOOL CBinaryMorphDib::Erosion(int Structure[3][3])
{
	LPBYTE	lpSrc;			// ָ��ԭͼ���ָ��	
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;	
	long i,j,n,m;			//ѭ������
	unsigned char pixel;	//����ֵ

	LPBYTE lpDIBBits=m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth=m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight=m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	
	lpNewDIBBits = (LPBYTE )LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpDst = (LPBYTE)lpNewDIBBits;
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	//ʹ�ýṹԪ�ؽ��и�ʴ
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			// ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
			// ���������غ����ϱߺ����±ߵ���������

			// ָ��ԭͼ������j�У���i�����ص�ָ��			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel = (unsigned char)*lpSrc;

			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			if(pixel != 255 && *lpSrc != 0)
			{
				return FALSE;
			}

			//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
			*lpDst = (unsigned char)255;

			//���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ�����ǰ�ɫ��
			//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
			//ע����DIBͼ�������������µ��õ�
			for (m = 0;m < 3;m++ )
			{
				for (n = 0;n < 3;n++)
				{
					if( Structure[m][n] == 0)
				    continue;

					pixel = *(lpSrc + (1-m)*lWidth + (n-1));
					if (pixel == 0 )
					{	
						*lpDst = (unsigned char)0;
						break;
					}
				}
			}			
		}
	}
	
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;


}


//=======================================================
// �������ܣ� ��ֵͼ����������
// ��������� int Structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   �ɹ�����TRUE�����򷵻�FALSE
//=======================================================

BOOL CBinaryMorphDib::Dilation(int Structure[3][3])
{
	
	LPBYTE	lpSrc;			// ָ��ԭͼ���ָ��	
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;	
	long i,j,n,m;			//ѭ������
	unsigned char pixel;	//����ֵ

	LPBYTE lpDIBBits=m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth=m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight=m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	
	lpNewDIBBits = (LPBYTE )LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpDst = (LPBYTE)lpNewDIBBits;
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	//ʹ�ýṹԪ�ؽ�������
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			// ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
			// ���������غ����ϱߺ����±ߵ���������

			// ָ��ԭͼ������j�У���i�����ص�ָ��			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel = (unsigned char)*lpSrc;

			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			if(pixel != 255 && *lpSrc != 0)
			{
				return FALSE;
			}

			//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
			*lpDst = (unsigned char)0;

			//ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ����ֻҪ��һ���ǰ�ɫ��
			//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
			//ע����DIBͼ�������������µ��õ�
			for (m = 0;m < 3;m++ )
			{
				for (n = 0;n < 3;n++)
				{
					if( Structure[m][n] == 0)
				    continue;
					pixel = *(lpSrc + (1-m)*lWidth + (n-1));
					if (pixel == 255 )
					{	
						*lpDst = (unsigned char)255;
						break;
					}
				}
			}				
		}
	}
	
	// �������ͺ��ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;


}


//=======================================================
// �������ܣ� ��ֵͼ������
// ��������� int Structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   �ɹ�����TRUE�����򷵻�FALSE
//=======================================================
BOOL CBinaryMorphDib::Opening(int Structure[3][3])
{
	//�Ƚ��и�ʴ����,���ɹ�����FALSE
	if(!Erosion(Structure))
	{
		return FALSE;
	}
	//�ٽ�����������,���ɹ�����FALSE
    if(!Dilation(Structure))
	{
		return FALSE;
	}

	return TRUE;

}


//=======================================================
// �������ܣ� ��ֵͼ�������
// ��������� int Structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   �ɹ�����TRUE�����򷵻�FALSE
//=======================================================
BOOL CBinaryMorphDib::Closing(int Structure[3][3])
{
	//�Ƚ�����������,���ɹ�����FALSE
    if(!Dilation(Structure))
	{
		return FALSE;
	}
    //�ٽ��и�ʴ����,���ɹ�����FALSE
	if(!Erosion(Structure))
	{
		return FALSE;
	}
	
	return TRUE;
   
}


//=======================================================
// �������ܣ� ��ֵͼ����������������
// ���������int hStructure[3][3]-�û������3��3�ṹԪ��h
//           int mStructure[3][3]-�û������3��3�ṹԪ��m  
// ����ֵ��   ��
//=======================================================
void CBinaryMorphDib::Hit_Miss_Transform(int hStructure[3][3], int mStructure[3][3])
{
	long i,j;			//ѭ������
	
	LPBYTE	lpSave_of_SrcDIBBits;	// ָ�����ڱ���ԭͼ�񻺴��ָ��
	LPBYTE	lpRst_of_hStrDIBBits;	// ָ��ṹԪ��h��ʴͼ�񻺴��ָ��
	LPBYTE	lpRst_of_mStrDIBBits;   // ָ��ṹԪ��m��ʴͼ�񻺴��ָ��
		
	LPBYTE lpDIBBits=m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth=m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight=m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

	//�����ڴ��Ա���ͼ��
	lpSave_of_SrcDIBBits=new BYTE[lWidth * lHeight];
	lpRst_of_hStrDIBBits=new BYTE[lWidth * lHeight];
	lpRst_of_mStrDIBBits=new BYTE[lWidth * lHeight];

	memcpy(lpSave_of_SrcDIBBits,lpDIBBits, lWidth * lHeight);//����ԭͼ��

	Erosion(hStructure);//�ýṹԪ��h��ʴԭͼ��

	memcpy(lpRst_of_hStrDIBBits, lpDIBBits, lWidth * lHeight);//����ṹԪ��h��ԭͼ��ĸ�ʴ���

	memcpy(lpDIBBits, lpSave_of_SrcDIBBits, lWidth * lHeight);//�ָ�ԭͼ��

	//��ԭͼ����
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			if(*(lpDIBBits+lWidth*j+i)==0)
			{
				*(lpDIBBits+lWidth*j+i)=255;
			}
			else
			{
				*(lpDIBBits+lWidth*j+i)=0;
			}
		}
	}

	Erosion(mStructure);//�ýṹԪ��m��ʴԭͼ�񲹼�

	memcpy(lpRst_of_mStrDIBBits, lpDIBBits, lWidth * lHeight);//����ṹԪ��m��ԭͼ�񲹼��ĸ�ʴ���

	//��������ʴ����Ľ�����Ϊ����������б任���
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			if((*(lpRst_of_hStrDIBBits+lWidth*j+i)==255)&&(*(lpRst_of_mStrDIBBits+lWidth*j+i)==255))
			{
				*(lpDIBBits+lWidth*j+i)=255;
			}
			else
			{
				*(lpDIBBits+lWidth*j+i)=0;
			}
		}
	}

	//�ͷ��ڴ�
	delete	lpSave_of_SrcDIBBits;
	delete	lpRst_of_hStrDIBBits;
	delete	lpRst_of_mStrDIBBits;
	
}


//=======================================================
// �������ܣ� �Ǽ���ȡ�㷨
// ��������� int structure[3][3]-�û������3��3�ṹԪ��
// ����ֵ��   ��
//=======================================================
void CBinaryMorphDib::SkeletonExtraction(int Structure[3][3])
{
	long i,j;			//ѭ������
	unsigned char mark=1;//��־����
	
	LPBYTE	lpRst_of_nEroDIBBits;	// ָ��n�θ�ʴ��������ָ��
	LPBYTE	lpRst_of_nSXDIBBits;   // ָ��n������󼯺ϲ���ָ��
	LPBYTE	lpRst_of_nSkeDIBBits;   // ָ��n�������õ��ĹǼܽ���Ļ���ָ��

		
	LPBYTE lpDIBBits=m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth=m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight=m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

	//�����ڴ��Ա���ͼ��
	lpRst_of_nEroDIBBits=new BYTE[lWidth * lHeight];
	lpRst_of_nSXDIBBits=new BYTE[lWidth * lHeight];
	lpRst_of_nSkeDIBBits=new BYTE[lWidth * lHeight];

	memset(lpRst_of_nEroDIBBits, (BYTE)0, lWidth * lHeight);
	memset(lpRst_of_nSXDIBBits, (BYTE)0, lWidth * lHeight);
	memset(lpRst_of_nSkeDIBBits, (BYTE)0, lWidth * lHeight);

	while(mark)
	{
		Erosion(Structure);//�ýṹԪ�ظ�ʴԭͼ��
		memcpy(lpRst_of_nEroDIBBits, lpDIBBits, lWidth * lHeight);//�����n�θ�ʴ���
		
		mark=0;
		//���ñ�־mark�����ж��Ƿ����whileѭ��
		for(j = 0; j <lHeight; j++)
	    {
		    for(i = 0;i <lWidth; i++)
		    {
			    if((*(lpDIBBits+lWidth*j+i)==255))
			    {
					mark=1;
					i=lWidth;
					j=lHeight;					
			    }
			   
		    }
	    }
		
		Opening(Structure); //������

		//��n�θ�ʴ�����뿪����Ĳ�
		for(j = 0; j <lHeight; j++)
	    {
		    for(i = 0;i <lWidth; i++)
		    {
			    if((*(lpRst_of_nEroDIBBits+lWidth*j+i)==255)&&(*(lpDIBBits+lWidth*j+i)==0))
			    {
					*(lpRst_of_nSXDIBBits+lWidth*j+i)=255;					
			    }
							   
		    }
	    }

		//���n�������ĹǼ�
		for(j = 0; j <lHeight; j++)
	    {
		    for(i = 0;i <lWidth; i++)
		    {
			    if((*(lpRst_of_nSkeDIBBits+lWidth*j+i)==255)||(*(lpRst_of_nSXDIBBits+lWidth*j+i)==255))
			    {
					*(lpRst_of_nSkeDIBBits+lWidth*j+i)=255;					
			    }
				
		    }
	    }

		memcpy(lpDIBBits, lpRst_of_nEroDIBBits, lWidth * lHeight);//��n�θ�ʴ�������ԭͼ��

	}

	memcpy(lpDIBBits, lpRst_of_nSkeDIBBits, lWidth * lHeight);//��������õĹǼܽ�����Ƶ�ԭͼ��

	//�ͷ��ڴ�
	delete	lpRst_of_nEroDIBBits;
	delete	lpRst_of_nSXDIBBits;
	delete	lpRst_of_nSkeDIBBits;

}