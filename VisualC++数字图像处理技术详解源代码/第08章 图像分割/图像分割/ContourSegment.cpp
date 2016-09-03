#include "StdAfx.h"
#include "ContourSegment.h"

CContourSegment::CContourSegment(CDib *pDib)
{
    m_pDib = pDib;
}

CContourSegment::~CContourSegment(void)
{
}

//=======================================================
// �������ܣ� ������ȡ
// ��������� void
// ����ֵ��   void 
//=======================================================
void CContourSegment::ContourExtract(void)
{
    // ѭ������
	int i, j;

	// ԭͼ��������ָ��
	LPBYTE p_data, lpSrc;
    p_data = m_pDib->GetData();

    // ͼ��ÿ��������ռ���ֽ���
    int nLineByte = m_pDib->GetLineByte();

	// ͼ��Ŀ��
	int nWidth = m_pDib->GetWidth();

	// ͼ��ĸ߶�
	int nHeight = m_pDib->GetHeight();

    // ��ͼ�񻺳�����ָ��
	LPBYTE lpTemp, lpDst; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	memset(lpTemp, 255, nLineByte * nHeight);

    // ��ǰ���ص��8���������ֵ
	int n1,n2,n3,n4,n5,n6,n7,n8;

	// ����ֵ
	int nPixel;

	// ��ԭͼ���ж�ֵ��Ԥ����
    for (j = 0; j < nHeight; j ++)
		for(i = 0; i< nWidth; i ++)
		{
			lpSrc = p_data + nLineByte * j + i;
			if(*lpSrc > 127)
				*lpSrc = 255;
			else
				*lpSrc = 0;
		}

    // ������ȡ����
	for(j = 1; j < nHeight-1; j ++)
		for(i = 1; i < nWidth-1; i ++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
			lpSrc = p_data + nLineByte * j + i;           

			// ָ����ͼ������j�У���i�����ص�ָ��			
			lpDst = lpTemp + nLineByte * j + i;

			// ��ȡԭͼ��ǰָ����ָ���صĻҶ�ֵ
			nPixel = *lpSrc;

			// �����ǰ���ص�Ϊ��ɫ����ȡ����8�������ص�
            if(nPixel == 0)
			{
				// ����ͼ�����õ�ǰ���ص�Ϊ��ɫ
                *lpDst =0;
				n1 = *(lpSrc + nLineByte -1);
				n2 = *(lpSrc + nLineByte);
				n3 = *(lpSrc + nLineByte +1);
				n4 = *(lpSrc -1);
				n5 = *(lpSrc +1);
				n6 = *(lpSrc - nLineByte -1);
				n7 = *(lpSrc - nLineByte);
				n8 = *(lpSrc - nLineByte +1);

				// ���8��������ص㶼�Ǻ�ɫ������ͼ�����õ�ǰ���ص�Ϊ��ɫ
				if(n1+n2+n3+n4+n5+n6+n7+n8==0)
					*lpDst = 255;
			}
		}

	// ����������ͼ���Ƶ�ԭͼ����
	memcpy(p_data,lpTemp, nLineByte * nHeight);

	// �ͷ��ڴ�
	delete [] lpTemp;	 
}

//=======================================================
// �������ܣ� �߽����
// ��������� void
// ����ֵ��   void 
//======================================================= 
void CContourSegment::ContourTrack(void)
{ 
    // ѭ������
	int i, j;

	// ԭͼ��������ָ��
	LPBYTE p_data, lpSrc;
    p_data=m_pDib->GetData();

	// ͼ��ÿ��������ռ���ֽ���
    int nLineByte = m_pDib->GetLineByte();

	// ͼ��Ŀ��
    int nWidth = m_pDib->GetWidth();
    
    // ͼ��ĸ߶�
	int nHeight = m_pDib->GetHeight();

    // ��ͼ�񻺳�����ָ��
	LPBYTE lpTemp, lpDst; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	memset(lpTemp, 255, nLineByte * nHeight);
    
	// ����ֵ
    int nPixel;

	// �Ƿ��ҵ���ʼ�߽���Լ��Ƿ�ص���ʼ��ʼ��
	BOOL bFindStartPoint;

	// �Ƿ�ɨ�赽һ���߽��
	BOOL bFindPoint;

	// ��ʼ�߽��͵�ǰ�߽��
	CPoint StartPoint,CurrentPoint;

	// ˳ʱ�붨���������ص��8��������
	int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};

    // ��ʼ����
	int BeginDirect;

    // ��ԭͼ������ֵ��Ԥ����
	for (j = 0; j < nHeight; j ++)
		for(i = 0; i < nWidth; i ++)
		{
			lpSrc = p_data + nLineByte * j + i;
			if(*lpSrc > 127)
				*lpSrc = 255;
			else
				*lpSrc = 0;
		}
	
	// Ѱ�������·��ĺ�ɫ�߽��
	bFindStartPoint = FALSE;
	for (j = 0; j < nHeight && !bFindStartPoint; j ++)
		for(i = 0; i < nWidth && !bFindStartPoint; i ++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
			lpSrc = p_data + nLineByte * j + i;

			// ��ȡԭͼ��ǰָ����ָ���صĻҶ�ֵ
			nPixel =  *lpSrc;
           
            // �����ǰ���ص�Ϊ��ɫ�����ҵ��������·��ı߽��
            if(nPixel ==0)
			{
                // �ҵ���ʼ�߽��
                bFindStartPoint= TRUE ;      
				StartPoint.y = j;
				StartPoint.x = i;

				// ָ����ͼ������j�У���i�����ص�ָ��			
				lpDst = lpTemp + nLineByte * j + i;

                // ����ͼ�����õ�ǰ���ص�Ϊ��ɫ
				*lpDst = 0;                
			}           
		}

	// ������ʼ���������·�������ʼɨ�������Ϸ���
    BeginDirect = 0;

	// ��δ�ص���ʼ�߽��
	bFindStartPoint = FALSE;

	// ����ʼ�߽�㿪ʼɨ��
 	CurrentPoint.y = StartPoint.y;
	CurrentPoint.x = StartPoint.x;
	while(!bFindStartPoint)
	{
		// ��δ�ҵ��߽��
        bFindPoint = FALSE;
		while(!bFindPoint)
		{
			// ��ɨ�跽��鿴һ������               
			lpSrc = p_data +nLineByte * ( CurrentPoint.y + Direction[BeginDirect][1])
				  + (CurrentPoint.x + Direction[BeginDirect][0]);
			nPixel =  *lpSrc;

            // ����ԭ��
			if(nPixel== 0)
			{
				// �ҵ��߽�㣬����������Ϊ��ǰ�߽��
                bFindPoint = TRUE;
				CurrentPoint.y = CurrentPoint.y + Direction[BeginDirect][1];
				CurrentPoint.x = CurrentPoint.x + Direction[BeginDirect][0];

                // �����ǰ�߽�������ʼ�߽�㣬˵���߽���ٻص�����㣬���ٽ���
				if(CurrentPoint.y == StartPoint.y && CurrentPoint.x == StartPoint.x)
					bFindStartPoint = TRUE;

                // ָ����ͼ���Ӧ��ǰ�߽���ָ�룬����������ָ����Ϊ��ɫ
				lpDst =  lpTemp + nLineByte * CurrentPoint.y + CurrentPoint.x;
				*lpDst = 0;

				// ��ɨ�跽����ʱ����ת90��
				BeginDirect -= 2;
				if(BeginDirect < 0)
					BeginDirect += 8;
			}
			else
			{
				// ��ɨ�跽��˳ʱ����ת45��
				BeginDirect++;
				if(BeginDirect == 8)
					BeginDirect = 0;
			}
		}
	}

	// ����ͼ��
	memcpy(p_data, lpTemp, nLineByte * nHeight);

	// �ͷ��ڴ�
    delete [] lpTemp; 
}
