#include "StdAfx.h"
#include "RegionSegment.h"

CRegionSegment::CRegionSegment(CDib *pDib)
{
    m_pDib=pDib;
}

CRegionSegment::~CRegionSegment(void)
{
}

//==========================================================
// �������ܣ� ��������
// ��������� point:�����û���ԭͼ�ϵ��������ȷ�������ӵ�
// ����ֵ��   ��
//==========================================================
void CRegionSegment::RegionGrow(CPoint point)
{
    // ѭ������
	int i, j;

	// ԭͼ��������ָ��
	LPBYTE p_data;
    p_data = m_pDib->GetData();

    // ͼ��ÿ��������ռ���ֽ���
    int nLineByte = m_pDib->GetLineByte();

	// ͼ��Ŀ��
	int nWidth = m_pDib->GetWidth();

	// ͼ��ĸ߶�
	int nHeight = m_pDib->GetHeight();
	                                                                                                            	
	// �������ӵ�ĻҶ�ֵ
	int nSeedPoint = *(p_data + (nHeight - point.y) * nLineByte + point.x);	

	// �Ը������ؽ��лҶ�ת��
	for (j = 0; j < nHeight; j ++)
		for (i = 0; i < nWidth; i ++)
		{
			//��ǰ���ص�ĻҶ�ֵ
			int nTemp = *(p_data + nLineByte * j + i);

            //�����ǰ�������ӵ�ĻҶ�ֵ֮��ľ���ֵ������ֵ10���򽫸õ���Ϊ255
			if ( abs(nTemp - nSeedPoint) > 10 )    
				*(p_data + nLineByte * j + i) = 255;
		}
}
